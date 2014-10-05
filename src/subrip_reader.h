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

#include "reader.h"
#include "time_pair.h"

namespace sub {

class SubripReader : public Reader
{
public:
	SubripReader (FILE* f);

private:
	TimePair convert_time (std::string t);
	void convert_line (std::string t, int line_number, TimePair from, TimePair to);
	void maybe_content (RawSubtitle& p);
};

}
