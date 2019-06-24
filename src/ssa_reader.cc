/*
    Copyright (C) 2016-2019 Carl Hetherington <cth@carlh.net>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#include "ssa_reader.h"
#include "util.h"
#include "sub_assert.h"
#include "raw_convert.h"
#include "subtitle.h"
#include "compose.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <vector>

using std::string;
using std::vector;
using std::map;
using std::cout;
using std::list;
using boost::optional;
using boost::function;
using namespace boost::algorithm;
using namespace sub;

/** @param s Subtitle string encoded in UTF-8 */
SSAReader::SSAReader (string s)
{
	this->read (boost::bind(&get_line_string, &s));
}

/** @param f Subtitle file encoded in UTF-8 */
SSAReader::SSAReader (FILE* f)
{
	this->read (boost::bind (&get_line_file, f));
}

class Style
{
public:
	Style ()
		: font_size (72)
		, primary_colour (255, 255, 255)
		, bold (false)
		, italic (false)
		, underline (false)
		, horizontal_reference (HORIZONTAL_CENTRE_OF_SCREEN)
		, vertical_reference (BOTTOM_OF_SCREEN)
		, vertical_margin (0)
	{}

	Style (string format_line, string style_line)
		: font_size (72)
		, primary_colour (255, 255, 255)
		, bold (false)
		, italic (false)
		, underline (false)
		, horizontal_reference (HORIZONTAL_CENTRE_OF_SCREEN)
		, vertical_reference (BOTTOM_OF_SCREEN)
		, vertical_margin (0)
	{
		vector<string> keys;
		split (keys, format_line, boost::is_any_of (","));
		vector<string> style;
		split (style, style_line, boost::is_any_of (","));

		SUB_ASSERT (!keys.empty());
		SUB_ASSERT (!style.empty());
		SUB_ASSERT (keys.size() == style.size());

		for (size_t i = 0; i < style.size(); ++i) {
			trim (keys[i]);
			trim (style[i]);
			if (keys[i] == "Name") {
				name = style[i];
			} else if (keys[i] == "Fontname") {
				font_name = style[i];
			} else if (keys[i] == "Fontsize") {
				font_size = raw_convert<int> (style[i]);
			} else if (keys[i] == "PrimaryColour") {
				primary_colour = colour (raw_convert<int> (style[i]));
			} else if (keys[i] == "BackColour") {
				back_colour = colour (raw_convert<int> (style[i]));
			} else if (keys[i] == "Bold") {
				bold = style[i] == "-1";
			} else if (keys[i] == "Italic") {
				italic = style[i] == "-1";
			} else if (keys[i] == "Underline") {
				underline = style[i] == "-1";
			} else if (keys[i] == "BorderStyle") {
				if (style[i] == "1") {
					effect = SHADOW;
				}
			} else if (keys[i] == "Alignment") {
				/* These values from libass' source code */
				switch ((raw_convert<int> (style[i]) - 1) % 3) {
				case 0:
					horizontal_reference = LEFT_OF_SCREEN;
					break;
				case 1:
					horizontal_reference = HORIZONTAL_CENTRE_OF_SCREEN;
					break;
				case 2:
					horizontal_reference = RIGHT_OF_SCREEN;
					break;
				}
				switch (raw_convert<int> (style[i]) & 12) {
				case 4:
					vertical_reference = TOP_OF_SCREEN;
					break;
				case 8:
					vertical_reference = VERTICAL_CENTRE_OF_SCREEN;
					break;
				case 0:
					vertical_reference = BOTTOM_OF_SCREEN;
					break;
				}
			} else if (keys[i] == "MarginV") {
				vertical_margin = raw_convert<int> (style[i]);
			}
		}
	}

	string name;
	optional<string> font_name;
	int font_size;
	Colour primary_colour;
	/** outline colour */
	optional<Colour> back_colour;
	bool bold;
	bool italic;
	bool underline;
	optional<Effect> effect;
	HorizontalReference horizontal_reference;
	VerticalReference vertical_reference;
	int vertical_margin;

private:
	Colour colour (int c) const
	{
		return Colour (
			((c & 0x0000ff) >>  0) / 255.0,
			((c & 0x00ff00) >>  8) / 255.0,
			((c & 0xff0000) >> 16) / 255.0
			);
	}
};

Time
SSAReader::parse_time (string t) const
{
	vector<string> bits;
	split (bits, t, is_any_of (":."));
	SUB_ASSERT (bits.size() == 4);
	return Time::from_hms (
		raw_convert<int> (bits[0]),
		raw_convert<int> (bits[1]),
		raw_convert<int> (bits[2]),
		raw_convert<int> (bits[3]) * 10
		);
}

