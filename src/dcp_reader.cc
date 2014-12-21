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
#include "vertical_reference.h"
#include "xml.h"
#include <libcxml/cxml.h>
#include <dcp/interop_subtitle_content.h>
#include <dcp/smpte_subtitle_content.h>
#include <dcp/subtitle.h>

using std::list;
using std::cout;
using boost::shared_ptr;
using namespace sub;

static MetricTime
dcp_to_metric (dcp::Time t)
{
	return MetricTime (t.h, t.m, t.s, t.t * 4);
}

static Colour
dcp_to_colour (dcp::Color c)
{
	return Colour (float (c.r) / 255, float (c.g) / 255, float (c.b) / 255);
}

/** @class DCPReader
 *  @brief A class to read DCP subtitles.
 */
DCPReader::DCPReader (boost::filesystem::path file, bool interop)
{
	shared_ptr<dcp::SubtitleContent> content;
	if (interop) {
		content.reset (new dcp::InteropSubtitleContent (file));
	} else {
		content.reset (new dcp::SMPTESubtitleContent (file));
	}
	
	list<dcp::SubtitleString> subs = content->subtitles ();
	for (list<dcp::SubtitleString>::const_iterator i = subs.begin(); i != subs.end(); ++i) {
		RawSubtitle sub;

		sub.vertical_position.proportional = float (i->v_position ()) / 100;
		switch (i->v_align ()) {
		case dcp::TOP:
			sub.vertical_position.reference = TOP_OF_SCREEN;
			break;
		case dcp::CENTER:
			sub.vertical_position.reference = CENTRE_OF_SCREEN;
			break;
		case dcp::BOTTOM:
			sub.vertical_position.reference = BOTTOM_OF_SCREEN;
			break;
		}
			
		sub.from.set_metric (dcp_to_metric (i->in ()));
		sub.to.set_metric (dcp_to_metric (i->out ()));
		sub.fade_up = dcp_to_metric (i->fade_up_time ());
		sub.fade_down = dcp_to_metric (i->fade_down_time ());
		
		sub.text = i->text ();
		/* XXX: should sub.font be optional? */
		sub.font = i->font().get_value_or ("");
		sub.font_size.set_proportional (float (i->size ()) / (72 * 11));
		switch (i->effect ()) {
		case dcp::NONE:
			break;
		case dcp::BORDER:
			sub.effect = BORDER;
			break;
		case dcp::SHADOW:
			sub.effect = SHADOW;
			break;
		}

		sub.effect_colour = dcp_to_colour (i->effect_color ());
		sub.colour = dcp_to_colour (i->color ());
		sub.italic = i->italic ();
		
		_subs.push_back (sub);
	}
}
