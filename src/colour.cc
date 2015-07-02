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

#include "colour.h"
#include "exceptions.h"
#include <string>
#include <cstdio>

using std::string;
using namespace sub;

Colour
Colour::from_argb_hex (string argb_hex)
{
	int alpha, ir, ig, ib;
	if (sscanf (argb_hex.c_str(), "%2x%2x%2x%2x", &alpha, &ir, &ig, &ib) < 4) {
		throw XMLError ("could not parse colour string");
	}

	return Colour (float (ir) / 255, float (ig) / 255, float (ib) / 255);
}

Colour
Colour::from_rgb_hex (string rgb_hex)
{
	int ir, ig, ib;
	if (sscanf (rgb_hex.c_str(), "%2x%2x%2x", &ir, &ig, &ib) < 3) {
		throw XMLError ("could not parse colour string");
	}

	return Colour (float (ir) / 255, float (ig) / 255, float (ib) / 255);
}

bool
sub::operator== (Colour const & a, Colour const & b)
{
	return a.r == b.r && a.g == b.g && a.b == b.b;
}

