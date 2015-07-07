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

#include <boost/test/unit_test.hpp>
#include "iso6937.h"
#include <boost/locale.hpp>
#include <iostream>

using std::cout;
using boost::locale::conv::utf_to_utf;

BOOST_AUTO_TEST_CASE (iso6937_to_utf16_test)
{
	BOOST_CHECK_EQUAL (utf_to_utf<char> (sub::iso6937_to_utf16 ("Hello world")), "Hello world");
	BOOST_CHECK_EQUAL (utf_to_utf<char> (sub::iso6937_to_utf16 ("Testing \xA9testing\xB9")), "Testing ‘testing’");
	BOOST_CHECK_EQUAL (utf_to_utf<char> (sub::iso6937_to_utf16 ("All must have \xCB""cedillas")), "All must have çedillas");
	BOOST_CHECK_EQUAL (utf_to_utf<char> (sub::iso6937_to_utf16 ("M\xC8otorhead")), "Mötorhead");
	BOOST_CHECK_EQUAL (utf_to_utf<char> (sub::iso6937_to_utf16 ("Pass\nnewlines\nthrough")), "Pass\nnewlines\nthrough");
}

BOOST_AUTO_TEST_CASE (utf16_to_iso6937_test)
{
	BOOST_CHECK_EQUAL (sub::utf16_to_iso6937 (utf_to_utf<wchar_t> ("Hello world")), "Hello world");
	BOOST_CHECK_EQUAL (sub::utf16_to_iso6937 (utf_to_utf<wchar_t> ("Testing ‘testing’")), "Testing \xA9testing\xB9");
	BOOST_CHECK_EQUAL (sub::utf16_to_iso6937 (utf_to_utf<wchar_t> ("All must have çedillas")), "All must have \xCB""cedillas");
	BOOST_CHECK_EQUAL (sub::utf16_to_iso6937 (utf_to_utf<wchar_t> ("Mötorhead")), "M\xC8otorhead");
	BOOST_CHECK_EQUAL (sub::utf16_to_iso6937 (utf_to_utf<wchar_t> ("Pass\nnewlines\nthrough")), "Pass\nnewlines\nthrough");
}
