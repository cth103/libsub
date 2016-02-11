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

using std::list;

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
