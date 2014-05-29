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

#include "time_pair.h"

using namespace sub;

FrameTime
TimePair::frame (float frames_per_second) const
{
	if (_frame) {
		return _frame.get ();
	}

	MetricTime const m = _metric.get ();
	return FrameTime (m.hours(), m.minutes(), m.seconds(), m.milliseconds() * frames_per_second / 1000);
}

MetricTime
TimePair::metric (float frames_per_second) const
{
	if (_metric) {
		return _metric.get ();
	}

	FrameTime const f = _frame.get ();
	return MetricTime (f.hours(), f.minutes(), f.seconds(), f.frames() * 1000 / frames_per_second);
}

bool
TimePair::operator== (TimePair const & other) const
{
	if (_metric && other._metric) {
		return _metric.get() == other._metric.get();
	} else if (_frame && other._frame) {
		return _frame.get() == other._frame.get();
	}

	return false;
}