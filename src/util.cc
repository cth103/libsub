/*
    Copyright (C) 2012-2015 Carl Hetherington <cth@carlh.net>

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

#include "util.h"
#include <string>
#include <sstream>
#include <iostream>
#include <cstdio>

using std::string;
using std::stringstream;
using std::getline;
using boost::optional;

/** @param s A string.
 *  @return true if the string contains only space, newline or tab characters, or is empty.
 */
bool
sub::empty_or_white_space (string s)
{
	for (size_t i = 0; i < s.length(); ++i) {
		if (s[i] != ' ' && s[i] != '\n' && s[i] != '\t') {
			return false;
		}
	}

	return true;
}

optional<string>
sub::get_line_stringstream (stringstream* str)
{
	if (!str->good ()) {
		return optional<string> ();
	}

	string s;
	getline (*str, s);
	return s;
}

optional<string>
sub::get_line_file (FILE* f)
{
	char buffer[256];
	char* r = fgets (buffer, sizeof (buffer), f);
	if (r == 0) {
		return optional<string> ();
	}

	return string (buffer);
}

void
sub::remove_unicode_bom (optional<string>& line)
{
	if (
		line->length() >= 3 &&
		static_cast<unsigned char> (line.get()[0]) == 0xef &&
		static_cast<unsigned char> (line.get()[1]) == 0xbb &&
		static_cast<unsigned char> (line.get()[2]) == 0xbf
		) {

		/* Skip Unicode byte order mark */
		line = line->substr (3);
	}
}
