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
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

using std::string;
using std::list;
using std::vector;
using std::istream;
using std::cout;
using boost::shared_ptr;
using boost::optional;
using boost::lexical_cast;
using boost::is_any_of;
using namespace sub;

namespace sub {

class DCPFont;

class DCPText
{
public:
	DCPText ()
		: v_position (0)
		, v_align (TOP)
	{}
	
	DCPText (shared_ptr<const cxml::Node> node)
		: v_align (CENTRE)
	{
		text = node->content ();
		v_position = node->number_attribute<float> ("VPosition");
		optional<string> v = node->optional_string_attribute ("VAlign");
		if (v) {
			v_align = string_to_vertical_reference (v.get ());
		}
		
		font_nodes = type_children<DCPFont> (node, "Font");
	}

	float v_position;
	VerticalReference v_align;
	string text;
	shared_ptr<DCPFont> foo;
	list<shared_ptr<DCPFont> > font_nodes;
};

class DCPSubtitle 
{
public:
	DCPSubtitle () {}
	DCPSubtitle (shared_ptr<const cxml::Node> node)
	{
		in = MetricTime (time (node->string_attribute ("TimeIn")));
		out = MetricTime (time (node->string_attribute ("TimeOut")));
		font_nodes = type_children<DCPFont> (node, "Font");
		text_nodes = type_children<DCPText> (node, "Text");
		fade_up_time = fade_time (node, "FadeUpTime");
		fade_down_time = fade_time (node, "FadeDownTime");
	}

	MetricTime in;
	MetricTime out;
	MetricTime fade_up_time;
	MetricTime fade_down_time;
	list<shared_ptr<DCPFont> > font_nodes;
	list<shared_ptr<DCPText> > text_nodes;

private:
	static MetricTime time (std::string time)
	{
		vector<string> b;
		split (b, time, is_any_of (":"));
		if (b.size() != 4) {
			boost::throw_exception (XMLError ("unrecognised time specification"));
		}

		return MetricTime (lexical_cast<int>(b[0]), lexical_cast<int> (b[1]), lexical_cast<int> (b[2]), lexical_cast<int> (b[3]) * 4);
	}
	
	MetricTime fade_time (shared_ptr<const cxml::Node> node, string name)
	{
		string const u = node->optional_string_attribute (name).get_value_or ("");
		MetricTime t;
		
		if (u.empty ()) {
			t = MetricTime (0, 0, 0, 80);
		} else if (u.find (":") != string::npos) {
			t = time (u);
		} else {
			t = MetricTime (0, 0, 0, lexical_cast<int>(u) * 4);
		}
		
		if (t > MetricTime (0, 0, 8, 0)) {
			t = MetricTime (0, 0, 8, 0);
		}
		
		return t;
	}
};

class DCPFont 
{
public:
	DCPFont ()
		: size (0)
	{}
	
	DCPFont (shared_ptr<const cxml::Node> node)
	{
		text = node->content ();
		
		id = node->optional_string_attribute ("Id").get_value_or ("");
		size = node->optional_number_attribute<int64_t> ("Size").get_value_or (0);
		italic = node->optional_bool_attribute ("Italic");
		optional<string> c = node->optional_string_attribute ("Color");
		if (c) {
			colour = Colour (c.get ());
		}
		optional<string> const e = node->optional_string_attribute ("Effect");
		if (e) {
			effect = string_to_effect (e.get ());
		}
		c = node->optional_string_attribute ( "EffectColor");
		if (c) {
			effect_colour = Colour (c.get ());
		}
		subtitle_nodes = type_children<DCPSubtitle> (node, "Subtitle");
		font_nodes = type_children<DCPFont> (node, "Font");
		text_nodes = type_children<DCPText> (node, "Text");
	}
	
	DCPFont (list<shared_ptr<DCPFont> > const & font_nodes)
		: size (0)
		, italic (false)
		, colour ("FFFFFFFF")
		, effect_colour ("FFFFFFFF")
	{
		for (list<shared_ptr<DCPFont> >::const_iterator i = font_nodes.begin(); i != font_nodes.end(); ++i) {
			if (!(*i)->id.empty ()) {
				id = (*i)->id;
			}
			if ((*i)->size != 0) {
				size = (*i)->size;
			}
			if ((*i)->italic) {
				italic = (*i)->italic.get ();
			}
			if ((*i)->colour) {
				colour = (*i)->colour.get ();
			}
			if ((*i)->effect) {
				effect = (*i)->effect.get ();
			}
			if ((*i)->effect_colour) {
				effect_colour = (*i)->effect_colour.get ();
			}
		}
	}

	string text;
	string id;
	int size;
	optional<bool> italic;
	optional<Colour> colour;
	optional<Effect> effect;
	optional<Colour> effect_colour;
	
