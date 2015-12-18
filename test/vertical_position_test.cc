/*
    Copyright (C) 2015 Carl Hetherington <cth@carlh.net>

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

#include "vertical_position.h"
#include <boost/test/unit_test.hpp>

using boost::optional;

/* Test operator< for VerticalPosition */
BOOST_AUTO_TEST_CASE (vertical_position_test)
{
	sub::VerticalPosition a;
	sub::VerticalPosition b;

	/* Simple */

	a.proportional = 0.3;
	a.reference = sub::TOP_OF_SCREEN;
	b.proportional = 0.4;
	b.reference = sub::TOP_OF_SCREEN;
	BOOST_CHECK (a < b);

	a.proportional = 0.5;
	a.reference = sub::TOP_OF_SCREEN;
	b.proportional = 0.1;
	b.reference = sub::TOP_OF_SCREEN;
	BOOST_CHECK (b < a);

	a.proportional = optional<float> ();
	b.proportional = optional<float> ();

	a.line = 4;
	a.lines = 25;
	a.reference = sub::TOP_OF_SCREEN;
	b.line = 6;
	b.lines = 25;
	b.reference = sub::TOP_OF_SCREEN;
	BOOST_CHECK (a < b);

	a.line = 17;
	a.lines = 25;
	a.reference = sub::TOP_OF_SCREEN;
	b.line = 2;
	b.lines = 25;
	b.reference = sub::TOP_OF_SCREEN;
	BOOST_CHECK (b < a);

	/* Different reference points with proportional */

	a.line = optional<int> ();
	a.lines = optional<int> ();
	b.line = optional<int> ();
	b.lines = optional<int> ();

	a.proportional = 0.7;
	a.reference = sub::TOP_OF_SCREEN;
	b.proportional = 0.3;
	b.reference = sub::CENTRE_OF_SCREEN;
	BOOST_CHECK (a < b);

	a.proportional = 0.9;
	a.reference = sub::BOTTOM_OF_SCREEN;
	b.proportional = 0.1;
	b.reference = sub::TOP_OF_SCREEN;
	BOOST_CHECK (b < a);

	/* Different line counts with lines */

	a.proportional = optional<float> ();
	b.proportional = optional<float> ();

	a.line = 12;
	a.lines = 99;
	a.reference = sub::TOP_OF_SCREEN;
	b.line = 12;
	b.lines = 25;
	b.reference = sub::TOP_OF_SCREEN;
	BOOST_CHECK (a < b);
}
