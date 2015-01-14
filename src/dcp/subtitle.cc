/*
    Copyright (C) 2012-2015 Carl Hetherington <cth@carlh.net>

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

#include "../exceptions.h"
#include "../raw_convert.h"
#include "subtitle.h"
#include <libcxml/cxml.h>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

using std::string;
using std::vector;
using std::list;
using boost::optional;
using boost::shared_ptr;
using boost::lexical_cast;
using boost::is_any_of;
using namespace sub;

dcp::Subtitle::Subtitle (boost::shared_ptr<const cxml::Node> node, optional<int> tcr)
{
	if (tcr) {
		in = smpte_time (node, "TimeIn", tcr.get ()).get ();
		out = smpte_time (node, "TimeOut", tcr.get ()).get ();
	} else {
		in = interop_time (node, "TimeIn").get ();
		out = interop_time (node, "TimeOut").get ();
	}

	if (tcr) {
		fade_up_time = smpte_time (node, "FadeUpTime", tcr.get ()).get_value_or (Time::from_hmsf (0, 0, 0, 2, Rational (tcr.get(), 1)));
		fade_down_time = smpte_time (node, "FadeDownTime", tcr.get ()).get_value_or (Time::from_hmsf (0, 0, 0, 2, Rational (tcr.get (), 1)));
	} else {
		fade_up_time = interop_time (node, "FadeUpTime").get_value_or (Time::from_hms (0, 0, 0, 80));
		if (fade_up_time > Time::from_hms (0, 0, 8, 0)) {
			fade_up_time = Time::from_hms (0, 0, 8, 0);
		}
		fade_down_time = interop_time (node, "FadeDownTime").get_value_or (Time::from_hms (0, 0, 0, 80));
		if (fade_down_time > Time::from_hms (0, 0, 8, 0)) {
			fade_down_time = Time::from_hms (0, 0, 8, 0);
		}
	}
}

optional<Time>
dcp::Subtitle::smpte_time (shared_ptr<const cxml::Node> node, string name, int tcr)
{
	optional<string> u = node->optional_string_attribute (name);
	if (!u) {
		return optional<Time> ();
	}
	
	vector<string> b;
	split (b, u.get (), is_any_of (":"));
	if (b.size() != 4) {
		boost::throw_exception (DCPError ("unrecognised time specification " + u.get ()));
	}
	
	return Time::from_hmsf (
		raw_convert<int> (b[0]),
		raw_convert<int> (b[1]),
		raw_convert<int> (b[2]),
		raw_convert<int> (b[3]),
		Rational (tcr, 1)
		);
}

optional<Time>
dcp::Subtitle::interop_time (shared_ptr<const cxml::Node> node, string name)
{
	optional<string> u = node->optional_string_attribute (name);
	if (!u) {
		return optional<Time> ();
	}

	if (u.get().find (":") != string::npos) {
		/* HH:MM:SS:TTT or HH:MM:SS.sss */
		vector<string> b;
		split (b, u.get(), is_any_of (":."));
		if (b.size() != 4) {
			boost::throw_exception (DCPError ("unrecognised time specification " + u.get ()));
		}

		if (u.get().find (".") != string::npos) {
			return Time::from_hms (
				raw_convert<int> (b[0]),
				raw_convert<int> (b[1]),
				raw_convert<int> (b[2]),
				rint (raw_convert<double> ("." + b[3]) * 1000)
				);
		} else {
			return Time::from_hms (
				raw_convert<int> (b[0]),
				raw_convert<int> (b[1]),
				raw_convert<int> (b[2]),
				raw_convert<int> (b[3]) * 4
				);
		}
	} else {
		return Time::from_hms (0, 0, 0, raw_convert<int> (u.get ()) * 4);
	}
}
  
