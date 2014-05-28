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

#include <string>
#include <boost/optional.hpp>
#include "iso6937_tables.h"
#include "iso6937.h"

using std::string;
using std::cout;
using std::wstring;
using namespace sub;

wstring
sub::iso6937_to_utf16 (char const * s)
{
	if (iso6937::grave.empty ()) {
		make_iso6937_tables ();
	}
	
	wstring o;

	boost::optional<unsigned char> diacritical;

	while (*s != '\0') {
		unsigned char const u = static_cast<unsigned char> (*s);
		if (u >= 0xc1 && u <= 0xcf) {
			diacritical = u;
		} else if (diacritical) {
			switch (diacritical.get ()) {
			case 0xC1:
				o += iso6937::grave[u];
				break;
			case 0xC2:
				o += iso6937::acute[u];
				break;
			case 0xC3:
				o += iso6937::circumflex[u];
				break;
			case 0xC4:
				o += iso6937::tilde[u];
				break;
			case 0xC5:
				o += iso6937::macron[u];
				break;
			case 0xC6:
				o += iso6937::breve[u];
				break;
			case 0xC7:
				o += iso6937::dot[u];
				break;
			case 0xC8:
				o += iso6937::diaeresis[u];
				break;
			case 0xCA:
				o += iso6937::ring[u];
				break;
			case 0xCB:
				o += iso6937::cedilla[u];
				break;
			case 0xCD:
				o += iso6937::double_acute[u];
				break;
			case 0xCE:
				o += iso6937::ogonek[u];
				break;
			case 0xCF:
				o += iso6937::caron[u];
				break;
			}

			diacritical.reset ();
		} else {
			o += iso6937::main[u];
		}

		++s;
	}

	return o;
}
