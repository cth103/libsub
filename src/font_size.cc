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

#include "font_size.h"

using namespace sub;

float
FontSize::proportional (int screen_height_in_points) const
{
	if (_proportional) {
		return _proportional.get ();
	}

	return float (_points.get ()) / screen_height_in_points;
}

int
FontSize::points (int screen_height_in_points) const
{
	if (_points) {
		return _points.get ();
	}

	return _proportional.get() * screen_height_in_points;
}

FontSize
FontSize::from_points (int p)
{
	FontSize s;
	s.set_points (p);
	return s;
}

FontSize
FontSize::from_proportional (float p)
{
	FontSize s;
	s.set_proportional (p);
	return s;
}

bool
FontSize::specified () const
{
	return _proportional || _points;
}

bool
sub::operator== (FontSize const & a, FontSize const & b)
{
	return a.proportional() == b.proportional() && a.points() == b.points();
}
