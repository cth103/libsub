/*
    Copyright (C) 2022 Carl Hetherington <cth@carlh.net>

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

#include "web_vtt_reader.h"
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


/* Test reading of a VTT file */
BOOST_AUTO_TEST_CASE(vtt_reader_test)
{
	auto f = fopen("test/data/test.vtt", "r");
	sub::WebVTTReader reader(f);
	fclose(f);
	auto subs = sub::collect<std::vector<sub::Subtitle>>(reader.subtitles());

	auto i = subs.begin();


	/* First subtitle */

	BOOST_REQUIRE(i != subs.end());
	BOOST_CHECK_EQUAL(i->from, sub::Time::from_hms(0, 0, 41, 90));
	BOOST_CHECK_EQUAL(i->to, sub::Time::from_hms(0, 0, 42, 210));

	auto j = i->lines.begin();
	BOOST_CHECK(j != i->lines.end());
	BOOST_REQUIRE_EQUAL(j->blocks.size(), 1U);
	auto b = j->blocks[0];
	BOOST_CHECK_EQUAL(b.text, "This is a subtitle");
	/* No font is specified by WebVTT, so none should be seen here */
	BOOST_CHECK(!b.font);
	BOOST_CHECK(!b.font_size.specified());
	BOOST_CHECK_EQUAL(b.bold, false);
	BOOST_CHECK_EQUAL(b.italic, false);
	BOOST_CHECK_EQUAL(b.underline, false);
	BOOST_REQUIRE(j->vertical_position.line);
	BOOST_CHECK_EQUAL(j->vertical_position.line.get(), 0);
	BOOST_CHECK_EQUAL(j->vertical_position.reference.get(), sub::TOP_OF_SUBTITLE);
	++j;

	BOOST_CHECK(j != i->lines.end());
	BOOST_REQUIRE_EQUAL(j->blocks.size(), 1U);
	b = j->blocks[0];
	BOOST_CHECK_EQUAL(b.text, "and that's a line break");
	/* No font is specified by WebVTT, so none should be seen here */
	BOOST_CHECK(!b.font);
	BOOST_CHECK(!b.font_size.specified());
	BOOST_CHECK_EQUAL(b.bold, false);
	BOOST_CHECK_EQUAL(b.italic, false);
	BOOST_CHECK_EQUAL(b.underline, false);
	BOOST_REQUIRE(j->vertical_position.line);
	BOOST_CHECK_EQUAL(j->vertical_position.line.get(), 1);
	BOOST_CHECK_EQUAL(j->vertical_position.reference.get(), sub::TOP_OF_SUBTITLE);
	++i;


	/* Second subtitle */

	BOOST_REQUIRE(i != subs.end());
	BOOST_CHECK_EQUAL(i->from, sub::Time::from_hms(0, 1, 1, 10));
	BOOST_CHECK_EQUAL(i->to, sub::Time::from_hms(0, 1, 2, 100));

	BOOST_CHECK_EQUAL(i->lines.size(), 1U);
	sub::Line l = i->lines[0];
	BOOST_CHECK_EQUAL(l.blocks.size(), 1U);
	BOOST_CHECK_EQUAL(l.vertical_position.line.get(), 0);
	BOOST_CHECK_EQUAL(l.vertical_position.reference.get(), sub::TOP_OF_SUBTITLE);

	BOOST_REQUIRE_EQUAL(l.blocks.size(), 1U);
	b = l.blocks[0];
	BOOST_CHECK_EQUAL(b.text, "This is some stuff.");
	/* No font is specified by WebVTT, so none should be seen here */
	BOOST_CHECK(!b.font);
	BOOST_CHECK(!b.font_size.specified());
	BOOST_CHECK_EQUAL(b.bold, false);
	BOOST_CHECK_EQUAL(b.italic, false);
	BOOST_CHECK_EQUAL(b.underline, false);
}

