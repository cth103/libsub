/*
    Copyright (C) 2014-2019 Carl Hetherington <cth@carlh.net>

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
#include <boost/static_assert.hpp>
#include <string>

namespace sub {

template <typename P, typename Q>
P
locale_convert (Q, int precision = 16, bool fixed = false)
{
	/* We can't write a generic version of locale_convert; all required
	   versions must be specialised.
	*/
	BOOST_STATIC_ASSERT (sizeof (Q) == 0);
	LIBSUB_UNUSED(precision);
	LIBSUB_UNUSED(fixed);
}

template <>
std::string
locale_convert (int x, int, bool);

template <>
std::string
locale_convert (long int x, int, bool);

template <>
std::string
locale_convert (unsigned long int x, int, bool);

template <>
std::string
locale_convert (unsigned long long x, int, bool);

template <>
std::string
locale_convert (std::string x, int, bool);

template <>
int
locale_convert (std::string x, int, bool);

template <>
float
locale_convert (std::string x, int, bool);

}
