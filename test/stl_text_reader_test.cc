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

#include "stl_text_reader.h"
#include "subtitle.h"
#include "collect.h"
#include <boost/test/unit_test.hpp>
#include <fstream>

using std::ifstream;
using std::vector;

/* Test reading of a textual STL file */
BOOST_AUTO_TEST_CASE (stl_text_reader_test)
{
	ifstream file ("test/data/test_text.stl");
	sub::STLTextReader reader (file);
	auto subs = sub::collect<vector<sub::Subtitle>> (reader.subtitles());

	auto i = subs.begin ();


	/* First subtitle */

	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hmsf (0, 0, 41, 9));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hmsf (0, 0, 42, 21));

	auto j = i->lines.begin ();
	BOOST_CHECK (j != i->lines.end ());
	BOOST_CHECK_EQUAL (j->blocks.size(), 1U);
	auto b = j->blocks.front ();
	BOOST_CHECK_EQUAL (b.text, " This is a subtitle ");
	BOOST_CHECK_EQUAL (b.font.get(), "Arial");
	BOOST_CHECK_EQUAL (b.font_size.points().get(), 42);
	BOOST_CHECK_EQUAL (b.bold, false);
	BOOST_CHECK_EQUAL (b.italic, false);
	BOOST_CHECK_EQUAL (b.underline, false);
	BOOST_CHECK_EQUAL (j->vertical_position.line.get(), 0);
	++j;

	BOOST_CHECK (j != i->lines.end ());
	BOOST_CHECK_EQUAL (j->blocks.size(), 1U);
	b = j->blocks.front ();
	BOOST_CHECK_EQUAL (b.text, " and that's a line break");
	BOOST_CHECK_EQUAL (b.font.get(), "Arial");
	BOOST_CHECK_EQUAL (b.font_size.points().get(), 42);
	BOOST_CHECK_EQUAL (b.bold, false);
	BOOST_CHECK_EQUAL (b.italic, false);
	BOOST_CHECK_EQUAL (b.underline, false);
	BOOST_CHECK_EQUAL (j->vertical_position.line.get(), 1);
	++i;


	/* Second subtitle */

	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hmsf (0, 1, 1, 1));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hmsf (0, 1, 2, 10));

	BOOST_CHECK_EQUAL (i->lines.size(), 1U);
	sub::Line l = i->lines.front ();
	BOOST_CHECK_EQUAL (l.blocks.size(), 7U);
	BOOST_CHECK_EQUAL (l.vertical_position.line.get(), 0);

	auto k = l.blocks.begin ();

	BOOST_CHECK (k != l.blocks.end ());
	BOOST_CHECK_EQUAL (k->text, " This is some ");
	BOOST_CHECK_EQUAL (k->font.get(), "Arial");
	BOOST_CHECK_EQUAL (k->font_size.points().get(), 42);
	BOOST_CHECK_EQUAL (k->bold, false);
	BOOST_CHECK_EQUAL (k->italic, false);
	BOOST_CHECK_EQUAL (k->underline, false);
	++k;

	BOOST_CHECK (k != l.blocks.end ());
	BOOST_CHECK_EQUAL (k->text, "bold");
	BOOST_CHECK_EQUAL (k->font.get(), "Arial");
	BOOST_CHECK_EQUAL (k->font_size.points().get(), 42);
	BOOST_CHECK_EQUAL (k->bold, true);
	BOOST_CHECK_EQUAL (k->italic, false);
	BOOST_CHECK_EQUAL (k->underline, false);
	++k;

	BOOST_CHECK (k != l.blocks.end ());
	BOOST_CHECK_EQUAL (k->text, " and some ");
	BOOST_CHECK_EQUAL (k->font.get(), "Arial");
	BOOST_CHECK_EQUAL (k->font_size.points().get(), 42);
	BOOST_CHECK_EQUAL (k->bold, false);
	BOOST_CHECK_EQUAL (k->italic, false);
	BOOST_CHECK_EQUAL (k->underline, false);
	++k;

	BOOST_CHECK (k != l.blocks.end ());
	BOOST_CHECK_EQUAL (k->text, "bold italic");
	BOOST_CHECK_EQUAL (k->font.get(), "Arial");
	BOOST_CHECK_EQUAL (k->font_size.points().get(), 42);
	BOOST_CHECK_EQUAL (k->bold, true);
	BOOST_CHECK_EQUAL (k->italic, true);
	BOOST_CHECK_EQUAL (k->underline, false);
	++k;

	BOOST_CHECK (k != l.blocks.end ());
	BOOST_CHECK_EQUAL (k->text, " and some ");
	BOOST_CHECK_EQUAL (k->font.get(), "Arial");
	BOOST_CHECK_EQUAL (k->font_size.points().get(), 42);
	BOOST_CHECK_EQUAL (k->bold, false);
	BOOST_CHECK_EQUAL (k->italic, false);
	BOOST_CHECK_EQUAL (k->underline, false);
	++k;

	BOOST_CHECK (k != l.blocks.end ());
	BOOST_CHECK_EQUAL (k->text, "underlined");
	BOOST_CHECK_EQUAL (k->font.get(), "Arial");
	BOOST_CHECK_EQUAL (k->font_size.points().get(), 42);
	BOOST_CHECK_EQUAL (k->bold, false);
	BOOST_CHECK_EQUAL (k->italic, false);
	BOOST_CHECK_EQUAL (k->underline, true);
	++k;

	BOOST_CHECK (k != l.blocks.end ());
	BOOST_CHECK_EQUAL (k->text, ".");
	BOOST_CHECK_EQUAL (k->font.get(), "Arial");
	BOOST_CHECK_EQUAL (k->font_size.points().get(), 42);
	BOOST_CHECK_EQUAL (k->bold, false);
	BOOST_CHECK_EQUAL (k->italic, false);
	BOOST_CHECK_EQUAL (k->underline, false);
	++k;

	BOOST_CHECK (k == l.blocks.end ());
}


