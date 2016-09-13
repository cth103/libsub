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

#include "stl_binary_reader.h"
#include "exceptions.h"
#include "iso6937.h"
#include "stl_util.h"
#include "compose.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/locale.hpp>
#include <iostream>

using std::map;
using std::vector;
using std::cout;
using std::string;
using std::istream;
using boost::lexical_cast;
using boost::algorithm::replace_all;
using boost::is_any_of;
using boost::locale::conv::utf_to_utf;
using namespace sub;

STLBinaryReader::STLBinaryReader (istream& in)
	: _buffer (new unsigned char[1024])
{
	in.read ((char *) _buffer, 1024);
	if (in.gcount() != 1024) {
		throw STLError ("Could not read GSI block from binary STL file");
	}

	code_page_number = atoi (get_string (0, 3).c_str ());
	frame_rate = stl_dfc_to_frame_rate (get_string (3, 8));
	display_standard = _tables.display_standard_file_to_enum (get_string (11, 1));
	language_group = _tables.language_group_file_to_enum (get_string (12, 2));
	language = _tables.language_file_to_enum (get_string (14, 2));
	original_programme_title = get_string (16, 32);
	original_episode_title = get_string (48, 32);
	translated_programme_title = get_string (80, 32);
	translated_episode_title = get_string (112, 32);
	translator_name = get_string (144, 32);
	translator_contact_details = get_string (176, 32);
	subtitle_list_reference_code = get_string (208, 16);
	creation_date = get_string (224, 6);
	revision_date = get_string (230, 6);
	revision_number = get_string (236, 2);

	tti_blocks = atoi (get_string (238, 5).c_str ());
	number_of_subtitles = atoi (get_string (243, 5).c_str ());
	subtitle_groups = atoi (get_string (248, 3).c_str ());
	maximum_characters = atoi (get_string (251, 2).c_str ());
	maximum_rows = atoi (get_string (253, 2).c_str ());
	timecode_status = _tables.timecode_status_file_to_enum (get_string (255, 1));
	start_of_programme = get_string (256, 8);
	first_in_cue = get_string (264, 8);
	disks = atoi (get_string (272, 1).c_str ());
	disk_sequence_number = atoi (get_string (273, 1).c_str ());
	country_of_origin = get_string (274, 3);
	publisher = get_string (277, 32);
	editor_name = get_string (309, 32);
	editor_contact_details = get_string (341, 32);

	for (int i = 0; i < tti_blocks; ++i) {

		in.read ((char *) _buffer, 128);
		if (in.gcount() != 128) {
			throw STLError ("Could not read TTI block from binary STL file");
		}

		if (_tables.comment_file_to_enum (get_int (15, 1)) == COMMENT_YES) {
			continue;
		}

		string const whole = get_string (16, 112);

		/* Split the text up into lines (8Ah is a new line) */
		vector<string> lines;
		split (lines, whole, is_any_of ("\x8a"));

		/* Italic / underline specifications can span lines, so we need to track them
		   outside the lines loop.
		*/
		bool italic = false;
		bool underline = false;

		for (size_t i = 0; i < lines.size(); ++i) {
			RawSubtitle sub;
			sub.from = get_timecode (5);
			sub.to = get_timecode (9);
			sub.vertical_position.line = get_int (13, 1) + i;
			sub.vertical_position.lines = maximum_rows;
			sub.vertical_position.reference = TOP_OF_SCREEN;
			sub.italic = italic;
			sub.underline = underline;

			/* Loop over characters */
			string text;
			for (size_t j = 0; j < lines[i].size(); ++j) {

				unsigned char const c = static_cast<unsigned char> (lines[i][j]);

				if (c == 0x8f) {
					/* Unused space i.e. end of line */
					break;
				}

				if (c >= 0x80 && c <= 0x83) {
					/* Italic or underline control code */
					sub.text = utf_to_utf<char> (iso6937_to_utf16 (text.c_str()));
					_subs.push_back (sub);
					text.clear ();
				}

				switch (c) {
				case 0x80:
					italic = true;
					break;
				case 0x81:
					italic = false;
					break;
				case 0x82:
					underline = true;
					break;
				case 0x83:
					underline = false;
					break;
				default:
					text += lines[i][j];
					break;
				}

				sub.italic = italic;
				sub.underline = underline;
			}

			if (!text.empty ()) {
				sub.text = utf_to_utf<char> (iso6937_to_utf16 (text.c_str()));
				_subs.push_back (sub);
			}

			/* XXX: justification */
		}
	}
}

STLBinaryReader::~STLBinaryReader ()
{
	delete[] _buffer;
}

string
STLBinaryReader::get_string (int offset, int length) const
{
	string s;
	for (int i = 0; i < length; ++i) {
		s += _buffer[offset + i];
	}

	return s;
}

int
STLBinaryReader::get_int (int offset, int length) const
{
	int v = 0;
	for (int i = 0; i < length; ++i) {
		v |= _buffer[offset + i] << (8 * i);
	}

	return v;
}

Time
STLBinaryReader::get_timecode (int offset) const
{
	return Time::from_hmsf (_buffer[offset], _buffer[offset + 1], _buffer[offset + 2], _buffer[offset + 3], Rational (frame_rate, 1));
}

map<string, string>
STLBinaryReader::metadata () const
{
	map<string, string> m;

	m["Code page number"] = lexical_cast<string> (code_page_number);
	m["Frame rate"] = lexical_cast<string> (frame_rate);
	m["Display standard"] = _tables.display_standard_enum_to_description (display_standard);
	m["Language group"] = _tables.language_group_enum_to_description (language_group);
	m["Language"] = _tables.language_enum_to_description (language);
	m["Original programme title"] = original_programme_title;
	m["Original episode title"] = original_episode_title;
	m["Translated programme title"] = translated_programme_title;
	m["Translated episode title"] = translated_episode_title;
	m["Translator name"] = translator_name;
	m["Translator contact details"] = translator_contact_details;
	m["Subtitle list reference code"] = subtitle_list_reference_code;
	m["Creation date"] = creation_date;
	m["Revision date"] = revision_date;
	m["Revision number"] = revision_number;
	m["TTI blocks"] = lexical_cast<string> (tti_blocks);
	m["Number of subtitles"] = lexical_cast<string> (number_of_subtitles);
	m["Subtitle groups"] = lexical_cast<string> (subtitle_groups);
	m["Maximum characters"] = lexical_cast<string> (maximum_characters);
	m["Maximum rows"] = lexical_cast<string> (maximum_rows);
	m["Timecode status"] = _tables.timecode_status_enum_to_description (timecode_status);
	m["Start of programme"] = start_of_programme;
	m["First in cue"] = first_in_cue;
	m["Disks"] = lexical_cast<string> (disks);
	m["Disk sequence number"] = lexical_cast<string> (disk_sequence_number);
	m["Country of origin"] = country_of_origin;
	m["Publisher"] = publisher;
	m["Editor name"] = editor_name;
	m["Editor contact details"] = editor_contact_details;

	return m;
}
