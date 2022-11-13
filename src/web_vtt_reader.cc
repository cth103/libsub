/*
    Copyright (C) 2022 Carl Hetherington <cth@carlh.net>

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


#include "exceptions.h"
#include "subrip_reader.h"
#include "util.h"
#include "web_vtt_reader.h"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string_regex.hpp>
#include <boost/bind.hpp>
#include <boost/regex.hpp>
#include <iostream>
#include <vector>


using std::function;
using std::string;
using std::vector;
using boost::optional;
using namespace sub;


WebVTTReader::WebVTTReader(FILE* file)
{
	this->read(boost::bind(&get_line_file, file));
}


WebVTTReader::WebVTTReader(string subs)
{
	this->read(boost::bind(&get_line_string, &subs));
}


void
WebVTTReader::read(std::function<optional<string> ()> get_line)
{
	enum class State {
		/* expecting WEBVTT */
		HEADER,
		/* awaiting a NOTE or a subtitle timing line */
		DATA,
		/* reading the text of a subtitle */
		SUBTITLE,
		/* reading a note */
		NOTE
	} state = State::HEADER;

	RawSubtitle rs;

	rs.vertical_position.line = 0;
	rs.vertical_position.reference = TOP_OF_SUBTITLE;

	while (true) {
		auto line = get_line();
		if (!line) {
			break;
		}

		trim_right_if(*line, boost::is_any_of("\n\r"));
		remove_unicode_bom(line);

		/* Keep some history in case there is an error to report */
		_context.push_back(*line);
		if (_context.size() > 5) {
			_context.pop_front();
		}

		switch (state) {
		case State::HEADER:
			if (!boost::starts_with(*line, "WEBVTT")) {
				throw WebVTTError("No WEBVTT header found");
			}
			state = State::DATA;
			break;
		case State::DATA:
			if (boost::starts_with(*line, "NOTE")) {
				state = State::NOTE;
			} else if (line->find("-->") != string::npos) {
				/* Further trim this line, removing spaces from the end */
				trim_right_if(*line, boost::is_any_of(" "));

				vector<string> parts;
				boost::algorithm::split(parts, *line, boost::algorithm::is_any_of(" "), boost::token_compress_on);

				if (parts.size() != 3 && parts.size() != 7) {
					for (int i = 0; i < 2; ++i) {
						auto ex = get_line();
						if (ex) {
							_context.push_back(*ex);
						}
					}
					throw WebVTTError(*line, "a time line", _context);
				}

				string expected;
				auto from = SubripReader::convert_time(parts[0], &expected);
				if (!from) {
					throw WebVTTError(parts[0], expected, _context);
				}
				rs.from = *from;

				auto to = SubripReader::convert_time(parts[2], &expected);
				if (!to) {
					throw WebVTTError(parts[2], expected, _context);
				}
				rs.to = *to;

				rs.vertical_position.line = 0;
				state = State::SUBTITLE;
			} else if (!line->empty()) {
				throw WebVTTError(*line, "a note or time", _context);
			}
			break;
		case State::SUBTITLE:
			if (line->empty()) {
				state = State::DATA;
			} else {
				/* Split up this line on unicode "LINE SEPARATOR".  This feels hacky but also
				 * the least unpleasant place to do it.
				 */
				vector<string> sub_lines;
				boost::algorithm::split_regex(sub_lines, *line, boost::regex("\xe2\x80\xa8"));
				for (auto sub_line: sub_lines) {
					rs.text = sub_line;
					_subs.push_back(rs);
					rs.vertical_position.line = rs.vertical_position.line.get() + 1;
				}
			}
			break;
		case State::NOTE:
			if (line->empty()) {
				state = State::DATA;
			}
			break;
		}

	}
}

