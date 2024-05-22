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
#include <boost/bind/bind.hpp>
#include <cstdlib>
#include <iostream>
#include <vector>

using std::string;
using std::vector;
using std::map;
using std::cout;
using boost::optional;
using boost::function;
using namespace boost::algorithm;
#if BOOST_VERSION >= 106100
using namespace boost::placeholders;
#endif
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

Colour
h_colour (string s)
{
	if (s.empty() || s[0] != '&' || s[1] != 'H') {
		throw SSAError(String::compose("Badly formatted colour tag %1", s));
	}

	auto start = s.c_str();
	auto const end = start + s.length();
	while (start < end && (*start == '&' || *start == 'H')) {
		++start;
	}

	auto const colour = strtoll(start, nullptr, 16);

	/* XXX: ignoring alpha channel here; note that 00 is opaque and FF is transparent */
	return sub::Colour(
		((colour & 0x000000ff) >> 0) / 255.0,
		((colour & 0x0000ff00) >> 8) / 255.0,
		((colour & 0x00ff0000) >> 16) / 255.0
		);
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
				primary_colour = colour (style[i]);
			} else if (keys[i] == "BackColour") {
				back_colour = colour (style[i]);
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
				if (style[i] == "7" || style[i] == "8" || style[i] == "9") {
					vertical_reference = TOP_OF_SCREEN;
				} else if (style[i] == "4" || style[i] == "5" || style[i] == "6") {
					vertical_reference = VERTICAL_CENTRE_OF_SCREEN;
				} else {
					vertical_reference = BOTTOM_OF_SCREEN;
				}
				if (style[i] == "1" || style[i] == "4" || style[i] == "7") {
					horizontal_reference = LEFT_OF_SCREEN;
				} else if (style[i] == "3" || style[i] == "6" || style[i] == "9") {
					horizontal_reference = RIGHT_OF_SCREEN;
				} else {
					horizontal_reference = HORIZONTAL_CENTRE_OF_SCREEN;
				}
			} else if (keys[i] == "MarginV") {
				vertical_margin = raw_convert<int> (style[i]);
			}
		}
	}

	string name;
	optional<string> font_name;
	int font_size; ///< points
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
	Colour colour (string c) const
	{
		if (c.length() > 0 && c[0] == '&') {
			/* &Hbbggrr or &Haabbggrr */
			return h_colour (c);
		} else {
			/* integer */
			int i = raw_convert<int>(c);
			return Colour (
				((i & 0x0000ff) >>  0) / 255.0,
				((i & 0x00ff00) >>  8) / 255.0,
				((i & 0xff0000) >> 16) / 255.0
				);
		}
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

void
SSAReader::parse_tag(RawSubtitle& sub, string tag, int play_res_x, int play_res_y, Colour primary_colour)
{
	if (tag == "\\i1") {
		sub.italic = true;
	} else if (tag == "\\i0" || tag == "\\i") {
		sub.italic = false;
	} else if (tag == "\\b1") {
		sub.bold = true;
	} else if (tag == "\\b0") {
		sub.bold = false;
	} else if (tag == "\\u1") {
		sub.underline = true;
	} else if (tag == "\\u0") {
		sub.underline = false;
	} else if (tag == "\\an1") {
		sub.horizontal_position.reference = sub::LEFT_OF_SCREEN;
		sub.vertical_position.reference = sub::BOTTOM_OF_SCREEN;
	} else if (tag == "\\an2") {
		sub.horizontal_position.reference = sub::HORIZONTAL_CENTRE_OF_SCREEN;
		sub.vertical_position.reference = sub::BOTTOM_OF_SCREEN;
	} else if (tag == "\\an3") {
		sub.horizontal_position.reference = sub::RIGHT_OF_SCREEN;
		sub.vertical_position.reference = sub::BOTTOM_OF_SCREEN;
	} else if (tag == "\\an4") {
		sub.horizontal_position.reference = sub::LEFT_OF_SCREEN;
		sub.vertical_position.reference = sub::VERTICAL_CENTRE_OF_SCREEN;
	} else if (tag == "\\an5") {
		sub.horizontal_position.reference = sub::HORIZONTAL_CENTRE_OF_SCREEN;
		sub.vertical_position.reference = sub::VERTICAL_CENTRE_OF_SCREEN;
	} else if (tag == "\\an6") {
		sub.horizontal_position.reference = sub::RIGHT_OF_SCREEN;
		sub.vertical_position.reference = sub::VERTICAL_CENTRE_OF_SCREEN;
	} else if (tag == "\\an7") {
		sub.horizontal_position.reference = sub::LEFT_OF_SCREEN;
		sub.vertical_position.reference = sub::TOP_OF_SCREEN;
	} else if (tag == "\\an8") {
		sub.horizontal_position.reference = sub::HORIZONTAL_CENTRE_OF_SCREEN;
		sub.vertical_position.reference = sub::TOP_OF_SCREEN;
	} else if (tag == "\\an9") {
		sub.horizontal_position.reference = sub::RIGHT_OF_SCREEN;
		sub.vertical_position.reference = sub::TOP_OF_SCREEN;
	} else if (boost::starts_with(tag, "\\pos")) {
		vector<string> bits;
		boost::algorithm::split (bits, tag, boost::is_any_of("(,"));
		SUB_ASSERT (bits.size() == 3);
		sub.horizontal_position.reference = sub::LEFT_OF_SCREEN;
		sub.horizontal_position.proportional = raw_convert<float>(bits[1]) / play_res_x;
		sub.vertical_position.reference = sub::TOP_OF_SCREEN;
		sub.vertical_position.proportional = raw_convert<float>(bits[2]) / play_res_y;
	} else if (boost::starts_with(tag, "\\fs")) {
		SUB_ASSERT (tag.length() > 3);
		sub.font_size.set_proportional(raw_convert<float>(tag.substr(3)) / play_res_y);
	} else if (boost::starts_with(tag, "\\c")) {
		/* \c&Hbbggrr& */
		if (tag.length() > 2) {
			sub.colour = h_colour(tag.substr(2, tag.length() - 3));
		} else if (tag.length() == 2) {
			sub.colour = primary_colour;
		} else {
			throw SSAError(String::compose("Badly formatted colour tag %1", tag));
		}
	}
}

/** @param base RawSubtitle filled in with any required common values.
 *  @param line SSA line string (i.e. just the subtitle, possibly with embedded stuff)
 *  @return List of RawSubtitles to represent line with vertical reference TOP_OF_SUBTITLE.
 */
vector<RawSubtitle>
SSAReader::parse_line(RawSubtitle base, string line, int play_res_x, int play_res_y, Colour primary_colour)
{
	enum {
		TEXT,
		TAG,
		BACKSLASH
	} state = TEXT;

	vector<RawSubtitle> subs;
	RawSubtitle current = base;
	string tag;

	if (!current.vertical_position.reference) {
		current.vertical_position.reference = BOTTOM_OF_SCREEN;
	}

	/* Any vertical_position that is set in base (and therefore current) is a margin, which
	 * we need to ignore if we end up vertically centering this subtitle.
	 * Clear out vertical_position from current; we'll re-add it from base later
	 * if required.
	 */
	current.vertical_position.proportional = 0;

	/* We must have a font size, as there could be a margin specified
	   in pixels and in that case we must know how big the subtitle
	   lines are to work out the position on screen.
	*/
	if (!current.font_size.proportional()) {
		current.font_size.set_proportional(72.0 / play_res_y);
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

	/* There are vague indications that with ASS 1 point should equal 1 pixel */
	double const line_size = current.font_size.proportional(play_res_y) * 1.2;

	for (size_t i = 0; i < line.length(); ++i) {
		char const c = line[i];
		switch (state) {
		case TEXT:
			if (c == '{') {
				state = TAG;
			} else if (c == '\\') {
				state = BACKSLASH;
			} else if (c != '\r' && c != '\n') {
				current.text += c;
			}
			break;
		case TAG:
			if (c == '}' || c == '\\') {
				if (!current.text.empty ()) {
					subs.push_back (current);
					current.text = "";
				}
				parse_tag(current, tag, play_res_x, play_res_y, primary_colour);
				tag = "";
			}

			if (c == '}') {
				state = TEXT;
			} else {
				tag += c;
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

	/* Now we definitely know the vertical position reference we can finish off the position */
	for (auto& sub: subs) {
		switch (sub.vertical_position.reference.get()) {
		case TOP_OF_SCREEN:
		case TOP_OF_SUBTITLE:
			/* Just re-add any margins we came in with */
			sub.vertical_position.proportional = sub.vertical_position.proportional.get() + base.vertical_position.proportional.get_value_or(0);
			break;
		case VERTICAL_CENTRE_OF_SCREEN:
			/* Margins are ignored, but we need to centre */
			sub.vertical_position.proportional = sub.vertical_position.proportional.get() - ((line_breaks + 1) * line_size) / 2;
			break;
		case BOTTOM_OF_SCREEN:
			/* Re-add margins and account for each line */
			sub.vertical_position.proportional =
				sub.vertical_position.proportional.get()
				+ base.vertical_position.proportional.get_value_or(0)
				+ line_breaks * line_size;
			break;
		}
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
				for (auto& i: event_format) {
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
				optional<Style> style;

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
						/* Use the specified style unless it's not defined, in which case use
						 * "Default" (if it exists).
						 */
						if (styles.find(event[i]) != styles.end()) {
							style = styles[event[i]];
						} else if (styles.find("Default") != styles.end()) {
							style = styles["Default"];
						} else {
							continue;
						}
						sub.font = style->font_name;
						sub.font_size = FontSize::from_proportional(static_cast<float>(style->font_size) / play_res_y);
						sub.colour = style->primary_colour;
						sub.effect_colour = style->back_colour;
						sub.bold = style->bold;
						sub.italic = style->italic;
						sub.underline = style->underline;
						sub.effect = style->effect;
						sub.horizontal_position.reference = style->horizontal_reference;
						sub.vertical_position.reference = style->vertical_reference;
						if (sub.vertical_position.reference != sub::VERTICAL_CENTRE_OF_SCREEN) {
							sub.vertical_position.proportional = float(style->vertical_margin) / play_res_y;
						}
					} else if (event_format[i] == "MarginV") {
						if (event[i] != "0" && sub.vertical_position.reference != sub::VERTICAL_CENTRE_OF_SCREEN) {
							/* Override the style if its non-zero */
							sub.vertical_position.proportional = raw_convert<float>(event[i]) / play_res_y;
						}
					} else if (event_format[i] == "Text") {
						for (auto j: parse_line(sub, event[i], play_res_x, play_res_y, style ? style->primary_colour : Colour(1, 1, 1))) {
							_subs.push_back (j);
						}
					}
				}
			}
		}

	}
}
