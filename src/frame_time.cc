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

#include "frame_time.h"
#include "compose.hpp"
#include <iostream>

using std::ostream;
using std::string;
using namespace sub;

bool
sub::operator== (FrameTime const & a, FrameTime const & b)
{
	return a._hours == b._hours && a._minutes == b._minutes && a._seconds == b._seconds && a._frames == b._frames;
}

bool
sub::operator< (FrameTime const & a, FrameTime const & b)
{
	if (a._hours != b._hours) {
		return a._hours < b._hours;
	}

	if (a._minutes != b._minutes) {
		return a._minutes < b._minutes;
	}

	if (a._seconds != b._seconds) {
		return a._seconds < b._seconds;
	}

	return a._frames < b._frames;
}

ostream&
sub::operator<< (ostream& s, FrameTime const & t)
{
	s << t._hours << ":" << t._minutes << ":" << t._seconds << ":" << t._frames;
	return s;
}

string
FrameTime::timecode () const
{
	return String::compose ("%1:%2:%3:%4", _hours, _minutes, _seconds, _frames);
}

FrameTime::FrameTime (int64_t f, int fps)
{
	_hours = f / (60 * 60 * fps);
	f -= _hours * 60 * 60 * fps;
	_minutes = f / (60 * fps);
	f -= _minutes * 60 * fps;
	_seconds = f / fps;
	f -= _seconds * fps;
	_frames = int (f);
}
