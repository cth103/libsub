/*
    Copyright (C) 2022 Carl Hetherington <cth@carlh.net>

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


#ifndef LIBSUB_WEB_VTT_READER_H
#define LIBSUB_WEB_VTT_READER_H


#include "reader.h"
#include <cstdio>
#include <list>
#include <string>


namespace sub {


class WebVTTReader : public Reader
{
public:
	WebVTTReader(FILE* file);
	WebVTTReader(std::string subs);

private:
	void read(std::function<boost::optional<std::string> ()> get_line);

	std::list<std::string> _context;
};


}

#endif

