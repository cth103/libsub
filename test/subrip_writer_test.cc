/*
    Copyright (C) 2026 Carl Hetherington <cth@carlh.net>

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

#include "subrip_writer.h"
#include "subtitle.h"
#include <boost/test/unit_test.hpp>
#include <fstream>


using std::vector;
using std::string;
using std::ifstream;
using std::getline;


/** Test writing of a SubRip file */
BOOST_AUTO_TEST_CASE(subrip_writer_test)
{
	vector<sub::Subtitle> subs;

	{
		sub::Subtitle s;
		s.from = sub::Time::from_hms(0, 0, 41, 90);
		s.to = sub::Time::from_hms(0, 0, 42, 210);

		{
			sub::Block b;
			b.text = "This is a subtitle";
			sub::Line l;
			l.blocks.push_back(b);
			s.lines.push_back(l);
		}

		{
			sub::Block b;
			b.text = "and that's a line break";
			sub::Line l;
			l.blocks.push_back(b);
			s.lines.push_back(l);
		}

		subs.push_back(s);
	}

	{
		sub::Subtitle s;
		s.from = sub::Time::from_hms(0, 1, 1, 10);
		s.to = sub::Time::from_hms(0, 1, 2, 100);

		sub::Line l;

		sub::Block b;
		b.text = "This is some ";
		l.blocks.push_back(b);

		b.text = "bold";
		b.bold = true;
		l.blocks.push_back(b);

		b.text = " and some ";
		b.bold = false;
		l.blocks.push_back(b);

		b.text = "underlined";
		b.underline = true;
		l.blocks.push_back(b);

		s.lines.push_back(l);
		subs.push_back(s);
	}

	sub::write_subrip(subs, "build/test/test_subrip_writer.srt");

	ifstream f("build/test/test_subrip_writer.srt");
	string line;
	vector<string> lines;
	while (getline(f, line)) {
		lines.push_back(line);
	}
	f.close();

	BOOST_CHECK_EQUAL(lines[0], "1");
	BOOST_CHECK_EQUAL(lines[1], "00:00:41,090 --> 00:00:42,210");
	BOOST_CHECK_EQUAL(lines[2], "This is a subtitle");
	BOOST_CHECK_EQUAL(lines[3], "and that's a line break");
	BOOST_CHECK_EQUAL(lines[4], "");

	BOOST_CHECK_EQUAL(lines[5], "2");
	BOOST_CHECK_EQUAL(lines[6], "00:01:01,010 --> 00:01:02,100");
	BOOST_CHECK_EQUAL(lines[7], "This is some <b>bold</b> and some <u>underlined</u>");
}
