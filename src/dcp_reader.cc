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
#include "vertical_reference.h"
#include "xml.h"
#include "util.h"
#include "dcp/font.h"
#include "dcp/text.h"
#include "dcp/subtitle.h"
#include <libcxml/cxml.h>
#include <libxml++/libxml++.h>
#include <iostream>

using std::list;
using std::cout;
using std::string;
using boost::shared_ptr;
using boost::optional;
using namespace sub;

void
DCPReader::parse_common (cxml::NodePtr root, optional<int> tcr)
{
	_reel_number = root->string_child ("ReelNumber");
	_language = root->string_child ("Language");

	ParseState parse_state;
	parse_node (root->node(), parse_state, tcr);
}

void
DCPReader::parse_node (xmlpp::Node const * node, ParseState& parse_state, optional<int> tcr)
{
	xmlpp::Node::NodeList children = node->get_children ();
	for (xmlpp::Node::NodeList::iterator i = children.begin(); i != children.end(); ++i) {
		xmlpp::ContentNode const * c = dynamic_cast<xmlpp::ContentNode const *> (*i);
		if (c) {
			maybe_add_subtitle (c->get_content (), parse_state);
		}

		xmlpp::Element* e = dynamic_cast<xmlpp::Element *> (*i);
		if (e) {
			cxml::NodePtr n (new cxml::Node (e));
			if (n->name() == "Font") {
				parse_state.font_nodes.push_back (shared_ptr<dcp::Font> (new dcp::Font (n)));
				parse_node (e, parse_state, tcr);
				parse_state.font_nodes.pop_back ();
			} else if (n->name() == "Text") {
				parse_state.text_nodes.push_back (shared_ptr<dcp::Text> (new dcp::Text (n)));
				parse_node (e, parse_state, tcr);
				parse_state.text_nodes.pop_back ();
			} else if (n->name() == "Subtitle") {
				parse_state.subtitle_nodes.push_back (shared_ptr<dcp::Subtitle> (new dcp::Subtitle (n, tcr)));
				parse_node (e, parse_state, tcr);
				parse_state.subtitle_nodes.pop_back ();
			} else if (n->name() == "SubtitleList") {
				parse_node (e, parse_state, tcr);
			}
		}
	}
}

void
DCPReader::maybe_add_subtitle (string text, ParseState const & parse_state)
{
	if (empty_or_white_space (text)) {
		return;
	}

	if (parse_state.text_nodes.empty() || parse_state.subtitle_nodes.empty ()) {
		return;
	}

	dcp::Font effective_font (parse_state.font_nodes);
	dcp::Text effective_text (*parse_state.text_nodes.back ());
	dcp::Subtitle effective_subtitle (*parse_state.subtitle_nodes.back ());

	RawSubtitle rs;

	rs.text = text;
	rs.font = effective_font.id;
	rs.font_size.set_proportional (float (effective_font.size) / (72 * 11));
	rs.effect = effective_font.effect;
	rs.effect_colour = effective_font.effect_colour;
	rs.colour = effective_font.colour.get();
	rs.bold = false;
	rs.italic = effective_font.italic.get();
	rs.underline = false;
	rs.vertical_position.proportional = float (effective_text.v_position) / 100;
	rs.vertical_position.reference = effective_text.v_align;
	rs.from = effective_subtitle.in;
	rs.to = effective_subtitle.out;
	rs.fade_up = effective_subtitle.fade_up_time;
	rs.fade_down = effective_subtitle.fade_down_time;

	_subs.push_back (rs);
}
