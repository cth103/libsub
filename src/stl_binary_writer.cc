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

#include "stl_binary_writer.h"
#include "subtitle.h"
#include "compose.hpp"
#include <list>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <set>

using std::list;
using std::set;
using std::ofstream;
using std::string;
using std::setw;
using std::setfill;
using std::max;
using namespace sub;

static void
put_string (char* p, string s)
{
	memcpy (p, s.c_str (), s.length ());
}

static void
put_string (char* p, unsigned int n, string s)
{
	assert (s.length() <= n);
	
	memcpy (p, s.c_str (), s.length ());
	memset (p + s.length(), ' ', n - s.length ());
}

static void
put_int (char* p, int v, unsigned int n)
{
	std::stringstream s;
	/* Be careful to ensure we get no thousands separators */
	s.imbue (std::locale::classic ());
	s << setw (n) << setfill ('0');
	s << v;
	assert (s.str().length() == n);
	put_string (p, s.str ());
}

/** @param language ISO 3-character country code for the language of the subtitles */
void
sub::write_stl_binary (
	list<Subtitle> subtitles,
	float frames_per_second,
	Language language,
	string original_programme_title,
	string original_episode_title,
	string translated_programme_title,
	string translated_episode_title,
	string translator_name,
	string translator_contact_details,
	string creation_date,
	string revision_date,
	int revision_number,
	string country_of_origin,
	string publisher,
	string editor_name,
	string editor_contact_details,
	boost::filesystem::path file_name
	)
{
	assert (original_programme_title.size() <= 32);
	assert (original_episode_title.size() <= 32);
	assert (translated_programme_title.size() <= 32);
	assert (translated_episode_title.size() <= 32);
	assert (translator_name.size() <= 32);
	assert (translator_contact_details.size() <= 32);
	assert (creation_date.size() == 6);
	assert (revision_date.size() == 6);
	assert (revision_number <= 99);
	assert (country_of_origin.size() == 3);
	assert (publisher.size() <= 32);
	assert (editor_name.size() <= 32);
	assert (editor_contact_details.size() <= 32);
	
	char* buffer = new char[1024];
	ofstream output (file_name.string().c_str ());
	STLBinaryTables tables;

	/* Find the longest subtitle in characters and the number of rows */

	int longest = 0;

	set<float> check_top;
	set<float> check_centre;
	set<float> check_bottom;
	set<int> check_rows;
	
	for (list<Subtitle>::const_iterator i = subtitles.begin(); i != subtitles.end(); ++i) {
		int t = 0;
		for (list<Block>::const_iterator j = i->blocks.begin(); j != i->blocks.end(); ++j) {
			t += j->text.size ();
		}
		longest = max (longest, t);

		if (i->vertical_position.proportional) {
			switch (i->vertical_position.reference.get ()) {
			case TOP:
				check_top.insert (i->vertical_position.proportional.get ());
				break;
			case CENTRE:
				check_centre.insert (i->vertical_position.proportional.get ());
				break;
			case BOTTOM:
				check_bottom.insert (i->vertical_position.proportional.get ());
				break;
			}
		} else {
			check_rows.insert (i->vertical_position.line.get ());
		}
	}

	int const rows = check_top.size() + check_centre.size() + check_bottom.size() + check_rows.size();
	
	/* Code page: 850 */
	put_string (buffer + 0, "850");
	/* Disk format code */
	put_string (buffer + 3, String::compose ("STL%1.01", rint (frames_per_second)));
	/* Display standard code: open subtitling */
	put_string (buffer + 11, "0");
	/* Character code table: Latin (ISO 6937) */
	put_string (buffer + 12, "00");
	put_string (buffer + 14, tables.language_enum_to_file (language));
	put_string (buffer + 16, 32, original_programme_title);
	put_string (buffer + 48, 32, original_episode_title);
	put_string (buffer + 80, 32, translated_programme_title);
	put_string (buffer + 112, 32, translated_episode_title);
	put_string (buffer + 144, 32, translator_name);
	put_string (buffer + 176, 32, translator_contact_details);
	/* Subtitle list reference code */
	put_string (buffer + 208, "0000000000000000");
	put_string (buffer + 224, creation_date);
	put_string (buffer + 230, revision_date);
	put_int (buffer + 236, revision_number, 2);
	/* TTI blocks */
	put_int (buffer + 238, subtitles.size (), 5);
	/* Total number of subtitles */
	put_int (buffer + 243, subtitles.size (), 5);
	/* Total number of subtitle groups */
	put_string (buffer + 248, "000");
	/* Maximum number of displayable characters in any text row */
	put_int (buffer + 251, 2, longest);
	/* Maximum number of displayable rows */
	put_int (buffer + 253, 2, rows);
	/* Time code status */
	put_string (buffer + 255, "1");
	/* Start-of-programme time code */
	put_string (buffer + 256, "00000000");
	/* First-in-cue time code */
	put_string (buffer + 264, "00000000");
	/* Total number of disks */
	put_string (buffer + 272, "1");
	/* Disk sequence number */
	put_string (buffer + 273, "1");
	put_string (buffer + 274, 32, country_of_origin);
	put_string (buffer + 277, 32, publisher);
	put_string (buffer + 309, 32, editor_name);
	put_string (buffer + 341, 32, editor_contact_details);

	output.write (buffer, 1024);

	for (list<Subtitle>::const_iterator i = subtitles.begin(); i != subtitles.end(); ++i) {
		
	}

	delete[] buffer;
}
