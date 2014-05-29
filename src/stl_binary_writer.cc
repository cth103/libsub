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

#include <list>
#include <cmath>
#include <fstream>
#include "stl_binary_writer.h"
#include "compose.hpp"

using std::list;
using std::ofstream;
using std::string;
using namespace sub;

static void
put_string (char* p, string s)
{
	memcpy (p, s.c_str (), s.length ());
}

static void
put_string (char* p, int n, string s)
{
	memcpy (p, s.c_str (), s.length ());
	memset (p + s.length(), ' ', s.length () - n);
}

/** @param language ISO 3-character country code for the language of the subtitles */
void
sub::write_stl_binary (
	list<Subtitle> subtitles,
	float frames_per_second,
	string language,
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
	assert (language.size() == 3);
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
	
	/* Code page: 850 */
	put_string (buffer + 0, "850");
	/* Disk format code */
	put_string (buffer + 3, String::compose ("STL%1.01", rint (frames_per_second)));
	/* Display standard code: open subtitling */
	put_string (buffer + 11, "0");
	/* Character code table: Latin (ISO 6937) */
	put_string (buffer + 12, "00");
	put_string (buffer + 14, language);
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
	put_string (buffer + 236, String::compose ("%02d", revision_number));
	/* TTI blocks */
	put_string (buffer + 238, String::compose ("%05d", subtitles.size ()));
	/* Total number of subtitles */
	put_string (buffer + 243, String::compose ("%05d", subtitles.size ()));
	/* Total number of subtitle groups */
	put_string (buffer + 248, "000");
	/* Maximum number of displayable characters in any text row */
	/* XXX */
	put_string (buffer + 251, "99");
	/* Maximum number of displayable rows */
	/* XXX */
	put_string (buffer + 253, "99");
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
	put_string (buffer + 274, country_of_origin);
	put_string (buffer + 277, publisher);
	put_string (buffer + 309, editor_name);
	put_string (buffer + 341, editor_contact_details);

	output.write (buffer, 1024);

	delete[] buffer;
}
