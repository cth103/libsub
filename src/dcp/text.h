/*
    Copyright (C) 2012-2015 Carl Hetherington <cth@carlh.net>

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

/** @file  src/text.h
 *  @brief Text class for parsing DCP subtitle XML.
 */

#include "../vertical_reference.h"
#include <boost/shared_ptr.hpp>
#include <list>

namespace cxml {
	class Node;
}

namespace sub {
namespace dcp {

class Font;

/** @class Text
 *  @brief Parser for Text nodes from subtitle XML.
 */
class Text
{
public:
	/** Construct a default text node */
	Text ()
		: v_position (0)
		, v_align (TOP_OF_SCREEN)
	{}
	
	Text (boost::shared_ptr<const cxml::Node> node);

	float v_position;
	VerticalReference v_align;
};

}

}
