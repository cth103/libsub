/*
    Copyright (C) 2014-2019 Carl Hetherington <cth@carlh.net>

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

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE libsub_test
#include "iso6937_tables.h"
#include "compose.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <string>
#include <iostream>

using std::string;
using std::cerr;
using std::cout;
using std::min;
using std::max;
using std::hex;
using std::ifstream;
using std::getline;

boost::filesystem::path private_test;

struct TestConfig
{
	TestConfig()
	{
		if (boost::unit_test::framework::master_test_suite().argc >= 2) {
			private_test = boost::unit_test::framework::master_test_suite().argv[1];
		} else {
			BOOST_TEST_MESSAGE ("Private data libsub-test-private not found; some tests will not run");
		}

		sub::make_iso6937_tables ();
	}
};

BOOST_GLOBAL_FIXTURE (TestConfig);

void
check_text (boost::filesystem::path a, boost::filesystem::path b)
{
	if (!boost::filesystem::exists (a)) {
		cerr << "File not found: " << a << "\n";
	}

	if (!boost::filesystem::exists (b)) {
		cerr << "File not found: " << b << "\n";
	}

	BOOST_CHECK (boost::filesystem::exists (a));
	BOOST_CHECK (boost::filesystem::exists (b));

	ifstream p (a.c_str ());
	ifstream q (b.c_str ());

	string x;
	string y;
	while (p.good() && q.good()) {
		getline (p, x);
		getline (q, y);
		BOOST_CHECK_EQUAL (x, y);
	}

	BOOST_CHECK (p.good() == false);
	BOOST_CHECK (q.good() == false);
}

void
check_file (boost::filesystem::path ref, boost::filesystem::path check)
{
	uintmax_t N = boost::filesystem::file_size (ref);
	BOOST_CHECK_EQUAL (N, boost::filesystem::file_size (check));
	FILE* ref_file = fopen (ref.string().c_str(), "rb");
	BOOST_CHECK (ref_file);
	FILE* check_file = fopen (check.string().c_str(), "rb");
	BOOST_CHECK (check_file);

	int const buffer_size = 65536;
	uint8_t* ref_buffer = new uint8_t[buffer_size];
	uint8_t* check_buffer = new uint8_t[buffer_size];

	uintmax_t offset = 0;
	while (offset < N) {
		uintmax_t this_time = min (uintmax_t (buffer_size), N - offset);
		size_t r = fread (ref_buffer, 1, this_time, ref_file);
		BOOST_CHECK_EQUAL (r, this_time);
		r = fread (check_buffer, 1, this_time, check_file);
		BOOST_CHECK_EQUAL (r, this_time);

		for (uintmax_t i = 0; i < this_time; ++i) {
			string const s = String::compose (
				"Files differ at offset %1; reference is %2, check is %3", (offset + i), ((int) ref_buffer[i]), ((int) check_buffer[i])
				);
			BOOST_CHECK_MESSAGE (ref_buffer[i] == check_buffer[i], s);
		}

		offset += this_time;
	}

	delete[] ref_buffer;
	delete[] check_buffer;

	fclose (ref_file);
	fclose (check_file);
}
