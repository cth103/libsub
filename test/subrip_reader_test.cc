/*
    Copyright (C) 2014-2020 Carl Hetherington <cth@carlh.net>

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

#include "subrip_reader.h"
#include "subtitle.h"
#include "test.h"
#include "exceptions.h"
#include "collect.h"
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <cmath>
#include <iostream>
#include <cstdio>

using std::cerr;
using std::vector;
using std::fabs;

/* Test reading of a Subrip file */
BOOST_AUTO_TEST_CASE (subrip_reader_test)
{
	FILE* f = fopen ("test/data/test.srt", "r");
	sub::SubripReader reader (f);
	fclose (f);
	auto subs = sub::collect<std::vector<sub::Subtitle>> (reader.subtitles());

	auto i = subs.begin ();


	/* First subtitle */

	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 0, 41, 90));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 0, 42, 210));

	auto j = i->lines.begin();
	BOOST_CHECK (j != i->lines.end ());
	BOOST_CHECK_EQUAL (j->blocks.size(), 1);
	auto b = j->blocks.front();
	BOOST_CHECK_EQUAL (b.text, "This is a subtitle");
	/* No font is specified by subrip, so none should be seen here */
	BOOST_CHECK (!b.font);
	BOOST_CHECK (!b.font_size.specified());
	BOOST_CHECK_EQUAL (b.bold, false);
	BOOST_CHECK_EQUAL (b.italic, false);
	BOOST_CHECK_EQUAL (b.underline, false);
	BOOST_REQUIRE (j->vertical_position.line);
	BOOST_CHECK_EQUAL (j->vertical_position.line.get(), 0);
	BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP_OF_SUBTITLE);
	++j;

	BOOST_CHECK (j != i->lines.end ());
	BOOST_CHECK_EQUAL (j->blocks.size(), 1);
	b = j->blocks.front ();
	BOOST_CHECK_EQUAL (b.text, "and that's a line break");
	/* No font is specified by subrip, so none should be seen here */
	BOOST_CHECK (!b.font);
	BOOST_CHECK (!b.font_size.specified());
	BOOST_CHECK_EQUAL (b.bold, false);
	BOOST_CHECK_EQUAL (b.italic, false);
	BOOST_CHECK_EQUAL (b.underline, false);
	BOOST_REQUIRE (j->vertical_position.line);
	BOOST_CHECK_EQUAL (j->vertical_position.line.get(), 1);
	BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP_OF_SUBTITLE);
	++i;


	/* Second subtitle */

	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 1, 1, 10));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 1, 2, 100));

	BOOST_CHECK_EQUAL (i->lines.size(), 1);
	sub::Line l = i->lines.front ();
	BOOST_CHECK_EQUAL (l.blocks.size(), 7);
	BOOST_CHECK_EQUAL (l.vertical_position.line.get(), 0);
	BOOST_CHECK_EQUAL (l.vertical_position.reference.get(), sub::TOP_OF_SUBTITLE);

	auto k = l.blocks.begin();

	BOOST_CHECK (k != l.blocks.end ());
	BOOST_CHECK_EQUAL (k->text, "This is some ");
	/* No font is specified by subrip, so none should be seen here */
	BOOST_CHECK (!b.font);
	BOOST_CHECK (!b.font_size.specified());
	BOOST_CHECK_EQUAL (k->bold, false);
	BOOST_CHECK_EQUAL (k->italic, false);
	BOOST_CHECK_EQUAL (k->underline, false);
	++k;

	BOOST_CHECK (k != l.blocks.end ());
	BOOST_CHECK_EQUAL (k->text, "bold");
	/* No font is specified by subrip, so none should be seen here */
	BOOST_CHECK (!b.font);
	BOOST_CHECK (!b.font_size.specified());
	BOOST_CHECK_EQUAL (k->bold, true);
	BOOST_CHECK_EQUAL (k->italic, false);
	BOOST_CHECK_EQUAL (k->underline, false);
	++k;

	BOOST_CHECK (k != l.blocks.end ());
	BOOST_CHECK_EQUAL (k->text, " and some ");
	/* No font is specified by subrip, so none should be seen here */
	BOOST_CHECK (!b.font);
	BOOST_CHECK (!b.font_size.specified());
	BOOST_CHECK_EQUAL (k->bold, false);
	BOOST_CHECK_EQUAL (k->italic, false);
	BOOST_CHECK_EQUAL (k->underline, false);
	++k;

	BOOST_CHECK (k != l.blocks.end ());
	BOOST_CHECK_EQUAL (k->text, "bold italic");
	/* No font is specified by subrip, so none should be seen here */
	BOOST_CHECK (!b.font);
	BOOST_CHECK (!b.font_size.specified());
	BOOST_CHECK_EQUAL (k->bold, true);
	BOOST_CHECK_EQUAL (k->italic, true);
	BOOST_CHECK_EQUAL (k->underline, false);
	++k;

	BOOST_CHECK (k != l.blocks.end ());
	BOOST_CHECK_EQUAL (k->text, " and some ");
	/* No font is specified by subrip, so none should be seen here */
	BOOST_CHECK (!b.font);
	BOOST_CHECK (!b.font_size.specified());
	BOOST_CHECK_EQUAL (k->bold, false);
	BOOST_CHECK_EQUAL (k->italic, false);
	BOOST_CHECK_EQUAL (k->underline, false);
	++k;

	BOOST_CHECK (k != l.blocks.end ());
	BOOST_CHECK_EQUAL (k->text, "underlined");
	/* No font is specified by subrip, so none should be seen here */
	BOOST_CHECK (!b.font);
	BOOST_CHECK (!b.font_size.specified());
	BOOST_CHECK_EQUAL (k->bold, false);
	BOOST_CHECK_EQUAL (k->italic, false);
	BOOST_CHECK_EQUAL (k->underline, true);
	++k;

	BOOST_CHECK (k != l.blocks.end ());
	BOOST_CHECK_EQUAL (k->text, ".");
	/* No font is specified by subrip, so none should be seen here */
	BOOST_CHECK (!b.font);
	BOOST_CHECK (!b.font_size.specified());
	BOOST_CHECK_EQUAL (k->bold, false);
	BOOST_CHECK_EQUAL (k->italic, false);
	BOOST_CHECK_EQUAL (k->underline, false);
	++k;

	BOOST_CHECK (k == l.blocks.end ());
}

