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

using std::ostream;
using namespace sub;

FrameTime
TimePair::frame (float fps) const
{
	if (_frame) {
		return _frame.get ();
	}

	MetricTime const m = _metric.get ();
	return FrameTime (m.hours(), m.minutes(), m.seconds(), m.milliseconds() * fps / 1000);
}

MetricTime
TimePair::metric (float fps) const
{
	if (_metric) {
		return _metric.get ();
	}

	FrameTime const f = _frame.get ();
	return MetricTime (f.hours(), f.minutes(), f.seconds(), f.frames() * 1000 / fps);
}

void
TimePair::add (FrameTime t, float fps)
{
	if (_frame) {
		_frame.get().add (t, fps);
	} else {
		_metric.get().add (MetricTime (t.hours(), t.minutes(), t.seconds(), t.frames() * 1000 / fps));
	}
}

void
TimePair::scale (float f, float fps)
{
	if (_frame) {
		_frame.get().scale (f, fps);
	} else {
		_metric.get().scale (f);
	}
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

ostream &
sub::operator<< (ostream& s, TimePair const & t)
{
	if (t.frame ()) {
		s << "[FRAME] " << t.frame().get();
	} else {
		s << "[METRIC]" << t.metric().get();
	}

	return s;
}
