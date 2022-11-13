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

/** @file  src/subrip_reader.h
 *  @brief SubripReader class.
 */

#ifndef LIBSUB_SUBRIP_READER_H
#define LIBSUB_SUBRIP_READER_H

#include "reader.h"
#include <boost/function.hpp>
#include <list>

struct subrip_reader_convert_line_test;
struct subrip_reader_convert_time_test;
struct subrip_reader_test5;
struct subrip_reader_test6;

namespace sub {

class SubripReader : public Reader
{
public:
	SubripReader (FILE* f);
	SubripReader (std::string subs);

	static boost::optional<Time> convert_time(std::string t, std::string* expected = nullptr);

private:
	/* For tests */
	friend struct ::subrip_reader_convert_line_test;
	friend struct ::subrip_reader_convert_time_test;
	friend struct ::subrip_reader_test5;
	friend struct ::subrip_reader_test6;
	SubripReader () {}

	void convert_line (std::string t, RawSubtitle& p);
	void maybe_content (RawSubtitle& p);
	void read (boost::function<boost::optional<std::string> ()> get_line);

	std::list<std::string> _context;
};

}

#endif
