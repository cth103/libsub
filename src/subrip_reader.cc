/*
    Copyright (C) 2014-2015 Carl Hetherington <cth@carlh.net>

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

/** @file  src/subrip_reader.cc
 *  @brief SubripReader class.
 */

#include "subrip_reader.h"
#include "exceptions.h"
#include "util.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/bind.hpp>
#include <cstdio>
#include <vector>
#include <iostream>

using std::string;
using std::vector;
using std::list;
using std::cout;
using std::hex;
using boost::lexical_cast;
using boost::to_upper;
using boost::optional;
using boost::function;
using boost::algorithm::replace_all;
using namespace sub;

/** @param s Subtitle string encoded in UTF-8 */
SubripReader::SubripReader (string s)
{
	this->read (boost::bind(&get_line_string, &s));
}

/** @param f Subtitle file encoded in UTF-8 */
SubripReader::SubripReader (FILE* f)
{
	this->read (boost::bind (&get_line_file, f));
}

void
SubripReader::read (function<optional<string> ()> get_line)
{
	enum {
		COUNTER,
		METADATA,
		CONTENT
	} state = COUNTER;

	RawSubtitle rs;

	/* This reader extracts no information about where the subtitle
	   should be on screen, so its reference is TOP_OF_SUBTITLE.
	*/
	rs.vertical_position.line = 0;
	rs.vertical_position.reference = TOP_OF_SUBTITLE;

	while (true) {
		optional<string> line = get_line ();
		if (!line) {
			break;
		}

		trim_right_if (*line, boost::is_any_of ("\n\r"));
		remove_unicode_bom (line);

		/* Keep some history in case there is an error to report */
		_context.push_back (*line);
		if (_context.size() > 5) {
			_context.pop_front ();
		}

		switch (state) {
		case COUNTER:
		{
			if (line->empty ()) {
				/* a blank line at the start is ok */
				break;
			}

			state = METADATA;

			/* Reset stuff that should not persist across separate subtitles */
			rs.bold = false;
			rs.italic = false;
			rs.underline = false;
			rs.vertical_position.line = 0;
		}
		break;
		case METADATA:
		{
			vector<string> p;

			/* Further trim this line, removing spaces from the end */
			trim_right_if (*line, boost::is_any_of (" "));

			boost::algorithm::split (p, *line, boost::algorithm::is_any_of (" "), boost::token_compress_on);
			if (p.size() != 3 && p.size() != 7) {
				for (int i = 0; i < 2; ++i) {
					optional<string> ex = get_line ();
					if (ex) {
						_context.push_back (*ex);
					}
				}
				throw SubripError (*line, "a time/position line", _context);
			}

			rs.from = convert_time (p[0]);
			rs.to = convert_time (p[2]);

			/* XXX: should not ignore coordinate specifications */

			state = CONTENT;
			break;
		}
		case CONTENT:
			if (line->empty ()) {
				state = COUNTER;
			} else {
				convert_line (*line, rs);
				rs.vertical_position.line = rs.vertical_position.line.get() + 1;
			}
			break;
		}
	}
}

Time
SubripReader::convert_time (string t)
{
	vector<string> a;
	boost::algorithm::split (a, t, boost::is_any_of (":"));
	if (a.size() != 3) {
		throw SubripError (t, "time in the format h:m:s,ms", _context);
	}

	vector<string> b;
	boost::algorithm::split (b, a[2], boost::is_any_of (","));
	if (b.size() != 2) {
		throw SubripError (t, "time in the format h:m:s,ms", _context);
	}

	int h, m, s, ms;

	try {
		h = lexical_cast<int>(a[0]);
	} catch (boost::bad_lexical_cast &) {
		throw SubripError (t, "integer hour value", _context);
	}

	try {
		m = lexical_cast<int>(a[1]);
	} catch (boost::bad_lexical_cast &) {
		throw SubripError (t, "integer minute value", _context);
	}

	try {
		s = lexical_cast<int>(b[0]);
	} catch (boost::bad_lexical_cast &) {
		throw SubripError (t, "integer second value", _context);
	}

	try {
		ms = lexical_cast<int>(b[1]);
	} catch (boost::bad_lexical_cast &) {
		throw SubripError (t, "integer millisecond value", _context);
	}

	return Time::from_hms (h, m, s, ms);
}

void
SubripReader::convert_line (string t, RawSubtitle& p)
{
	enum {
		TEXT,
		TAG
	} state = TEXT;

	string tag;

	list<Colour> colours;
	colours.push_back (Colour (1, 1, 1));

	/* XXX: missing <font> support */
	/* XXX: nesting of tags e.g. <b>foo<i>bar<b>baz</b>fred</i>jim</b> might
	   not work, I think.
	*/

	for (size_t i = 0; i < t.size(); ++i) {
		switch (state) {
		case TEXT:
			if (t[i] == '<' || t[i] == '{') {
				state = TAG;
			} else {
				p.text += t[i];
			}
			break;
		case TAG:
			if (t[i] == '>' || t[i] == '}') {
				if (tag == "b") {
					maybe_content (p);
					p.bold = true;
				} else if (tag == "/b") {
					maybe_content (p);
					p.bold = false;
				} else if (tag == "i") {
					maybe_content (p);
					p.italic = true;
				} else if (tag == "/i") {
					maybe_content (p);
					p.italic = false;
				} else if (tag == "u") {
					maybe_content (p);
					p.underline = true;
				} else if (tag == "/u") {
					maybe_content (p);
					p.underline = false;
				} else if (boost::starts_with (tag, "font")) {
					maybe_content (p);
					boost::regex re (".*color=\"#([0123456789abcdef]+)\"");
					boost::smatch match;
					if (boost::regex_search (tag, match, re) && string (match[1]).size() == 6) {
						p.colour = Colour::from_rgb_hex (match[1]);
						colours.push_back (p.colour);
					}
				} else if (tag == "/font") {
					maybe_content (p);
					colours.pop_back ();
					p.colour = colours.back ();
				}
				tag.clear ();
				state = TEXT;
			} else {
				tag += tolower (t[i]);
			}
			break;
		}
	}

	/* Strip Unicode U+202B (right-to-left embedding) as sometimes it is rendered
	   as a missing character.  This may be a hack.
	*/
	replace_all (p.text, "\xe2\x80\xab", "");

	maybe_content (p);
}

/* Push p into _subs if it has some text, and clear the text out of p */
void
SubripReader::maybe_content (RawSubtitle& p)
{
	if (!p.text.empty ()) {
		_subs.push_back (p);
		p.text.clear ();
	}
}
