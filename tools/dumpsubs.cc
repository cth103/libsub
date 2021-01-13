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

#include "reader_factory.h"
#include "reader.h"
#include "collect.h"
#include "util.h"
#include <getopt.h>
#include <boost/filesystem.hpp>
#include <map>
#include <iostream>

using std::string;
using std::cerr;
using std::cout;
using std::map;
using std::shared_ptr;
using namespace sub;

static void
help (string n)
{
	cerr << "Syntax: " << n << " <file>\n";
}

int
main (int argc, char* argv[])
{
	int option_index = 0;
	while (1) {
		static struct option long_options[] = {
			{ "help", no_argument, 0, 'h'},
			{ 0, 0, 0, 0 }
		};

		int c = getopt_long (argc, argv, "h", long_options, &option_index);

		if (c == -1) {
			break;
		}

		switch (c) {
		case 'h':
			help (argv[0]);
			exit (EXIT_SUCCESS);
		}
	}

	if (argc <= optind || argc > (optind + 1)) {
		help (argv[0]);
		exit (EXIT_FAILURE);
	}

	if (!boost::filesystem::exists (argv[optind])) {
		cerr << argv[0] << ": file " << argv[optind] << " not found.\n";
		exit (EXIT_FAILURE);
	}

	shared_ptr<Reader> reader = reader_factory (argv[optind]);
	if (!reader) {
		cerr << argv[0] << ": could not read subtitle file " << argv[optind] << "\n";
		exit (EXIT_FAILURE);
	}

	sub::dump (reader, cout);

	return 0;
}
