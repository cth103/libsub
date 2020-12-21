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

#ifndef LIBSUB_RAW_CONVERT_H
#define LIBSUB_RAW_CONVERT_H

#include "util.h"
#include <boost/static_assert.hpp>
#include <iomanip>

namespace sub {

/** A sort-of version of boost::lexical_cast that does uses the "C"
 *  locale (i.e. no thousands separators and a . for the decimal separator).
 */
template <typename P, typename Q>
P
raw_convert (Q, int precision = 16)
{
	/* We can't write a generic version of raw_convert; all required
	   versions must be specialised.
	*/
	BOOST_STATIC_ASSERT (sizeof (Q) == 0);
	LIBSUB_UNUSED (precision);
}

template <>
int
raw_convert (std::string v, int);

template <>
float
raw_convert (std::string v, int);

template <>
std::string
raw_convert (unsigned long v, int);

};

#endif
