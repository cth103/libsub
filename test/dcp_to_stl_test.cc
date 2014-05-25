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

#include <fstream>
#include <boost/test/unit_test.hpp>
#include "test.h"
#include "dcp_reader.h"
#include "stl_writer.h"

using std::string;
using std::ifstream;
using std::ofstream;

BOOST_AUTO_TEST_CASE (dcp_to_stl_test)
{
	if (private_test.empty ()) {
		return;
	}

	string const p = private_test + "/fd586c30-6d38-48f2-8241-27359acf184c_sub.xml";
	ifstream f (p.c_str ());
	sub::DCPReader r (f);
	string const q = "build/test/fd586c30-6d38-48f2-8241-27359acf184c_sub.stl";
	ofstream g (q.c_str ());
	sub::STLWriter w (r.subtitles (), 72 * 11, 24, g);
	string const c = private_test + "/fd586c30-6d38-48f2-8241-27359acf184c_sub.stl";
	check_text (q, c);
}