/* Test reading of another Subrip file */
BOOST_AUTO_TEST_CASE (subrip_reader_test2)
{
	FILE* f = fopen ("test/data/test2.srt", "r");
	sub::SubripReader reader (f);
	fclose (f);
	auto subs = sub::collect<vector<sub::Subtitle>> (reader.subtitles());

	auto i = subs.begin();

	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 1, 49, 200));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 1, 52, 351));
	BOOST_CHECK_EQUAL (i->lines.size(), 2);
	BOOST_CHECK_EQUAL (i->lines.front().blocks.front().text, "This is a subtitle, and it goes ");
	BOOST_CHECK_EQUAL (i->lines.back().blocks.front().text, "over two lines.");

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 1, 52, 440));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 1, 54, 351));
	BOOST_CHECK_EQUAL (i->lines.size(), 1);
	BOOST_CHECK_EQUAL (i->lines.front().blocks.front().text, "We have emboldened this");
	BOOST_CHECK_EQUAL (i->lines.front().blocks.front().bold, true);

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 1, 54, 440));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 1, 56, 590));
	BOOST_CHECK_EQUAL (i->lines.size(), 1);
	BOOST_CHECK_EQUAL (i->lines.front().blocks.front().text, "And italicised this.");
	BOOST_CHECK_EQUAL (i->lines.front().blocks.front().italic, true);

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 1, 56, 680));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 1, 58, 955));
	BOOST_CHECK_EQUAL (i->lines.size(), 1);
	BOOST_CHECK_EQUAL (i->lines.front().blocks.front().text, "Shall I compare thee to a summers' day?");

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 2, 0, 840));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 2, 3, 400));
	BOOST_CHECK_EQUAL (i->lines.size(), 1);
	BOOST_CHECK_EQUAL (i->lines.front().blocks.front().text, "Is this a dagger I see before me?");

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 3, 54, 560));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 3, 56, 471));
	BOOST_CHECK_EQUAL (i->lines.size(), 1);
	BOOST_CHECK_EQUAL (i->lines.front().blocks.front().text, "Hello world.");

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 4, 50, 123));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 4, 55, 23));
	BOOST_CHECK_EQUAL (i->lines.size(), 2);
	BOOST_CHECK_EQUAL (i->lines.front().blocks.front().text, "Some italics over");
	BOOST_CHECK_EQUAL (i->lines.front().blocks.front().italic, true);
	BOOST_CHECK_EQUAL (i->lines.back().blocks.front().text, "multiple lines");
	BOOST_CHECK_EQUAL (i->lines.back().blocks.front().italic, true);

	++i;
	BOOST_CHECK (i == subs.end ());
}

