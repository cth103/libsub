/*
    Copyright (C) 2026 Carl Hetherington <cth@carlh.net>

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


#include "subrip_writer.h"
#include "subtitle.h"
#include <fmt/format.h>
#include <algorithm>
#include <fstream>


using std::string;
using std::vector;
using namespace sub;


static string
time_to_subrip_string(Time t)
{
	return fmt::format("{:02d}:{:02d}:{:02d},{:03d}", t.hours(), t.minutes(), t.seconds(), t.milliseconds());
}


static string
wrap_with_tags(string text, bool bold, bool italic, bool underline)
{
	if (bold) {
		text = "<b>" + text + "</b>";
	}
	if (italic) {
		text = "<i>" + text + "</i>";
	}
	if (underline) {
		text = "<u>" + text + "</u>";
	}
	return text;
}


void
sub::write_subrip(vector<Subtitle> const& subtitles, boost::filesystem::path file_name)
{
	std::ofstream f(file_name.string());

	int index = 1;
	for (auto const& sub: subtitles) {
		f << index << "\n";
		f << time_to_subrip_string(sub.from) << " --> " << time_to_subrip_string(sub.to) << "\n";

		auto sorted_lines = sub.lines;
		std::sort(sorted_lines.begin(), sorted_lines.end(), [](Line const& a, Line const& b) { return a.vertical_position < b.vertical_position; });
		for (auto const& line: sorted_lines) {
			for (auto const& block: line.blocks) {
				f << wrap_with_tags(block.text, block.bold, block.italic, block.underline);
			}
			f << "\n";
		}
		f << "\n";
		++index;
	}

	f.close();
}