	list<shared_ptr<DCPSubtitle> > subtitle_nodes;
	list<shared_ptr<DCPFont> > font_nodes;
	list<shared_ptr<DCPText> > text_nodes;
};

class DCPLoadFont 
{
public:
	DCPLoadFont () {}
	DCPLoadFont (shared_ptr<const cxml::Node> node)
	{
		id = node->string_attribute ("Id");
		uri = node->string_attribute ("URI");
	}

	string id;
	string uri;
};

}

/** @param s A string.
 *  @return true if the string contains only space, newline or tab characters, or is empty.
 */
static bool
empty_or_white_space (string s)
{
	for (size_t i = 0; i < s.length(); ++i) {
		if (s[i] != ' ' && s[i] != '\n' && s[i] != '\t') {
			return false;
		}
	}

	return true;
}

string
DCPReader::font_id_to_name (string id) const
{
	list<shared_ptr<DCPLoadFont> >::const_iterator i = _load_font_nodes.begin();
	while (i != _load_font_nodes.end() && (*i)->id != id) {
		++i;
	}

	if (i == _load_font_nodes.end ()) {
		return "";
	}

	if ((*i)->uri == "arial.ttf") {
		return "Arial";
	}

	return "";
}

DCPReader::DCPReader (istream& in)
{
	shared_ptr<cxml::Document> xml (new cxml::Document ("DCSubtitle"));
	xml->read_stream (in);

	xml->ignore_child ("SubtitleID");
	xml->ignore_child ("MovieTitle");
	xml->ignore_child ("ReelNumber");
	xml->ignore_child ("Language");

	list<shared_ptr<DCPFont> > font_nodes = type_children<DCPFont> (xml, "Font");
	_load_font_nodes = type_children<DCPLoadFont> (xml, "LoadFont");
	
	/* Now make Subtitle objects to represent the raw XML nodes
	   in a sane way.
	*/

	ParseState parse_state;
	examine_font_nodes (xml, font_nodes, parse_state);

	_subs.sort ();
}

void
DCPReader::examine_font_nodes (
	shared_ptr<const cxml::Node> xml,
	list<shared_ptr<DCPFont> > const & font_nodes,
	ParseState& parse_state
	)
{
	for (list<shared_ptr<DCPFont> >::const_iterator i = font_nodes.begin(); i != font_nodes.end(); ++i) {

		parse_state.font_nodes.push_back (*i);
		maybe_add_subtitle ((*i)->text, parse_state);

		for (list<shared_ptr<DCPSubtitle> >::iterator j = (*i)->subtitle_nodes.begin(); j != (*i)->subtitle_nodes.end(); ++j) {
			parse_state.subtitle_nodes.push_back (*j);
			examine_text_nodes (xml, (*j)->text_nodes, parse_state);
			examine_font_nodes (xml, (*j)->font_nodes, parse_state);
			parse_state.subtitle_nodes.pop_back ();
		}
	
		examine_font_nodes (xml, (*i)->font_nodes, parse_state);
		examine_text_nodes (xml, (*i)->text_nodes, parse_state);
		
		parse_state.font_nodes.pop_back ();
	}
}

void
DCPReader::examine_text_nodes (
	shared_ptr<const cxml::Node> xml,
	list<shared_ptr<DCPText> > const & text_nodes,
	ParseState& parse_state
	)
{
	for (list<shared_ptr<DCPText> >::const_iterator i = text_nodes.begin(); i != text_nodes.end(); ++i) {
		parse_state.text_nodes.push_back (*i);
		maybe_add_subtitle ((*i)->text, parse_state);
		examine_font_nodes (xml, (*i)->font_nodes, parse_state);
		parse_state.text_nodes.pop_back ();
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

	assert (!parse_state.text_nodes.empty ());
	assert (!parse_state.subtitle_nodes.empty ());
	
	DCPFont effective_font (parse_state.font_nodes);
	DCPText effective_text (*parse_state.text_nodes.back ());
	DCPSubtitle effective_subtitle (*parse_state.subtitle_nodes.back ());

	Subtitle s;
	s.text = text;
	s.font = font_id_to_name (effective_font.id);
	s.font_size.proportional = float (effective_font.size) / (72 * 11);
	s.vertical_position.proportional = float (effective_text.v_position) / 100;
	s.vertical_position.reference = effective_text.v_align;
	s.effect = effective_font.effect;
	s.effect_colour = effective_font.effect_colour;
	s.colour = effective_font.colour.get ();
	s.italic = effective_font.italic.get ();
	s.from.metric = effective_subtitle.in;
	s.to.metric = effective_subtitle.out;
	s.fade_up = effective_subtitle.fade_up_time;
	s.fade_down = effective_subtitle.fade_down_time;
	_subs.push_back (s);
}
