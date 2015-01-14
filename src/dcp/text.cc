/*
    Copyright (C) 2012-2014 Carl Hetherington <cth@carlh.net>

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

/** @file  src/text.cc
 *  @brief Text class for parsing DCP subtitle XML.
 */

#include "../xml.h"
#include "text.h"
#include "font.h"
#include <libcxml/cxml.h>
#include <boost/foreach.hpp>

using std::string;
using std::list;
using boost::shared_ptr;
using boost::optional;
using namespace sub;

/** Read a &lt;Text&gt; node from a subtitle XML file, noting its contents
 *  in this object's member variables.
 *  @param node Node to read.
 */
dcp::Text::Text (boost::shared_ptr<const cxml::Node> node)
	: v_align (CENTRE_OF_SCREEN)
{
	optional<float> x = node->optional_number_attribute<float> ("VPosition");
	if (!x) {
		x = node->number_attribute<float> ("Vposition");
	}
	v_position = x.get ();
	
	optional<string> v = node->optional_string_attribute ("VAlign");
	if (!v) {
		v = node->optional_string_attribute ("Valign");
	}
	
	if (v) {
		if (v.get() == "top") {
			v_align = TOP_OF_SCREEN;
		} else if (v.get() == "center") {
			v_align = CENTRE_OF_SCREEN;
		} else if (v.get() == "bottom") {
			v_align = BOTTOM_OF_SCREEN;
		} else {
			boost::throw_exception (DCPError ("unknown subtitle valign type"));
		}
	}
}
