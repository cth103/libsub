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

#include "raw_subtitle.h"
#include "subtitle.h"
#include "collect.h"

using std::list;
using boost::optional;
using namespace sub;

list<Subtitle>
sub::collect (list<RawSubtitle> raw)
{
	raw.sort ();

	list<Subtitle> out;

	optional<Subtitle> current;
	for (list<RawSubtitle>::const_iterator i = raw.begin (); i != raw.end(); ++i) {
		if (current && current->same_metadata (*i)) {
			/* This RawSubtitle can be added to current... */
			if (!current->lines.empty() && current->lines.back().same_metadata (*i)) {
				/* ... and indeed to its last line */
				current->lines.back().blocks.push_back (Block (*i));
			} else {
				/* ... as a new line */
				current->lines.push_back (Line (*i));
			}
		} else {
			/* We must start a new Subtitle */
			if (current) {
				out.push_back (current.get ());
			}
			current = Subtitle (*i);
		}
	}

	if (current) {
		out.push_back (current.get ());
	}

	return out;
}