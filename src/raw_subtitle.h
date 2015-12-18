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

#ifndef LIBSUB_RAW_SUBTITLE_H
#define LIBSUB_RAW_SUBTITLE_H

#include "sub_time.h"
#include "colour.h"
#include "vertical_reference.h"
#include "effect.h"
#include "font_size.h"
#include "vertical_position.h"
#include <boost/optional.hpp>
#include <string>
#include <list>

namespace sub {

/** @class RawSubtitle
 *  @brief A bit of a subtitle, created with no regard for any nearby bits.
 */
class RawSubtitle
{
public:
	RawSubtitle ()
		: colour (1, 1, 1)
		, bold (false)
		, italic (false)
		, underline (false)
	{}

	/** Subtitle text in UTF-8 */
	std::string text;
	boost::optional<std::string> font;

	/** font size */
	FontSize font_size;

	boost::optional<Effect> effect;
	boost::optional<Colour> effect_colour;

	Colour colour;
	bool bold;      ///< true to use a bold version of font
	bool italic;    ///< true to use an italic version of font
	bool underline; ///< true to underline

	/** vertical position of the baseline of the text */
	VerticalPosition vertical_position;

	/** from time */
	Time from;
	/** to time */
	Time to;

	boost::optional<Time> fade_up;
	boost::optional<Time> fade_down;
};

bool operator< (RawSubtitle const &, RawSubtitle const &);

}

#endif
