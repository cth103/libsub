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
	return a.hours() == b.hours() && a.minutes() == b.minutes() && a.seconds() == b.seconds() && a.frames() == b.frames();
}

bool
sub::operator< (FrameTime const & a, FrameTime const & b)
{
	if (a.hours() != b.hours()) {
		return a.hours() < b.hours();
	}

	if (a.minutes() != b.minutes()) {
		return a.minutes() < b.minutes();
	}

	if (a.seconds() != b.seconds()) {
		return a.seconds() < b.seconds();
	}

	return a.frames() < b.frames();
}

ostream&
sub::operator<< (ostream& s, FrameTime const & t)
{
	s << t.hours() << ":" << t.minutes() << ":" << t.seconds() << ":" << t.frames();
	return s;
}

string
FrameTime::timecode () const
{
	return String::compose ("%1:%2:%3:%4", _hours, _minutes, _seconds, _frames);
}

FrameTime::FrameTime (int64_t f, float fps)
{
	_hours = f / (60 * 60 * fps);
	f -= _hours * 60 * 60 * fps;
	_minutes = f / (60 * fps);
	f -= _minutes * 60 * fps;
	_seconds = f / fps;
	f -= _seconds * fps;
	_frames = int (f);
}

void
FrameTime::add (FrameTime t, float fps)
{
	_frames += t.frames ();
	if (_frames > fps) {
		_frames -= fps;
		_seconds++;
	}

	_seconds += t.seconds ();
	if (_seconds >= 60) {
		_seconds -= 60;
		++_minutes;
	}

	_minutes += t.minutes ();
	if (_minutes >= 60) {
		_minutes -= 60;
		++_hours;
	}

	_hours += t.hours ();
}
