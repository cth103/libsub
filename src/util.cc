/*
    Copyright (C) 2012-2020 Carl Hetherington <cth@carlh.net>

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
#include "reader.h"
#include "subtitle.h"
#include "collect.h"
#include <memory>
#include <string>
#include <iostream>
#include <cstdio>
#include <map>

using std::string;
using std::getline;
using std::ostream;
using std::map;
using std::list;
using boost::optional;
using std::shared_ptr;
using namespace sub;

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
sub::get_line_string (string* s)
{
	if (s->length() == 0) {
		return optional<string>();
	}

	size_t pos = s->find ("\n");
	if (pos == string::npos) {
		string const c = *s;
		*s = "";
		return c;
	}

	string const c = s->substr (0, pos);
	s->erase (0, pos + 1);
	return c;
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

void
sub::dump (shared_ptr<const Reader> reader, ostream& os)
{
	map<string, string> metadata = reader->metadata ();
	for (map<string, string>::const_iterator i = metadata.begin(); i != metadata.end(); ++i) {
		os << i->first << ": " << i->second << "\n";
	}

	list<sub::Subtitle> subs = collect<list<sub::Subtitle> > (reader->subtitles ());
	int n = 0;
	for (list<sub::Subtitle>::const_iterator i = subs.begin(); i != subs.end(); ++i) {
		os << "Subtitle " << n << " at " << i->from << " -> " << i->to << "\n";
		for (list<sub::Line>::const_iterator j = i->lines.begin(); j != i->lines.end(); ++j) {

			os << "\t";

			if (j->vertical_position.proportional) {
				os << j->vertical_position.proportional.get() << " of screen";
			} else if (j->vertical_position.line && j->vertical_position.lines) {
				os << j->vertical_position.line.get() << " lines of " << j->vertical_position.lines.get();
			}
			if (j->vertical_position.reference) {
				os << " from ";
				switch (j->vertical_position.reference.get()) {
				case TOP_OF_SCREEN:
					os << "top";
					break;
				case VERTICAL_CENTRE_OF_SCREEN:
					os << "centre";
					break;
				case BOTTOM_OF_SCREEN:
					os << "bottom";
					break;
				case TOP_OF_SUBTITLE:
					os << "top of subtitle";
					break;
				}
			}

			os << "\t";
			bool italic = false;
			bool underline = false;
			for (list<sub::Block>::const_iterator k = j->blocks.begin(); k != j->blocks.end(); ++k) {
				if (k->italic && !italic) {
					os << "<i>";
				} else if (italic && !k->italic) {
					os << "</i>";
				}
				if (k->underline && !underline) {
					os << "<u>";
				} else if (underline && !k->underline) {
					os << "</u>";
				}

				italic = k->italic;
				underline = k->underline;

				os << k->text;
			}

			if (italic) {
				os << "</i>";
			}
			if (underline) {
				os << "</u>";
			}
			os << "\n";
		}

		++n;
	}
}
