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

#ifndef LIBSUB_SUBTITLE_H
#define LIBSUB_SUBTITLE_H

#include "sub_time.h"
#include <string>

namespace sub {

class Subtitle
{
public:
	Subtitle ()
		: font_size (0)
		, bold (false)
		, italic (false)
		, underline (false)
	{}
	
	std::string text;
	std::string font;
	int font_size;
	bool bold;
	bool italic;
	bool underline;
	Time from;
	Time to;
};

}

#endif
