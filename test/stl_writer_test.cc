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
#include <fstream>
#include "stl_writer.h"
#include "subtitle.h"
#include "test.h"

using std::list;
using std::ifstream;
using std::ofstream;

/* Test writing of an STL file */
BOOST_AUTO_TEST_CASE (stl_writer_test)
{
	list<sub::Subtitle> subs;
	subs.push_back (sub::Subtitle (" This is a subtitle ", "Arial", 42, false, false, false, 0, sub::Time (0, 0, 41, 9), sub::Time (0, 0, 42, 21)));
	subs.push_back (sub::Subtitle (" and that's a line break", "Arial", 42, false, false, false, 1, sub::Time (0, 0, 41, 9), sub::Time (0, 0, 42, 21)));
	subs.push_back (sub::Subtitle (" This is some ", "Arial", 42, false, false, false, 0, sub::Time (0, 1, 1, 1), sub::Time (0, 1, 2, 10)));
	subs.push_back (sub::Subtitle ("bold", "Arial", 42, true, false, false, 0, sub::Time (0, 1, 1, 1), sub::Time (0, 1, 2, 10)));
	subs.push_back (sub::Subtitle (" and some ", "Arial", 42, false, false, false, 0, sub::Time (0, 1, 1, 1), sub::Time (0, 1, 2, 10)));
	subs.push_back (sub::Subtitle ("bold italic", "Arial", 42, true, true, false, 0, sub::Time (0, 1, 1, 1), sub::Time (0, 1, 2, 10)));
	subs.push_back (sub::Subtitle (" and some ", "Arial", 42, false, false, false, 0, sub::Time (0, 1, 1, 1), sub::Time (0, 1, 2, 10)));
	subs.push_back (sub::Subtitle ("underlined", "Arial", 42, false, false, true, 0, sub::Time (0, 1, 1, 1), sub::Time (0, 1, 2, 10)));
	subs.push_back (sub::Subtitle (".", "Arial", 42, false, false, false, 0, sub::Time (0, 1, 1, 1), sub::Time (0, 1, 2, 10)));

	ofstream f ("build/test/test.stl");
	sub::STLWriter writer (subs, f);
	f.close ();

	check_text ("test/ref/test.stl", "build/test/test.stl");
}


