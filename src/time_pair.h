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

/** @file  src/time_pair.h
 *  @brief TimePair class.
 */

#ifndef LIBSUB_TIME_PAIR_H
#define LIBSUB_TIME_PAIR_H

#include "frame_time.h"
#include "metric_time.h"
#include <boost/optional.hpp>

namespace sub {

/** @class TimePair
 *  @brief A time, expressed either in metric (h:m:s:ms) or frames (h:m:s:f).
 */
class TimePair
{
public:
	TimePair () {}
	
	TimePair (FrameTime t)
		: _frame (t)
	{}
	
	TimePair (MetricTime t)
		: _metric (t)
	{}
	
	void set_frame (FrameTime t) {
		_frame = t;
		_metric = boost::optional<MetricTime> ();
	}

	void set_metric (MetricTime t) {
		_metric = t;
		_frame = boost::optional<FrameTime> ();
	}

	boost::optional<FrameTime> frame () const {
		return _frame;
	}
	
	boost::optional<MetricTime> metric () const {
		return _metric;
	}
	
	FrameTime  frame  (float fps) const;
	MetricTime metric (float fps) const;

	void add (FrameTime t, float fps);
	void scale (float f, float fps);

	bool operator== (TimePair const & other) const;
	
private:
	boost::optional<FrameTime> _frame;
	boost::optional<MetricTime> _metric;
};

std::ostream& operator<< (std::ostream & s, TimePair const &);	

}

#endif
