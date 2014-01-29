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
#include <boost/optional.hpp>
#include <string>

namespace sub {

class Subtitle
{
public:
	Subtitle ()
		: colour (1, 1, 1)
		, bold (false)
		, italic (false)
		, underline (false)
		, line (0)
	{}

	std::string text;
	std::string font;

	/** font size */
	struct {
		/** as a proportion of screen height */
		boost::optional<float> proportional;
		/** in points */
		boost::optional<int> points;
	} font_size;

	/** vertical position of the baseline of the text */
	struct {
		/** as a proportion of screen height offset from some reference point */
		boost::optional<float> proportional;
		/** reference position for proportional */
		boost::optional<VerticalReference> reference;
	} vertical_position;

	boost::optional<Effect> effect;
	boost::optional<Colour> effect_colour;
	
	Colour colour;
	bool bold;      ///< true to use a bold version of font
	bool italic;    ///< true to use an italic version of font
	bool underline; ///< true to underline
	int line;

	/** from time */
	struct {
		boost::optional<FrameTime> frame;
		boost::optional<MetricTime> metric;
	} from;

	/** to time */
	struct {
		boost::optional<FrameTime> frame;
		boost::optional<MetricTime> metric;
	} to;

	boost::optional<MetricTime> fade_up;
	boost::optional<MetricTime> fade_down;
};

bool operator< (Subtitle const & a, Subtitle const & b);	

}

#endif