/** Test SubripReader::convert_line */
BOOST_AUTO_TEST_CASE (subrip_reader_convert_line_test)
{
	sub::SubripReader r;

	sub::RawSubtitle rs;
	r.convert_line ("Hello world", rs);
	BOOST_CHECK_EQUAL (r._subs.size(), 1);
	BOOST_CHECK_EQUAL (r._subs.front().text, "Hello world");
	r._subs.clear ();

	rs = sub::RawSubtitle();
	r.convert_line ("<b>Hello world</b>", rs);
	BOOST_CHECK_EQUAL (r._subs.size(), 1);
	BOOST_CHECK_EQUAL (r._subs.front().text, "Hello world");
	BOOST_CHECK_EQUAL (r._subs.front().bold, true);
	r._subs.clear ();

	rs = sub::RawSubtitle();
	r.convert_line ("<i>Hello world</i>", rs);
	BOOST_CHECK_EQUAL (r._subs.size(), 1);
	BOOST_CHECK_EQUAL (r._subs.front().text, "Hello world");
	BOOST_CHECK_EQUAL (r._subs.front().italic, true);
	r._subs.clear ();

	rs = sub::RawSubtitle();
	r.convert_line ("<u>Hello world</u>", rs);
	BOOST_CHECK_EQUAL (r._subs.size(), 1);
	BOOST_CHECK_EQUAL (r._subs.front().text, "Hello world");
	BOOST_CHECK_EQUAL (r._subs.front().underline, true);
	r._subs.clear ();

	rs = sub::RawSubtitle();
	r.convert_line ("{b}Hello world{/b}", rs);
	BOOST_CHECK_EQUAL (r._subs.size(), 1);
	BOOST_CHECK_EQUAL (r._subs.front().text, "Hello world");
	BOOST_CHECK_EQUAL (r._subs.front().bold, true);
	r._subs.clear ();

	rs = sub::RawSubtitle();
	r.convert_line ("{i}Hello world{/i}", rs);
	BOOST_CHECK_EQUAL (r._subs.size(), 1);
	BOOST_CHECK_EQUAL (r._subs.front().text, "Hello world");
	BOOST_CHECK_EQUAL (r._subs.front().italic, true);
	r._subs.clear ();

	rs = sub::RawSubtitle();
	r.convert_line ("{u}Hello world{/u}", rs);
	BOOST_CHECK_EQUAL (r._subs.size(), 1);
	BOOST_CHECK_EQUAL (r._subs.front().text, "Hello world");
	BOOST_CHECK_EQUAL (r._subs.front().underline, true);
	r._subs.clear ();

	rs = sub::RawSubtitle();
	r.convert_line ("<b>This is <i>nesting</i> of subtitles</b>", rs);
	BOOST_CHECK_EQUAL (r._subs.size(), 3);
	auto i = r._subs.begin();
	BOOST_CHECK_EQUAL (i->text, "This is ");
	BOOST_CHECK_EQUAL (i->bold, true);
	BOOST_CHECK_EQUAL (i->italic, false);
	++i;
	BOOST_CHECK_EQUAL (i->text, "nesting");
	BOOST_CHECK_EQUAL (i->bold, true);
	BOOST_CHECK_EQUAL (i->italic, true);
	++i;
	BOOST_CHECK_EQUAL (i->text, " of subtitles");
	BOOST_CHECK_EQUAL (i->bold, true);
	BOOST_CHECK_EQUAL (i->italic, false);
	++i;
	r._subs.clear ();

	rs = sub::RawSubtitle();
	r.convert_line ("<B>This is <I>nesting</I> of subtitles</B>", rs);
	BOOST_CHECK_EQUAL (r._subs.size(), 3);
	i = r._subs.begin();
	BOOST_CHECK_EQUAL (i->text, "This is ");
	BOOST_CHECK_EQUAL (i->bold, true);
	BOOST_CHECK_EQUAL (i->italic, false);
	++i;
	BOOST_CHECK_EQUAL (i->text, "nesting");
	BOOST_CHECK_EQUAL (i->bold, true);
	BOOST_CHECK_EQUAL (i->italic, true);
	++i;
	BOOST_CHECK_EQUAL (i->text, " of subtitles");
	BOOST_CHECK_EQUAL (i->bold, true);
	BOOST_CHECK_EQUAL (i->italic, false);
	++i;
	r._subs.clear ();

	rs = sub::RawSubtitle();
	r.convert_line ("<font color=\"#ff00ff\">simple color</font>", rs);
	BOOST_CHECK_EQUAL (r._subs.size(), 1);
	BOOST_CHECK_EQUAL (r._subs.front().text, "simple color");
	BOOST_CHECK_EQUAL (r._subs.front().bold, false);
	BOOST_CHECK_CLOSE (r._subs.front().colour.r, 1, 0.1);
	BOOST_CHECK (fabs (r._subs.front().colour.g) < 0.01);
	BOOST_CHECK_CLOSE (r._subs.front().colour.b, 1, 0.1);
	r._subs.clear ();

	rs = sub::RawSubtitle();
	r.convert_line ("<font color=\"#FF00FF\">simple color in capitals</font>", rs);
	BOOST_CHECK_EQUAL (r._subs.size(), 1);
	BOOST_CHECK_EQUAL (r._subs.front().text, "simple color in capitals");
	BOOST_CHECK_EQUAL (r._subs.front().bold, false);
	BOOST_CHECK_CLOSE (r._subs.front().colour.r, 1, 0.1);
	BOOST_CHECK (fabs (r._subs.front().colour.g) < 0.01);
	BOOST_CHECK_CLOSE (r._subs.front().colour.b, 1, 0.1);
	r._subs.clear ();

	rs = sub::RawSubtitle();
	r.convert_line ("<font color=\"#ff0000\">some red text <b>in bold</b></font>", rs);
	BOOST_CHECK_EQUAL (r._subs.size(), 2);
	i = r._subs.begin ();
	BOOST_CHECK_EQUAL (i->text, "some red text ");
	BOOST_CHECK_EQUAL (i->bold, false);
	BOOST_CHECK_CLOSE (i->colour.r, 1, 0.1);
	BOOST_CHECK (fabs (i->colour.g) < 0.01);
	BOOST_CHECK (fabs (i->colour.b) < 0.01);
	++i;
	BOOST_CHECK_EQUAL (i->text, "in bold");
	BOOST_CHECK_EQUAL (i->bold, true);
	BOOST_CHECK_CLOSE (i->colour.r, 1, 0.1);
	BOOST_CHECK (fabs (i->colour.g) < 0.01);
	BOOST_CHECK (fabs (i->colour.b) < 0.01);
	r._subs.clear ();

	rs = sub::RawSubtitle();
	r.convert_line ("<Font color=\"#ff0000\">some red text <b>in bold</b></font>", rs);
	BOOST_CHECK_EQUAL (r._subs.size(), 2);
	i = r._subs.begin ();
	BOOST_CHECK_EQUAL (i->text, "some red text ");
	BOOST_CHECK_EQUAL (i->bold, false);
	BOOST_CHECK_CLOSE (i->colour.r, 1, 0.1);
	BOOST_CHECK (fabs (i->colour.g) < 0.01);
	BOOST_CHECK (fabs (i->colour.b) < 0.01);
	++i;
	BOOST_CHECK_EQUAL (i->text, "in bold");
	BOOST_CHECK_EQUAL (i->bold, true);
	BOOST_CHECK_CLOSE (i->colour.r, 1, 0.1);
	BOOST_CHECK (fabs (i->colour.g) < 0.01);
	BOOST_CHECK (fabs (i->colour.b) < 0.01);
	r._subs.clear ();

	rs = sub::RawSubtitle();
	r.convert_line ("<font color=\"#0000ff\">some blue text <b>in bold</b></font>", rs);
	BOOST_CHECK_EQUAL (r._subs.size(), 2);
	i = r._subs.begin ();
	BOOST_CHECK_EQUAL (i->text, "some blue text ");
	BOOST_CHECK_EQUAL (i->bold, false);
	BOOST_CHECK (fabs (i->colour.r) < 0.01);
	BOOST_CHECK (fabs (i->colour.g) < 0.01);
	BOOST_CHECK_CLOSE (i->colour.b, 1, 0.1);
	++i;
	BOOST_CHECK_EQUAL (i->text, "in bold");
	BOOST_CHECK_EQUAL (i->bold, true);
	BOOST_CHECK (fabs (i->colour.r) < 0.01);
	BOOST_CHECK (fabs (i->colour.g) < 0.01);
	BOOST_CHECK_CLOSE (i->colour.b, 1, 0.1);
	r._subs.clear ();

	rs = sub::RawSubtitle();
	r.convert_line("<font color=\"#0000ffdd\">some blue text with alpha</font>", rs);
	BOOST_CHECK_EQUAL(r._subs.size(), 1);
	i = r._subs.begin();
	BOOST_CHECK_EQUAL(i->text, "some blue text with alpha");
	BOOST_CHECK_EQUAL(i->bold, false);
	BOOST_CHECK(fabs(i->colour.r) < 0.01);
	BOOST_CHECK(fabs(i->colour.g) < 0.01);
	BOOST_CHECK_CLOSE(i->colour.b, 1, 0.1);
	r._subs.clear();

	rs = sub::RawSubtitle();
	r.convert_line ("<< angle brackets but no HTML >>", rs);
	BOOST_CHECK_EQUAL (r._subs.size(), 1);
	i = r._subs.begin ();
	BOOST_CHECK_EQUAL (i->text, "<< angle brackets but no HTML >>");
	r._subs.clear();
}

