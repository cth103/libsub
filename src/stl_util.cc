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

#include "stl_util.h"
#include "exceptions.h"
#include "compose.hpp"
#include <string>
#include <cmath>

using std::string;
using namespace sub;

bool
about_equal (float a, float b)
{
	return fabs (a - b) < 1e-4;
}

string
sub::stl_frame_rate_to_dfc (float r)
{
	/* As requested by TJ in January 2018, and as apparently used by Annotation Edit in recent versions */
	if (about_equal (r, 23.976)) {
		return "STL23.01";
	} else if (about_equal (r, 24)) {
		return "STL24.01";
	} else if (about_equal (r, 25)) {
		return "STL25.01";
	} else if (about_equal (r, 30)) {
		return "STL30.01";
	}

	return "STL25.01";
}

float
sub::stl_dfc_to_frame_rate (string s)
{
	if (s == "STL23.01") {
		return 23.976;
	} else if (s == "STL24.01") {
		return 24;
	} else if (s == "STL25.01") {
		return 25;
	} else if (s == "STL30.01") {
		return 30;
	}

	throw STLError (String::compose ("Unknown disk format code %1 in binary STL file", s));
}
