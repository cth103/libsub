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

#include <string>
#include <boost/filesystem.hpp>

namespace sub {

class Subtitle;

extern void write_stl_binary (
	std::list<Subtitle> subtitles,
	float frames_per_second,
	std::string language,
	std::string original_programme_title,
	std::string original_episode_title,
	std::string translated_programme_title,
	std::string translated_episode_title,
	std::string translator_name,
	std::string translator_contact_details,
	std::string creation_date,
	std::string revision_date,
	int revision_number,
	std::string country_of_origin,
	std::string publisher,
	std::string editor_name,
	std::string editor_contact_details,
	boost::filesystem::path file_name
	);

}
