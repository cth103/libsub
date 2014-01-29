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

#include "vertical_reference.h"
#include "exceptions.h"
#include <string>

using std::string;
using namespace sub;

VerticalReference
sub::string_to_vertical_reference (string s)
{
	if (s == "top") {
		return TOP;
	} else if (s == "center") {
		return CENTRE;
	} else if (s == "bottom") {
		return BOTTOM;
	}
	
	throw XMLError ("unknown subtitle valign type");
}
