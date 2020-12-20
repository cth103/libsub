/*
    Copyright (C) 2014-2015 Carl Hetherington <cth@carlh.net>

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

#include "dcp_reader.h"
#include "collect.h"
#include <boost/test/unit_test.hpp>
#include <boost/optional/optional_io.hpp>

using std::list;
using std::shared_ptr;

/* Test reading of a DCP XML file */
BOOST_AUTO_TEST_CASE (dcp_reader_test1)
{
	sub::DCPReader reader ("test/data/test1.xml");
	list<sub::Subtitle> subs = sub::collect<list<sub::Subtitle> > (reader.subtitles ());

	list<sub::Subtitle>::iterator i = subs.begin ();
	BOOST_REQUIRE (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 0, 5, 198 * 4));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 0, 7, 115 * 4));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::Time::from_hms (0, 0, 0, 4));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::Time::from_hms (0, 0, 0, 4));

	{
		list<sub::Line>::iterator j = i->lines.begin ();
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		sub::Block b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFontId");
		BOOST_CHECK_EQUAL (b.italic, false);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (39) / (72 * 11));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.15, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::BOTTOM_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "My jacket was Idi Amin's");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));

		++j;
		BOOST_REQUIRE (j == i->lines.end ());
	}

	++i;
	BOOST_REQUIRE (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 0, 7, 177 * 4));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 0, 11, 31 * 4));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::Time::from_hms (0, 0, 0, 4));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::Time::from_hms (0, 0, 0, 4));

	{
		list<sub::Line>::iterator j = i->lines.begin ();
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		sub::Block b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFontId");
		BOOST_CHECK_EQUAL (b.italic, true);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (39) / (72 * 11));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.21, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::BOTTOM_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "My corset was H.M. The Queen's");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));

		++j;
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFontId");
		BOOST_CHECK_EQUAL (b.italic, false);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (39) / (72 * 11));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.15, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::BOTTOM_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "My large wonderbra");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));

		++j;
		BOOST_REQUIRE (j == i->lines.end ());
	}

	++i;
	BOOST_REQUIRE (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 0, 11, 94 * 4));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 0, 13, 63 * 4));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::Time::from_hms (0, 0, 0, 4));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::Time::from_hms (0, 0, 0, 4));

	{
		list<sub::Line>::iterator j = i->lines.begin ();
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		sub::Block b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFontId");
		BOOST_CHECK_EQUAL (b.italic, false);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (39) / (72 * 11));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.15, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::BOTTOM_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "Once belonged to the Shah");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));

		++j;
		BOOST_REQUIRE (j == i->lines.end ());
	}

	++i;
	BOOST_REQUIRE (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 0, 13, 104 * 4));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 0, 15, 177 * 4));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::Time::from_hms (0, 0, 0, 4));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::Time::from_hms (0, 0, 0, 4));

	{
		list<sub::Line>::iterator j = i->lines.begin ();
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		sub::Block b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFontId");
		BOOST_CHECK_EQUAL (b.italic, false);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (39) / (72 * 11));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.15, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::BOTTOM_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "And these are Roy Hattersley's jeans");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));

		++j;
		BOOST_REQUIRE (j == i->lines.end ());
	}
}

