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
#include "reader.h"
#include "collect.h"
#include <getopt.h>
#include <boost/filesystem.hpp>
#include <map>

using std::string;
using std::cerr;
using std::cout;
using std::map;
using std::list;
using boost::shared_ptr;
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

	map<string, string> metadata = reader->metadata ();
	for (map<string, string>::const_iterator i = metadata.begin(); i != metadata.end(); ++i) {
		cout << i->first << ": " << i->second << "\n";
	}

	list<sub::Subtitle> subs = collect<list<sub::Subtitle> > (reader->subtitles ());
	int n = 0;
	for (list<sub::Subtitle>::const_iterator i = subs.begin(); i != subs.end(); ++i) {
		cout << "Subtitle " << n << " at " << i->from << " -> " << i->to << "\n";
		for (list<sub::Line>::const_iterator j = i->lines.begin(); j != i->lines.end(); ++j) {

			cout << "\t";
			
			if (j->vertical_position.proportional) {
				cout << j->vertical_position.proportional.get() << " of screen";
			} else if (j->vertical_position.line) {
				cout << j->vertical_position.line.get() << " lines";
			}
			if (j->vertical_position.reference) {
				cout << " from ";
				switch (j->vertical_position.reference.get()) {
				case TOP_OF_SCREEN:
					cout << "top";
					break;
				case CENTRE_OF_SCREEN:
					cout << "centre";
					break;
				case BOTTOM_OF_SCREEN:
					cout << "bottom";
					break;
				case TOP_OF_SUBTITLE:
					cout << "top of subtitle";
					break;
				}
			}

			cout << "\t";
			bool italic = false;
			bool underline = false;
			for (list<sub::Block>::const_iterator k = j->blocks.begin(); k != j->blocks.end(); ++k) {
				if (k->italic && !italic) {
					cout << "<i>";
				} else if (italic && !k->italic) {
					cout << "</i>";
				}
				if (k->underline && !underline) {
					cout << "<u>";
				} else if (underline && !k->underline) {
					cout << "</u>";
				}

				italic = k->italic;
				underline = k->underline;
					
				cout << k->text;
			}

			if (italic) {
				cout << "</i>";
			}
			if (underline) {
				cout << "</u>";
			}
			cout << "\n";
		}

		++n;
	}

	return 0;
}
