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

#include "stl_binary_tables.h"
#include "exceptions.h"
#include "compose.hpp"
#include <cassert>

using std::map;
using std::string;
using namespace sub;

template <class E, class F>
void
code (map<F, STLBinaryCode<E> >& m, F k, E v, string d)
{
	m[k] = STLBinaryCode<E> (v, d);
}

template <class E, class F>
E
file_to_enum (F k, map<F, STLBinaryCode<E> > m, string name)
{
	typename map<F, STLBinaryCode<E> >::const_iterator i = m.find (k);
	if (i == m.end ()) {
		throw STLError (String::compose ("Unknown %1 %2 in binary STL file", name, k));
	}

	return i->second.value;
}

template <class E, class F>
F
enum_to_file (E k, map<F, STLBinaryCode<E> > m)
{
	for (typename map<F, STLBinaryCode<E> >::const_iterator i = m.begin(); i != m.end(); ++i) {
		if (i->second.value == k) {
			return i->first;
		}
	}

	assert (false);
	return F ();
}

template <class E, class F>
string
enum_to_description (E v, map<F, STLBinaryCode<E> > const & m)
{
	for (typename map<F, STLBinaryCode<E> >::const_iterator i = m.begin(); i != m.end(); ++i) {
		if (i->second.value == v) {
			return i->second.description;
		}
	}

	return "";
}

template <class E, class F>
boost::optional<E>
description_to_enum (string d, map<F, STLBinaryCode<E> > const & m)
{
	for (typename map<F, STLBinaryCode<E> >::const_iterator i = m.begin(); i != m.end(); ++i) {
		if (i->second.description == d) {
			return i->second.value;
		}
	}

	return boost::optional<E> ();
}

DisplayStandard
STLBinaryTables::display_standard_file_to_enum (string s) const
{
	return file_to_enum (s, _display_standard_map, "display standard code");
}

LanguageGroup
STLBinaryTables::language_group_file_to_enum (string s) const
{
	return file_to_enum (s, _language_group_map, "language group code");
}

Language
STLBinaryTables::language_file_to_enum (string s) const
{
	return file_to_enum (s, _language_map, "language code");
}

TimecodeStatus
STLBinaryTables::timecode_status_file_to_enum (string s) const
{
	return file_to_enum (s, _timecode_status_map, "timecode status code");
}

CumulativeStatus
STLBinaryTables::cumulative_status_file_to_enum (int s) const
{
	return file_to_enum (s, _cumulative_status_map, "cumulative status code");
}

Justification
STLBinaryTables::justification_file_to_enum (int s) const
{
	return file_to_enum (s, _justification_map, "justification code");
}

Comment
STLBinaryTables::comment_file_to_enum (int s) const
{
	return file_to_enum (s, _comment_map, "comment code");
}

string
STLBinaryTables::language_enum_to_file (Language e) const
{
	return enum_to_file (e, _language_map);
}

int
STLBinaryTables::cumulative_status_enum_to_file (CumulativeStatus v) const
{
	return enum_to_file (v, _cumulative_status_map);
}

int
STLBinaryTables::justification_enum_to_file (Justification v) const
{
	return enum_to_file (v, _justification_map);
}

int
STLBinaryTables::comment_enum_to_file (Comment v) const
{
	return enum_to_file (v, _comment_map);
}

string
STLBinaryTables::display_standard_enum_to_description (DisplayStandard v) const
{
	return enum_to_description (v, _display_standard_map);
}

string
STLBinaryTables::language_group_enum_to_description (LanguageGroup v) const
{
	return enum_to_description (v, _language_group_map);
}

string
STLBinaryTables::language_enum_to_description (Language v) const
{
	return enum_to_description (v, _language_map);
}

string
STLBinaryTables::timecode_status_enum_to_description (TimecodeStatus v) const
{
	return enum_to_description (v, _timecode_status_map);
}

string
STLBinaryTables::cumulative_status_enum_to_description (CumulativeStatus v) const
{
	return enum_to_description (v, _cumulative_status_map);
}

string
STLBinaryTables::justification_enum_to_description (Justification v) const
{
	return enum_to_description (v, _justification_map);
}

string
STLBinaryTables::comment_enum_to_description (Comment v) const
{
	return enum_to_description (v, _comment_map);
}

boost::optional<Language>
STLBinaryTables::language_description_to_enum (string d) const
{
	return description_to_enum (d, _language_map);
}

