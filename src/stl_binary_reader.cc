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

#include <boost/lexical_cast.hpp>
#include "stl_binary_reader.h"
#include "exceptions.h"
#include "compose.hpp"

using std::map;
using std::cout;
using std::string;
using std::istream;
using boost::lexical_cast;
using namespace sub;

template <class T>
T
string_to_value (string k, map<string, STLCode<T> > m, string name)
{
	typename map<string, STLCode<T> >::const_iterator i = m.find (k);
	if (i == m.end ()) {
		throw STLError (String::compose ("Unknown %1 %2 in binary STL file", name, k));
	}

	return i->second.value;
}

template <class T>
string
value_to_description (T v, map<string, STLCode<T> > const & m)
{
	for (typename map<string, STLCode<T> >::const_iterator i = m.begin(); i != m.end(); ++i) {
		if (i->second.value == v) {
			return i->second.description;
		}
	}

	return "";
}

template <class T>
void
code (map<string, STLCode<T> >& m, string k, T v, string d)
{
	m[k] = STLCode<T> (v, d);
}


STLBinaryReader::STLBinaryReader (istream& in)
	: _buffer (new unsigned char[1024])
{
	create_maps ();
	
	in.read ((char *) _buffer, 1024);
	if (in.gcount() != 1024) {
		throw STLError ("Could not read GSI block from binary STL file");
	}
		
	code_page_number = atoi (fixed_string (0, 3).c_str ());
	
	string const dfc = fixed_string (3, 8);
	if (dfc == "STL24.01") {
		frame_rate = 24;
	} else if (dfc == "STL25.01") {
		frame_rate = 25;
	} else if (dfc == "STL30.01") {
		frame_rate = 30;
	} else {
		throw STLError (String::compose ("Unknown disk format code %1 in binary STL file", dfc));
	}

	display_standard = string_to_value (fixed_string (11, 1), _display_standard_map, "display standard code");
	language_group = string_to_value (fixed_string (12, 2), _language_group_map, "character code");
	language = string_to_value (fixed_string (14, 2), _language_map, "language code");
	original_programme_title = fixed_string (16, 32);
	original_episode_title = fixed_string (48, 32);
	translated_programme_title = fixed_string (80, 32);
	translated_episode_title = fixed_string (112, 32);
	translator_name = fixed_string (144, 32);
	translator_contact_details = fixed_string (176, 32);
	subtitle_list_reference_code = fixed_string (208, 32);
	creation_date = fixed_string (224, 6);
	revision_date = fixed_string (230, 6);
	revision_number = fixed_string (236, 2);

	tti_blocks = atoi (fixed_string (238, 6).c_str ());
	number_of_subtitles = atoi (fixed_string (243, 5).c_str ());
	subtitle_groups = atoi (fixed_string (248, 3).c_str ());
	maximum_characters = atoi (fixed_string (251, 2).c_str ());
	maximum_rows = atoi (fixed_string (253, 2).c_str ());
	timecode_status = string_to_value (fixed_string (255, 1), _timecode_status_map, "timecode status code");
	start_of_programme = fixed_string (256, 8);
	first_in_cue = fixed_string (256, 8);
	disks = atoi (fixed_string (272, 1).c_str ());
	disk_sequence_number = atoi (fixed_string (273, 1).c_str ());
	country_of_origin = fixed_string (274, 3);
	publisher = fixed_string (277, 32);
	editor_name = fixed_string (309, 32);
	editor_contact_details = fixed_string (341, 32);
}

STLBinaryReader::~STLBinaryReader ()
{
	delete[] _buffer;
}

string
STLBinaryReader::fixed_string (int offset, int length) const
{
	string s;
	for (int i = 0; i < length; ++i) {
		s += _buffer[offset + i];
	}

	return s;
}