/** Test SubripReader::convert_time */
BOOST_AUTO_TEST_CASE (subrip_reader_convert_time_test)
{
	sub::SubripReader reader;
	auto t = reader.convert_time("00:03:10,500", ",");
	BOOST_REQUIRE(t);
	BOOST_CHECK_EQUAL(*t, sub::Time::from_hms(0, 3, 10, 500));
	t = reader.convert_time("04:19:51,782", ",");
	BOOST_REQUIRE(t);
	BOOST_CHECK_EQUAL(*t, sub::Time::from_hms(4, 19, 51, 782));
}

static void
test (boost::filesystem::path p)
{
	p = private_test / p;
	FILE* f = fopen (p.string().c_str(), "r");
	BOOST_CHECK (f);
	if (!f) {
		cerr << p << " not found.\n";
		return;
	}
	sub::SubripReader r (f);
	fclose (f);
}

static void
test_throw (boost::filesystem::path p)
{
	p = private_test / p;
	FILE* f = fopen (p.string().c_str(), "r");
	BOOST_CHECK (f);
	if (!f) {
		cerr << p << " not found.\n";
		return;
	}
	BOOST_CHECK_THROW (sub::SubripReader r(f), sub::SubripError);
	fclose (f);
}

/** Test of reading some typical .srt files */
BOOST_AUTO_TEST_CASE (subrip_read_test)
{
	test ("sintel_en.srt");
	test ("sintel_fr.srt");
	test ("FC.srt");
	test ("EU13.srt");
	test ("Subtitulos_H_eng.srt");
	test ("SWING.srt");
	test_throw ("subtitulo1.srt");
}

