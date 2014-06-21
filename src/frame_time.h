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

#ifndef LIBSUB_FRAME_TIME_H
#define LIBSUB_FRAME_TIME_H

#include <iostream>
#include <stdint.h>

namespace sub {

/** @class FrameTime
 *  @brief A time stored in hours, minutes, seconds and frames.
 */
class FrameTime
{
public:
	FrameTime ()
		: _hours (0)
		, _minutes (0)
		, _seconds (0)
		, _frames (0)
	{}

	/** @param f Number of frames.
	 *  @param fps Frames per second.
	 */
	FrameTime (int64_t f, float fps);
			  
	FrameTime (int h, int m, int s, int f)
		: _hours (h)
		, _minutes (m)
		, _seconds (s)
		, _frames (f)
	{}

	int hours () const {
		return _hours;
	}
	
	int minutes () const {
		return _minutes;
	}
	
	int seconds () const {
		return _seconds;
	}
	
	int frames () const {
		return _frames;
	}

	std::string timecode () const;

	void add (FrameTime t, float fps);

private:
	int _hours;
	int _minutes;
	int _seconds;
	int _frames;
};

bool operator== (FrameTime const & a, FrameTime const & b);
bool operator< (FrameTime const & a, FrameTime const & b);
std::ostream& operator<< (std::ostream&, FrameTime const & t);
	
}

#endif
