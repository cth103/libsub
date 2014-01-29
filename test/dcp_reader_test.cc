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

#include "dcp_reader.h"
#include <boost/test/unit_test.hpp>
#include <fstream>

using std::list;
using std::cout;
using std::ifstream;
using boost::shared_ptr;

/* Test reading of a DCP XML file */
BOOST_AUTO_TEST_CASE (dcp_reader_test1)
{
	ifstream file ("test/data/test1.xml");
	sub::DCPReader reader (file);
	list<sub::Subtitle> subs = reader.subtitles ();

	list<sub::Subtitle>::iterator i = subs.begin ();

	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, false);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (39) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric, sub::MetricTime (0, 0, 5, 198 * 4));
	BOOST_CHECK_EQUAL (i->to.metric, sub::MetricTime (0, 0, 7, 115 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.15, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::BOTTOM);
	BOOST_CHECK_EQUAL (i->text, "My jacket was Idi Amin's");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 4));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 4));

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, true);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (39) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric, sub::MetricTime (0, 0, 7, 177 * 4));
	BOOST_CHECK_EQUAL (i->to.metric, sub::MetricTime (0, 0, 11, 31 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.21, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::BOTTOM);
	BOOST_CHECK_EQUAL (i->text, "My corset was H.M. The Queen's");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 4));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 4));

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, false);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (39) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric, sub::MetricTime (0, 0, 7, 177 * 4));
	BOOST_CHECK_EQUAL (i->to.metric, sub::MetricTime (0, 0, 11, 31 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.15, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::BOTTOM);
	BOOST_CHECK_EQUAL (i->text, "My large wonderbra");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 4));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 4));

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, false);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (39) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric, sub::MetricTime (0, 0, 11, 94 * 4));
	BOOST_CHECK_EQUAL (i->to.metric, sub::MetricTime (0, 0, 13, 63 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.15, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::BOTTOM);
	BOOST_CHECK_EQUAL (i->text, "Once belonged to the Shah");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 4));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 4));

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, false);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (39) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric, sub::MetricTime (0, 0, 13, 104 * 4));
	BOOST_CHECK_EQUAL (i->to.metric, sub::MetricTime (0, 0, 15, 177 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.15, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::BOTTOM);
	BOOST_CHECK_EQUAL (i->text, "And these are Roy Hattersley's jeans");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 4));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 4));
}

