/*
    Copyright (C) 2016 Carl Hetherington <cth@carlh.net>

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

#include "test.h"
#include "ssa_reader.h"
#include "collect.h"
#include "subtitle.h"
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <cstdio>
#include <cmath>

using std::list;
using std::fabs;

BOOST_AUTO_TEST_CASE (ssa_reader_test)
{
	boost::filesystem::path p = private_test / "example.ssa";
	FILE* f = fopen (p.string().c_str(), "r");
	sub::SSAReader reader (f);
	fclose (f);
	list<sub::Subtitle> subs = sub::collect<std::list<sub::Subtitle> > (reader.subtitles ());

	list<sub::Subtitle>::iterator i = subs.begin ();

	BOOST_REQUIRE (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 2, 40, 650));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 2, 41, 790));
	list<sub::Line>::iterator j = i->lines.begin();
	BOOST_REQUIRE (j != i->lines.end ());
	BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
	sub::Block b = j->blocks.front ();
	BOOST_CHECK_EQUAL (b.text, "Et les enregistrements de ses ondes delta ?");
	BOOST_CHECK_EQUAL (b.font.get(), "Wolf_Rain");
	BOOST_CHECK_EQUAL (b.font_size.points().get(), 56);
	BOOST_CHECK_EQUAL (b.bold, false);
	BOOST_CHECK_EQUAL (b.italic, false);
	BOOST_CHECK_EQUAL (b.underline, false);
	++i;

	BOOST_REQUIRE (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 2, 42, 420));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 2, 44, 150));
	j = i->lines.begin();
	BOOST_REQUIRE (j != i->lines.end ());
	BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
	b = j->blocks.front ();
	BOOST_CHECK_EQUAL (b.text, "Toujours rien.");
	BOOST_CHECK_EQUAL (b.font.get(), "Wolf_Rain");
	BOOST_CHECK_EQUAL (b.font_size.points().get(), 56);
	BOOST_CHECK_EQUAL (b.bold, false);
	BOOST_CHECK_EQUAL (b.italic, false);
	BOOST_CHECK_EQUAL (b.underline, false);
	++i;

	BOOST_CHECK (i == subs.end());
}

BOOST_AUTO_TEST_CASE (ssa_reader_line_test1)
{
	sub::RawSubtitle base;
	list<sub::RawSubtitle> r = sub::SSAReader::parse_line (base, "This is a line with some {\\i1}italics{\\i0} and then\\nthere is a new line.");

	list<sub::RawSubtitle>::const_iterator i = r.begin ();
	BOOST_CHECK_EQUAL (i->text, "This is a line with some ");
	BOOST_CHECK_EQUAL (i->italic, false);
	++i;
	BOOST_REQUIRE (i != r.end ());

	BOOST_CHECK_EQUAL (i->text, "italics");
	BOOST_CHECK_EQUAL (i->italic, true);
	++i;
	BOOST_REQUIRE (i != r.end ());

	BOOST_CHECK_EQUAL (i->text, " and then");
	BOOST_CHECK_EQUAL (i->italic, false);
	++i;
	BOOST_REQUIRE (i != r.end ());

	BOOST_CHECK_EQUAL (i->text, "there is a new line.");
	++i;
	BOOST_REQUIRE (i == r.end ());
}

BOOST_AUTO_TEST_CASE (ssa_reader_line_test2)
{
	sub::RawSubtitle base;
	list<sub::RawSubtitle> r = sub::SSAReader::parse_line (base, "{\\i1}It's all just italics{\\i0}");

	list<sub::RawSubtitle>::const_iterator i = r.begin ();
	BOOST_CHECK_EQUAL (i->text, "It's all just italics");
	BOOST_CHECK_EQUAL (i->italic, true);
	++i;
	BOOST_REQUIRE (i == r.end ());
}

static void
test (boost::filesystem::path p)
{
	p = private_test / p;
	FILE* f = fopen (p.string().c_str(), "r");
	BOOST_REQUIRE (f);
	sub::SSAReader r (f);
	fclose (f);
}

/** Test of reading some typical .ssa files */
BOOST_AUTO_TEST_CASE (ssa_reader_test2)
{
	test ("DKH_UT_EN20160601def.ssa");
	test ("dcpsubtest-en.ssa");
}

#define SUB_START(f, t) \
	BOOST_REQUIRE (i != subs.end ()); \
	BOOST_CHECK_EQUAL (i->from, f); \
	BOOST_CHECK_EQUAL (i->to, t); \
	j = i->lines.begin ();

#define LINE(p, r)							\
	BOOST_REQUIRE (j != i->lines.end ()); \
	BOOST_CHECK (j->vertical_position.proportional); \
	BOOST_CHECK (fabs (j->vertical_position.proportional.get() - p) < 1e-5); \
	BOOST_CHECK (j->vertical_position.reference); \
	BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), r); \
	k = j->blocks.begin (); \
	++j;