map<string, string>
STLBinaryReader::metadata () const
{
	map<string, string> m;

	m["Code page number"] = lexical_cast<string> (code_page_number);
	m["Frame rate"] = lexical_cast<string> (frame_rate);
	m["Display standard"] = value_to_description (display_standard, _display_standard_map);
	m["Language group"] = value_to_description (language_group, _language_group_map);
	m["Language"] = value_to_description (language, _language_map);
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
	m["Timecode status"] = value_to_description (timecode_status, _timecode_status_map);
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

void
STLBinaryReader::create_maps ()
{
	code (_display_standard_map, " ", DISPLAY_STANDARD_UNDEFINED, "Undefined");
	code (_display_standard_map, "0", DISPLAY_STANDARD_OPEN_SUBTITLING, "Open subtitling");
	code (_display_standard_map, "1", DISPLAY_STANDARD_LEVEL_1_TELETEXT, "Level 1 teletext");
	code (_display_standard_map, "2", DISPLAY_STANDARD_LEVEL_2_TELETEXT, "Level 2 teletext");
	
	code (_language_group_map, "00", LANGUAGE_GROUP_LATIN, "Latin");
	code (_language_group_map, "01", LANGUAGE_GROUP_LATIN_CYRILLIC, "Latin/Cyrillic");
	code (_language_group_map, "02", LANGUAGE_GROUP_LATIN_ARABIC, "Latin/Arabic");
	code (_language_group_map, "03", LANGUAGE_GROUP_LATIN_GREEK, "Latin/Greek");
	code (_language_group_map, "04", LANGUAGE_GROUP_LATIN_HEBREW, "Latin/Hebrew");
	
	code (_language_map, "00", LANGUAGE_UNKNOWN, "Unknown");
	code (_language_map, "01", LANGUAGE_ALBANIAN, "Albanian");
	code (_language_map, "02", LANGUAGE_BRETON, "Breton");
	code (_language_map, "03", LANGUAGE_CATALAN, "Catalan");
	code (_language_map, "04", LANGUAGE_CROATIAN, "Croatian");
	code (_language_map, "05", LANGUAGE_WELSH, "Welsh");
	code (_language_map, "06", LANGUAGE_CZECH, "Czech");
	code (_language_map, "07", LANGUAGE_DANISH, "Danish");
	code (_language_map, "08", LANGUAGE_GERMAN, "German");
	code (_language_map, "09", LANGUAGE_ENGLISH, "English");
	code (_language_map, "0A", LANGUAGE_SPANISH, "Spanish");
	code (_language_map, "0B", LANGUAGE_ESPERANTO, "Esperanto");
	code (_language_map, "0C", LANGUAGE_ESTONIAN, "Estonian");
	code (_language_map, "0D", LANGUAGE_BASQUE, "Basque");
	code (_language_map, "0E", LANGUAGE_FAROESE, "Faroese");
	code (_language_map, "0F", LANGUAGE_FRENCH, "French");
	code (_language_map, "10", LANGUAGE_FRISIAN, "Frisian");
	code (_language_map, "11", LANGUAGE_IRISH, "Irish");
	code (_language_map, "12", LANGUAGE_GAELIC, "Gaelic");
	code (_language_map, "13", LANGUAGE_GALACIAN, "Galacian");
	code (_language_map, "14", LANGUAGE_ICELANDIC, "Icelandic");
	code (_language_map, "15", LANGUAGE_ITALIAN, "Italian");
	code (_language_map, "16", LANGUAGE_LAPPISH, "Lappish");
	code (_language_map, "17", LANGUAGE_LATIN, "Latin");
	code (_language_map, "18", LANGUAGE_LATVIAN, "Latvian");
	code (_language_map, "19", LANGUAGE_LUXEMBORGIAN, "Luxemborgian");
	code (_language_map, "1A", LANGUAGE_LITHUANIAN, "Lithuanian");
	code (_language_map, "1B", LANGUAGE_HUNGARIAN, "Hungarian");
	code (_language_map, "1C", LANGUAGE_MALTESE, "Maltese");
	code (_language_map, "1D", LANGUAGE_DUTCH, "Dutch");
	code (_language_map, "1E", LANGUAGE_NORWEGIAN, "Norwegian");
	code (_language_map, "1F", LANGUAGE_OCCITAN, "Occitan");
	code (_language_map, "20", LANGUAGE_POLISH, "Polish");
	code (_language_map, "21", LANGUAGE_PORTUGESE, "Portugese");
	code (_language_map, "22", LANGUAGE_ROMANIAN, "Romanian");
	code (_language_map, "23", LANGUAGE_ROMANSH, "Romansh");
	code (_language_map, "24", LANGUAGE_SERBIAN, "Serbian");
	code (_language_map, "25", LANGUAGE_SLOVAK, "Slovak");
	code (_language_map, "26", LANGUAGE_SLOVENIAN, "Slovenian");
	code (_language_map, "27", LANGUAGE_FINNISH, "Finnish");
	code (_language_map, "28", LANGUAGE_SWEDISH, "Swedish");
	code (_language_map, "29", LANGUAGE_TURKISH, "Turkish");
	code (_language_map, "2A", LANGUAGE_FLEMISH, "Flemish");
	code (_language_map, "2B", LANGUAGE_WALLON, "Wallon");
	code (_language_map, "7F", LANGUAGE_AMHARIC, "Amharic");
	code (_language_map, "7E", LANGUAGE_ARABIC, "Arabic");
	code (_language_map, "7D", LANGUAGE_ARMENIAN, "Armenian");
	code (_language_map, "7C", LANGUAGE_ASSAMESE, "Assamese");
	code (_language_map, "7B", LANGUAGE_AZERBAIJANI, "Azerbaijani");
	code (_language_map, "7A", LANGUAGE_BAMBORA, "Bambora");
	code (_language_map, "79", LANGUAGE_BIELORUSSIAN, "Bielorussian");
	code (_language_map, "78", LANGUAGE_BENGALI, "Bengali");
	code (_language_map, "77", LANGUAGE_BULGARIAN, "Bulgarian");
	code (_language_map, "76", LANGUAGE_BURMESE, "Burmese");
	code (_language_map, "75", LANGUAGE_CHINESE, "Chinese");
	code (_language_map, "74", LANGUAGE_CHURASH, "Churash");
	code (_language_map, "73", LANGUAGE_DARI, "Dari");
	code (_language_map, "72", LANGUAGE_FULANI, "Fulani");
	code (_language_map, "71", LANGUAGE_GEORGIAN, "Georgian");
	code (_language_map, "70", LANGUAGE_GREEK, "Greek");
	code (_language_map, "6F", LANGUAGE_GUJURATI, "Gujarati");
	code (_language_map, "6E", LANGUAGE_GURANI, "Gurani");
	code (_language_map, "6D", LANGUAGE_HAUSA, "Hausa");
	code (_language_map, "6C", LANGUAGE_HEBREW, "Hebrew");
	code (_language_map, "6B", LANGUAGE_HINDI, "Hindi");
	code (_language_map, "6A", LANGUAGE_INDONESIAN, "Indonesian");
	code (_language_map, "69", LANGUAGE_JAPANESE, "Japanese");
	code (_language_map, "68", LANGUAGE_KANNADA, "Kannada");
	code (_language_map, "67", LANGUAGE_KAZAKH, "Kazakh");
	code (_language_map, "66", LANGUAGE_KHMER, "Khmer");
	code (_language_map, "65", LANGUAGE_KOREAN, "Korean");
	code (_language_map, "64", LANGUAGE_LAOTIAN, "Laotian");
	code (_language_map, "63", LANGUAGE_MACEDONIAN, "Macedonian");
	code (_language_map, "62", LANGUAGE_MALAGASAY, "Malagasay");
	code (_language_map, "61", LANGUAGE_MALAYSIAN, "Malaysian");
	code (_language_map, "60", LANGUAGE_MOLDAVIAN, "Moldavian");
	code (_language_map, "5F", LANGUAGE_MARATHI, "Marathi");
	code (_language_map, "5E", LANGUAGE_NDEBELE, "Ndebele");
	code (_language_map, "5D", LANGUAGE_NEPALI, "Nepali");
	code (_language_map, "5C", LANGUAGE_ORIYA, "Oriya");
	code (_language_map, "5B", LANGUAGE_PAPAMIENTO, "Papamiento");
	code (_language_map, "5A", LANGUAGE_PERSIAN, "Persian");
	code (_language_map, "59", LANGUAGE_PUNJABI, "Punjabi");
	code (_language_map, "58", LANGUAGE_PUSHTU, "Pushtu");
	code (_language_map, "57", LANGUAGE_QUECHUA, "Quechua");
	code (_language_map, "56", LANGUAGE_RUSSIAN, "Russian");
	code (_language_map, "55", LANGUAGE_RUTHENIAN, "Ruthenian");
	code (_language_map, "54", LANGUAGE_SERBO_CROAT, "Serbo Croat");
	code (_language_map, "53", LANGUAGE_SHONA, "Shona");
	code (_language_map, "52", LANGUAGE_SINHALESE, "Sinhalese");
	code (_language_map, "51", LANGUAGE_SOMALI, "Somali");
	code (_language_map, "50", LANGUAGE_SRANAN_TONGO, "Sranan Tongo");
	code (_language_map, "4F", LANGUAGE_SWAHILI, "Swahili");
	code (_language_map, "4E", LANGUAGE_TADZHIK, "Tadzhik");
	code (_language_map, "4D", LANGUAGE_TAMIL, "Tamil");
	code (_language_map, "4C", LANGUAGE_TATAR, "Tatar");
	code (_language_map, "4B", LANGUAGE_TELUGU, "Telugu");
	code (_language_map, "4A", LANGUAGE_THAI, "Thai");
	code (_language_map, "49", LANGUAGE_UKRANIAN, "Ukranian");
	code (_language_map, "48", LANGUAGE_URDU, "Urdu");
	code (_language_map, "47", LANGUAGE_UZBEK, "Uzbek");
	code (_language_map, "46", LANGUAGE_VIETNAMESE, "Vietnamese");
	code (_language_map, "45", LANGUAGE_ZULU, "Zulu");

	code (_timecode_status_map, "0", TIMECODE_STATUS_NOT_INTENDED_FOR_USE, "Not intended for use");
	code (_timecode_status_map, "1", TIMECODE_STATUS_INTENDED_FOR_USE, "Intended for use");
}
