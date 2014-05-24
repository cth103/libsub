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

void
sub::convert_font_sizes (list<Subtitle>& subs, int screen_height_in_points)
{
	for (list<Subtitle>::iterator i = subs.begin(); i != subs.end(); ++i) {
		if (i->font_size.proportional) {
			i->font_size.points = i->font_size.proportional.get() * screen_height_in_points;
		} else {
			i->font_size.proportional = float (i->font_size.points.get()) / screen_height_in_points;
		}
	}
}

/** Take a list of Subtitles and convert their times either from metric to frame, or vice-versa,
 *  depending on what the Subtitles currently have.
 *  @param sub Subtitles.
 *  @param frames_per_second Video frames-per-second value to use in the conversion.
 */
void
sub::convert_times (list<Subtitle>& subs, float frames_per_second)
{
	for (list<Subtitle>::iterator i = subs.begin(); i != subs.end(); ++i) {
		if (i->from.frame) {
			i->from.metric = frame_to_metric (i->from.frame.get(), frames_per_second);
		} else {
			i->from.frame = metric_to_frame (i->from.metric.get(), frames_per_second);
		}

		if (i->to.frame) {
			i->to.metric = frame_to_metric (i->to.frame.get(), frames_per_second);
		} else {
			i->to.frame = metric_to_frame (i->to.metric.get(), frames_per_second);
		}
	}
}