#define BLOCK(t, f, s, b, i, u) \
	BOOST_REQUIRE (k != j->blocks.end ()); \
	BOOST_CHECK_EQUAL (k->text, t); \
        BOOST_CHECK_EQUAL (k->font.get(), f); \
	BOOST_CHECK_EQUAL (k->font_size.points().get(), s); \
	BOOST_CHECK_EQUAL (k->bold, b); \
	BOOST_CHECK_EQUAL (k->italic, i); \
	BOOST_CHECK_EQUAL (k->underline, u); \
	++k;

#define SUB_END() \
	++i;

/** Test reading of a file within the libsub tree which exercises the parser */
BOOST_AUTO_TEST_CASE (ssa_reader_test3)
{
	boost::filesystem::path p = "test/data/test.ssa";
	FILE* f = fopen (p.string().c_str(), "r");
	sub::SSAReader reader (f);
	fclose (f);
	list<sub::Subtitle> subs = sub::collect<std::list<sub::Subtitle> > (reader.subtitles ());

	list<sub::Subtitle>::iterator i = subs.begin ();
	list<sub::Line>::iterator j;
	list<sub::Block>::iterator k;

	/* Hello world */
	SUB_START (sub::Time::from_hms (0, 0, 1, 230), sub::Time::from_hms (0, 0, 4, 550));
	LINE (0, sub::BOTTOM_OF_SCREEN);
	BLOCK ("Hello world", "Arial", 20, false, false, false);
	SUB_END();

	/* This is vertically moved\nand has two lines. */
	SUB_START (sub::Time::from_hms (0, 0, 5, 740), sub::Time::from_hms (0, 0, 11, 0));
	/* The first line should be 900 pixels and one line (20
	   points, 1.2 times spaced, as a proportion of the total
	   screen height 729 points) up.
	*/
	LINE((900.0 / 1080) - (20.0 * 1.2 / 792), sub::BOTTOM_OF_SCREEN);
	BLOCK("This is vertically moved", "Arial", 20, false, false, false);
	LINE((900.0 / 1080), sub::BOTTOM_OF_SCREEN);
	BLOCK("and has two lines.", "Arial", 20, false, false, false);
	SUB_END();

	/* Some {\i1}italics{\i} are here. */
	SUB_START (sub::Time::from_hms (0, 0, 7, 740), sub::Time::from_hms (0, 0, 9, 0));
	LINE(0, sub::BOTTOM_OF_SCREEN);
	BLOCK("Some ", "Arial", 20, false, false, false);
	BLOCK("italics", "Arial", 20, false, true, false);
	BLOCK(" are here.", "Arial", 20, false, false, false);
	SUB_END();

	/* Alignments */

	SUB_START (sub::Time::from_hms (0, 0, 9, 230), sub::Time::from_hms (0, 0, 11, 560));
	LINE (0, sub::BOTTOM_OF_SCREEN);
	BLOCK("bottom left", "Arial", 20, false, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 9, 240), sub::Time::from_hms (0, 0, 11, 560));
	LINE (0, sub::BOTTOM_OF_SCREEN);
	BLOCK("bottom centre", "Arial", 20, false, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 9, 250), sub::Time::from_hms (0, 0, 11, 560));
	LINE (0, sub::BOTTOM_OF_SCREEN);
	BLOCK("bottom right", "Arial", 20, false, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 9, 260), sub::Time::from_hms (0, 0, 11, 560));
	LINE (0, sub::CENTRE_OF_SCREEN);
	BLOCK("middle left", "Arial", 20, false, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 9, 270), sub::Time::from_hms (0, 0, 11, 560));
	LINE (0, sub::CENTRE_OF_SCREEN);
	BLOCK("middle centre", "Arial", 20, false, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 9, 280), sub::Time::from_hms (0, 0, 11, 560));
	LINE (0, sub::CENTRE_OF_SCREEN);
	BLOCK("middle right", "Arial", 20, false, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 9, 290), sub::Time::from_hms (0, 0, 11, 560));
	LINE (0, sub::TOP_OF_SCREEN);
	BLOCK("top left", "Arial", 20, false, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 9, 300), sub::Time::from_hms (0, 0, 11, 560));
	LINE (0, sub::TOP_OF_SCREEN);
	BLOCK("top centre", "Arial", 20, false, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 9, 310), sub::Time::from_hms (0, 0, 11, 560));
	LINE (0, sub::TOP_OF_SCREEN);
	BLOCK("top right", "Arial", 20, false, false, false);
	SUB_END ();

	BOOST_REQUIRE (i == subs.end ());
}
