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

#include <list>
#include <iostream>

namespace sub {

class Subtitle;	

class Writer
{
public:
	Writer (std::list<Subtitle> subtitles, int screen_height_in_points, float frames_per_second)
		: _subs (subtitles)
		, _screen_height_in_points (screen_height_in_points)
		, _frames_per_second (frames_per_second)
	{}

protected:
	std::list<Subtitle> _subs;
	int _screen_height_in_points;
	float _frames_per_second;
};

}
