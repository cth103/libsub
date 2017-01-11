/*
    Copyright (C) 2014-2017 Carl Hetherington <cth@carlh.net>

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
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>

using std::list;
using std::cout;
using std::string;
using std::exception;
using boost::shared_ptr;
using boost::optional;
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


	BOOST_FOREACH (dcp::SubtitleString const & i, sc->subtitles ()) {
		RawSubtitle rs;
		rs.text = i.text ();
		rs.font = i.font ();
		rs.font_size = FontSize::from_proportional (i.size() / (72.0 * 11.0));

		switch (i.effect ()) {
		case dcp::BORDER:
			rs.effect = BORDER;
			break;
		case dcp::SHADOW:
			rs.effect = SHADOW;
			break;
		default:
			break;
		}

		rs.effect_colour = dcp_to_sub_colour (i.effect_colour());

		rs.colour = dcp_to_sub_colour (i.colour());
		rs.bold = i.bold ();
		rs.italic = i.italic ();
		rs.underline = i.underline ();

		switch (i.h_align()) {
		case dcp::HALIGN_LEFT:
			rs.horizontal_position = LEFT;
			break;
		case dcp::HALIGN_CENTER:
			rs.horizontal_position = CENTRE;
			break;
		case dcp::HALIGN_RIGHT:
			rs.horizontal_position = RIGHT;
			break;
		}

		rs.vertical_position.proportional = i.v_position();
		switch (i.v_align()) {
		case dcp::VALIGN_TOP:
			rs.vertical_position.reference = TOP_OF_SCREEN;
			break;
		case dcp::VALIGN_CENTER:
			rs.vertical_position.reference = CENTRE_OF_SCREEN;
			break;
		case dcp::VALIGN_BOTTOM:
			rs.vertical_position.reference = BOTTOM_OF_SCREEN;
			break;
		}

		rs.from = dcp_to_sub_time (i.in ());
		rs.to = dcp_to_sub_time (i.out ());

		rs.fade_up = dcp_to_sub_time (i.fade_up_time ());
		rs.fade_down = dcp_to_sub_time (i.fade_down_time ());

		_subs.push_back (rs);
	}
}