/* And another one */
BOOST_AUTO_TEST_CASE (dcp_reader_test2)
{
	ifstream file ("test/data/test2.xml");
	sub::DCPReader reader (file);
	list<sub::Subtitle> subs = reader.subtitles ();

	list<sub::Subtitle>::iterator i = subs.begin ();

	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, true);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (42) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric, sub::MetricTime (0, 0, 41, 62 * 4));
	BOOST_CHECK_EQUAL (i->to.metric, sub::MetricTime (0, 0, 43, 52 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.89, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::TOP);
	BOOST_CHECK_EQUAL (i->text, "At afternoon tea with John Peel");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 0));

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, true);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (42) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric, sub::MetricTime (0, 0, 41, 62 * 4));
	BOOST_CHECK_EQUAL (i->to.metric, sub::MetricTime (0, 0, 43, 52 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.95, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::TOP);
	BOOST_CHECK_EQUAL (i->text, "I enquired if his accent was real");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 0));
	
	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, true);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (42) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric, sub::MetricTime (0, 0, 50, 42 * 4));
	BOOST_CHECK_EQUAL (i->to.metric, sub::MetricTime (0, 0, 52, 21 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.89, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::TOP);
	BOOST_CHECK_EQUAL (i->text, "He said \"out of the house");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 0));

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, true);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (42) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric, sub::MetricTime (0, 0, 50, 42 * 4));
	BOOST_CHECK_EQUAL (i->to.metric, sub::MetricTime (0, 0, 52, 21 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.95, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::TOP);
	BOOST_CHECK_EQUAL (i->text, "I'm incredibly scouse");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 0));

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, true);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (42) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric.get(), sub::MetricTime (0, 1, 2, 208 * 4));
	BOOST_CHECK_EQUAL (i->to.metric.get(), sub::MetricTime (0, 1, 04, 10 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.89, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::TOP);
	BOOST_CHECK_EQUAL (i->text, "At home it depends how I feel.\"");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 0));

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, true);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (42) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric.get(), sub::MetricTime (0, 1, 2, 208 * 4));
	BOOST_CHECK_EQUAL (i->to.metric.get(), sub::MetricTime (0, 1, 4, 10 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.95, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::TOP);
	BOOST_CHECK_EQUAL (i->text, "I spent a long weekend in Brighton");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 0));

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, true);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (42) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric.get(), sub::MetricTime (0, 1, 15, 42 * 4));
	BOOST_CHECK_EQUAL (i->to.metric.get(), sub::MetricTime (0, 1, 16, 42 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.89, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::TOP);
	BOOST_CHECK_EQUAL (i->text, "With the legendary Miss Enid Blyton");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 0));

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, true);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (42) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric.get(), sub::MetricTime (0, 1, 15, 42 * 4));
	BOOST_CHECK_EQUAL (i->to.metric.get(), sub::MetricTime (0, 1, 16, 42 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.95, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::TOP);
	BOOST_CHECK_EQUAL (i->text, "She said \"you be Noddy");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 0));

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, true);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (42) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric.get(), sub::MetricTime (0, 1, 20, 219 * 4));
	BOOST_CHECK_EQUAL (i->to.metric.get(), sub::MetricTime (0, 1, 22, 73 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.89, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::TOP);
	BOOST_CHECK_EQUAL (i->text, "and I'll show you my body\"");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 0));	

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, true);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (42) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric.get(), sub::MetricTime (0, 1, 20, 219 * 4));
	BOOST_CHECK_EQUAL (i->to.metric.get(), sub::MetricTime (0, 1, 22, 73 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.95, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::TOP);
	BOOST_CHECK_EQUAL (i->text, "But Big Ears kept turning the light on.");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 0));
	
	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, true);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (42) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric.get(), sub::MetricTime (0, 1, 27, 115 * 4));
	BOOST_CHECK_EQUAL (i->to.metric.get(), sub::MetricTime (0, 1, 28, 208 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.89, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::TOP);
	BOOST_CHECK_EQUAL (i->text, "That curious creature the Sphinx");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 0));

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, true);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (42) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric.get(), sub::MetricTime (0, 1, 27, 115 * 4));
	BOOST_CHECK_EQUAL (i->to.metric.get(), sub::MetricTime (0, 1, 28, 208 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.95, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::TOP);
	BOOST_CHECK_EQUAL (i->text, "Is smarter than anyone thinks");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 0));

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, false);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (42) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric.get(), sub::MetricTime (0, 1, 42, 229 * 4));
	BOOST_CHECK_EQUAL (i->to.metric.get(), sub::MetricTime (0, 1, 45, 62 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.89, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::TOP);
	BOOST_CHECK_EQUAL (i->text, "It sits there and smirks");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 0));

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, false);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (42) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric.get(), sub::MetricTime (0, 1, 42, 229 * 4));
	BOOST_CHECK_EQUAL (i->to.metric.get(), sub::MetricTime (0, 1, 45, 62 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.95, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::TOP);
	BOOST_CHECK_EQUAL (i->text, "And you don't think it works");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 0));

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, false);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (42) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric.get(), sub::MetricTime (0, 1, 45, 146 * 4));
	BOOST_CHECK_EQUAL (i->to.metric.get(), sub::MetricTime (0, 1, 47, 94 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.89, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::TOP);
	BOOST_CHECK_EQUAL (i->text, "Then when you're not looking, it winks.");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 0));

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, false);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (42) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric.get(), sub::MetricTime (0, 1, 45, 146 * 4));
	BOOST_CHECK_EQUAL (i->to.metric.get(), sub::MetricTime (0, 1, 47, 94 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.95, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::TOP);
	BOOST_CHECK_EQUAL (i->text, "When it snows you will find Sister Sledge");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 0));

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, false);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (42) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric.get(), sub::MetricTime (0, 1, 47, 146 * 4));
	BOOST_CHECK_EQUAL (i->to.metric.get(), sub::MetricTime (0, 1, 48, 167 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.89, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::TOP);
	BOOST_CHECK_EQUAL (i->text, "Out mooning, at night, on the ledge");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 0));

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, false);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (42) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric.get(), sub::MetricTime (0, 1, 47, 146 * 4));
	BOOST_CHECK_EQUAL (i->to.metric.get(), sub::MetricTime (0, 1, 48, 167 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.95, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::TOP);
	BOOST_CHECK_EQUAL (i->text, "One storey down");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 0));

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, false);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (42) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric.get(), sub::MetricTime (0, 1, 53, 21 * 4));
	BOOST_CHECK_EQUAL (i->to.metric.get(), sub::MetricTime (0, 1, 56, 10 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.89, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::TOP);
	BOOST_CHECK_EQUAL (i->text, "Is the maestro, James Brown");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 0));

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, false);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (42) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric.get(), sub::MetricTime (0, 1, 53, 21 * 4));
	BOOST_CHECK_EQUAL (i->to.metric.get(), sub::MetricTime (0, 1, 56, 10 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.95, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::TOP);
	BOOST_CHECK_EQUAL (i->text, "Displaying his meat and two veg.");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 0));
	
	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, true);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (42) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric.get(), sub::MetricTime (0, 2, 5, 208 * 4));
	BOOST_CHECK_EQUAL (i->to.metric.get(), sub::MetricTime (0, 2, 7, 31 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.89, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::TOP);
	BOOST_CHECK_EQUAL (i->text, "HELLO");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 0));

	++i;
	BOOST_CHECK (i != subs.end ());
	BOOST_CHECK_EQUAL (i->font, "Arial");
	BOOST_CHECK_EQUAL (i->italic, true);
	BOOST_CHECK (i->colour == sub::Colour (1, 1, 1));
	BOOST_CHECK_EQUAL (i->font_size.proportional.get(), float (42) / (72 * 11));
	BOOST_CHECK_EQUAL (i->from.metric.get(), sub::MetricTime (0, 2, 5, 208 * 4));
	BOOST_CHECK_EQUAL (i->to.metric.get(), sub::MetricTime (0, 2, 7, 31 * 4));
	BOOST_CHECK_CLOSE (i->vertical_position.proportional.get(), 0.95, 1);
	BOOST_CHECK_EQUAL (i->vertical_position.reference.get(), sub::TOP);
	BOOST_CHECK_EQUAL (i->text, "WORLD");
	BOOST_CHECK_EQUAL (i->effect, sub::BORDER);
	BOOST_CHECK (i->effect_colour.get() == sub::Colour (0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_up.get(), sub::MetricTime (0, 0, 0, 0));
	BOOST_CHECK_EQUAL (i->fade_down.get(), sub::MetricTime (0, 0, 0, 0));
}
