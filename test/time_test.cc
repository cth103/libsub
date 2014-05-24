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

#include <boost/test/unit_test.hpp>
#include "metric_time.h"
#include "frame_time.h"
#include "convert_time.h"

/* Check time construction */
BOOST_AUTO_TEST_CASE (time_construction_test)
{
	{
		sub::MetricTime t (3, 5, 7, 40);
		BOOST_CHECK_EQUAL (t.hours(), 3);
		BOOST_CHECK_EQUAL (t.minutes(), 5);
		BOOST_CHECK_EQUAL (t.seconds(), 7);
		BOOST_CHECK_EQUAL (t.milliseconds(), 40);
	}

	{
		sub::MetricTime t (591353, 1, 2, 3);
		BOOST_CHECK_EQUAL (t.hours(), 591353);
		BOOST_CHECK_EQUAL (t.minutes(), 1);
		BOOST_CHECK_EQUAL (t.seconds(), 2);
		BOOST_CHECK_EQUAL (t.milliseconds(), 3);
	}
}

/* Check time conversions */
BOOST_AUTO_TEST_CASE (time_conversion_test)
{
	/* 40ms = 1 frame at 25fps */
	BOOST_CHECK_EQUAL (metric_to_frame (sub::MetricTime (3, 5, 7, 40), 25), sub::FrameTime (3, 5, 7, 1));
	BOOST_CHECK_EQUAL (frame_to_metric (sub::FrameTime  (3, 5, 7, 1), 25), sub::MetricTime (3, 5, 7, 40));

	/* 120ms = 3 frames at 25fps */
	BOOST_CHECK_EQUAL (metric_to_frame (sub::MetricTime (3, 5, 7, 120), 25), sub::FrameTime (3, 5, 7, 3));
	BOOST_CHECK_EQUAL (frame_to_metric (sub::FrameTime  (3, 5, 7, 3), 25), sub::MetricTime (3, 5, 7, 120));
}

