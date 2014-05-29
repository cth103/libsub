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
#include "stl_text_reader.h"
#include "subtitle.h"

using std::list;
using std::ifstream;
using std::vector;

/* Test reading of a textual STL file */
BOOST_AUTO_TEST_CASE (stl_text_reader_test)
{
	ifstream file ("test/data/test_text.stl");
	sub::STLTextReader reader (file);
	list<sub::Subtitle> subs = reader.subtitles ();

	list<sub::Subtitle>::iterator i = subs.begin ();

	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->blocks.size(), 1);
	sub::Block b = i->blocks.front ();
	BOOST_CHECK_EQUAL (b.text, " This is a subtitle ");
	BOOST_CHECK_EQUAL (b.font, "Arial");
	BOOST_CHECK_EQUAL (b.font_size.points().get(), 42);
	BOOST_CHECK_EQUAL (b.bold, false);
	BOOST_CHECK_EQUAL (b.italic, false);
	BOOST_CHECK_EQUAL (b.underline, false);
	BOOST_CHECK_EQUAL (i->vertical_position.line.get(), 0);
	BOOST_CHECK_EQUAL (i->from.frame().get(), sub::FrameTime (0, 0, 41, 9));
	BOOST_CHECK_EQUAL (i->to.frame().get(), sub::FrameTime (0, 0, 42, 21));
	++i;

	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->blocks.size(), 1);
	b = i->blocks.front ();
	BOOST_CHECK_EQUAL (b.text, " and that's a line break");
	BOOST_CHECK_EQUAL (b.font, "Arial");
	BOOST_CHECK_EQUAL (b.font_size.points().get(), 42);
	BOOST_CHECK_EQUAL (b.bold, false);
	BOOST_CHECK_EQUAL (b.italic, false);
	BOOST_CHECK_EQUAL (b.underline, false);
	BOOST_CHECK_EQUAL (i->vertical_position.line.get(), 1);
	BOOST_CHECK_EQUAL (i->from.frame().get(), sub::FrameTime (0, 0, 41, 9));
	BOOST_CHECK_EQUAL (i->to.frame().get(), sub::FrameTime (0, 0, 42, 21));
	++i;

	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->blocks.size(), 7);
	BOOST_CHECK_EQUAL (i->vertical_position.line.get(), 0);
	BOOST_CHECK_EQUAL (i->from.frame().get(), sub::FrameTime (0, 1, 1, 1));
	BOOST_CHECK_EQUAL (i->to.frame().get(), sub::FrameTime (0, 1, 2, 10));

	list<sub::Block>::iterator j = i->blocks.begin ();
	
	BOOST_CHECK (j != i->blocks.end ());
	BOOST_CHECK_EQUAL (j->text, " This is some ");
	BOOST_CHECK_EQUAL (j->font, "Arial");
	BOOST_CHECK_EQUAL (j->font_size.points().get(), 42);
	BOOST_CHECK_EQUAL (j->bold, false);
	BOOST_CHECK_EQUAL (j->italic, false);
	BOOST_CHECK_EQUAL (j->underline, false);
	++j;

	BOOST_CHECK (j != i->blocks.end ());
	BOOST_CHECK_EQUAL (j->text, "bold");
	BOOST_CHECK_EQUAL (j->font, "Arial");
	BOOST_CHECK_EQUAL (j->font_size.points().get(), 42);
	BOOST_CHECK_EQUAL (j->bold, true);
	BOOST_CHECK_EQUAL (j->italic, false);
	BOOST_CHECK_EQUAL (j->underline, false);
	++j;

	BOOST_CHECK (j != i->blocks.end ());
	BOOST_CHECK_EQUAL (j->text, " and some ");
	BOOST_CHECK_EQUAL (j->font, "Arial");
	BOOST_CHECK_EQUAL (j->font_size.points().get(), 42);
	BOOST_CHECK_EQUAL (j->bold, false);
	BOOST_CHECK_EQUAL (j->italic, false);
	BOOST_CHECK_EQUAL (j->underline, false);
	++j;

	BOOST_CHECK (j != i->blocks.end ());
	BOOST_CHECK_EQUAL (j->text, "bold italic");
	BOOST_CHECK_EQUAL (j->font, "Arial");
	BOOST_CHECK_EQUAL (j->font_size.points().get(), 42);
	BOOST_CHECK_EQUAL (j->bold, true);
	BOOST_CHECK_EQUAL (j->italic, true);
	BOOST_CHECK_EQUAL (j->underline, false);
	++j;

	BOOST_CHECK (j != i->blocks.end ());
	BOOST_CHECK_EQUAL (j->text, " and some ");
	BOOST_CHECK_EQUAL (j->font, "Arial");
	BOOST_CHECK_EQUAL (j->font_size.points().get(), 42);
	BOOST_CHECK_EQUAL (j->bold, false);
	BOOST_CHECK_EQUAL (j->italic, false);
	BOOST_CHECK_EQUAL (j->underline, false);
	++j;

	BOOST_CHECK (j != i->blocks.end ());
	BOOST_CHECK_EQUAL (j->text, "underlined");
	BOOST_CHECK_EQUAL (j->font, "Arial");
	BOOST_CHECK_EQUAL (j->font_size.points().get(), 42);
	BOOST_CHECK_EQUAL (j->bold, false);
	BOOST_CHECK_EQUAL (j->italic, false);
	BOOST_CHECK_EQUAL (j->underline, true);
	++j;

	BOOST_CHECK (j != i->blocks.end ());
	BOOST_CHECK_EQUAL (j->text, ".");
	BOOST_CHECK_EQUAL (j->font, "Arial");
	BOOST_CHECK_EQUAL (j->font_size.points().get(), 42);
	BOOST_CHECK_EQUAL (j->bold, false);
	BOOST_CHECK_EQUAL (j->italic, false);
	BOOST_CHECK_EQUAL (j->underline, false);
	++j;
	
	BOOST_CHECK (j == i->blocks.end ());
}


