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

#include "subrip_reader.h"
#include "subtitle.h"
#include "test.h"
#include "collect.h"
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <fstream>

using std::list;
using std::ifstream;
using std::vector;

/* Test reading of a Subrip file */
BOOST_AUTO_TEST_CASE (subrip_reader_test)
{
	FILE* f = fopen ("test/data/test.srt", "r");
	sub::SubripReader reader (f);
	fclose (f);
	list<sub::Subtitle> subs = sub::collect (reader.subtitles ());

	list<sub::Subtitle>::iterator i = subs.begin ();

	
	/* First subtitle */

	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from.metric().get(), sub::MetricTime (0, 0, 41, 90));
	BOOST_CHECK_EQUAL (i->to.metric().get(), sub::MetricTime (0, 0, 42, 210));
	
	list<sub::Line>::iterator j = i->lines.begin ();
	BOOST_CHECK (j != i->lines.end ());
	BOOST_CHECK_EQUAL (j->blocks.size(), 1);
	sub::Block b = j->blocks.front ();
	BOOST_CHECK_EQUAL (b.text, "This is a subtitle");
	BOOST_CHECK_EQUAL (b.font, "Arial");
	BOOST_CHECK_EQUAL (b.font_size.points().get(), 48);
	BOOST_CHECK_EQUAL (b.bold, false);
	BOOST_CHECK_EQUAL (b.italic, false);
	BOOST_CHECK_EQUAL (b.underline, false);
	BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.7, 1);
	BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP);
	++j;

	BOOST_CHECK (j != i->lines.end ());
	BOOST_CHECK_EQUAL (j->blocks.size(), 1);
	b = j->blocks.front ();
	BOOST_CHECK_EQUAL (b.text, "and that's a line break");
	BOOST_CHECK_EQUAL (b.font, "Arial");
	BOOST_CHECK_EQUAL (b.font_size.points().get(), 48);
	BOOST_CHECK_EQUAL (b.bold, false);
	BOOST_CHECK_EQUAL (b.italic, false);
	BOOST_CHECK_EQUAL (b.underline, false);
	BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.8, 1);
	BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP);
	++i;

	
	/* Second subtitle */
	
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from.metric().get(), sub::MetricTime (0, 1, 1, 10));
	BOOST_CHECK_EQUAL (i->to.metric().get(), sub::MetricTime (0, 1, 2, 100));
	
	BOOST_CHECK_EQUAL (i->lines.size(), 1);
	sub::Line l = i->lines.front ();
	BOOST_CHECK_EQUAL (l.blocks.size(), 7);
	BOOST_CHECK_CLOSE (l.vertical_position.proportional.get(), 0.7, 1);

	list<sub::Block>::iterator k = l.blocks.begin ();
	
	BOOST_CHECK (k != l.blocks.end ());
	BOOST_CHECK_EQUAL (k->text, "This is some ");
	BOOST_CHECK_EQUAL (k->font, "Arial");
	BOOST_CHECK_EQUAL (k->font_size.points().get(), 48);
	BOOST_CHECK_EQUAL (k->bold, false);
	BOOST_CHECK_EQUAL (k->italic, false);
	BOOST_CHECK_EQUAL (k->underline, false);
	++k;

	BOOST_CHECK (k != l.blocks.end ());
	BOOST_CHECK_EQUAL (k->text, "bold");
	BOOST_CHECK_EQUAL (k->font, "Arial");
	BOOST_CHECK_EQUAL (k->font_size.points().get(), 48);
	BOOST_CHECK_EQUAL (k->bold, true);
	BOOST_CHECK_EQUAL (k->italic, false);
	BOOST_CHECK_EQUAL (k->underline, false);
	++k;

	BOOST_CHECK (k != l.blocks.end ());
	BOOST_CHECK_EQUAL (k->text, " and some ");
	BOOST_CHECK_EQUAL (k->font, "Arial");
	BOOST_CHECK_EQUAL (k->font_size.points().get(), 48);
	BOOST_CHECK_EQUAL (k->bold, false);
	BOOST_CHECK_EQUAL (k->italic, false);
	BOOST_CHECK_EQUAL (k->underline, false);
	++k;

	BOOST_CHECK (k != l.blocks.end ());
	BOOST_CHECK_EQUAL (k->text, "bold italic");
	BOOST_CHECK_EQUAL (k->font, "Arial");
	BOOST_CHECK_EQUAL (k->font_size.points().get(), 48);
	BOOST_CHECK_EQUAL (k->bold, true);
	BOOST_CHECK_EQUAL (k->italic, true);
	BOOST_CHECK_EQUAL (k->underline, false);
	++k;

	BOOST_CHECK (k != l.blocks.end ());
	BOOST_CHECK_EQUAL (k->text, " and some ");
	BOOST_CHECK_EQUAL (k->font, "Arial");
	BOOST_CHECK_EQUAL (k->font_size.points().get(), 48);
	BOOST_CHECK_EQUAL (k->bold, false);
	BOOST_CHECK_EQUAL (k->italic, false);
	BOOST_CHECK_EQUAL (k->underline, false);
	++k;

	BOOST_CHECK (k != l.blocks.end ());
	BOOST_CHECK_EQUAL (k->text, "underlined");
	BOOST_CHECK_EQUAL (k->font, "Arial");
	BOOST_CHECK_EQUAL (k->font_size.points().get(), 48);
	BOOST_CHECK_EQUAL (k->bold, false);
	BOOST_CHECK_EQUAL (k->italic, false);
	BOOST_CHECK_EQUAL (k->underline, true);
	++k;

	BOOST_CHECK (k != l.blocks.end ());
	BOOST_CHECK_EQUAL (k->text, ".");
	BOOST_CHECK_EQUAL (k->font, "Arial");
	BOOST_CHECK_EQUAL (k->font_size.points().get(), 48);
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
	list<sub::Subtitle> subs = sub::collect (reader.subtitles ());

	list<sub::Subtitle>::const_iterator i = subs.begin();

	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from.metric().get(), sub::MetricTime (0, 1, 49, 200));
	BOOST_CHECK_EQUAL (i->to.metric().get(), sub::MetricTime (0, 1, 52, 351));
	BOOST_CHECK_EQUAL (i->lines.size(), 2);
	BOOST_CHECK_EQUAL (i->lines.front().blocks.front().text, "This is a subtitle, and it goes ");
	BOOST_CHECK_EQUAL (i->lines.back().blocks.front().text, "over two lines.");

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from.metric().get(), sub::MetricTime (0, 1, 52, 440));
	BOOST_CHECK_EQUAL (i->to.metric().get(), sub::MetricTime (0, 1, 54, 351));
	BOOST_CHECK_EQUAL (i->lines.size(), 1);
	BOOST_CHECK_EQUAL (i->lines.front().blocks.front().text, "We have emboldened this");
	BOOST_CHECK_EQUAL (i->lines.front().blocks.front().bold, true);

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from.metric().get(), sub::MetricTime (0, 1, 54, 440));
	BOOST_CHECK_EQUAL (i->to.metric().get(), sub::MetricTime (0, 1, 56, 590));
	BOOST_CHECK_EQUAL (i->lines.size(), 1);
	BOOST_CHECK_EQUAL (i->lines.front().blocks.front().text, "And italicised this.");
	BOOST_CHECK_EQUAL (i->lines.front().blocks.front().italic, true);

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from.metric().get(), sub::MetricTime (0, 1, 56, 680));
	BOOST_CHECK_EQUAL (i->to.metric().get(), sub::MetricTime (0, 1, 58, 955));
	BOOST_CHECK_EQUAL (i->lines.size(), 1);
	BOOST_CHECK_EQUAL (i->lines.front().blocks.front().text, "Shall I compare thee to a summers' day?");

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from.metric().get(), sub::MetricTime (0, 2, 0, 840));
	BOOST_CHECK_EQUAL (i->to.metric().get(), sub::MetricTime (0, 2, 3, 400));
	BOOST_CHECK_EQUAL (i->lines.size(), 1);
	BOOST_CHECK_EQUAL (i->lines.front().blocks.front().text, "Is this a dagger I see before me?");

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from.metric().get(), sub::MetricTime (0, 3, 54, 560));
	BOOST_CHECK_EQUAL (i->to.metric().get(), sub::MetricTime (0, 3, 56, 471));
	BOOST_CHECK_EQUAL (i->lines.size(), 1);
	BOOST_CHECK_EQUAL (i->lines.front().blocks.front().text, "Hello world.");

	++i;
	BOOST_CHECK (i == subs.end ());
}

