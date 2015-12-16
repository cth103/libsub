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

#ifndef LIBSUB_READER_H
#define LIBSUB_READER_H

#include "raw_subtitle.h"
#include <list>
#include <map>
#include <string>

struct subrip_reader_convert_line_test;

namespace sub {

/** @class Reader
 *  @brief Parent for classes which can read particular subtitle formats.
 */
class Reader
{
public:
	virtual ~Reader () {}

	std::list<RawSubtitle> subtitles () const {
		return _subs;
	}

	virtual std::map<std::string, std::string> metadata () const {
		return std::map<std::string, std::string> ();
	}

protected:
	friend struct ::subrip_reader_convert_line_test;

	void warn (std::string) const;

	std::list<RawSubtitle> _subs;
};

}

#endif
