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


using std::ifstream;
using std::make_shared;
using std::ofstream;
using std::shared_ptr;
using std::vector;


/* Test reading of a binary STL file */
BOOST_AUTO_TEST_CASE (stl_binary_reader_test1)
{
	if (private_test.empty ()) {
		return;
	}

	using boost::filesystem::path;

	path stl = private_test / "VA_24fps_Reel_6_DE_FR.stl";
	ifstream in (stl.string().c_str());
	auto r = make_shared<sub::STLBinaryReader>(in);
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
	auto in = fopen (stl.string().c_str(), "rb");
	BOOST_REQUIRE (in);
	auto r = make_shared<sub::STLBinaryReader>(in);
	fclose (in);
	path dump = path("build") / path("test") / path("VA_24fps_Reel_6_DE_FR.dump");
	ofstream dump_stream (dump.string().c_str());
	sub::dump (r, dump_stream);
	dump_stream.close ();
}


/** Test reading a file which raised "Unknown language group code U8" and which has
 *  bizarre line numbering.
 */
BOOST_AUTO_TEST_CASE (stl_binary_reader_test3)
{
	if (private_test.empty()) {
		return;
	}

	/* This file has a MNR value of 99, which (as per the spec recommendation)
	 * we "fix" to 12.  But it also has line numbers which start at 99 and go up from there,
	 * so if we don't also alter the line numbers they end up way off the bottom of the screen.
	 * Check that the line numbers are brought into the range of our "fix".
	 */

	auto path = private_test / "hsk.stl";
	ifstream in (path.string().c_str());
	auto reader = make_shared<sub::STLBinaryReader>(in);
	for (auto i: reader->subtitles()) {
		BOOST_REQUIRE(*i.vertical_position.line >= 0);
		BOOST_REQUIRE(*i.vertical_position.line <= 12);
	}
}


BOOST_AUTO_TEST_CASE(stl_binary_reader_with_colour)
{
	auto path = private_test / "at.stl";
	ifstream in(path.string().c_str());
	auto reader = make_shared<sub::STLBinaryReader>(in);

	vector<sub::Colour> reference = {
		{ 1, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
		{ 1, 0, 1 },
		{ 1, 1, 1 },
		{ 1, 1, 0 },
		{ 1, 1, 1 },
		{ 1, 1, 0 },
		{ 1, 1, 1 },
		{ 1, 1, 0 },
		{ 1, 1, 1 },
		{ 1, 1, 0 },
		{ 1, 1, 1 },
		{ 1, 1, 0 }
	};

	/* Check the first few lines */
	auto subs = reader->subtitles();
	BOOST_REQUIRE(subs.size() >= reference.size());

	for (size_t i = 0; i < reference.size(); ++i) {
		BOOST_CHECK(subs[i].colour == reference[i]);
	}
}