#define SUB_START(f, t) \
	BOOST_REQUIRE (i != subs.end ()); \
	BOOST_CHECK_EQUAL (i->from, f); \
	BOOST_CHECK_EQUAL (i->to, t); \
	j = i->lines.begin ();

#define LINE(p)							\
	BOOST_REQUIRE (j != i->lines.end ()); \
	BOOST_CHECK (j->vertical_position.line); \
	BOOST_CHECK_EQUAL (j->vertical_position.line.get(), p); \
	BOOST_CHECK (j->vertical_position.reference); \
	BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP_OF_SUBTITLE); \
	k = j->blocks.begin (); \
	++j;

#define BLOCK(t, f, s, b, i, u) \
	BOOST_REQUIRE (k != j->blocks.end ()); \
	BOOST_CHECK_EQUAL (k->text, t); \
	BOOST_CHECK_EQUAL (k->bold, b); \
	BOOST_CHECK_EQUAL (k->italic, i); \
	BOOST_CHECK_EQUAL (k->underline, u); \
	++k;

#define SUB_END() \
	++i;

/** Test reading of another .srt file */
BOOST_AUTO_TEST_CASE (subrip_reader_test3)
{
	boost::filesystem::path p = private_test / "DCP-o-matic_test_subs_1.srt";
	FILE* f = fopen (p.string().c_str(), "r");
	sub::SubripReader reader (f);
	fclose (f);
	auto subs = sub::collect<std::vector<sub::Subtitle>> (reader.subtitles());

	auto i = subs.begin ();
	vector<sub::Line>::iterator j;
	vector<sub::Block>::iterator k;

	BOOST_REQUIRE (i != subs.end ());

	SUB_START (sub::Time::from_hms (0, 0, 0, 76), sub::Time::from_hms (0, 0, 1, 116));
	LINE (0);
	BLOCK ("This line is normal", "Arial", 30, false, false, false);
	LINE (1);
	BLOCK ("This line is bold", "Arial", 30, true, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 1, 206), sub::Time::from_hms (0, 0, 2, 246));
	LINE (0);
	BLOCK ("This line is bold", "Arial", 30, true, false, false);
	LINE (1);
	BLOCK ("This line is normal", "Arial", 30, false, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 2, 308), sub::Time::from_hms (0, 0, 3, 380));
	LINE (0);
	BLOCK ("This line is bold", "Arial", 30, true, false, false);
	LINE (1);
	BLOCK ("This line is italic", "Arial", 30, false, true, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 3, 404), sub::Time::from_hms (0, 0, 4, 484));
	LINE (0);
	BLOCK ("This line is italic", "Arial", 30, false, true, false);
	LINE (1);
	BLOCK ("This line is bold", "Arial", 30, true, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 4, 519), sub::Time::from_hms (0, 0, 5, 604));
	LINE (0);
	BLOCK ("Last three words are ", "Arial", 30, false, false, false);
	BLOCK ("bold AND italic", "Arial", 30, true, true, false);
	LINE (1);
	BLOCK ("Last three words are ", "Arial", 30, false, false, false);
	BLOCK ("italic AND bold", "Arial", 30, true, true, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 5, 628), sub::Time::from_hms (0, 0, 6, 712));
	LINE (0);
	BLOCK ("Last three words are ", "Arial", 30, false, false, false);
	BLOCK ("bold AND italic", "Arial", 30, true, true, false);
	LINE (1);
	BLOCK ("First three words", "Arial", 30, true, true, false);
	BLOCK (" are italic AND bold", "Arial", 30, false, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 6, 736), sub::Time::from_hms (0, 0, 8, 31));
	LINE (0);
	BLOCK ("Last three words are ", "Arial", 30, false, false, false);
	BLOCK ("bold AND italic", "Arial", 30, true, true, false);
	LINE (1);
	BLOCK ("This line is normal", "Arial", 30, false, false, false);
	SUB_END ();

	SUB_START (sub::Time::from_hms (0, 0, 8, 94), sub::Time::from_hms (0, 0, 9, 211));
	LINE (0);
	BLOCK ("Both lines are bold AND italic", "Arial", 30, true, true, false);
	LINE (1);
	BLOCK ("Both lines are bold AND italic", "Arial", 30, true, true, false);
	SUB_END ();
}

