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

#include "reader.h"
#include <boost/shared_ptr.hpp>

namespace cxml {
	class Node;
}

namespace sub {

class DCPFont;	
class DCPText;
class DCPSubtitle;
class DCPLoadFont;

class DCPReader : public Reader
{
public:
	DCPReader (std::istream &);

private:
	
	struct ParseState {
		std::list<boost::shared_ptr<DCPFont> > font_nodes;
		std::list<boost::shared_ptr<DCPText> > text_nodes;
		std::list<boost::shared_ptr<DCPSubtitle> > subtitle_nodes;
	};

	void maybe_add_subtitle (std::string text, ParseState const & parse_state);
	
	void examine_font_nodes (
		boost::shared_ptr<const cxml::Node> xml,
		std::list<boost::shared_ptr<DCPFont> > const & font_nodes,
		ParseState& parse_state
		);
	
	void examine_text_nodes (
		boost::shared_ptr<const cxml::Node> xml,
		std::list<boost::shared_ptr<DCPText> > const & text_nodes,
		ParseState& parse_state
		);

	std::string font_id_to_name (std::string id) const;

	std::list<boost::shared_ptr<DCPLoadFont> > _load_font_nodes;
};

}
