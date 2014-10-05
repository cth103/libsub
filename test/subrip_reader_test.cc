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
#include "collect.h"
#include <boost/test/unit_test.hpp>
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


