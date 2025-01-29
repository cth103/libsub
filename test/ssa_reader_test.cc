/*
    Copyright (C) 2016-2021 Carl Hetherington <cth@carlh.net>

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


#include "collect.h"
#include "compose.hpp"
#include "exceptions.h"
#include "ssa_reader.h"
#include "subtitle.h"
#include "test.h"
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <cstdio>
#include <cmath>
#include <iostream>


using std::fabs;
using std::string;
using std::vector;


static
vector<sub::Subtitle>
read_file(boost::filesystem::path filename)
{
	auto file = fopen(filename.string().c_str(), "r");
	BOOST_REQUIRE_MESSAGE(file, "Could not open " << filename.c_str());
	sub::SSAReader reader(file);
	fclose(file);
	return sub::collect<vector<sub::Subtitle>>(reader.subtitles());
}

BOOST_AUTO_TEST_CASE (ssa_reader_test)
{
	auto subs = read_file(private_test / "example.ssa");

	auto i = subs.begin ();

	/* Convert a font size in points to a proportional size for this file */
	auto fs = [](int x) {
		return static_cast<float>(x) / 1024;
	};

	BOOST_REQUIRE (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 2, 40, 650));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 2, 41, 790));
	auto j = i->lines.begin();
	BOOST_REQUIRE (j != i->lines.end ());
	BOOST_REQUIRE_EQUAL(j->blocks.size(), 1U);
	sub::Block b = j->blocks.front ();
	BOOST_CHECK_EQUAL (b.text, "Et les enregistrements de ses ondes delta ?");
	BOOST_CHECK_EQUAL (b.font.get(), "Wolf_Rain");
	BOOST_CHECK_CLOSE(b.font_size.proportional().get(), fs(56), 0.1);
	BOOST_CHECK_EQUAL (b.bold, false);
	BOOST_CHECK_EQUAL (b.italic, false);
	BOOST_CHECK_EQUAL (b.underline, false);
	++i;

	BOOST_REQUIRE (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 2, 42, 420));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 2, 44, 150));
	j = i->lines.begin();
	BOOST_REQUIRE (j != i->lines.end ());
	BOOST_REQUIRE_EQUAL(j->blocks.size(), 1U);
	b = j->blocks.front ();
	BOOST_CHECK_EQUAL (b.text, "Toujours rien.");
	BOOST_CHECK_EQUAL (b.font.get(), "Wolf_Rain");
	BOOST_CHECK_CLOSE(b.font_size.proportional().get(), fs(56), 0.1);
	BOOST_CHECK_EQUAL (b.bold, false);
	BOOST_CHECK_EQUAL (b.italic, false);
	BOOST_CHECK_EQUAL (b.underline, false);
	++i;

	BOOST_CHECK (i == subs.end());
}


BOOST_AUTO_TEST_CASE (ssa_reader_line_test1)
{
	sub::RawSubtitle base;
	auto r = sub::SSAReader::parse_line (
		base,
		"This is a line with some {\\i1}italics{\\i0} and then\\nthere is a new line.",
		sub::SSAReader::Context(1920, 1080, sub::Colour(1, 1, 1))
		);

	auto i = r.begin();
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
	auto r = sub::SSAReader::parse_line (
		base,
		"{\\i1}It's all just italics{\\i0}",
		sub::SSAReader::Context(1920, 1080, sub::Colour(1, 1, 1))
		);

	/* Convert a font size in points to a vertical position for this file */
	auto vp = [](int x) {
		return x * 1.2 / 1080;
	};

	auto i = r.begin ();
	BOOST_CHECK_EQUAL (i->text, "It's all just italics");
	BOOST_CHECK_EQUAL (i->italic, true);
	++i;
	BOOST_REQUIRE (i == r.end ());

	r = sub::SSAReader::parse_line (
		base,
		"{\\i1}Italic{\\i0}\\Nand new line",
		sub::SSAReader::Context(1920, 1080, sub::Colour(1, 1, 1))
		);

	i = r.begin ();
	BOOST_CHECK_EQUAL (i->text, "Italic");
	BOOST_CHECK_EQUAL (i->italic, true);
	BOOST_CHECK(fabs(vp(72) - i->vertical_position.proportional.get()) < 1e-5);
	++i;
	BOOST_CHECK_EQUAL (i->text, "and new line");
	BOOST_CHECK_EQUAL (i->italic, false);
	BOOST_CHECK (i->vertical_position.proportional.get() < 1e-5);
}


