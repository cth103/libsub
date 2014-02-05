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

/* Check time conversions */
BOOST_AUTO_TEST_CASE (time_test)
{
	BOOST_CHECK_EQUAL (metric_to_frame (sub::MetricTime (3, 5, 7, 40), 25), sub::FrameTime (3, 5, 7, 1));
	BOOST_CHECK_EQUAL (frame_to_metric (sub::FrameTime  (3, 5, 7, 1), 25), sub::MetricTime (3, 5, 7, 40));
	BOOST_CHECK_EQUAL (metric_to_frame (sub::MetricTime (3, 5, 7, 120), 25), sub::FrameTime (3, 5, 7, 3));
	BOOST_CHECK_EQUAL (frame_to_metric (sub::FrameTime  (3, 5, 7, 3), 25), sub::MetricTime (3, 5, 7, 120));
}

