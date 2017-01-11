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

#ifndef LIBSUB_SUBTITLE_H
#define LIBSUB_SUBTITLE_H

#include "colour.h"
#include "vertical_reference.h"
#include "effect.h"
#include "font_size.h"
#include "vertical_position.h"
#include "horizontal_position.h"
#include "raw_subtitle.h"
#include <boost/optional.hpp>
#include <string>
#include <list>

namespace sub {

/** @class Block
 *  @brief A block of text within a subtitle's line
 *
 *  This represents a block of text which has a particular style (font, size, effect, colour etc.)
 */
class Block
{
public:
	Block ()
		: colour (1, 1, 1)
		, bold (false)
		, italic (false)
		, underline (false)
	{}

	/** Construct a Block taking any relevant information from a RawSubtitle */
	Block (RawSubtitle s);

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
};

/** @class Line
 *  @brief A line of text within a subtitle.
 *
 *  This represents a line of text which has a particular vertical position.
 */
class Line
{
public:
	Line ()
		: horizontal_position (CENTRE)
	{}

	/** Construct a Line taking any relevant information from a RawSubtitle */
	Line (RawSubtitle s);

	HorizontalPosition horizontal_position;

	/** vertical position of the baseline of the text */
	VerticalPosition vertical_position;

	std::list<Block> blocks;

	bool same_metadata (RawSubtitle) const;
};

/** @class Subtitle
 *  @brief A subtitle which has been collected into lines and blocks.
 *
 *  This represents a chunk of text which appears and disappears at some particular
 *  times.
 */
class Subtitle
{
public:
	Subtitle ()
	{}

	/** Construct a Line taking any relevant information from a RawSubtitle */
	Subtitle (RawSubtitle s);

	/** from time */
	Time from;
	/** to time */
	Time to;

	boost::optional<Time> fade_up;
	boost::optional<Time> fade_down;

	std::list<Line> lines;

	bool same_metadata (RawSubtitle) const;
};

}

#endif