/** Test reading of a .srt file with RTL text */
BOOST_AUTO_TEST_CASE (subrip_reader_test4)
{
	boost::filesystem::path p = private_test / "rtl.srt";
	FILE* f = fopen (p.string().c_str(), "r");
	sub::SubripReader reader (f);
	fclose (f);
	auto subs = sub::collect<std::vector<sub::Subtitle>>(reader.subtitles());

	auto i = subs.begin();
	std::cout << i->lines.front().blocks.front().text << "\n";

	std::string const t = i->lines.front().blocks.front().text;
	for (size_t i = 0; i < t.length() - 2; ++i) {
		/* Check that unicode U+202B (right-to-left embedding) has been stripped */
		unsigned char const a = t[i];
		unsigned char const b = t[i+1];
		unsigned char const c = t[i+2];
		BOOST_CHECK ((a != 0xe2 || b != 0x80 || c != 0xab));
	}

	BOOST_CHECK (t == "- \"(دريه فابينار)\"");
}

/** Test <font color="rgba(255,255,255,255)"> */
BOOST_AUTO_TEST_CASE (subrip_reader_test5)
{
	sub::RawSubtitle rs;
	sub::SubripReader r;
	r.convert_line ("<font color=\"rgba(255,128,64,15)\">Foo bar</font>", rs);
	BOOST_REQUIRE_EQUAL (r._subs.size(), 1);
	BOOST_CHECK_EQUAL (r._subs.front().text, "Foo bar");
	BOOST_CHECK_CLOSE (r._subs.front().colour.r, 255.0 / 255, 0.1);
	BOOST_CHECK_CLOSE (r._subs.front().colour.g, 128.0 / 255, 0.1);
	BOOST_CHECK_CLOSE (r._subs.front().colour.b, 64.0 / 255, 0.1);
	r._subs.clear ();

	rs = sub::RawSubtitle ();
	r.convert_line ("<font color=\"rgba(1, 2 , 3, 4)\">Foo bar</font>", rs);
	BOOST_REQUIRE_EQUAL (r._subs.size(), 1);
	BOOST_CHECK_EQUAL (r._subs.front().text, "Foo bar");
	BOOST_CHECK_CLOSE (r._subs.front().colour.r, 1.0 / 255, 0.1);
	BOOST_CHECK_CLOSE (r._subs.front().colour.g, 2.0 / 255, 0.1);
	BOOST_CHECK_CLOSE (r._subs.front().colour.b, 3.0 / 255, 0.1);
}

