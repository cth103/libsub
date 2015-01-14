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

#include "stl_binary_writer.h"
#include "subtitle.h"
#include <boost/test/unit_test.hpp>

using std::list;

/** Test writing of a binary STL file */
BOOST_AUTO_TEST_CASE (stl_binary_writer_test)
{
	list<sub::Subtitle> subs;

	{
		sub::Subtitle s;
		s.from.set_frame (sub::FrameTime (0, 0, 41, 9));
		s.to.set_frame (sub::FrameTime (0, 0, 42, 21));

		{
			sub::Block b;
			b.text = "This is a subtitle";
			b.font = "Arial";
			b.font_size.set_points (42);
			sub::Line l;
			l.vertical_position.line = 0;
			l.vertical_position.lines = 32;
			l.vertical_position.reference = sub::TOP_OF_SCREEN;
			l.blocks.push_back (b);
			s.lines.push_back (l);
		}

		{
			sub::Block b;
			b.text = "and that's a line break";
			b.font = "Arial";
			b.font_size.set_points (42);
			sub::Line l;
			l.vertical_position.line = 1;
			l.vertical_position.lines = 32;
			l.vertical_position.reference = sub::TOP_OF_SCREEN;
			l.blocks.push_back (b);
			s.lines.push_back (l);
		}

		subs.push_back (s);
	}

	{
		sub::Subtitle s;
		s.from.set_frame (sub::FrameTime (0, 1, 1, 1));
		s.to.set_frame (sub::FrameTime (0, 1, 2, 10));

		sub::Line l;
		l.vertical_position.line = 0;
		l.vertical_position.lines = 32;
		l.vertical_position.reference = sub::TOP_OF_SCREEN;
		
		sub::Block b;
		b.text = "This is some ";
		b.font = "Arial";
		b.font_size.set_points (42);
		l.blocks.push_back (b);

		b.text = "underline";
		b.underline = true;
		l.blocks.push_back (b);

		b.text = " and some ";
		b.underline = false;
		l.blocks.push_back (b);

		b.text = "underlined italic";
		b.underline = true;
		b.italic = true;
		l.blocks.push_back (b);

		s.lines.push_back (l);
		subs.push_back (s);
	}

	sub::write_stl_binary (
		subs,
		25,
		sub::LANGUAGE_GERMAN,
		"Original programme title",
		"Original episode title",
		"TX program title",
		"TX episode title",
		"TX name",
		"TX contact",
		"140212",
		"140213",
		0,
		"GBR",
		"Publisher",
		"Editor name",
		"Editor contact",
		"build/test/test.stl"
		);
		
}

