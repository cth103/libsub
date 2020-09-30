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

#include "test.h"
#include "dcp_reader.h"
#include "stl_binary_writer.h"
#include "collect.h"
#include <boost/test/unit_test.hpp>
#include <fstream>

using std::ifstream;
using std::list;

BOOST_AUTO_TEST_CASE (dcp_to_stl_binary_test1)
{
	if (private_test.empty ()) {
		return;
	}

	boost::filesystem::path p = private_test / "fd586c30-6d38-48f2-8241-27359acf184c_sub.xml";
	sub::write_stl_binary (
		sub::collect<list<sub::Subtitle> > (sub::DCPReader(p).subtitles ()),
		25,
		sub::LANGUAGE_FRENCH,
		"", "",
		"", "",
		"", "",
		"300514", "300514", 0,
		"GBR",
		"",
		"", "",
		"build/test/fd586c30-6d38-48f2-8241-27359acf184c_sub.stl"
		);

	check_file (
		private_test / "fd586c30-6d38-48f2-8241-27359acf184c_sub.stl",
		"build/test/fd586c30-6d38-48f2-8241-27359acf184c_sub.stl"
		);
}

BOOST_AUTO_TEST_CASE (dcp_to_stl_binary_test2)
{
	if (private_test.empty ()) {
		return;
	}

	boost::filesystem::path p = private_test / "93e8a6bf-499e-4d36-9350-a9bfa2e6758a_sub.xml";
	sub::write_stl_binary (
		sub::collect<list<sub::Subtitle> > (sub::DCPReader(p).subtitles ()),
		25,
		sub::LANGUAGE_FRENCH,
		"", "",
		"", "",
		"", "",
		"300514", "300514", 0,
		"GBR",
		"",
		"", "",
		"build/test/93e8a6bf-499e-4d36-9350-a9bfa2e6758a_sub.stl"
		);

	check_file (
		private_test / "93e8a6bf-499e-4d36-9350-a9bfa2e6758a_sub.stl",
		"build/test/93e8a6bf-499e-4d36-9350-a9bfa2e6758a_sub.stl"
		);
}

BOOST_AUTO_TEST_CASE (dcp_to_stl_binary_test3)
{
	if (private_test.empty ()) {
		return;
	}

	boost::filesystem::path p = private_test / "P_FTR_Subs_DE-FR_24fps_R1.xml";
	sub::write_stl_binary (
		sub::collect<list<sub::Subtitle> > (sub::DCPReader(p).subtitles ()),
		25,
		sub::LANGUAGE_FRENCH,
		"", "",
		"", "",
		"", "",
		"300514", "300514", 0,
		"GBR",
		"",
		"", "",
		"build/test/P_FTR_Subs_DE-FR_24fps_R1.stl"
		);

	check_file (
		private_test / "P_FTR_Subs_DE-FR_24fps_R1.stl",
		"build/test/P_FTR_Subs_DE-FR_24fps_R1.stl"
		);
}

BOOST_AUTO_TEST_CASE (dcp_to_stl_binary_test4)
{
	sub::write_stl_binary (
		sub::collect<list<sub::Subtitle> > (sub::DCPReader("test/data/test1.xml").subtitles ()),
		25,
		sub::LANGUAGE_FRENCH,
		"", "",
		"", "",
		"", "",
		"300514", "300514", 0,
		"GBR",
		"",
		"", "",
		"build/test/test1.stl"
		);
}

BOOST_AUTO_TEST_CASE (dcp_to_stl_binary_test5)
{
	if (private_test.empty ()) {
		return;
	}

	boost::filesystem::path p = private_test / "065d39ff-6723-4dbf-a94f-849cde82f5e1_sub.mxf";
	sub::write_stl_binary (
		sub::collect<list<sub::Subtitle> > (sub::DCPReader(p).subtitles ()),
		25,
		sub::LANGUAGE_FRENCH,
		"", "",
		"", "",
		"", "",
		"300514", "300514", 0,
		"GBR",
		"",
		"", "",
		"build/test/065d39ff-6723-4dbf-a94f-849cde82f5e1_sub.stl"
		);

	check_file (
		private_test / "065d39ff-6723-4dbf-a94f-849cde82f5e1_sub.stl",
		"build/test/065d39ff-6723-4dbf-a94f-849cde82f5e1_sub.stl"
		);
}

BOOST_AUTO_TEST_CASE (dcp_to_stl_binary_test6)
{
	if (private_test.empty ()) {
		return;
	}

	boost::filesystem::path p = private_test / "P_FTR_FullSubs_DE_24fps.xml";
	sub::write_stl_binary (
		sub::collect<list<sub::Subtitle> > (sub::DCPReader(p).subtitles ()),
		24,
		sub::LANGUAGE_GERMAN,
		"", "",
		"", "",
		"", "",
		"300514", "300514", 0,
		"GBR",
		"",
		"", "",
		"build/test/P_FTR_FullSubs_DE_24fps.stl"
		);

	check_file (
		private_test / "P_FTR_FullSubs_DE_24fps.stl",
		"build/test/P_FTR_FullSubs_DE_24fps.stl"
		);
}

BOOST_AUTO_TEST_CASE (dcp_to_stl_binary_test7)
{
	boost::filesystem::path p = "test/data/test3.xml";
	sub::write_stl_binary (
		sub::collect<list<sub::Subtitle> > (sub::DCPReader(p).subtitles ()),
		24,
		sub::LANGUAGE_GERMAN,
		"", "",
		"", "",
		"", "",
		"300514", "300514", 0,
		"GBR",
		"",
		"", "",
		"build/test/test3.stl"
		);

	check_file (
		"test/ref/test3.stl",
		"build/test/test3.stl"
		);
}

BOOST_AUTO_TEST_CASE (dcp_to_stl_binary_test8)
{
	boost::filesystem::path p = private_test / "91a30f25-b415-4ffe-9623-bdae43a381d3_sub.xml";
	sub::write_stl_binary (
		sub::collect<list<sub::Subtitle> > (sub::DCPReader(p).subtitles()),
		24,
		sub::LANGUAGE_GERMAN,
		"", "",
		"", "",
		"", "",
		"300514", "300514", 0,
		"GBR",
		"",
		"", "",
		"build/test/91a30f25-b415-4ffe-9623-bdae43a381d3_sub.stl"
		);

	check_file (
		private_test / "91a30f25-b415-4ffe-9623-bdae43a381d3_sub.stl",
		"build/test/91a30f25-b415-4ffe-9623-bdae43a381d3_sub.stl"
		);
}

BOOST_AUTO_TEST_CASE (dcp_to_stl_binary_test9)
{
	boost::filesystem::path p = private_test / "8b95f204-f2a6-4586-8e67-2dc671a78e72_sub.xml";
	sub::write_stl_binary (
		sub::collect<list<sub::Subtitle> > (sub::DCPReader(p).subtitles()),
		24,
		sub::LANGUAGE_GERMAN,
		"", "",
		"", "",
		"", "",
		"300514", "300514", 0,
		"GBR",
		"",
		"", "",
		"build/test/8b95f204-f2a6-4586-8e67-2dc671a78e72_sub.stl"
		);

	check_file (
		private_test / "8b95f204-f2a6-4586-8e67-2dc671a78e72_sub.stl",
		"build/test/8b95f204-f2a6-4586-8e67-2dc671a78e72_sub.stl"
		);
}