/** Test alignment */
BOOST_AUTO_TEST_CASE (subrip_reader_test6)
{
	sub::RawSubtitle rs;
	rs.vertical_position.line = 0;
	rs.vertical_position.reference = sub::TOP_OF_SUBTITLE;
	sub::SubripReader r;
	r.convert_line ("Hello world", rs);
	BOOST_REQUIRE_EQUAL (r._subs.size(), 1);
	BOOST_CHECK_EQUAL (r._subs.front().text, "Hello world");
	BOOST_REQUIRE (r._subs.front().vertical_position.line);
	BOOST_CHECK_EQUAL (r._subs.front().vertical_position.line.get(), 0);
	BOOST_REQUIRE (r._subs.front().vertical_position.reference);
	BOOST_CHECK_EQUAL (r._subs.front().vertical_position.reference.get(), sub::TOP_OF_SUBTITLE);
	r._subs.clear ();

	rs = sub::RawSubtitle ();
	rs.vertical_position.line = 0;
	r.convert_line ("{\\an1}Hello", rs);
	BOOST_REQUIRE_EQUAL (r._subs.size(), 1);
	BOOST_CHECK_EQUAL (r._subs.front().text, "Hello");
	BOOST_REQUIRE (r._subs.front().vertical_position.line);
	BOOST_CHECK_EQUAL (r._subs.front().vertical_position.line.get(), 0);
	BOOST_REQUIRE (r._subs.front().vertical_position.reference);
	BOOST_CHECK_EQUAL (r._subs.front().vertical_position.reference.get(), sub::BOTTOM_OF_SCREEN);
	BOOST_CHECK_EQUAL (r._subs.front().horizontal_position.proportional, 0);
	BOOST_CHECK_EQUAL (r._subs.front().horizontal_position.reference, sub::LEFT_OF_SCREEN);
	r._subs.clear ();

	rs = sub::RawSubtitle ();
	rs.vertical_position.line = 0;
	r.convert_line ("{\\an2}to", rs);
	BOOST_REQUIRE_EQUAL (r._subs.size(), 1);
	BOOST_CHECK_EQUAL (r._subs.front().text, "to");
	BOOST_REQUIRE (r._subs.front().vertical_position.line);
	BOOST_CHECK_EQUAL (r._subs.front().vertical_position.line.get(), 0);
	BOOST_REQUIRE (r._subs.front().vertical_position.reference);
	BOOST_CHECK_EQUAL (r._subs.front().vertical_position.reference.get(), sub::BOTTOM_OF_SCREEN);
	BOOST_CHECK_EQUAL (r._subs.front().horizontal_position.proportional, 0);
	BOOST_CHECK_EQUAL (r._subs.front().horizontal_position.reference, sub::HORIZONTAL_CENTRE_OF_SCREEN);
	r._subs.clear ();

	rs = sub::RawSubtitle ();
	rs.vertical_position.line = 0;
	r.convert_line ("{\\an3}you", rs);
	BOOST_CHECK_EQUAL (r._subs.front().text, "you");
	BOOST_REQUIRE (r._subs.front().vertical_position.line);
	BOOST_CHECK_EQUAL (r._subs.front().vertical_position.line.get(), 0);
	BOOST_REQUIRE (r._subs.front().vertical_position.reference);
	BOOST_CHECK_EQUAL (r._subs.front().vertical_position.reference.get(), sub::BOTTOM_OF_SCREEN);
	BOOST_CHECK_EQUAL (r._subs.front().horizontal_position.proportional, 0);
	BOOST_CHECK_EQUAL (r._subs.front().horizontal_position.reference, sub::RIGHT_OF_SCREEN);
	r._subs.clear ();
}


