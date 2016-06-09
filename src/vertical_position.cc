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

#include "vertical_position.h"

using namespace sub;


float
VerticalPosition::fraction_from_screen_top () const
{
	if (!reference || (!proportional && !line)) {
		return 0;
	}

	float const prop = proportional ? proportional.get() : (float (line.get()) / lines.get ());

	switch (reference.get ()) {
	case TOP_OF_SCREEN:
		return prop;
	case CENTRE_OF_SCREEN:
		return prop + 0.5;
	case BOTTOM_OF_SCREEN:
		return 1 - prop;
	case TOP_OF_SUBTITLE:
		return prop;
	}

	return 0;
}

bool
VerticalPosition::operator== (VerticalPosition const & other) const
{
	if (proportional && reference && other.proportional && other.reference) {
		return proportional == other.proportional && reference == other.reference;
	} else if (reference && line && lines && other.reference && other.line && other.lines) {
		return line == other.line && lines == other.lines && reference == other.reference;
	} else if (reference && line && other.reference && other.line) {
		return reference == other.reference && line == other.line;
	}

	return false;
}

bool
VerticalPosition::operator< (VerticalPosition const & other) const
{
	return fraction_from_screen_top() < other.fraction_from_screen_top();
}
