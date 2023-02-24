/*
    Copyright (C) 2014-2020 Carl Hetherington <cth@carlh.net>

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


#include "compose.hpp"
#include "exceptions.h"
#include "raw_convert.h"
#include "ssa_reader.h"
#include "sub_assert.h"
#include "subrip_reader.h"
#include "util.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string_regex.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <cstdio>
#include <iostream>
#include <vector>


using std::string;
using std::vector;
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

	rs.vertical_position.line = 0;
	rs.vertical_position.reference = TOP_OF_SUBTITLE;

	while (true) {
		auto line = get_line ();
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
			rs.vertical_position.reference = TOP_OF_SUBTITLE;
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

			string expected;
			auto from = convert_time(p[0], ",", &expected);
			if (!from) {
				throw SubripError(p[0], expected, _context);
			}
			rs.from = *from;

			auto to = convert_time(p[2], ",", &expected);
			if (!to) {
				throw SubripError(p[2], expected, _context);
			}
			rs.to = *to;

			/* XXX: should not ignore coordinate specifications */

			state = CONTENT;
			break;
		}
		case CONTENT:
			if (line->empty ()) {
				state = COUNTER;
			} else {
				vector<string> sub_lines;
				/* Split up this line on unicode "LINE SEPARATOR".  This feels hacky but also
				 * the least unpleasant place to do it.
				 */
				boost::algorithm::split_regex(sub_lines, *line, boost::regex("\xe2\x80\xa8"));
				for (auto sub_line: sub_lines) {
					convert_line(sub_line, rs);
					rs.vertical_position.line = rs.vertical_position.line.get() + 1;
					rs.text.clear();
				}
			}
			break;
		}
	}
}

optional<Time>
SubripReader::convert_time(string t, string milliseconds_separator, string* expected)
{
	auto report_expected = [expected](string const& s) {
		if (expected) {
			*expected = s;
		}
	};

	vector<string> a;
	boost::algorithm::split (a, t, boost::is_any_of (":"));
	if (a.size() != 3) {
		report_expected("time in the format h:m:s,ms");
		return {};
	}

	vector<string> b;
	boost::algorithm::split(b, a[2], boost::is_any_of(milliseconds_separator));
	if (b.size() != 2) {
		report_expected(String::compose("time in the format h:m:s%1ms", milliseconds_separator));
		return {};
	}

	int h, m, s, ms;

	try {
		h = lexical_cast<int>(a[0]);
	} catch (boost::bad_lexical_cast &) {
		report_expected("integer hour value");
		return {};
	}

	try {
		m = lexical_cast<int>(a[1]);
	} catch (boost::bad_lexical_cast &) {
		report_expected("integer minute value");
		return {};
	}

	try {
		s = lexical_cast<int>(b[0]);
	} catch (boost::bad_lexical_cast &) {
		report_expected("integer second value");
		return {};
	}

	try {
		ms = lexical_cast<int>(b[1]);
	} catch (boost::bad_lexical_cast &) {
		report_expected("integer millisecond value");
		return {};
	}

	return Time::from_hms (h, m, s, ms);
}

void
SubripReader::convert_line (string t, RawSubtitle& p)
{
	vector<Colour> colours;
	colours.push_back (Colour (1, 1, 1));

	auto has_next = [](string line, size_t& index, string s) {
		boost::to_lower(s);
		auto next = line.substr(index, s.size());
		boost::to_lower(next);
		if (next != s) {
			return false;
		}

		index += s.size();
		return true;
	};

	size_t i = 0;
	while (i < t.size()) {
		if (has_next(t, i, "<b>") || has_next(t, i, "{b}")) {
			maybe_content (p);
			p.bold = true;
		} else if (has_next(t, i, "</b>") || has_next(t, i, "{/b}")) {
			maybe_content (p);
			p.bold = false;
		} else if (has_next(t, i, "<i>") || has_next(t, i, "{i}")) {
			maybe_content (p);
			p.italic = true;
		} else if (has_next(t, i, "</i>") || has_next(t, i, "{/i}")) {
			maybe_content (p);
			p.italic = false;
		} else if (has_next(t, i, "<u>") || has_next(t, i, "{u}")) {
			maybe_content (p);
			p.underline = true;
		} else if (has_next(t, i, "</u>") || has_next(t, i, "{/u}")) {
			maybe_content (p);
			p.underline = false;
		} else if (has_next(t, i, "<font") || has_next(t, i, "<Font")) {
			maybe_content (p);
			boost::regex re (".*color=\"?#([[:xdigit:]]+)\"?");
			boost::smatch match;
			string tag;
			while (i < t.size() && t[i] != '>') {
				tag += t[i];
				++i;
			}
			++i;
			if (boost::regex_search(tag, match, re)) {
				if (string(match[1]).size() == 6) {
					p.colour = Colour::from_rgb_hex(match[1]);
					colours.push_back(p.colour);
				} else if (string(match[1]).size() == 8) {
					p.colour = Colour::from_rgba_hex(match[1]);
					colours.push_back(p.colour);
				} else {
					throw SubripError(tag, "a colour in the format #rrggbb #rrggbbaa or rgba(rr,gg,bb,aa)", _context);
				}
			} else {
				re = boost::regex (
					".*color=\"rgba\\("
					"[[:space:]]*([[:digit:]]+)[[:space:]]*,"
					"[[:space:]]*([[:digit:]]+)[[:space:]]*,"
					"[[:space:]]*([[:digit:]]+)[[:space:]]*,"
					"[[:space:]]*([[:digit:]]+)[[:space:]]*"
					"\\)\""
					);
				if (boost::regex_search (tag, match, re) && match.size() == 5) {
					p.colour.r = raw_convert<int>(string(match[1])) / 255.0;
					p.colour.g = raw_convert<int>(string(match[2])) / 255.0;
					p.colour.b = raw_convert<int>(string(match[3])) / 255.0;
					colours.push_back (p.colour);
				} else {
					throw SubripError (tag, "a colour in the format #rrggbb #rrggbbaa or rgba(rr,gg,bb,aa)", _context);
				}
			}
		} else if (has_next(t, i, "</font>")) {
			maybe_content (p);
			SUB_ASSERT (!colours.empty());
			colours.pop_back ();
			p.colour = colours.back ();
		} else if (has_next(t, i, "{\\")) {
			string ssa = "\\";
			while (i < t.size() && t[i] != '}') {
				ssa += t[i];
				++i;
			}
			++i;
			SSAReader::parse_style (p, ssa, 288, 288, Colour(1, 1, 1));
		} else {
			p.text += t[i];
			++i;
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
