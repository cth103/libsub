/*
    Copyright (C) 2015 Carl Hetherington <cth@carlh.net>

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

#ifndef LIBSUB_HORIZONTAL_POSITION_H
#define LIBSUB_HORIZONTAL_POSITION_H

#include "horizontal_reference.h"

namespace sub {

class HorizontalPosition
{
public:
	HorizontalPosition ()
		: reference(HORIZONTAL_CENTRE_OF_SCREEN)
		, proportional(0)
	{}

	HorizontalReference reference;
	/** proportion of screen width offset from reference */
	float proportional;

	bool operator== (HorizontalPosition const & other) const;
};

}

#endif