/** @param base RawSubtitle filled in with any required common values.
 *  @param line SSA line string (i.e. just the subtitle, possibly with embedded stuff)
 *  @return List of RawSubtitles to represent line with vertical reference TOP_OF_SUBTITLE.
 */
list<RawSubtitle>
SSAReader::parse_line (RawSubtitle base, string line, int play_res_x, int play_res_y)
{
	enum {
		TEXT,
		STYLE,
		BACKSLASH
	} state = TEXT;

	list<RawSubtitle> subs;
	RawSubtitle current = base;
	string style;

	if (!current.vertical_position.reference) {
		current.vertical_position.reference = BOTTOM_OF_SCREEN;
	}

	if (!current.vertical_position.proportional) {
		current.vertical_position.proportional = 0;
	}

	/* We must have a font size, as there could be a margin specified
	   in pixels and in that case we must know how big the subtitle
	   lines are to work out the position on screen.
	*/
	if (!current.font_size.points()) {
		current.font_size.set_points (72);
	}

	/* Count the number of line breaks */
	int line_breaks = 0;
	if (line.length() > 0) {
		for (size_t i = 0; i < line.length() - 1; ++i) {
			if (line[i] == '\\' && (line[i+1] == 'n' || line[i+1] == 'N')) {
				++line_breaks;
			}
		}
	}

	/* Imagine that the screen is 792 points (i.e. 11 inches) high (as with DCP) */
	double const line_size = current.font_size.proportional(792) * 1.2;

	/* Tweak vertical_position accordingly */
	switch (current.vertical_position.reference.get()) {
	case TOP_OF_SCREEN:
	case TOP_OF_SUBTITLE:
		/* Nothing to do */
		break;
	case VERTICAL_CENTRE_OF_SCREEN:
		current.vertical_position.proportional = current.vertical_position.proportional.get() - ((line_breaks + 1) * line_size) / 2;
		break;
	case BOTTOM_OF_SCREEN:
		current.vertical_position.proportional = current.vertical_position.proportional.get() + line_breaks * line_size;
		break;
	}

	for (size_t i = 0; i < line.length(); ++i) {
		char const c = line[i];
		switch (state) {
		case TEXT:
			if (c == '{') {
				state = STYLE;
			} else if (c == '\\') {
				state = BACKSLASH;
			} else if (c != '\r' && c != '\n') {
				current.text += c;
			}
			break;
		case STYLE:
			if (c == '}' || c == '\\') {
				if (!current.text.empty ()) {
					subs.push_back (current);
					current.text = "";
				}
				if (style == "\\i1") {
					current.italic = true;
				} else if (style == "\\i0" || style == "\\i") {
					current.italic = false;
				} else if (style == "\\b1") {
					current.bold = true;
				} else if (style == "\\b0") {
					current.bold = false;
				} else if (style == "\\u1") {
					current.underline = true;
				} else if (style == "\\u0") {
					current.underline = false;
				} else if (style == "\\an1" || style == "\\an2" || style == "\\an3") {
					current.vertical_position.reference = sub::BOTTOM_OF_SCREEN;
				} else if (style == "\\an4" || style == "\\an5" || style == "\\an6") {
					current.vertical_position.reference = sub::VERTICAL_CENTRE_OF_SCREEN;
				} else if (style == "\\an7" || style == "\\an8" || style == "\\an9") {
					current.vertical_position.reference = sub::TOP_OF_SCREEN;
				} else if (boost::starts_with(style, "\\pos")) {
					vector<string> bits;
					boost::algorithm::split (bits, style, boost::is_any_of("(,"));
					SUB_ASSERT (bits.size() == 3);
					current.horizontal_position.reference = sub::LEFT_OF_SCREEN;
					current.horizontal_position.proportional = raw_convert<float>(bits[1]) / play_res_x;
					current.vertical_position.reference = sub::TOP_OF_SCREEN;
					current.vertical_position.proportional = raw_convert<float>(bits[2]) / play_res_y;
				} else if (boost::starts_with(style, "\\fs")) {
					SUB_ASSERT (style.length() > 3);
					current.font_size.set_points (raw_convert<int>(style.substr(3)));
				} else if (boost::starts_with(style, "\\c")) {
					/* \c&Hbbggrr& */
					if (style.length() != 11 || style[2] != '&' || style[3] != 'H' || style[10] != '&') {
						throw SSAError(String::compose("Badly formatted colour tag %1", style));
					}
					int ir, ig, ib;
					if (sscanf(style.c_str() + 4, "%2x%2x%2x", &ib, &ig, &ir) < 3) {
						throw SSAError(String::compose("Badly formatted colour tag %1", style));
					}
					current.colour = sub::Colour(ir / 255.0, ig / 255.0, ib / 255.0);
				}
				style = "";
			}

			if (c == '}') {
				state = TEXT;
			} else {
				style += c;
			}
			break;
		case BACKSLASH:
			if (c == 'n' || c == 'N') {
				if (!current.text.empty ()) {
					subs.push_back (current);
					current.text = "";
				}
				/* Move down one line (1.2 times the font size) */
				if (current.vertical_position.reference.get() == BOTTOM_OF_SCREEN) {
					current.vertical_position.proportional = current.vertical_position.proportional.get() - line_size;
				} else {
					current.vertical_position.proportional = current.vertical_position.proportional.get() + line_size;
				}
			}
			state = TEXT;
			break;
		}
	}

	if (!current.text.empty ()) {
		subs.push_back (current);
	}

	return subs;
}

