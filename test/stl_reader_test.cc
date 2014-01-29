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
#include "stl_reader.h"
#include "subtitle.h"

using std::list;
using std::ifstream;

/* Test reading of an STL file */
BOOST_AUTO_TEST_CASE (stl_reader_test)
{
	ifstream file ("test/data/test.stl");
	sub::STLReader reader (file);
	list<sub::Subtitle> subs = reader.subtitles ();

	list<sub::Subtitle>::iterator i = subs.begin ();

	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->text, " This is a subtitle ");
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->font_size, 42);
	BOOST_CHECK_EQUAL (i->bold, false);
	BOOST_CHECK_EQUAL (i->italic, false);
	BOOST_CHECK_EQUAL (i->underline, false);
	BOOST_CHECK_EQUAL (i->line, 0);
	BOOST_CHECK_EQUAL (i->frame_from, sub::FrameTime (0, 0, 41, 9));
	BOOST_CHECK_EQUAL (i->frame_to, sub::FrameTime (0, 0, 42, 21));
	++i;

	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->text, " and that's a line break");
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->font_size, 42);
	BOOST_CHECK_EQUAL (i->bold, false);
	BOOST_CHECK_EQUAL (i->italic, false);
	BOOST_CHECK_EQUAL (i->underline, false);
	BOOST_CHECK_EQUAL (i->line, 1);
	BOOST_CHECK_EQUAL (i->frame_from, sub::FrameTime (0, 0, 41, 9));
	BOOST_CHECK_EQUAL (i->frame_to, sub::FrameTime (0, 0, 42, 21));
	++i;

	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->text, " This is some ");
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->font_size, 42);
	BOOST_CHECK_EQUAL (i->bold, false);
	BOOST_CHECK_EQUAL (i->italic, false);
	BOOST_CHECK_EQUAL (i->underline, false);
	BOOST_CHECK_EQUAL (i->line, 0);
	BOOST_CHECK_EQUAL (i->frame_from, sub::FrameTime (0, 1, 1, 1));
	BOOST_CHECK_EQUAL (i->frame_to, sub::FrameTime (0, 1, 2, 10));
	++i;

	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->text, "bold");
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->font_size, 42);
	BOOST_CHECK_EQUAL (i->bold, true);
	BOOST_CHECK_EQUAL (i->italic, false);
	BOOST_CHECK_EQUAL (i->underline, false);
	BOOST_CHECK_EQUAL (i->frame_from, sub::FrameTime (0, 1, 1, 1));
	BOOST_CHECK_EQUAL (i->frame_to, sub::FrameTime (0, 1, 2, 10));
	++i;

	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->text, " and some ");
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->font_size, 42);
	BOOST_CHECK_EQUAL (i->frame_from, sub::FrameTime (0, 1, 1, 1));
	BOOST_CHECK_EQUAL (i->frame_to, sub::FrameTime (0, 1, 2, 10));
	BOOST_CHECK_EQUAL (i->bold, false);
	BOOST_CHECK_EQUAL (i->italic, false);
	BOOST_CHECK_EQUAL (i->underline, false);
	BOOST_CHECK_EQUAL (i->line, 0);
	++i;

	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->text, "bold italic");
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->font_size, 42);
	BOOST_CHECK_EQUAL (i->bold, true);
	BOOST_CHECK_EQUAL (i->italic, true);
	BOOST_CHECK_EQUAL (i->underline, false);
	BOOST_CHECK_EQUAL (i->line, 0);
	BOOST_CHECK_EQUAL (i->frame_from, sub::FrameTime (0, 1, 1, 1));
	BOOST_CHECK_EQUAL (i->frame_to, sub::FrameTime (0, 1, 2, 10));
	++i;

	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->text, " and some ");
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->font_size, 42);
	BOOST_CHECK_EQUAL (i->bold, false);
	BOOST_CHECK_EQUAL (i->italic, false);
	BOOST_CHECK_EQUAL (i->underline, false);
	BOOST_CHECK_EQUAL (i->frame_from, sub::FrameTime (0, 1, 1, 1));
	BOOST_CHECK_EQUAL (i->frame_to, sub::FrameTime (0, 1, 2, 10));
	++i;

	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->text, "underlined");
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->font_size, 42);
	BOOST_CHECK_EQUAL (i->bold, false);
	BOOST_CHECK_EQUAL (i->italic, false);
	BOOST_CHECK_EQUAL (i->underline, true);
	BOOST_CHECK_EQUAL (i->line, 0);
	BOOST_CHECK_EQUAL (i->frame_from, sub::FrameTime (0, 1, 1, 1));
	BOOST_CHECK_EQUAL (i->frame_to, sub::FrameTime (0, 1, 2, 10));
	++i;

	BOOST_CHECK_EQUAL (i->text, ".");
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->font_size, 42);
	BOOST_CHECK_EQUAL (i->bold, false);
	BOOST_CHECK_EQUAL (i->italic, false);
	BOOST_CHECK_EQUAL (i->underline, false);
	BOOST_CHECK_EQUAL (i->line, 0);
	BOOST_CHECK_EQUAL (i->frame_from, sub::FrameTime (0, 1, 1, 1));
	BOOST_CHECK_EQUAL (i->frame_to, sub::FrameTime (0, 1, 2, 10));
	++i;
	
	BOOST_CHECK (i == subs.end ());
}


