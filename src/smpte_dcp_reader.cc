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

#include "smpte_dcp_reader.h"
#include "exceptions.h"
#include "xml.h"
#include "dcp/font.h"
#include "dcp/smpte_load_font.h"
#include <asdcp/KM_util.h>
#include <asdcp/AS_DCP.h>
#include <libcxml/cxml.h>
#include <boost/foreach.hpp>

using std::string;
using std::list;
using std::stringstream;
using boost::shared_ptr;
using namespace sub;

SMPTEDCPReader::SMPTEDCPReader (boost::filesystem::path file, bool mxf)
{
	shared_ptr<cxml::Document> xml (new cxml::Document ("SubtitleReel"));

	if (mxf) {
		ASDCP::TimedText::MXFReader reader;
		Kumu::Result_t r = reader.OpenRead (file.string().c_str ());
		if (ASDCP_FAILURE (r)) {
			boost::throw_exception (MXFError ("could not open MXF file for reading"));
		}

		string s;
		reader.ReadTimedTextResource (s, 0, 0);
		xml->read_string (s);

		ASDCP::WriterInfo info;
		reader.FillWriterInfo (info);

		char buffer[64];
		Kumu::bin2UUIDhex (info.AssetUUID, ASDCP::UUIDlen, buffer, sizeof (buffer));
		_id = buffer;
	} else {
		xml->read_file (file);
		_id = xml->string_child("Id").substr (9);
	}

	_load_font_nodes = type_children<dcp::SMPTELoadFont> (xml, "LoadFont");

	parse_common (xml, xml->number_child<int> ("TimeCodeRate"));
}
