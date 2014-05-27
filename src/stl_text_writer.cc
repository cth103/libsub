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

#include "stl_text_writer.h"
#include <boost/optional.hpp>

using std::list;
using std::ostream;
using std::string;
using boost::optional;
using namespace sub;

STLTextWriter::STLTextWriter (list<Subtitle> subtitles, int screen_height_in_points, float frames_per_second, ostream& out)
	: Writer (subtitles, screen_height_in_points, frames_per_second)
{
	optional<string> font;
	optional<int> font_size;
	bool bold = false;
	bool italic = false;
	bool underline = false;
	int line = 0;
	optional<FrameTime> from;
	optional<FrameTime> to;
	
	for (list<Subtitle>::const_iterator i = subtitles.begin(); i != subtitles.end(); ++i) {
		bool started_new = false;
		if (!font || font.get() != i->font) {
			out << "\n$FontName = " << i->font;
			font = i->font;
			started_new = true;
		}
		if (!font_size || font_size.get() != i->font_size_points (screen_height_in_points)) {
			out << "\n$FontSize = " << i->font_size_points (screen_height_in_points);
			font_size = i->font_size_points (screen_height_in_points);
			started_new = true;
		}
		string text;
		if (bold != i->bold) {
			text += "^B";
			bold = i->bold;
		}
		if (italic != i->italic) {
			text += "^I";
			italic = i->italic;
		}
		if (underline != i->underline) {
			text += "^U";
			underline = i->underline;
		}

		text += i->text;

		if (from && from.get() == i->from_frame(frames_per_second) && to && to.get() == i->to_frame(frames_per_second) && !started_new) {
		        for (int j = line; j < i->line; ++j) {
				out << "|";
			}
			out << text;
			line = i->line;
		} else {
			out << "\n" << i->from_frame(frames_per_second).timecode() << "," << i->to_frame(frames_per_second).timecode() << "," << text;
			from = i->from_frame (frames_per_second);
			to = i->to_frame (frames_per_second);
			line = 0;
		}
	}

	out << "\n";
}
