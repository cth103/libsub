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
using std::shared_ptr;
using namespace sub;

namespace sub {

class InputReader : public boost::noncopyable
{
public:
	InputReader ()
		: _buffer (new unsigned char[1024])
	{

	}

	virtual ~InputReader ()
	{
		delete[] _buffer;
	}

	virtual void read (int size, string what) = 0;

	string get_string (int offset, int length) const
	{
		string s;
		for (int i = 0; i < length; ++i) {
			s += _buffer[offset + i];
		}

		return s;
	}

	int get_int (int offset, int length) const
	{
		int v = 0;
		for (int i = 0; i < length; ++i) {
			v |= _buffer[offset + i] << (8 * i);
		}

		return v;
	}

	Time get_timecode (int offset, int frame_rate) const
	{
		return Time::from_hmsf (_buffer[offset], _buffer[offset + 1], _buffer[offset + 2], _buffer[offset + 3], Rational (frame_rate, 1));
	}

protected:
	unsigned char* _buffer;
};


class StreamInputReader : public InputReader
{
public:
	StreamInputReader (istream& in)
		: _in (in)
	{

	}

	void read (int size, string what)
	{
		_in.read (reinterpret_cast<char *>(_buffer), size);
		if (_in.gcount() != size) {
			throw STLError (String::compose("Could not read %1 block from binary STL file", what));
		}
	}

private:
	std::istream& _in;
};

class FILEInputReader : public InputReader
{
public:
	FILEInputReader (FILE* in)
		: _in (in)
	{

	}

	void read (int size, string what)
	{
		size_t const N = fread (_buffer, 1, size, _in);
		if (static_cast<int>(N) != size) {
			throw STLError (String::compose("Could not read %1 block from binary STL file", what));
		}
	}

private:
	FILE* _in;
};

}

STLBinaryReader::STLBinaryReader (istream& in)
{
	read (shared_ptr<InputReader>(new StreamInputReader(in)));
}

STLBinaryReader::STLBinaryReader (FILE* in)
{
	read (shared_ptr<InputReader>(new FILEInputReader(in)));
}

void STLBinaryReader::read (shared_ptr<InputReader> reader)
{
	reader->read (1024, "GSI");

	code_page_number = atoi (reader->get_string(0, 3).c_str());
	frame_rate = stl_dfc_to_frame_rate (reader->get_string(3, 8));
	display_standard = _tables.display_standard_file_to_enum (reader->get_string(11, 1));
	language_group = _tables.language_group_file_to_enum (reader->get_string(12, 2));
	language = _tables.language_file_to_enum (reader->get_string(14, 2));
	original_programme_title = reader->get_string(16, 32);
	original_episode_title = reader->get_string(48, 32);
	translated_programme_title = reader->get_string(80, 32);
	translated_episode_title = reader->get_string(112, 32);
	translator_name = reader->get_string(144, 32);
	translator_contact_details = reader->get_string(176, 32);
	subtitle_list_reference_code = reader->get_string(208, 16);
	creation_date = reader->get_string(224, 6);
	revision_date = reader->get_string(230, 6);
	revision_number = reader->get_string(236, 2);

	tti_blocks = atoi (reader->get_string(238, 5).c_str());
	number_of_subtitles = atoi (reader->get_string(243, 5).c_str());
	subtitle_groups = atoi (reader->get_string(248, 3).c_str());
	maximum_characters = atoi (reader->get_string(251, 2).c_str());
	maximum_rows = atoi (reader->get_string(253, 2).c_str());

	if (maximum_rows == 99) {
		/* https://tech.ebu.ch/docs/tech/tech3360.pdf says
		   "It is recommended that for files with a large MNR value (e.g. '99') the
		   font size (height) should be defined as ~ 1/15 of the 'Subtitle Safe Area'
		   and a lineHeight of 120% is used to achieve a row height of ~ 1/12 of the height
		   of the 'Subtitle Safe Area'.
		*/
		maximum_rows = 12;
	}

	timecode_status = _tables.timecode_status_file_to_enum (reader->get_string(255, 1));
	start_of_programme = reader->get_string(256, 8);
	first_in_cue = reader->get_string(264, 8);
	disks = atoi (reader->get_string(272, 1).c_str());
	disk_sequence_number = atoi (reader->get_string(273, 1).c_str());
	country_of_origin = reader->get_string(274, 3);
	publisher = reader->get_string(277, 32);
	editor_name = reader->get_string(309, 32);
	editor_contact_details = reader->get_string(341, 32);

	int highest_line = 0;
	for (int i = 0; i < tti_blocks; ++i) {

		reader->read (128, "TTI");

		if (_tables.comment_file_to_enum (reader->get_int(15, 1)) == COMMENT_YES) {
			continue;
		}

		string const whole = reader->get_string(16, 112);

		/* Split the text up into lines (8Ah is a new line) */
		vector<string> lines;
		split (lines, whole, is_any_of ("\x8a"));

		/* Italic / underline specifications can span lines, so we need to track them
		   outside the lines loop.
		*/
		bool italic = false;
		bool underline = false;

		for (size_t j = 0; j < lines.size(); ++j) {
			RawSubtitle sub;
			sub.from = reader->get_timecode(5, frame_rate);
			sub.to = reader->get_timecode(9, frame_rate);
			/* XXX: vertical position of TTI extension blocks should be ignored (spec page 10) so this
			 * is wrong if the EBN of this TTI block is not 255 (I think).
			 */
			sub.vertical_position.line = reader->get_int(13, 1) + j;
			highest_line = std::max(highest_line, *sub.vertical_position.line);
			sub.vertical_position.lines = maximum_rows;
			sub.vertical_position.reference = TOP_OF_SCREEN;
			sub.italic = italic;
			sub.underline = underline;

			/* XXX: not sure what to do with JC = 0, "unchanged presentation" */
			int const h = reader->get_int(14, 1);
			switch (h) {
			case 0:
			case 2:
				sub.horizontal_position.reference = HORIZONTAL_CENTRE_OF_SCREEN;
				break;
			case 1:
				sub.horizontal_position.reference = LEFT_OF_SCREEN;
				break;
			case 3:
				sub.horizontal_position.reference = RIGHT_OF_SCREEN;
				break;
			}

			/* Loop over characters */
			string text;
			for (size_t k = 0; k < lines[j].size(); ++k) {

				unsigned char const c = static_cast<unsigned char> (lines[j][k]);

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
					text += lines[j][k];
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

	/* Fix line numbers so they don't go off the bottom of the screen */
	if (highest_line > maximum_rows) {
		int correction = highest_line - maximum_rows;
		for (auto& i: _subs) {
			*i.vertical_position.line -= correction;
		}
	}
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
