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
#include "stl_text_writer.h"
#include "subtitle.h"
#include "test.h"

using std::list;
using std::string;
using std::ifstream;
using std::ofstream;

static sub::Subtitle
make (string text, bool bold, bool italic, bool underline, int line, sub::FrameTime from, sub::FrameTime to)
{
	sub::Subtitle s;
	s.text = text;
	s.font = "Arial";
	s.font_size.points = 42;
	s.bold = bold;
	s.italic = italic;
	s.underline = underline;
	s.line = line;
	s.from.frame = from;
	s.to.frame = to;
	return s;
}

/* Test writing of an textual STL file */
BOOST_AUTO_TEST_CASE (stl_text_writer_test)
{
	list<sub::Subtitle> subs;
	subs.push_back (make (" This is a subtitle ",     false, false, false, 0, sub::FrameTime (0, 0, 41, 9), sub::FrameTime (0, 0, 42, 21)));
	subs.push_back (make (" and that's a line break", false, false, false, 1, sub::FrameTime (0, 0, 41, 9), sub::FrameTime (0, 0, 42, 21)));
	subs.push_back (make (" This is some ",           false, false, false, 0, sub::FrameTime (0, 1,  1, 1), sub::FrameTime (0, 1,  2, 10)));
	subs.push_back (make ("bold",                     true,  false, false, 0, sub::FrameTime (0, 1,  1, 1), sub::FrameTime (0, 1,  2, 10)));
	subs.push_back (make (" and some ",               false, false, false, 0, sub::FrameTime (0, 1,  1, 1), sub::FrameTime (0, 1,  2, 10)));
	subs.push_back (make ("bold italic",              true,  true,  false, 0, sub::FrameTime (0, 1,  1, 1), sub::FrameTime (0, 1,  2, 10)));
	subs.push_back (make (" and some ",               false, false, false, 0, sub::FrameTime (0, 1,  1, 1), sub::FrameTime (0, 1,  2, 10)));
	subs.push_back (make ("underlined",               false, false, true,  0, sub::FrameTime (0, 1,  1, 1), sub::FrameTime (0, 1,  2, 10)));
	subs.push_back (make (".",                        false, false, false, 0, sub::FrameTime (0, 1,  1, 1), sub::FrameTime (0, 1,  2, 10)));

	ofstream f ("build/test/test.stl");
	write_stl_text (subs, 24, 72 * 11, f);
	f.close ();

	check_text ("test/ref/test.stl", "build/test/test.stl");
}


