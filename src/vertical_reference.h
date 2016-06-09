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

#ifndef LIBSUB_VERTICAL_REFERENCE_H
#define LIBSUB_VERTICAL_REFERENCE_H

#include <string>

namespace sub {

enum VerticalReference
{
	TOP_OF_SCREEN,
	CENTRE_OF_SCREEN,
	BOTTOM_OF_SCREEN,
	/** position on screen is not known, so we are referring
	 *  to the top of the subtitle.
	 */
	TOP_OF_SUBTITLE
};

VerticalReference string_to_vertical_reference (std::string s);

}

#endif
