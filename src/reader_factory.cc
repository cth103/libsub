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

#include "reader_factory.h"
#include "stl_binary_reader.h"
#include "stl_text_reader.h"
#include "dcp_reader.h"
#include "subrip_reader.h"
#include "sub_assert.h"
#include <libxml++/libxml++.h>
#include <boost/algorithm/string.hpp>
#include <fstream>

using std::string;
using std::ifstream;
using boost::algorithm::ends_with;
using boost::shared_ptr;
using namespace sub;

shared_ptr<Reader>
sub::reader_factory (boost::filesystem::path file_name)
{
	string ext = file_name.extension().string();
	transform (ext.begin(), ext.end(), ext.begin(), ::tolower);

	if (ext == ".xml") {
		return shared_ptr<Reader> (new DCPReader (file_name));
        }

        if (ext == ".mxf") {
                /* Assume this is some MXF-wrapped SMPTE subtitles */
                return shared_ptr<Reader> (new DCPReader (file_name));
        }

	if (ext == ".stl") {
		/* Check the start of the DFC */
		ifstream f (file_name.string().c_str ());
		char buffer[11];
		f.read (buffer, 11);
		f.seekg (0);
		if (f.gcount() == 11 && buffer[3] == 'S' && buffer[4] == 'T' && buffer[5] == 'L') {
			return shared_ptr<Reader> (new STLBinaryReader (f));
		} else {
			return shared_ptr<Reader> (new STLTextReader (f));
		}
	}

	if (ext == ".srt") {
		FILE* f = fopen (file_name.string().c_str(), "r");
		SUB_ASSERT (f);
		shared_ptr<Reader> r (new SubripReader(f));
		fclose (f);
		return r;
	}

	return shared_ptr<Reader> ();
}
