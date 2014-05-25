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

#include "subtitle.h"
#include "convert_time.h"

using std::list;
using namespace sub;

bool
sub::operator< (Subtitle const & a, Subtitle const & b)
{
	if (a.from.frame && b.from.frame) {
		return a.from.frame.get() < b.from.frame.get();
	}

	if (a.from.metric && b.from.metric) {
		return a.from.metric.get() < b.from.metric.get();
	}

	assert (false);
}

FrameTime
Subtitle::from_frame (float frames_per_second) const
{
	if (from.frame) {
		return from.frame.get ();
	}

	return metric_to_frame (from.metric.get(), frames_per_second);
}

FrameTime
Subtitle::to_frame (float frames_per_second) const
{
	if (to.frame) {
		return to.frame.get ();
	}

	return metric_to_frame (to.metric.get(), frames_per_second);
}

MetricTime
Subtitle::from_metric (float frames_per_second) const
{
	if (from.metric) {
		return from.metric.get ();
	}

	return frame_to_metric (from.frame.get(), frames_per_second);
}

MetricTime
Subtitle::to_metric (float frames_per_second) const
{
	if (to.metric) {
		return to.metric.get ();
	}

	return frame_to_metric (to.frame.get(), frames_per_second);
}

float
Subtitle::font_size_proportional (int screen_height_in_points) const
{
	if (font_size.proportional) {
		return font_size.proportional.get ();
	}

	return float (font_size.points.get ()) / screen_height_in_points;
}

int
Subtitle::font_size_points (int screen_height_in_points) const
{
	if (font_size.points) {
		return font_size.points.get ();
	}

	return font_size.proportional.get() * screen_height_in_points;
}
