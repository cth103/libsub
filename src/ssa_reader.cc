/*
    Copyright (C) 2016 Carl Hetherington <cth@carlh.net>

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
#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <sstream>
#include <iostream>
#include <vector>

using std::string;
using std::stringstream;
using std::vector;
using std::map;
using std::cout;
using std::list;
using boost::optional;
using boost::function;
using namespace boost::algorithm;
using namespace sub;

/** @param s Subtitle string encoded in UTF-8 */
SSAReader::SSAReader (string const & s)
{
	stringstream str (s);
	this->read (boost::bind (&get_line_stringstream, &str));
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
		: font_size (24)
		, primary_colour (255, 255, 255)
		, bold (false)
		, italic (false)
	{}

	Style (string format_line, string style_line)
		: font_size (24)
		, primary_colour (255, 255, 255)
		, bold (false)
		, italic (false)
	{
		vector<string> keys;
		split (keys, format_line, is_any_of (","));
		vector<string> style;
		split (style, style_line, is_any_of (","));

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
			} else if (keys[i] == "BorderStyle") {
				if (style[i] == "1") {
					effect = SHADOW;
				}
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
	optional<Effect> effect;

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
 *  @param line SSA line string.
 *  @return List of RawSubtitles to represent line with vertical reference TOP_OF_SUBTITLE.
 */
list<RawSubtitle>
SSAReader::parse_line (RawSubtitle base, string line)
{
	enum {
		TEXT,
		STYLE,
		BACKSLASH
	} state = TEXT;

	list<RawSubtitle> subs;
	RawSubtitle current = base;
	string style;

	current.vertical_position.line = 0;
	/* XXX: arbitrary */
	current.vertical_position.lines = 32;
	current.vertical_position.reference = TOP_OF_SUBTITLE;

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
			if (c == '}') {
				if (!current.text.empty ()) {
					subs.push_back (current);
					current.text = "";
				}
				if (style == "i1") {
					current.italic = true;
				} else if (style == "i0") {
					current.italic = false;
				}
				style = "";
				state = TEXT;
			} else {
				style += c;
			}
			break;
		case BACKSLASH:
			if ((c == 'n' || c == 'N') && !current.text.empty ()) {
				subs.push_back (current);
				current.text = "";
				current.vertical_position.line = current.vertical_position.line.get() + 1;
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

	map<string, Style> styles;
	string style_format_line;
	vector<string> event_format;

	while (true) {
		optional<string> line = get_line ();
		if (!line) {
			break;
		}

		trim (*line);

		if (starts_with (*line, ";") || line->empty ()) {
			continue;
		}

		if (starts_with (*line, "[")) {
			/* Section heading */
			if (line.get() == "[Script Info]") {
				part = INFO;
			} else if (line.get() == "[V4 Styles]") {
				part = STYLES;
			} else if (line.get() == "[Events]") {
				part = EVENTS;
			}
			continue;
		}

		size_t const colon = line->find (":");
		SUB_ASSERT (colon != string::npos);
		SUB_ASSERT (line->length() > colon + 1);
		string const type = line->substr (0, colon);
		string const body = line->substr (colon + 2);

		switch (part) {
		case INFO:
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
						SUB_ASSERT (styles.find(event[i]) != styles.end());
						Style style = styles[event[i]];
						sub.font = style.font_name;
						sub.font_size = FontSize::from_points (style.font_size);
						sub.colour = style.primary_colour;
						sub.effect_colour = style.back_colour;
						sub.bold = style.bold;
						sub.italic = style.italic;
						sub.effect = style.effect;

						/* XXX: arbitrary */
						sub.vertical_position.lines = 32;
						sub.vertical_position.reference = TOP_OF_SUBTITLE;
						sub.vertical_position.line = 0;

					} else if (event_format[i] == "Text") {
						sub.text = event[i];
					}
				}

				_subs.push_back (sub);
			}
		}

	}
}