void
SSAReader::read (function<optional<string> ()> get_line)
{
	enum {
		INFO,
		STYLES,
		EVENTS
	} part = INFO;

	int play_res_x = 288;
	int play_res_y = 288;
	map<string, Style> styles;
	string style_format_line;
	vector<string> event_format;

	while (true) {
		optional<string> line = get_line ();
		if (!line) {
			break;
		}

		trim (*line);
		remove_unicode_bom (line);

		if (starts_with (*line, ";") || line->empty ()) {
			continue;
		}

		if (starts_with (*line, "[")) {
			/* Section heading */
			if (line.get() == "[Script Info]") {
				part = INFO;
			} else if (line.get() == "[V4 Styles]" || line.get() == "[V4+ Styles]") {
				part = STYLES;
			} else if (line.get() == "[Events]") {
				part = EVENTS;
			}
			continue;
		}

		size_t const colon = line->find (":");
		SUB_ASSERT (colon != string::npos);
		string const type = line->substr (0, colon);
		string body = line->substr (colon + 1);
		trim (body);

		switch (part) {
		case INFO:
			if (type == "PlayResX") {
				play_res_x = raw_convert<int> (body);
			} else if (type == "PlayResY") {
				play_res_y = raw_convert<int> (body);
			}
			break;
		case STYLES:
			if (type == "Format") {
				style_format_line = body;
			} else if (type == "Style") {
				SUB_ASSERT (!style_format_line.empty ());
				Style s (style_format_line, body);
				styles[s.name] = s;
			}
			break;
		case EVENTS:
			if (type == "Format") {
				split (event_format, body, is_any_of (","));
				BOOST_FOREACH (string& i, event_format) {
					trim (i);
				}
			} else if (type == "Dialogue") {
				SUB_ASSERT (!event_format.empty ());
				vector<string> event;
				split (event, body, is_any_of (","));

				/* There may be commas in the subtitle part; reassemble any extra parts
				   from when we just split it.
				*/
				while (event.size() > event_format.size()) {
					string const ex = event.back ();
					event.pop_back ();
					event.back() += "," + ex;
				}

				SUB_ASSERT (!event.empty());
				SUB_ASSERT (event_format.size() == event.size());

				RawSubtitle sub;

				for (size_t i = 0; i < event.size(); ++i) {
					trim (event[i]);
					if (event_format[i] == "Start") {
						sub.from = parse_time (event[i]);
					} else if (event_format[i] == "End") {
						sub.to = parse_time (event[i]);
					} else if (event_format[i] == "Style") {
						/* libass trims leading '*'s from style names, commenting that
						   "they seem to mean literally nothing".  Go figure...
						*/
						trim_left_if (event[i], boost::is_any_of ("*"));
						SUB_ASSERT (styles.find(event[i]) != styles.end());
						Style style = styles[event[i]];
						sub.font = style.font_name;
						sub.font_size = FontSize::from_points (style.font_size);
						sub.colour = style.primary_colour;
						sub.effect_colour = style.back_colour;
						sub.bold = style.bold;
						sub.italic = style.italic;
						sub.underline = style.underline;
						sub.effect = style.effect;
						sub.horizontal_position.reference = style.horizontal_reference;
						sub.vertical_position.reference = style.vertical_reference;
						sub.vertical_position.proportional = float(style.vertical_margin) / play_res_y;
					} else if (event_format[i] == "MarginV") {
						sub.vertical_position.proportional = raw_convert<float>(event[i]) / play_res_y;
					} else if (event_format[i] == "Text") {
						BOOST_FOREACH (sub::RawSubtitle j, parse_line (sub, event[i], play_res_x, play_res_y)) {
							_subs.push_back (j);
						}
					}
				}
			}
		}

	}
}
