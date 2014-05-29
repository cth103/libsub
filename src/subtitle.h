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

#include "frame_time.h"
#include "metric_time.h"
#include "colour.h"
#include "vertical_reference.h"
#include "effect.h"
#include "time_pair.h"
#include <boost/optional.hpp>
#include <string>
#include <list>

namespace sub {

/** A piece of text with a single font, style, size etc. */       
class Block
{
public:
	Block ()
		: colour (1, 1, 1)
		, bold (false)
		, italic (false)
		, underline (false)
	{}
	
	/** Subtitle text in UTF-8 */
	std::string text;
	std::string font;

	/** font size */
	class FontSize {
	public:
		void set_proportional (float p) {
			_proportional = p;
		}

		void set_points (int p) {
			_points = p;
		}

		boost::optional<float> proportional () const {
			return _proportional;
		}

		boost::optional<int> points () const {
			return _points;
		}
		
		float proportional (int screen_height_in_points) const;
		int points (int screen_height_in_points) const;

	private:		
		/** as a proportion of screen height */
		boost::optional<float> _proportional;
		/** in points */
		boost::optional<int> _points;
		
	} font_size;

	boost::optional<Effect> effect;
	boost::optional<Colour> effect_colour;
	
	Colour colour;
	bool bold;      ///< true to use a bold version of font
	bool italic;    ///< true to use an italic version of font
	bool underline; ///< true to underline
};

/** A line of text which starts and stops at specific times */	
class Subtitle
{
public:
	Subtitle ()
	{}

	/** vertical position of the baseline of the text */
	struct VerticalPosition {

		/** as a proportion of screen height offset from some reference point */
		boost::optional<float> proportional;
		/** reference position for proportional */
		boost::optional<VerticalReference> reference;
		/** line number from the top of the screen */
		boost::optional<int> line;

		bool operator== (VerticalPosition const & other) const;
		
	} vertical_position;

	/** from time */
	TimePair from;
	/** to time */
	TimePair to;
	
	boost::optional<MetricTime> fade_up;
	boost::optional<MetricTime> fade_down;

	std::list<Block> blocks;

	bool same_metadata (Subtitle const &) const;
};

bool operator< (Subtitle const & a, Subtitle const & b);	

}

#endif
