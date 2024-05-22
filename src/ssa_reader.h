/*
    Copyright (C) 2016 Carl Hetherington <cth@carlh.net>

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

/** @file  src/ssa_reader.h
 *  @brief SSAReader class.
 */

#ifndef LIBSUB_SSA_READER_H
#define LIBSUB_SSA_READER_H

#include "reader.h"
#include <boost/function.hpp>

namespace sub {

/** @class SSAReader
 *  @brief Reader for SubStation Alpha (SSA) and Advanced Substation Alpha (ASS) subtitles.
 *
 *  This reader implements some of the SSA and ASS "standards", as gathered from various
 *  documents on the web.
 */
class SSAReader : public Reader
{
public:
	SSAReader (FILE* f);
	SSAReader (std::string subs);

	class Context
	{
	public:
		Context(int play_res_x_, int play_res_y_, Colour primary_colour_, int left_margin_ = 0, int right_margin_ = 0)
			: play_res_x(play_res_x_)
			, play_res_y(play_res_y_)
			, primary_colour(primary_colour_)
			, left_margin(left_margin_)
			, right_margin(right_margin_)
		{}

		int play_res_x;
		int play_res_y;
		Colour primary_colour;
		int left_margin;
		int right_margin;

		void update_horizontal_position(RawSubtitle& sub) const;
	};

	static std::vector<RawSubtitle> parse_line(RawSubtitle base, std::string line, Context const& context);
	static void parse_tag(RawSubtitle& sub, std::string style, Context const& context);

private:
	void read (boost::function<boost::optional<std::string> ()> get_line);
	Time parse_time (std::string t) const;
};

}

#endif
