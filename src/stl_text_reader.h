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
#include <boost/optional.hpp>

namespace sub {

class STLTextReader : public Reader
{
public:
	STLTextReader (std::istream &);

private:
	void set (std::string name, std::string value);
	void maybe_push_subtitle ();
	void maybe_push_block ();
	boost::optional<FrameTime> time (std::string t) const;

	Subtitle _subtitle;
	Block _block;
};

}
