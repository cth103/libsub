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

#ifndef LIBSUB_STL_BINARY_READER_H
#define LIBSUB_STL_BINARY_READER_H

#include "reader.h"
#include "stl_binary_tables.h"
#include <map>

namespace sub {

/** @class STLBinaryReader
 *  @brief A class to read binary STL files.
 */
class STLBinaryReader : public Reader
{
public:
	STLBinaryReader (std::istream &);
	~STLBinaryReader ();

	std::map<std::string, std::string> metadata () const;

	int code_page_number;
	int frame_rate;
	DisplayStandard display_standard;
	LanguageGroup language_group;
	Language language;
	std::string original_programme_title;
	std::string original_episode_title;
	std::string translated_programme_title;
	std::string translated_episode_title;
	std::string translator_name;
	std::string translator_contact_details;
	std::string subtitle_list_reference_code;
	std::string creation_date;
	std::string revision_date;
	std::string revision_number;
	int tti_blocks;
	int number_of_subtitles;
	int subtitle_groups;
	int maximum_characters;
	int maximum_rows;
	TimecodeStatus timecode_status;
	std::string start_of_programme;
	std::string first_in_cue;
	int disks;
	int disk_sequence_number;
	std::string country_of_origin;
	std::string publisher;
	std::string editor_name;
	std::string editor_contact_details;

private:
	std::string get_string (int, int) const;
	int get_int (int, int) const;
	FrameTime get_timecode (int) const;

	STLBinaryTables _tables;
	unsigned char* _buffer;
};

}

#endif
