/*
    Copyright (C) 2014-2018 Carl Hetherington <cth@carlh.net>

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

/** @file  src/colour.h
 *  @brief Colour class.
 */

#ifndef LIBSUB_COLOUR_H
#define LIBSUB_COLOUR_H

#include <string>
#include <cmath>

namespace sub {

/** @class Colour
 *  @brief An RGB colour.
 */
class Colour
{
public:
	Colour ()
		: r (0)
		, g (0)
		, b (0)
	{}

	Colour (float r, float g, float b)
		: r (r)
		, g (g)
		, b (b)
	{}

	static Colour from_rgba_hex(std::string);
	static Colour from_rgb_hex (std::string);

	/** red component (from 0 to 1) */
	float r;
	/** green component (from 0 to 1) */
	float g;
	/** blue component (from 0 to 1) */
	float b;
};

bool
operator== (Colour const & a, Colour const & b);

}

#endif