/** Test SubripReader::convert_line */
BOOST_AUTO_TEST_CASE (subrip_reader_convert_line_test)
{
	sub::SubripReader r;
	
	r.convert_line ("Hello world", 0, sub::TimePair (), sub::TimePair ());
	BOOST_CHECK_EQUAL (r._subs.size(), 1);
	BOOST_CHECK_EQUAL (r._subs.front().text, "Hello world");
	r._subs.clear ();

	r.convert_line ("<b>Hello world</b>", 0, sub::TimePair (), sub::TimePair ());
	BOOST_CHECK_EQUAL (r._subs.size(), 1);
	BOOST_CHECK_EQUAL (r._subs.front().text, "Hello world");
	BOOST_CHECK_EQUAL (r._subs.front().bold, true);
	r._subs.clear ();

	r.convert_line ("<i>Hello world</i>", 0, sub::TimePair (), sub::TimePair ());
	BOOST_CHECK_EQUAL (r._subs.size(), 1);
	BOOST_CHECK_EQUAL (r._subs.front().text, "Hello world");
	BOOST_CHECK_EQUAL (r._subs.front().italic, true);
	r._subs.clear ();

	r.convert_line ("<u>Hello world</u>", 0, sub::TimePair (), sub::TimePair ());
	BOOST_CHECK_EQUAL (r._subs.size(), 1);
	BOOST_CHECK_EQUAL (r._subs.front().text, "Hello world");
	BOOST_CHECK_EQUAL (r._subs.front().underline, true);
	r._subs.clear ();

	r.convert_line ("{b}Hello world{/b}", 0, sub::TimePair (), sub::TimePair ());
	BOOST_CHECK_EQUAL (r._subs.size(), 1);
	BOOST_CHECK_EQUAL (r._subs.front().text, "Hello world");
	BOOST_CHECK_EQUAL (r._subs.front().bold, true);
	r._subs.clear ();

	r.convert_line ("{i}Hello world{/i}", 0, sub::TimePair (), sub::TimePair ());
	BOOST_CHECK_EQUAL (r._subs.size(), 1);
	BOOST_CHECK_EQUAL (r._subs.front().text, "Hello world");
	BOOST_CHECK_EQUAL (r._subs.front().italic, true);
	r._subs.clear ();

	r.convert_line ("{u}Hello world{/u}", 0, sub::TimePair (), sub::TimePair ());
	BOOST_CHECK_EQUAL (r._subs.size(), 1);
	BOOST_CHECK_EQUAL (r._subs.front().text, "Hello world");
	BOOST_CHECK_EQUAL (r._subs.front().underline, true);
	r._subs.clear ();

	r.convert_line ("<b>This is <i>nesting</i> of subtitles</b>", 0, sub::TimePair (), sub::TimePair ());
	BOOST_CHECK_EQUAL (r._subs.size(), 3);
	list<sub::RawSubtitle>::iterator i = r._subs.begin ();	
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
}


/** Test SubripReader::convert_time */
BOOST_AUTO_TEST_CASE (subrip_reader_convert_time_test)
{
	BOOST_CHECK_EQUAL (sub::SubripReader::convert_time ("00:03:10,500"), sub::TimePair (sub::MetricTime (0, 3, 10, 500)));
	BOOST_CHECK_EQUAL (sub::SubripReader::convert_time ("04:19:51,782"), sub::TimePair (sub::MetricTime (4, 19, 51, 782)));
}


static void
test (boost::filesystem::path p)
{
	p = private_test / p;
	FILE* f = fopen (p.string().c_str(), "r");
	sub::SubripReader r (f);
	fclose (f);
}

/** Test of reading some typical .srt files */
BOOST_AUTO_TEST_CASE (subrip_read_test)
{
	test ("sintel_en.srt");
	test ("Fight.Club.1999.720p.BRRip.x264-x0r.srt");
}
