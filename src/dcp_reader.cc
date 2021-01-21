/*
    Copyright (C) 2014-2021 Carl Hetherington <cth@carlh.net>

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

#include "dcp_reader.h"
#include "compose.hpp"
#include "exceptions.h"
#include <dcp/subtitle_string.h>
#include <dcp/interop_subtitle_asset.h>
#include <dcp/smpte_subtitle_asset.h>
#include <boost/filesystem.hpp>

using std::cout;
using std::string;
using std::exception;
using std::shared_ptr;
using boost::optional;
using std::dynamic_pointer_cast;
using namespace sub;

static Time
dcp_to_sub_time (dcp::Time t)
{
	return Time::from_hms (t.h, t.m, t.s, t.e * 1000.0 / t.tcr);
}

static Colour
dcp_to_sub_colour (dcp::Colour c)
{
	return Colour (c.r / 255.0, c.g / 255.0, c.b / 255.0);
}

DCPReader::DCPReader (boost::filesystem::path file)
{
	shared_ptr<dcp::SubtitleAsset> sc;
	string interop_error;
	string smpte_error;

	try {
		sc.reset (new dcp::InteropSubtitleAsset (file));
	} catch (exception& e) {
		interop_error = e.what ();
	}

	if (!sc) {
		try {
			sc.reset (new dcp::SMPTESubtitleAsset (file));
		} catch (exception& e) {
			smpte_error = e.what();
		}
	}

	if (!sc) {
		throw DCPError (String::compose ("Could not read subtitles (%1 / %2)", interop_error, smpte_error));
	}


	for (auto i: sc->subtitles()) {

		/* We don't deal with image subs */
		auto is = dynamic_pointer_cast<const dcp::SubtitleString>(i);
		if (!is) {
			continue;
		}

		RawSubtitle rs;
		rs.text = is->text ();
		rs.font = is->font ();
		rs.font_size = FontSize::from_proportional (is->size() / (72.0 * 11.0));

		switch (is->effect ()) {
		case dcp::BORDER:
			rs.effect = BORDER;
			break;
		case dcp::SHADOW:
			rs.effect = SHADOW;
			break;
		default:
			break;
		}

		rs.effect_colour = dcp_to_sub_colour (is->effect_colour());

		rs.colour = dcp_to_sub_colour (is->colour());
		rs.bold = is->bold ();
		rs.italic = is->italic ();
		rs.underline = is->underline ();

		switch (is->h_align()) {
		case dcp::HALIGN_LEFT:
			rs.horizontal_position.reference = LEFT_OF_SCREEN;
			break;
		case dcp::HALIGN_CENTER:
			rs.horizontal_position.reference = HORIZONTAL_CENTRE_OF_SCREEN;
			break;
		case dcp::HALIGN_RIGHT:
			rs.horizontal_position.reference = RIGHT_OF_SCREEN;
			break;
		}

		rs.vertical_position.proportional = is->v_position();
		switch (is->v_align()) {
		case dcp::VALIGN_TOP:
			rs.vertical_position.reference = TOP_OF_SCREEN;
			break;
		case dcp::VALIGN_CENTER:
			rs.vertical_position.reference = VERTICAL_CENTRE_OF_SCREEN;
			break;
		case dcp::VALIGN_BOTTOM:
			rs.vertical_position.reference = BOTTOM_OF_SCREEN;
			break;
		}

		rs.from = dcp_to_sub_time (is->in ());
		rs.to = dcp_to_sub_time (is->out ());

		rs.fade_up = dcp_to_sub_time (is->fade_up_time ());
		rs.fade_down = dcp_to_sub_time (is->fade_down_time ());

		_subs.push_back (rs);
	}
}
