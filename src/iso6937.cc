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

#include "iso6937_tables.h"
#include "iso6937.h"
#include <boost/optional.hpp>
#include <boost/locale.hpp>
#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::wcout;
using std::wstring;
using std::map;
using boost::optional;
using boost::locale::conv::utf_to_utf;
using namespace sub;

wstring
sub::iso6937_to_utf16 (string s)
{
	if (iso6937::diacriticals.empty ()) {
		make_iso6937_tables ();
	}

	wstring o;

	boost::optional<unsigned char> diacritical;

	int i = 0;
	while (s[i] != '\0') {
		unsigned char const u = static_cast<unsigned char> (s[i]);
		if (u >= 0xc1 && u <= 0xcf) {
			diacritical = u;
		} else if (diacritical) {
			o += (*iso6937::diacriticals[diacritical.get()])[u];
			diacritical.reset ();
		} else {
			o += iso6937::main[u];
		}

		++i;
	}

	return o;
}

static optional<char>
find (map<char, wchar_t> const & m, wchar_t c)
{
	for (map<char, wchar_t>::const_iterator i = m.begin(); i != m.end(); ++i) {
		if (i->second == c) {
			return i->first;
		}
	}

	return optional<char> ();
}

string
sub::utf16_to_iso6937 (wstring s)
{
	if (iso6937::diacriticals.empty ()) {
		make_iso6937_tables ();
	}

	/* XXX: slow */

	string o;
	for (size_t i = 0; i < s.size(); ++i) {
		optional<char> c = find (iso6937::main, s[i]);
		if (c) {
			o += c.get ();
		} else {
			for (map<char, map<char, wchar_t> *>::const_iterator j = iso6937::diacriticals.begin(); j != iso6937::diacriticals.end(); ++j) {
				c = find (*(j->second), s[i]);
				if (c) {
					o += j->first;
					o += c.get ();
					break;
				}
			}
		}

		if (s[i] == 0x201e) {
			/* ISO6397 does not support German (lower) quotation mark (UTF 0x201e) so use
			   a normal opening one (0x201c, which is 170 in ISO6937).
			*/
			o += (char) 170;
		} else if (s[i] == 0x2013 || s[i] == 0x2014) {
			/* ISO6397 does not support en- or em-dashes, so use a horizontal bar (0x2015,
			   which is 208 in ISO6937).
			*/
			o += (char) 208;
		} else if (s[i] == 0x2010 || s[i] == 0x2011 || s[i] == 0x2012) {
			/* Similar story with hyphen, non-breaking hyphen, figure dash */
			o += '-';
		} else if (s[i] == 0x2032) {
			/* And prime */
			o += '\'';
		}
	}

	return o;
}
