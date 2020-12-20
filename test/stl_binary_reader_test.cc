/*
    Copyright (C) 2014-2020 Carl Hetherington <cth@carlh.net>

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

#include "stl_binary_reader.h"
#include "subtitle.h"
#include "test.h"
#include "util.h"
#include <boost/test/unit_test.hpp>
#include <fstream>

using std::list;
using std::ifstream;
using std::ofstream;
using std::shared_ptr;

/* Test reading of a binary STL file */
BOOST_AUTO_TEST_CASE (stl_binary_reader_test1)
{
	if (private_test.empty ()) {
		return;
	}

	using boost::filesystem::path;

	path stl = private_test / "VA_24fps_Reel_6_DE_FR.stl";
	ifstream in (stl.string().c_str());
	shared_ptr<sub::STLBinaryReader> r (new sub::STLBinaryReader(in));
	path dump = path("build") / path("test") / path("VA_24fps_Reel_6_DE_FR.dump");
	ofstream dump_stream (dump.string().c_str());
	sub::dump (r, dump_stream);
	dump_stream.close ();

	check_file (private_test / "VA_24fps_Reel_6_DE_FR.dump", dump);
}

/* Test reading the same file but with the FILE* interface */
BOOST_AUTO_TEST_CASE (stl_binary_reader_test2)
{
	if (private_test.empty ()) {
		return;
	}

	using boost::filesystem::path;

	path stl = private_test / "VA_24fps_Reel_6_DE_FR.stl";
	FILE* in = fopen (stl.string().c_str(), "rb");
	BOOST_REQUIRE (in);
	shared_ptr<sub::STLBinaryReader> r (new sub::STLBinaryReader(in));
	fclose (in);
	path dump = path("build") / path("test") / path("VA_24fps_Reel_6_DE_FR.dump");
	ofstream dump_stream (dump.string().c_str());
	sub::dump (r, dump_stream);
	dump_stream.close ();
}
