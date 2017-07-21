/*
    Copyright (C) 2014-2015 Carl Hetherington <cth@carlh.net>

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

#include "sub_time.h"
#include "exceptions.h"
#include <boost/test/unit_test.hpp>
#include <iostream>

/* Check time construction */
BOOST_AUTO_TEST_CASE (time_construction_test)
{
	{
		sub::Time t = sub::Time::from_hms (3, 5, 7, 40);
		BOOST_CHECK_EQUAL (t.hours(), 3);
		BOOST_CHECK_EQUAL (t.minutes(), 5);
		BOOST_CHECK_EQUAL (t.seconds(), 7);
		BOOST_CHECK_EQUAL (t.milliseconds(), 40);
	}

	{
		sub::Time t = sub::Time::from_hms (591353, 1, 2, 3);
		BOOST_CHECK_EQUAL (t.hours(), 591353);
		BOOST_CHECK_EQUAL (t.minutes(), 1);
		BOOST_CHECK_EQUAL (t.seconds(), 2);
		BOOST_CHECK_EQUAL (t.milliseconds(), 3);
	}
}

/* Check time conversions */
BOOST_AUTO_TEST_CASE (time_conversion_test)
{
	sub::Time p;

	/* 40ms = 1 frame at 25fps */
	p = sub::Time::from_hms (3, 5, 7, 40);
	BOOST_CHECK_EQUAL (p.frames_at (sub::Rational (25, 1)), 1);
	p = sub::Time::from_hmsf (3, 5, 7, 1, sub::Rational (25, 1));
	BOOST_CHECK_EQUAL (p.milliseconds (), 40);

	/* 120ms = 3 frames at 25fps */
	p = sub::Time::from_hms (3, 5, 7, 120);
	BOOST_CHECK_EQUAL (p.frames_at (sub::Rational (25, 1)), 3);
	p = sub::Time::from_hmsf (3, 5, 7, 3, sub::Rational (25, 1));
	BOOST_CHECK_EQUAL (p.milliseconds (), 120);
}

/* Check some operators */
BOOST_AUTO_TEST_CASE (time_operator_test)
{
	BOOST_CHECK_EQUAL (sub::Time::from_hms (0, 0, 5, 198 * 4), sub::Time::from_hms (0, 0, 5, 198 * 4));
	BOOST_CHECK (sub::Time::from_hms (0, 0, 55, 332) != sub::Time::from_hms (0, 0, 58, 332));
}

/* Check some other bits of Time */
BOOST_AUTO_TEST_CASE (time_other_test)
{
	BOOST_CHECK_THROW (sub::Time::from_hmsf (2, 1, 58, 4).all_as_seconds(), sub::UnknownFrameRateError);
	BOOST_CHECK_CLOSE (sub::Time::from_hmsf (2, 1, 58, 4, sub::Rational (24, 1)).all_as_seconds(), 7318.1667, 0.001);
}

/* Check an addition case that gave an odd result */
BOOST_AUTO_TEST_CASE (time_add_test)
{
	sub::Time t = sub::Time::from_hmsf (0, 4, 8, 208, sub::Rational(1000, 1));
	t.add (sub::Time::from_frames(54641, sub::Rational(24, 1)));
	BOOST_CHECK_EQUAL (t, sub::Time::from_hmsf (0, 42, 4, 916, sub::Rational(1000, 1)));
}

BOOST_AUTO_TEST_CASE (time_scale_test1)
{
	sub::Time t = sub::Time::from_hmsf (0, 0, 1, 0, sub::Rational(1000, 1));
	t.scale (0.96);
	BOOST_CHECK_EQUAL (t.seconds(), 0);
	BOOST_CHECK_EQUAL (t.frames_at(sub::Rational(1000, 1)), 960);
}

BOOST_AUTO_TEST_CASE (time_scale_test2)
{
	sub::Time t = sub::Time::from_hmsf (0, 0, 2, 0, sub::Rational(1000, 1));
	t.scale (0.96);
	BOOST_CHECK_EQUAL (t.seconds(), 1);
	BOOST_CHECK_EQUAL (t.frames_at(sub::Rational(1000, 1)), 920);
}
