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

/** @file  src/subrip_reader.h
 *  @brief SubripReader class.
 */

#ifndef LIBSUB_SUBRIP_READER_H
#define LIBSUB_SUBRIP_READER_H

#include "reader.h"
#include <boost/function.hpp>

struct subrip_reader_convert_line_test;
struct subrip_reader_convert_time_test;

namespace sub {

class SubripReader : public Reader
{
public:
	SubripReader (FILE* f);
	SubripReader (std::string const & subs);

private:
	/* For tests */
	friend struct ::subrip_reader_convert_line_test;
	friend struct ::subrip_reader_convert_time_test;
	SubripReader () {}

	static Time convert_time (std::string t);
	void convert_line (std::string t, int line_number, Time from, Time to);
	void maybe_content (RawSubtitle& p);
	boost::optional<std::string> get_line_stringstream (std::stringstream* str) const;
	boost::optional<std::string> get_line_file (FILE* file) const;
	void read (boost::function<boost::optional<std::string> ()> get_line);
};

}

#endif