BOOST_AUTO_TEST_CASE(subrip_with_unicode_line_separator_test)
{
	auto f = fopen ("test/data/newline.srt", "r");
	BOOST_REQUIRE(f);
	sub::SubripReader reader(f);
	fclose(f);
	auto subs = sub::collect<std::vector<sub::Subtitle>>(reader.subtitles());

	BOOST_REQUIRE_EQUAL(subs.size(), 2U);

	BOOST_REQUIRE_EQUAL(subs[0].lines.size(), 2U);
	BOOST_REQUIRE_EQUAL(subs[0].lines[0].blocks.size(), 1U);
	BOOST_CHECK_EQUAL(subs[0].lines[0].blocks[0].text, "Du fühlst dich danach besser.");
	BOOST_REQUIRE_EQUAL(subs[0].lines[1].blocks.size(), 1U);
	BOOST_CHECK_EQUAL(subs[0].lines[1].blocks[0].text, "Okay, Kleiner?");

	BOOST_REQUIRE_EQUAL(subs[1].lines.size(), 2U);
	BOOST_REQUIRE_EQUAL(subs[1].lines[0].blocks.size(), 1U);
	BOOST_CHECK_EQUAL(subs[1].lines[0].blocks[0].text, "Sie kann es nicht machen");
	BOOST_REQUIRE_EQUAL(subs[1].lines[1].blocks.size(), 1U);
	BOOST_CHECK_EQUAL(subs[1].lines[1].blocks[0].text, "wenn du dich bewegst.");
}

