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

#include "interop_dcp_reader.h"
#include "dcp/interop_load_font.h"
#include "xml.h"
#include "dcp/font.h"
#include <libcxml/cxml.h>
#include <boost/foreach.hpp>

using std::list;
using boost::shared_ptr;
using boost::optional;
using namespace sub;

InteropDCPReader::InteropDCPReader (boost::filesystem::path file)
{
	shared_ptr<cxml::Document> xml (new cxml::Document ("DCSubtitle"));
	xml->read_file (file);
	_id = xml->string_child ("SubtitleID");

	_movie_title = xml->string_child ("MovieTitle");
	_load_font_nodes = type_children<dcp::InteropLoadFont> (xml, "LoadFont");

	parse_common (xml, optional<int> ());
}
