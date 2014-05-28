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

#include <map>
#include "reader.h"

namespace sub {

template<class T>
class STLCode
{
public:
	STLCode ()
		: value ((T) 0)
	{}

	STLCode (T v, std::string d)
		: value (v)
		, description (d)
	{}
	
	T value;
	std::string description;
};

class STLBinaryReader : public Reader
{
public:
	STLBinaryReader (std::istream &);
	~STLBinaryReader ();

	std::map<std::string, std::string> metadata () const;

	enum DisplayStandard {
		DISPLAY_STANDARD_UNDEFINED,
		DISPLAY_STANDARD_OPEN_SUBTITLING,
		DISPLAY_STANDARD_LEVEL_1_TELETEXT,
		DISPLAY_STANDARD_LEVEL_2_TELETEXT
	};

	enum LanguageGroup {
		LANGUAGE_GROUP_LATIN,
		LANGUAGE_GROUP_LATIN_CYRILLIC,
		LANGUAGE_GROUP_LATIN_ARABIC,
		LANGUAGE_GROUP_LATIN_GREEK,
		LANGUAGE_GROUP_LATIN_HEBREW
	};

	enum Language {
		LANGUAGE_UNKNOWN,
		LANGUAGE_ALBANIAN,
		LANGUAGE_BRETON,
		LANGUAGE_CATALAN,
		LANGUAGE_CROATIAN,
		LANGUAGE_WELSH,
		LANGUAGE_CZECH,
		LANGUAGE_DANISH,
		LANGUAGE_GERMAN,
		LANGUAGE_ENGLISH,
		LANGUAGE_SPANISH,
		LANGUAGE_ESPERANTO,
		LANGUAGE_ESTONIAN,
		LANGUAGE_BASQUE,
		LANGUAGE_FAROESE,
		LANGUAGE_FRENCH,
		LANGUAGE_FRISIAN,
		LANGUAGE_IRISH,
		LANGUAGE_GAELIC,
		LANGUAGE_GALACIAN,
		LANGUAGE_ICELANDIC,
		LANGUAGE_ITALIAN,
		LANGUAGE_LAPPISH,
		LANGUAGE_LATIN,
		LANGUAGE_LATVIAN,
		LANGUAGE_LUXEMBORGIAN,
		LANGUAGE_LITHUANIAN,
		LANGUAGE_HUNGARIAN,
		LANGUAGE_MALTESE,
		LANGUAGE_DUTCH,
		LANGUAGE_NORWEGIAN,
		LANGUAGE_OCCITAN,
		LANGUAGE_POLISH,
		LANGUAGE_PORTUGESE,
		LANGUAGE_ROMANIAN,
		LANGUAGE_ROMANSH,
		LANGUAGE_SERBIAN,
		LANGUAGE_SLOVAK,
		LANGUAGE_SLOVENIAN,
		LANGUAGE_FINNISH,
		LANGUAGE_SWEDISH,
		LANGUAGE_TURKISH,
		LANGUAGE_FLEMISH,
		LANGUAGE_WALLON,
		LANGUAGE_AMHARIC,
		LANGUAGE_ARABIC,
		LANGUAGE_ARMENIAN,
		LANGUAGE_ASSAMESE,
		LANGUAGE_AZERBAIJANI,
		LANGUAGE_BAMBORA,
		LANGUAGE_BIELORUSSIAN,
		LANGUAGE_BENGALI,
		LANGUAGE_BULGARIAN,
		LANGUAGE_BURMESE,
		LANGUAGE_CHINESE,
		LANGUAGE_CHURASH,
		LANGUAGE_DARI,
		LANGUAGE_FULANI,
		LANGUAGE_GEORGIAN,
		LANGUAGE_GREEK,
		LANGUAGE_GUJURATI,
		LANGUAGE_GURANI,
		LANGUAGE_HAUSA,
		LANGUAGE_HEBREW,
		LANGUAGE_HINDI,
		LANGUAGE_INDONESIAN,
		LANGUAGE_JAPANESE,
		LANGUAGE_KANNADA,
		LANGUAGE_KAZAKH,
		LANGUAGE_KHMER,
		LANGUAGE_KOREAN,
		LANGUAGE_LAOTIAN,
		LANGUAGE_MACEDONIAN,
		LANGUAGE_MALAGASAY,
		LANGUAGE_MALAYSIAN,
		LANGUAGE_MOLDAVIAN,
		LANGUAGE_MARATHI,
		LANGUAGE_NDEBELE,
		LANGUAGE_NEPALI,
		LANGUAGE_ORIYA,
		LANGUAGE_PAPAMIENTO,
		LANGUAGE_PERSIAN,
		LANGUAGE_PUNJABI,
		LANGUAGE_PUSHTU,
		LANGUAGE_QUECHUA,
		LANGUAGE_RUSSIAN,
		LANGUAGE_RUTHENIAN,
		LANGUAGE_SERBO_CROAT,
		LANGUAGE_SHONA,
		LANGUAGE_SINHALESE,
		LANGUAGE_SOMALI,
		LANGUAGE_SRANAN_TONGO,
		LANGUAGE_SWAHILI,
		LANGUAGE_TADZHIK,
		LANGUAGE_TAMIL,
		LANGUAGE_TATAR,
		LANGUAGE_TELUGU,
		LANGUAGE_THAI,
		LANGUAGE_UKRANIAN,
		LANGUAGE_URDU,
		LANGUAGE_UZBEK,
		LANGUAGE_VIETNAMESE,
		LANGUAGE_ZULU
	};

	enum TimecodeStatus {
		TIMECODE_STATUS_NOT_INTENDED_FOR_USE,
		TIMECODE_STATUS_INTENDED_FOR_USE
	};

	enum CumulativeStatus {
		CUMULATIVE_STATUS_NOT_CUMULATIVE,
		CUMULATIVE_STATUS_FIRST,
		CUMULATIVE_STATUS_INTERMEDIATE,
		CUMULATIVE_STATUS_LAST
	};

	enum Justification {
		JUSTIFICATION_NONE,
		JUSTIFICATION_LEFT,
		JUSTIFICATION_CENTRE,
		JUSTIFICATION_RIGHT
	};

	enum Comment {
		COMMENT_NO,
		COMMENT_YES
	};

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
	
	void create_tables ();
	
	std::map<std::string, STLCode<DisplayStandard> > _display_standard_map;
	std::map<std::string, STLCode<LanguageGroup> > _language_group_map;
	std::map<std::string, STLCode<Language> > _language_map;
	std::map<std::string, STLCode<TimecodeStatus> > _timecode_status_map;
	std::map<int, STLCode<CumulativeStatus> > _cumulative_status_map;
	std::map<int, STLCode<Justification> > _justification_map;
	std::map<int, STLCode<Comment> > _comment_map;

	unsigned char* _buffer;
};

}
