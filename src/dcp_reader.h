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

#ifndef LIBSUB_DCP_READER_H
#define LIBSUB_DCP_READER_H

#include "reader.h"
#include <libcxml/cxml.h>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>

namespace sub {

namespace dcp {
	class Font;
	class Text;
	class Subtitle;
}

/** @class DCPReader
 *  @brief A class which reads DCP subtitles.
 */
class DCPReader : public Reader
{
protected:

	struct ParseState {
		std::list<boost::shared_ptr<dcp::Font> > font_nodes;
		std::list<boost::shared_ptr<dcp::Text> > text_nodes;
		std::list<boost::shared_ptr<dcp::Subtitle> > subtitle_nodes;
	};

	void parse_common (cxml::NodePtr root, boost::optional<int> tcr);

	std::string _id;

private:
	void parse_node (xmlpp::Node const * node, ParseState& parse_state, boost::optional<int> tcr);
	void maybe_add_subtitle (std::string text, ParseState const & parse_state);

	std::string _reel_number;
	std::string _language;
};

}

#endif
