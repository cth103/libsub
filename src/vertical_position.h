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

#ifndef LIBSUB_VERTICAL_POSITION_H
#define LIBSUB_VERTICAL_POSITION_H

#include "vertical_reference.h"
#include <boost/optional.hpp>

namespace sub {

/** @class VerticalPosition
 *  @brief Vertical position of the baseline of some text, expressed in one of a number of ways.
 */
class VerticalPosition
{
public:
	/** as a proportion of screen height offset from some reference point */
	boost::optional<float> proportional;
	/** line number offset from some reference point */
	boost::optional<int> line;
	/** number of lines on the whole screen (i.e. height of the screen in lines) */
	boost::optional<int> lines;
	/** reference point */
	boost::optional<VerticalReference> reference;
	
	bool operator== (VerticalPosition const & other) const;
	bool operator< (VerticalPosition const & other) const;

	float fraction_from_screen_top () const;
};
	
}

#endif
