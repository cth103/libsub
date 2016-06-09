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

#ifndef LIBSUB_FONT_SIZE_H
#define LIBSUB_FONT_SIZE_H

#include <boost/optional.hpp>

namespace sub {

/** @class FontSize
 *  @brief A description of a font's size in one of a number of ways.
 *
 *  An object of this class holds a font's size in one way, and it can convert
 *  to another on supply of the required conversion parameters.
 */
class FontSize
{
public:
	void set_proportional (float p) {
		_proportional = p;
	}

	void set_points (int p) {
		_points = p;
	}

	boost::optional<float> proportional () const {
		return _proportional;
	}

	boost::optional<int> points () const {
		return _points;
	}

	bool specified () const;

	float proportional (int screen_height_in_points) const;
	int points (int screen_height_in_points) const;

	static FontSize from_points (int p);

private:
	/** as a proportion of screen height */
	boost::optional<float> _proportional;
	/** in points */
	boost::optional<int> _points;

};

}

#endif