static void
test (boost::filesystem::path p)
{
	read_file(private_test / p);
}

/** Test of reading some typical .ssa files */
BOOST_AUTO_TEST_CASE (ssa_reader_test2)
{
	test ("DKH_UT_EN20160601def.ssa");
	test ("dcpsubtest-en.ssa");
	test ("dcpsubtest-en.ssa");
	test ("W_GERMAN_SUBS_grey.ass");
	test ("XxxHolic (2022) ITA 071223.ass");
}

#define SUB_START(f, t) \
	BOOST_REQUIRE (i != subs.end ()); \
	BOOST_CHECK_EQUAL (i->from, f); \
	BOOST_CHECK_EQUAL (i->to, t); \
	j = i->lines.begin ();

#define LINE(vp, vr, hp, hr)		      \
	BOOST_REQUIRE (j != i->lines.end ()); \
	BOOST_CHECK (j->vertical_position.proportional); \
	BOOST_CHECK (fabs (j->vertical_position.proportional.get() - vp) < 1e-5); \
	BOOST_CHECK (j->vertical_position.reference); \
	BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), vr); \
	BOOST_CHECK (fabs (j->horizontal_position.proportional - hp) < 1e-5); \
	BOOST_CHECK_EQUAL (j->horizontal_position.reference, hr); \
	k = j->blocks.begin (); \
	++j;

#define BLOCK(t, f, s, b, i, u) \
	BOOST_REQUIRE (k != j->blocks.end ()); \
	BOOST_CHECK_EQUAL (k->text, t); \
        BOOST_CHECK_EQUAL (k->font.get(), f); \
	BOOST_CHECK_CLOSE(k->font_size.proportional().get(), s, 0.1); \
	BOOST_CHECK_EQUAL (k->bold, b); \
	BOOST_CHECK_EQUAL (k->italic, i); \
	BOOST_CHECK_EQUAL (k->underline, u); \
	++k;

#define SUB_END() \
	++i;

/** Test reading of a file within the libsub tree which exercises the parser */
BOOST_AUTO_TEST_CASE (ssa_reader_test3)
{
	auto subs = read_file("test/data/test.ssa");

	/* Convert a font size in points to a proportional size for this file */
	auto fs = [](int x) {
		return static_cast<float>(x) / 1080;
	};

	/* Convert a font size in points to a vertical position for this file */
	auto vp = [&fs](int x) {
		return fs(x) * 1.2;
	};

	auto i = subs.begin();
	vector<sub::Line>::iterator j;
	vector<sub::Block>::iterator k;

	/* Hello world */
	SUB_START (sub::Time::from_hms (0, 0, 1, 230), sub::Time::from_hms (0, 0, 4, 550));
	LINE((10.0 / 1080), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("Hello world", "Arial", fs(20), false, false, false);
	SUB_END();

	/* This is vertically moved\nand has two lines. */
	SUB_START (sub::Time::from_hms (0, 0, 5, 740), sub::Time::from_hms (0, 0, 11, 0));
	/* The first line should be 900 pixels and one line (20
	   points, 1.2 times spaced, as a proportion of the total
	   screen height 729 points) up.
	*/
	LINE((900.0 / 1080) - vp(20), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("This is vertically moved", "Arial", fs(20), false, false, false);
	LINE((900.0 / 1080), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("and has two lines.", "Arial", fs(20), false, false, false);
	SUB_END();

	/* Some {\i1}italics{\i} are here. */
	SUB_START (sub::Time::from_hms (0, 0, 7, 740), sub::Time::from_hms (0, 0, 9, 0));
	LINE((10.0 / 1080), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("Some ", "Arial", fs(20), false, false, false);
	BLOCK("italics", "Arial", fs(20), false, true, false);
	BLOCK(" are here.", "Arial", fs(20), false, false, false);
	SUB_END();

	/* Alignments */

	SUB_START (sub::Time::from_hms (0, 0, 9, 230), sub::Time::from_hms (0, 0, 11, 560));
	LINE ((10.0 / 1080), sub::BOTTOM_OF_SCREEN, (10.0 / 1920), sub::LEFT_OF_SCREEN);
	BLOCK("bottom left", "Arial", fs(20), false, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 9, 240), sub::Time::from_hms (0, 0, 11, 560));
	LINE ((10.0 / 1080), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("bottom centre", "Arial", fs(20), false, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 9, 250), sub::Time::from_hms (0, 0, 11, 560));
	LINE ((10.0 / 1080), sub::BOTTOM_OF_SCREEN, (10.0 / 1920), sub::RIGHT_OF_SCREEN);
	BLOCK("bottom right", "Arial", fs(20), false, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 9, 260), sub::Time::from_hms (0, 0, 11, 560));
	/* Position is half of a 20pt line (with line spacing) above vertical centre */
	LINE (-vp(10), sub::VERTICAL_CENTRE_OF_SCREEN, (10.0 / 1920), sub::LEFT_OF_SCREEN);
	BLOCK("middle left", "Arial", fs(20), false, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 9, 270), sub::Time::from_hms (0, 0, 11, 560));
	LINE (-vp(10), sub::VERTICAL_CENTRE_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("middle centre", "Arial", fs(20), false, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 9, 280), sub::Time::from_hms (0, 0, 11, 560));
	LINE (-vp(10), sub::VERTICAL_CENTRE_OF_SCREEN, (10.0 / 1920), sub::RIGHT_OF_SCREEN);
	BLOCK("middle right", "Arial", fs(20), false, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 9, 290), sub::Time::from_hms (0, 0, 11, 560));
	LINE ((10.0 / 1080), sub::TOP_OF_SCREEN, (10.0 / 1920), sub::LEFT_OF_SCREEN);
	BLOCK("top left", "Arial", fs(20), false, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 9, 300), sub::Time::from_hms (0, 0, 11, 560));
	LINE ((10.0 / 1080), sub::TOP_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("top centre", "Arial", fs(20), false, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 9, 310), sub::Time::from_hms (0, 0, 11, 560));
	LINE ((10.0 / 1080), sub::TOP_OF_SCREEN, (10.0 / 1920), sub::RIGHT_OF_SCREEN);
	BLOCK("top right", "Arial", fs(20), false, false, false);
	SUB_END ();

	BOOST_REQUIRE (i == subs.end ());
}