/* And another one */
BOOST_AUTO_TEST_CASE (dcp_reader_test2)
{
	sub::DCPReader reader ("test/data/test2.xml");
	list<sub::Subtitle> subs = sub::collect<list<sub::Subtitle> > (reader.subtitles ());

	list<sub::Subtitle>::iterator i = subs.begin ();
	BOOST_REQUIRE (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 0, 41, 62 * 4));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 0, 43, 52 * 4));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::Time::from_hms (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::Time::from_hms (0, 0, 0, 0));

	{
		list<sub::Line>::iterator j = i->lines.begin ();
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		sub::Block b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFont");
		BOOST_CHECK_EQUAL (b.italic, true);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (42) / (72 * 11));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.89, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "At afternoon tea with John Peel");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));

		++j;
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFont");
		BOOST_CHECK_EQUAL (b.italic, true);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (42) / (72 * 11));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.95, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "I enquired if his accent was real");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));

		++j;
		BOOST_CHECK (j == i->lines.end ());
	}

	++i;
	BOOST_REQUIRE (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 0, 50, 42 * 4));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 0, 52, 21 * 4));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::Time::from_hms (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::Time::from_hms (0, 0, 0, 0));

	{
		list<sub::Line>::iterator j = i->lines.begin ();
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		sub::Block b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFont");
		BOOST_CHECK_EQUAL (b.italic, true);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (42) / (72 * 11));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.89, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "He said \"out of the house");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));

		++j;
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFont");
		BOOST_CHECK_EQUAL (b.italic, true);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (42) / (72 * 11));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.95, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "I'm incredibly scouse");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));

		++j;
		BOOST_CHECK (j == i->lines.end ());
	}

	++i;
	BOOST_REQUIRE (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 1, 2, 208 * 4));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 1, 04, 10 * 4));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::Time::from_hms (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::Time::from_hms (0, 0, 0, 0));

	{
		list<sub::Line>::iterator j = i->lines.begin ();
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		sub::Block b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFont");
		BOOST_CHECK_EQUAL (b.italic, true);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (42) / (72 * 11));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.89, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "At home it depends how I feel.\"");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));

		++j;
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFont");
		BOOST_CHECK_EQUAL (b.italic, true);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (42) / (72 * 11));
		BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 1, 2, 208 * 4));
		BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 1, 4, 10 * 4));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.95, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "I spent a long weekend in Brighton");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));
		BOOST_CHECK_EQUAL (i->fade_up.get(), sub::Time::from_hms (0, 0, 0, 0));
		BOOST_CHECK_EQUAL (i->fade_down.get(), sub::Time::from_hms (0, 0, 0, 0));

		++j;
		BOOST_CHECK (j == i->lines.end ());
	}

	++i;
	BOOST_REQUIRE (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 1, 15, 42 * 4));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 1, 16, 42 * 4));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::Time::from_hms (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::Time::from_hms (0, 0, 0, 0));

	{
		list<sub::Line>::iterator j = i->lines.begin ();
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		sub::Block b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFont");
		BOOST_CHECK_EQUAL (b.italic, true);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (42) / (72 * 11));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.89, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "With the legendary Miss Enid Blyton");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));

		++j;
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFont");
		BOOST_CHECK_EQUAL (b.italic, true);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (42) / (72 * 11));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.95, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "She said \"you be Noddy");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));

		++j;
		BOOST_CHECK (j == i->lines.end ());
	}

	++i;
	BOOST_REQUIRE (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 1, 20, 219 * 4));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 1, 22, 73 * 4));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::Time::from_hms (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::Time::from_hms (0, 0, 0, 0));

	{
		list<sub::Line>::iterator j = i->lines.begin ();
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		sub::Block b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFont");
		BOOST_CHECK_EQUAL (b.italic, true);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (42) / (72 * 11));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.89, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "and I'll show you my body\"");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));

		++j;
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFont");
		BOOST_CHECK_EQUAL (b.italic, true);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (42) / (72 * 11));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.95, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "But Big Ears kept turning the light on.");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));

		++j;
		BOOST_CHECK (j == i->lines.end ());
	}

	++i;
	BOOST_REQUIRE (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 1, 27, 115 * 4));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 1, 28, 208 * 4));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::Time::from_hms (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::Time::from_hms (0, 0, 0, 0));

	{
		list<sub::Line>::iterator j = i->lines.begin ();
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		sub::Block b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFont");
		BOOST_CHECK_EQUAL (b.italic, true);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (42) / (72 * 11));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.89, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "That curious creature the Sphinx");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));

		++j;
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFont");
		BOOST_CHECK_EQUAL (b.italic, true);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (42) / (72 * 11));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.95, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "Is smarter than anyone thinks");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));

		++j;
		BOOST_CHECK (j == i->lines.end ());
	}

	++i;
	BOOST_REQUIRE (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 1, 42, 229 * 4));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 1, 45, 62 * 4));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::Time::from_hms (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::Time::from_hms (0, 0, 0, 0));

	{
		list<sub::Line>::iterator j = i->lines.begin ();
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		sub::Block b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFont");
		BOOST_CHECK_EQUAL (b.italic, false);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (42) / (72 * 11));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.89, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "It sits there and smirks");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));

		++j;
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFont");
		BOOST_CHECK_EQUAL (b.italic, false);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (42) / (72 * 11));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.95, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "And you don't think it works");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));

		++j;
		BOOST_CHECK (j == i->lines.end ());
	}

	++i;
	BOOST_REQUIRE (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 1, 45, 146 * 4));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 1, 47, 94 * 4));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::Time::from_hms (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::Time::from_hms (0, 0, 0, 0));

	{
		list<sub::Line>::iterator j = i->lines.begin ();
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		sub::Block b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFont");
		BOOST_CHECK_EQUAL (b.italic, false);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (42) / (72 * 11));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.89, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "Then when you're not looking, it winks.");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));

		++j;
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFont");
		BOOST_CHECK_EQUAL (b.italic, false);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (42) / (72 * 11));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.95, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "When it snows you will find Sister Sledge");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));

		++j;
		BOOST_CHECK (j == i->lines.end ());
	}

	++i;
	BOOST_REQUIRE (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 1, 47, 146 * 4));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 1, 48, 167 * 4));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::Time::from_hms (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::Time::from_hms (0, 0, 0, 0));

	{
		list<sub::Line>::iterator j = i->lines.begin ();
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		sub::Block b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFont");
		BOOST_CHECK_EQUAL (b.italic, false);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (42) / (72 * 11));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.89, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "Out mooning, at night, on the ledge");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));

		++j;
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFont");
		BOOST_CHECK_EQUAL (b.italic, false);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (42) / (72 * 11));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.95, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "One storey down");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));

		++j;
		BOOST_CHECK (j == i->lines.end ());
	}

	++i;
	BOOST_REQUIRE (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 1, 53, 21 * 4));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 1, 56, 10 * 4));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::Time::from_hms (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::Time::from_hms (0, 0, 0, 0));

	{
		list<sub::Line>::iterator j = i->lines.begin ();
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		sub::Block b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFont");
		BOOST_CHECK_EQUAL (b.italic, false);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (42) / (72 * 11));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.89, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "Is the maestro, James Brown");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));

		++j;
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFont");
		BOOST_CHECK_EQUAL (b.italic, false);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (42) / (72 * 11));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.95, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "Displaying his meat and two veg.");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));

		++j;
		BOOST_CHECK (j == i->lines.end ());
	}

	++i;
	BOOST_REQUIRE (i != subs.end ());
	BOOST_CHECK_EQUAL (i->from, sub::Time::from_hms (0, 2, 5, 208 * 4));
	BOOST_CHECK_EQUAL (i->to, sub::Time::from_hms (0, 2, 7, 31 * 4));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::Time::from_hms (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::Time::from_hms (0, 0, 0, 0));

	{
		list<sub::Line>::iterator j = i->lines.begin ();
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		sub::Block b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFont");
		BOOST_CHECK_EQUAL (b.italic, true);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (42) / (72 * 11));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.89, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "HELLO");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));

		++j;
		BOOST_REQUIRE (j != i->lines.end ());
		BOOST_REQUIRE_EQUAL (j->blocks.size(), 1);
		b = j->blocks.front ();
		BOOST_CHECK_EQUAL (b.font.get(), "theFont");
		BOOST_CHECK_EQUAL (b.italic, true);
		BOOST_CHECK (b.colour == sub::Colour (1, 1, 1));
		BOOST_CHECK_EQUAL (b.font_size.proportional().get(), float (42) / (72 * 11));
		BOOST_CHECK_CLOSE (j->vertical_position.proportional.get(), 0.95, 1);
		BOOST_CHECK_EQUAL (j->vertical_position.reference.get(), sub::TOP_OF_SCREEN);
		BOOST_CHECK_EQUAL (b.text, "WORLD");
		BOOST_CHECK_EQUAL (b.effect, sub::BORDER);
		BOOST_CHECK (b.effect_colour.get() == sub::Colour (0, 0, 0));

		++j;
		BOOST_CHECK (j == i->lines.end ());
	}
}
