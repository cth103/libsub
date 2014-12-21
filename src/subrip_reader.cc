/*
    Copyright (C) 2014 Carl Hetherington <cth@carlh.net>

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

#include "subrip_reader.h"
#include "exceptions.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <cstdio>
#include <vector>

using std::string;
using std::vector;
using boost::lexical_cast;
using namespace sub;

SubripReader::SubripReader (FILE* f)
{
	enum {
		COUNTER,
		METADATA,
		CONTENT
	} state = COUNTER;

	char buffer[256];

	TimePair from;
	TimePair to;

	string line;
	int line_number = 0;

	while (!feof (f)) {
		char* r = fgets (buffer, sizeof (buffer), f);
		if (r == 0 || feof (f)) {
			break;
		}

		line = string (buffer);
		trim_right_if (line, boost::is_any_of ("\n\r"));

		switch (state) {
		case COUNTER:
		{
			if (line.empty ()) {
				/* a blank line at the start is ok */
				break;
			}

			state = METADATA;
		}
		break;
		case METADATA:
		{
			vector<string> p;
			boost::algorithm::split (p, line, boost::algorithm::is_any_of (" "));
			if (p.size() != 3 && p.size() != 7) {
				throw SubripError (line, "a time/position line");
			}

			from = convert_time (p[0]);
			to = convert_time (p[2]);

			/* XXX: should not ignore coordinate specifications */
			
			state = CONTENT;
			break;
		}
		case CONTENT:
			if (line.empty ()) {
				/* XXX: I think this line_number should be set to some sensible value and TOP_OF_SUBTITLE
				   should not be used.
				*/
				state = COUNTER;
				line_number = 0;
			} else {
				convert_line (line, line_number, from, to);
				line_number++;
			}
			break;
		}
	}
}

TimePair
SubripReader::convert_time (string t)
{
	vector<string> a;
	boost::algorithm::split (a, t, boost::is_any_of (":"));
	if (a.size() != 3) {
		throw SubripError (t, "time in the format h:m:s,ms");
	}

	vector<string> b;
	boost::algorithm::split (b, a[2], boost::is_any_of (","));

	return TimePair (
		MetricTime (
			lexical_cast<int> (a[0]),
			lexical_cast<int> (a[1]),
			lexical_cast<int> (b[0]),
			lexical_cast<int> (b[1])
			)
		);
}

void
SubripReader::convert_line (string t, int line_number, TimePair from, TimePair to)
{
	enum {
		TEXT,
		TAG
	} state = TEXT;
	
	string tag;

	RawSubtitle p;
	p.font = "Arial";
	p.font_size.set_points (48);
	p.from = from;
	p.to = to;
	p.vertical_position.line = line_number;
	p.vertical_position.reference = TOP_OF_SUBTITLE;
	
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
				}
				tag.clear ();
				state = TEXT;
			} else {
				tag += t[i];
			}
			break;
		}
	}

	maybe_content (p);
}

void
SubripReader::maybe_content (RawSubtitle& p)
{
	if (!p.text.empty ()) {
		_subs.push_back (p);
		p.text.clear ();
	}
}
