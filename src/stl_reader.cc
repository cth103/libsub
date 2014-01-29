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

#include "stl_reader.h"
#include "compose.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>

using std::list;
using std::ostream;
using std::istream;
using std::string;
using std::vector;
using std::cout;
using boost::algorithm::trim;
using boost::algorithm::starts_with;
using boost::is_any_of;
using boost::optional;
using boost::lexical_cast;
using namespace sub;

STLReader::STLReader (istream& in)
{
	while (in.good ()) {
		string line;
		getline (in, line);
		if (!in.good ()) {
			return;
		}

		trim (line);

		if (starts_with (line, "//")) {
			continue;
		}

		if (line.size() > 0 && line[0] == '$') {
			/* $ variables */
			vector<string> bits;
			split (bits, line, is_any_of ("="));
			if (bits.size() == 2) {
				string name = bits[0];
				trim (name);
				string value = bits[1];
				trim (value);

				set (name, value);
			} else {
				warn (String::compose ("Unrecognised line %1", line));
			}
		} else {
			/* "Normal" lines */
			size_t divider[2];
			divider[0] = line.find_first_of (",");
			if (divider[0] != string::npos) {
				divider[1] = line.find_first_of (",", divider[0] + 1);
			}
			
			if (divider[0] == string::npos || divider[1] == string::npos || divider[0] <= 1 || divider[1] >= line.length() - 1) {
				warn (String::compose ("Unrecognised line %1", line));
				continue;
			}

			string from_string = line.substr (0, divider[0] - 1);
			trim (from_string);
			string to_string = line.substr (divider[0] + 1, divider[1] - divider[0] - 1);
			trim (to_string);

			optional<FrameTime> from = time (from_string);
			optional<FrameTime> to = time (to_string);

			if (!from || !to) {
				warn (String::compose ("Unrecognised line %1", line));
				continue;
			}

			_current.from.frame = from.get ();
			_current.to.frame = to.get ();

			/* Parse ^B/^I/^U */
			string text = line.substr (divider[1] + 1);
			for (size_t i = 0; i < text.length(); ++i) {
				if (text[i] == '|') {
					maybe_push ();
					_current.line++;
				} else if (text[i] == '^') {
					maybe_push ();
					if ((i + 1) < text.length()) {
						switch (text[i + 1]) {
						case 'B':
							_current.bold = !_current.bold;
							break;
						case 'I':
							_current.italic = !_current.italic;
							break;
						case 'U':
							_current.underline = !_current.underline;
							break;
						}
					}
					++i;
				} else {
					_current.text += text[i];
				}
			}

			maybe_push ();
			_current.line = 0;
		}
	}
}

optional<FrameTime>
STLReader::time (string t) const
{
	vector<string> b;
	split (b, t, is_any_of (":"));
	if (b.size() != 4) {
		warn (String::compose ("Unrecognised time %1", t));
		return optional<FrameTime> ();
	}

	return FrameTime (lexical_cast<int> (b[0]), lexical_cast<int> (b[1]), lexical_cast<int> (b[2]), lexical_cast<int> (b[3]));
}

void
STLReader::set (string name, string value)
{
	if (name == "$FontName") {
		_current.font = value;
	} else if (name == "$Bold") {
		_current.bold = value == "True";
	} else if (name == "$Italic") {
		_current.italic = value == "True";
	} else if (name == "$Underlined") {
		_current.underline = value == "True";
	} else if (name == "$FontSize") {
		_current.font_size.points = lexical_cast<int> (value);
	}
}

void
STLReader::maybe_push ()
{
	if (!_current.text.empty ()) {
		_subs.push_back (_current);
		_current.text.clear ();
	}
}
	