STLBinaryTables::STLBinaryTables ()
{
	code<DisplayStandard, string> (_display_standard_map, " ", DISPLAY_STANDARD_UNDEFINED, "Undefined");
	code<DisplayStandard, string> (_display_standard_map, "0", DISPLAY_STANDARD_OPEN_SUBTITLING, "Open subtitling");
	code<DisplayStandard, string> (_display_standard_map, "1", DISPLAY_STANDARD_LEVEL_1_TELETEXT, "Level 1 teletext");
	code<DisplayStandard, string> (_display_standard_map, "2", DISPLAY_STANDARD_LEVEL_2_TELETEXT, "Level 2 teletext");

	code<LanguageGroup, string> (_language_group_map, "00", LANGUAGE_GROUP_LATIN, "Latin");
	code<LanguageGroup, string> (_language_group_map, "01", LANGUAGE_GROUP_LATIN_CYRILLIC, "Latin/Cyrillic");
	code<LanguageGroup, string> (_language_group_map, "02", LANGUAGE_GROUP_LATIN_ARABIC, "Latin/Arabic");
	code<LanguageGroup, string> (_language_group_map, "03", LANGUAGE_GROUP_LATIN_GREEK, "Latin/Greek");
	code<LanguageGroup, string> (_language_group_map, "04", LANGUAGE_GROUP_LATIN_HEBREW, "Latin/Hebrew");

	code<Language, string> (_language_map, "00", LANGUAGE_UNKNOWN, "Unknown");
	code<Language, string> (_language_map, "01", LANGUAGE_ALBANIAN, "Albanian");
	code<Language, string> (_language_map, "02", LANGUAGE_BRETON, "Breton");
	code<Language, string> (_language_map, "03", LANGUAGE_CATALAN, "Catalan");
	code<Language, string> (_language_map, "04", LANGUAGE_CROATIAN, "Croatian");
	code<Language, string> (_language_map, "05", LANGUAGE_WELSH, "Welsh");
	code<Language, string> (_language_map, "06", LANGUAGE_CZECH, "Czech");
	code<Language, string> (_language_map, "07", LANGUAGE_DANISH, "Danish");
	code<Language, string> (_language_map, "08", LANGUAGE_GERMAN, "German");
	code<Language, string> (_language_map, "09", LANGUAGE_ENGLISH, "English");
	code<Language, string> (_language_map, "0A", LANGUAGE_SPANISH, "Spanish");
	code<Language, string> (_language_map, "0B", LANGUAGE_ESPERANTO, "Esperanto");
	code<Language, string> (_language_map, "0C", LANGUAGE_ESTONIAN, "Estonian");
	code<Language, string> (_language_map, "0D", LANGUAGE_BASQUE, "Basque");
	code<Language, string> (_language_map, "0E", LANGUAGE_FAROESE, "Faroese");
	code<Language, string> (_language_map, "0F", LANGUAGE_FRENCH, "French");
	code<Language, string> (_language_map, "10", LANGUAGE_FRISIAN, "Frisian");
	code<Language, string> (_language_map, "11", LANGUAGE_IRISH, "Irish");
	code<Language, string> (_language_map, "12", LANGUAGE_GAELIC, "Gaelic");
	code<Language, string> (_language_map, "13", LANGUAGE_GALACIAN, "Galacian");
	code<Language, string> (_language_map, "14", LANGUAGE_ICELANDIC, "Icelandic");
	code<Language, string> (_language_map, "15", LANGUAGE_ITALIAN, "Italian");
	code<Language, string> (_language_map, "16", LANGUAGE_LAPPISH, "Lappish");
	code<Language, string> (_language_map, "17", LANGUAGE_LATIN, "Latin");
	code<Language, string> (_language_map, "18", LANGUAGE_LATVIAN, "Latvian");
	code<Language, string> (_language_map, "19", LANGUAGE_LUXEMBORGIAN, "Luxemborgian");
	code<Language, string> (_language_map, "1A", LANGUAGE_LITHUANIAN, "Lithuanian");
	code<Language, string> (_language_map, "1B", LANGUAGE_HUNGARIAN, "Hungarian");
	code<Language, string> (_language_map, "1C", LANGUAGE_MALTESE, "Maltese");
	code<Language, string> (_language_map, "1D", LANGUAGE_DUTCH, "Dutch");
	code<Language, string> (_language_map, "1E", LANGUAGE_NORWEGIAN, "Norwegian");
	code<Language, string> (_language_map, "1F", LANGUAGE_OCCITAN, "Occitan");
	code<Language, string> (_language_map, "20", LANGUAGE_POLISH, "Polish");
	code<Language, string> (_language_map, "21", LANGUAGE_PORTUGESE, "Portugese");
	code<Language, string> (_language_map, "22", LANGUAGE_ROMANIAN, "Romanian");
	code<Language, string> (_language_map, "23", LANGUAGE_ROMANSH, "Romansh");
	code<Language, string> (_language_map, "24", LANGUAGE_SERBIAN, "Serbian");
	code<Language, string> (_language_map, "25", LANGUAGE_SLOVAK, "Slovak");
	code<Language, string> (_language_map, "26", LANGUAGE_SLOVENIAN, "Slovenian");
	code<Language, string> (_language_map, "27", LANGUAGE_FINNISH, "Finnish");
	code<Language, string> (_language_map, "28", LANGUAGE_SWEDISH, "Swedish");
	code<Language, string> (_language_map, "29", LANGUAGE_TURKISH, "Turkish");
	code<Language, string> (_language_map, "2A", LANGUAGE_FLEMISH, "Flemish");
	code<Language, string> (_language_map, "2B", LANGUAGE_WALLON, "Wallon");
	code<Language, string> (_language_map, "7F", LANGUAGE_AMHARIC, "Amharic");
	code<Language, string> (_language_map, "7E", LANGUAGE_ARABIC, "Arabic");
	code<Language, string> (_language_map, "7D", LANGUAGE_ARMENIAN, "Armenian");
	code<Language, string> (_language_map, "7C", LANGUAGE_ASSAMESE, "Assamese");
	code<Language, string> (_language_map, "7B", LANGUAGE_AZERBAIJANI, "Azerbaijani");
	code<Language, string> (_language_map, "7A", LANGUAGE_BAMBORA, "Bambora");
	code<Language, string> (_language_map, "79", LANGUAGE_BIELORUSSIAN, "Bielorussian");
	code<Language, string> (_language_map, "78", LANGUAGE_BENGALI, "Bengali");
	code<Language, string> (_language_map, "77", LANGUAGE_BULGARIAN, "Bulgarian");
	code<Language, string> (_language_map, "76", LANGUAGE_BURMESE, "Burmese");
	code<Language, string> (_language_map, "75", LANGUAGE_CHINESE, "Chinese");
	code<Language, string> (_language_map, "74", LANGUAGE_CHURASH, "Churash");
	code<Language, string> (_language_map, "73", LANGUAGE_DARI, "Dari");
	code<Language, string> (_language_map, "72", LANGUAGE_FULANI, "Fulani");
	code<Language, string> (_language_map, "71", LANGUAGE_GEORGIAN, "Georgian");
	code<Language, string> (_language_map, "70", LANGUAGE_GREEK, "Greek");
	code<Language, string> (_language_map, "6F", LANGUAGE_GUJURATI, "Gujarati");
	code<Language, string> (_language_map, "6E", LANGUAGE_GURANI, "Gurani");
	code<Language, string> (_language_map, "6D", LANGUAGE_HAUSA, "Hausa");
	code<Language, string> (_language_map, "6C", LANGUAGE_HEBREW, "Hebrew");
	code<Language, string> (_language_map, "6B", LANGUAGE_HINDI, "Hindi");
	code<Language, string> (_language_map, "6A", LANGUAGE_INDONESIAN, "Indonesian");
	code<Language, string> (_language_map, "69", LANGUAGE_JAPANESE, "Japanese");
	code<Language, string> (_language_map, "68", LANGUAGE_KANNADA, "Kannada");
	code<Language, string> (_language_map, "67", LANGUAGE_KAZAKH, "Kazakh");
	code<Language, string> (_language_map, "66", LANGUAGE_KHMER, "Khmer");
	code<Language, string> (_language_map, "65", LANGUAGE_KOREAN, "Korean");
	code<Language, string> (_language_map, "64", LANGUAGE_LAOTIAN, "Laotian");
	code<Language, string> (_language_map, "63", LANGUAGE_MACEDONIAN, "Macedonian");
	code<Language, string> (_language_map, "62", LANGUAGE_MALAGASAY, "Malagasay");
	code<Language, string> (_language_map, "61", LANGUAGE_MALAYSIAN, "Malaysian");
	code<Language, string> (_language_map, "60", LANGUAGE_MOLDAVIAN, "Moldavian");
	code<Language, string> (_language_map, "5F", LANGUAGE_MARATHI, "Marathi");
	code<Language, string> (_language_map, "5E", LANGUAGE_NDEBELE, "Ndebele");
	code<Language, string> (_language_map, "5D", LANGUAGE_NEPALI, "Nepali");
	code<Language, string> (_language_map, "5C", LANGUAGE_ORIYA, "Oriya");
	code<Language, string> (_language_map, "5B", LANGUAGE_PAPAMIENTO, "Papamiento");
	code<Language, string> (_language_map, "5A", LANGUAGE_PERSIAN, "Persian");
	code<Language, string> (_language_map, "59", LANGUAGE_PUNJABI, "Punjabi");
	code<Language, string> (_language_map, "58", LANGUAGE_PUSHTU, "Pushtu");
	code<Language, string> (_language_map, "57", LANGUAGE_QUECHUA, "Quechua");
	code<Language, string> (_language_map, "56", LANGUAGE_RUSSIAN, "Russian");
	code<Language, string> (_language_map, "55", LANGUAGE_RUTHENIAN, "Ruthenian");
	code<Language, string> (_language_map, "54", LANGUAGE_SERBO_CROAT, "Serbo Croat");
	code<Language, string> (_language_map, "53", LANGUAGE_SHONA, "Shona");
	code<Language, string> (_language_map, "52", LANGUAGE_SINHALESE, "Sinhalese");
	code<Language, string> (_language_map, "51", LANGUAGE_SOMALI, "Somali");
	code<Language, string> (_language_map, "50", LANGUAGE_SRANAN_TONGO, "Sranan Tongo");
	code<Language, string> (_language_map, "4F", LANGUAGE_SWAHILI, "Swahili");
	code<Language, string> (_language_map, "4E", LANGUAGE_TADZHIK, "Tadzhik");
	code<Language, string> (_language_map, "4D", LANGUAGE_TAMIL, "Tamil");
	code<Language, string> (_language_map, "4C", LANGUAGE_TATAR, "Tatar");
	code<Language, string> (_language_map, "4B", LANGUAGE_TELUGU, "Telugu");
	code<Language, string> (_language_map, "4A", LANGUAGE_THAI, "Thai");
	code<Language, string> (_language_map, "49", LANGUAGE_UKRANIAN, "Ukranian");
	code<Language, string> (_language_map, "48", LANGUAGE_URDU, "Urdu");
	code<Language, string> (_language_map, "47", LANGUAGE_UZBEK, "Uzbek");
	code<Language, string> (_language_map, "46", LANGUAGE_VIETNAMESE, "Vietnamese");
	code<Language, string> (_language_map, "45", LANGUAGE_ZULU, "Zulu");

	code<TimecodeStatus, string> (_timecode_status_map, "0", TIMECODE_STATUS_NOT_INTENDED_FOR_USE, "Not intended for use");
	code<TimecodeStatus, string> (_timecode_status_map, "1", TIMECODE_STATUS_INTENDED_FOR_USE, "Intended for use");

	code<CumulativeStatus, int> (_cumulative_status_map, 0, CUMULATIVE_STATUS_NOT_CUMULATIVE, "Not part of a cumulative set");
	code<CumulativeStatus, int> (_cumulative_status_map, 1, CUMULATIVE_STATUS_FIRST, "First subtitle of a cumulative set");
	code<CumulativeStatus, int> (_cumulative_status_map, 2, CUMULATIVE_STATUS_INTERMEDIATE, "Intermediate subtitle of a cumulative set");
	code<CumulativeStatus, int> (_cumulative_status_map, 3, CUMULATIVE_STATUS_LAST, "Last subtitle of a cumulative set");

	code<Justification, int> (_justification_map, 0, JUSTIFICATION_NONE, "No justification");
	code<Justification, int> (_justification_map, 1, JUSTIFICATION_LEFT, "Left justification");
	code<Justification, int> (_justification_map, 2, JUSTIFICATION_CENTRE, "Centre justification");
	code<Justification, int> (_justification_map, 3, JUSTIFICATION_RIGHT, "Right justification");

	code<Comment, int> (_comment_map, 0, COMMENT_NO, "Not a comment");
	code<Comment, int> (_comment_map, 1, COMMENT_YES, "Is a comment");
}