/** Test reading of a file within the libsub-test-private tree which exercises the parser */
BOOST_AUTO_TEST_CASE (ssa_reader_test4)
{
	auto subs = read_file(private_test / "dcpsubtest2-en.ssa");

	auto i = subs.begin();
	vector<sub::Line>::iterator j;
	vector<sub::Block>::iterator k;

	BOOST_REQUIRE (i != subs.end ());

	/* Convert a font size in points to a proportional size for this file */
	auto fs = [](int x) {
		return static_cast<float>(x) / 288;
	};

	/* Convert a font size in points to a vertical position for this file */
	auto vp = [&fs](int x) {
		return fs(x) * 1.2;
	};

	SUB_START (sub::Time::from_hms (0, 0, 1, 0), sub::Time::from_hms (0, 0, 3, 0));
	/* The first line should be one line (50 points, 1.2 times
	   spaced, as a proportion of the total screen height 729
	   points) up.
	*/
	LINE(vp(50), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("1st line: This is normal", "Verdana", fs(50), false, false, false);
	LINE(0, sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("2d line: this is bold", "Verdana", fs(50), true, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 3, 100), sub::Time::from_hms (0, 0, 5, 100));
	LINE(vp(50), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("1st line: this is bold", "Verdana", fs(50), true, false, false);
	LINE(0, sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("2nd line: This is normal", "Verdana", fs(50), false, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 5, 200), sub::Time::from_hms (0, 0, 7, 200));
	LINE(vp(50), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("1st line: this is bold", "Verdana", fs(50), true, false, false);
	LINE(0, sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("2nd line: this is italics", "Verdana", fs(50), false, true, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 7, 300), sub::Time::from_hms (0, 0, 9, 300));
	LINE(vp(50), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("1st line: this is italics", "Verdana", fs(50), false, true, false);
	LINE(0, sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("2nd line: this is bold", "Verdana", fs(50), true, false, false);
	SUB_END ();
}


/** Test reading of a .ass file */
BOOST_AUTO_TEST_CASE (ssa_reader_test5)
{
	auto subs = read_file(private_test / "dcpsubtest-en.ass");

	/* Convert a font size in points to a proportional size for this file */
	auto fs = [](int x) {
		return static_cast<float>(x) / 288;
	};

	/* Convert a font size in points to a vertical position for this file */
	auto vp = [&fs](int x) {
		return fs(x) * 1.2;
	};

	auto i = subs.begin ();
	vector<sub::Line>::iterator j;
	vector<sub::Block>::iterator k;

	BOOST_REQUIRE (i != subs.end ());

	SUB_START (sub::Time::from_hms (0, 0, 1, 0), sub::Time::from_hms (0, 0, 3, 0));
	/* The first line should be one line (26 points, 1.2 times
	   spaced, as a proportion of the total screen height 729
	   points) up.
	*/
	LINE(vp(26), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("1st subtitle, 1st line", "arial", fs(26), true, false, false);
	LINE(0, sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("2nd subtitle, 2nd line", "arial", fs(26), true, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 3, 100), sub::Time::from_hms (0, 0, 5, 100));
	LINE(vp(26), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("2nd subtitle, 1st line", "arial", fs(26), true, false, false);
	LINE(0, sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("2nd subtitle, 2nd line", "arial", fs(26), true, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 5, 200), sub::Time::from_hms (0, 0, 7, 200));
	LINE(vp(26), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("3rd subtitle, 1st line", "arial", fs(26), true, false, false);
	LINE(0, sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("3rd subtitle, 2nd line", "arial", fs(26), true, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 7, 300), sub::Time::from_hms (0, 0, 9, 300));
	LINE(vp(26), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("4th subtitle, 1st line", "arial", fs(26), true, false, false);
	LINE(0, sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("4th subtitle, 2nd line", "arial", fs(26), true, false, false);
	SUB_END ();
}


/** Test reading of another .ass file */
BOOST_AUTO_TEST_CASE (ssa_reader_test6)
{
	auto subs = read_file(private_test / "DCP-o-matic_test_subs_1.ass");

	/* Convert a font size in points to a proportional size for this file */
	auto fs = [](int x) {
		return static_cast<float>(x) / 288;
	};

	/* Convert a font size in points to a vertical position for this file */
	auto vp = [&fs](int x) {
		return fs(x) * 1.2;
	};

	auto i = subs.begin ();
	vector<sub::Line>::iterator j;
	vector<sub::Block>::iterator k;

	BOOST_REQUIRE (i != subs.end ());

	SUB_START (sub::Time::from_hms (0, 0, 0, 70), sub::Time::from_hms (0, 0, 1, 110));
	/* The first line should be one line (30 points, 1.2 times
	   spaced, as a proportion of the total screen height 792
	   points) up.  There's also a 10 pixel (with respect to a
	   288-pixel-high screen) margin.
	*/
	LINE((vp(30) + (10.0 / 288.0)), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("This line is normal", "Arial", fs(30), false, false, false);
	LINE ((10.0 / 288.0), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("This line is bold", "Arial", fs(30), true, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 1, 200), sub::Time::from_hms (0, 0, 2, 240));
	LINE((vp(30) + (10.0 / 288.0)), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("This line is bold", "Arial", fs(30), true, false, false);
	LINE ((10.0 / 288.0), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("This line is normal", "Arial", fs(30), false, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 2, 300), sub::Time::from_hms (0, 0, 3, 380));
	LINE ((vp(30) + (10.0 / 288.0)), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("This line is bold", "Arial", fs(30), true, false, false);
	LINE ((10.0 / 288.0), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("This line is italic", "Arial", fs(30), false, true, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 3, 400), sub::Time::from_hms (0, 0, 4, 480));
	LINE ((vp(30) + (10.0 / 288.0)), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("This line is italic", "Arial", fs(30), false, true, false);
	LINE ((10.0 / 288.0), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("This line is bold", "Arial", fs(30), true, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 4, 510), sub::Time::from_hms (0, 0, 5, 600));
	LINE ((vp(30) + (10.0 / 288.0)), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("Last three words are ", "Arial", fs(30), false, false, false);
	BLOCK("bold AND italic", "Arial", fs(30), true, true, false);
	LINE ((10.0 / 288.0), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("Last three words are ", "Arial", fs(30), false, false, false);
	BLOCK("italic AND bold", "Arial", fs(30), true, true, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 5, 620), sub::Time::from_hms (0, 0, 6, 710));
	LINE((vp(30) + (10.0 / 288.0)), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("Last three words are ", "Arial", fs(30), false, false, false);
	BLOCK("bold AND italic", "Arial", fs(30), true, true, false);
	LINE ((10.0 / 288.0), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("First three words", "Arial", fs(30), true, true, false);
	BLOCK(" are italic AND bold", "Arial", fs(30), false, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 6, 730), sub::Time::from_hms (0, 0, 8, 30));
	LINE ((vp(30) + (10.0 / 288.0)), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("Last three words are ", "Arial", fs(30), false, false, false);
	BLOCK("bold AND italic", "Arial", fs(30), true, true, false);
	LINE ((10.0 / 288.0), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("This line is normal", "Arial", fs(30), false, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 8, 90), sub::Time::from_hms (0, 0, 9, 210));
	LINE((vp(30) + (10.0 / 288.0)), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("Both lines are bold AND italic", "Arial", fs(30), true, true, false);
	LINE((10.0 / 288.0), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("Both lines are bold AND italic", "Arial", fs(30), true, true, false);
	SUB_END ();
}


BOOST_AUTO_TEST_CASE (ssa_reader_test7)
{
	auto subs = read_file("test/data/test3.ssa");

	/* Convert a font size in points to a proportional size for this file */
	auto fs = [](int x) {
		return static_cast<float>(x) / 1080;
	};

	/* Convert a font size in points to a vertical position for this file */
	auto vp = [&fs](int x) {
		return fs(x) * 1.2;
	};

	auto i = subs.begin();
	vector<sub::Line>::iterator j;
	vector<sub::Block>::iterator k;

	BOOST_REQUIRE (i != subs.end());

	SUB_START(sub::Time::from_hms(0, 0, 1, 0), sub::Time::from_hms(0, 0, 3, 0));
	LINE((vp(60) + (100.0 / 1080)), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("Helvetica Neue 60pt - Default", "Helvetica Neue", fs(60), false, false, false);
	LINE((100.0 / 1080), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("Bottom 100 pt off edge", "Helvetica Neue", fs(60), false, false, false);
	SUB_END();

	SUB_START(sub::Time::from_hms(0, 0, 4, 0), sub::Time::from_hms(0, 0, 6, 0));
	LINE((vp(30) + (100.0 / 1080)), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("Helvetica Neue 30pt", "Helvetica Neue", fs(30), false, false, false);
	LINE((100.0 / 1080), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("Bottom 100pt off edge", "Helvetica Neue", fs(30), false, false, false);
	SUB_END();

	SUB_START(sub::Time::from_hms(0, 0, 7, 0), sub::Time::from_hms(0, 0, 9, 0));
	LINE((vp(120) + (100.0 / 1080)), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("Helvetica Neue 120pt", "Helvetica Neue", fs(120), false, false, false);
	LINE((100.0 / 1080), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("Bottom 100pt off edge", "Helvetica Neue", fs(120), false, false, false);
	SUB_END();

	SUB_START(sub::Time::from_hms(0, 0, 10, 0), sub::Time::from_hms(0, 0, 12, 0));
	LINE((100.0 / 1080), sub::TOP_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("Helvetica Neue 60pt", "Helvetica Neue", fs(60), false, false, false);
	LINE((vp(60) + (100.0 / 1080)), sub::TOP_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("Top Alignment 100pt off edge", "Helvetica Neue", fs(60), false, false, false);
	SUB_END();

	SUB_START(sub::Time::from_hms(0, 0, 13, 0), sub::Time::from_hms(0, 0, 15, 0));
	LINE(vp(-60), sub::VERTICAL_CENTRE_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK("Helvetica Neue 60pt", "Helvetica Neue 60 Center", fs(60), false, false, false);
	LINE(0, sub::VERTICAL_CENTRE_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK(" Vertical Center Alignment", "Helvetica Neue 60 Center", fs(60), false, false, false);
	SUB_END();
}


/** Test \pos */
BOOST_AUTO_TEST_CASE (ssa_reader_pos)
{
	auto subs = read_file("test/data/test2.ssa");

	/* Convert a font size in points to a proportional size for this file */
	auto fs = [](int x) {
		return static_cast<float>(x) / 1080;
	};

	auto i = subs.begin ();
	vector<sub::Line>::iterator j;
	vector<sub::Block>::iterator k;

	/* Hello world */
	SUB_START (sub::Time::from_hms (0, 0, 1, 230), sub::Time::from_hms (0, 0, 4, 550));
	LINE ((10.0 / 1080), sub::BOTTOM_OF_SCREEN, 0, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BLOCK ("Hello world this is ", "Arial", fs(20), false, false, false);
	LINE ((310.0 / 1080), sub::TOP_OF_SCREEN, 400.0 / 1920, sub::LEFT_OF_SCREEN);
	BLOCK ("positioning.", "Arial", fs(20), false, false, false);
	SUB_END();
}


/** Test \fs */
BOOST_AUTO_TEST_CASE (ssa_reader_fs)
{
	sub::RawSubtitle base;
	auto r = sub::SSAReader::parse_line (
		base,
		"This is a line with some {\\fs64}font sizing.",
		sub::SSAReader::Context(1920, 1080, sub::Colour(1, 1, 1))
		);

	auto i = r.begin ();
	BOOST_CHECK_EQUAL (i->text, "This is a line with some ");
	++i;
	BOOST_REQUIRE (i != r.end ());

	BOOST_CHECK_EQUAL (i->text, "font sizing.");
	BOOST_REQUIRE(i->font_size.proportional());
	BOOST_CHECK_CLOSE(i->font_size.proportional().get(), 64.0 / 1080, 0.1);
	++i;
	BOOST_REQUIRE (i == r.end ());
}


static void
test_c(string command, string colour)
{
	sub::RawSubtitle base;
	auto r = sub::SSAReader::parse_line (
		base,
		String::compose("{\\c%1}Hello world", command),
		sub::SSAReader::Context(1920, 1080, sub::Colour(1, 0, 1))
		);

	auto i = r.begin ();
	BOOST_CHECK_EQUAL (i->text, "Hello world");
	BOOST_CHECK (i->colour == sub::Colour::from_rgb_hex(colour));
	BOOST_REQUIRE(std::next(i) == r.end());
}


/** Test a valid \c */
BOOST_AUTO_TEST_CASE (ssa_reader_c)
{
	test_c("&H00FFFF&", "ffff00");
	test_c("&H123456&", "563412");
	test_c("&H0&", "000000");
	test_c("&HFF&", "ff0000");
	test_c("&HFF00&", "00ff00");
	test_c("&HFF0000&", "0000ff");
	test_c("&HFFFFFF&", "ffffff");
	/* \c with no parameter seems to be parsed as "return to primary colour" */
	test_c("", "ff00ff");
}


BOOST_AUTO_TEST_CASE(ssa_reader_horizontal_margin)
{
	auto subs = read_file("test/data/horizontal_margin.ssa");
	BOOST_REQUIRE_EQUAL(subs.size(), 5U);

	int n = 0;

	BOOST_REQUIRE_EQUAL(subs[n].lines.size(), 1U);
	BOOST_CHECK(subs[n].lines[0].horizontal_position.reference == sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BOOST_CHECK_CLOSE(subs[n].lines[0].horizontal_position.proportional, 0, 1);
	++n;

	BOOST_REQUIRE_EQUAL(subs[n].lines.size(), 1U);
	BOOST_CHECK(subs[n].lines[0].horizontal_position.reference == sub::HORIZONTAL_CENTRE_OF_SCREEN);
	BOOST_CHECK_CLOSE(subs[n].lines[0].horizontal_position.proportional, -90.0f / (2 * 1920.0f), 1);
	++n;

	BOOST_REQUIRE_EQUAL(subs[n].lines.size(), 1U);
	BOOST_CHECK(subs[n].lines[0].horizontal_position.reference == sub::LEFT_OF_SCREEN);
	BOOST_CHECK_CLOSE(subs[n].lines[0].horizontal_position.proportional, 10.0f / 1920.f, 1);
	++n;

	BOOST_REQUIRE_EQUAL(subs[n].lines.size(), 1U);
	BOOST_CHECK(subs[n].lines[0].horizontal_position.reference == sub::RIGHT_OF_SCREEN);
	BOOST_CHECK_CLOSE(subs[n].lines[0].horizontal_position.proportional, 100.0f / 1920.f, 1);
	++n;

	BOOST_REQUIRE_EQUAL(subs[n].lines.size(), 1U);
	BOOST_CHECK(subs[n].lines[0].horizontal_position.reference == sub::LEFT_OF_SCREEN);
	BOOST_CHECK_CLOSE(subs[n].lines[0].horizontal_position.proportional, 200.0f / 1920.f, 1);
	++n;
}

