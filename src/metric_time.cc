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

#include "metric_time.h"
#include "compose.hpp"
#include <iostream>

using std::ostream;
using std::string;
using namespace sub;

MetricTime::MetricTime (int h, int m, int s, int ms)
	: _milliseconds ((h * 3600 + m * 60 + s) * 1000 + ms)
{

}

bool
sub::operator== (MetricTime const & a, MetricTime const & b)
{
	return a._milliseconds == b._milliseconds;
}

bool
sub::operator> (MetricTime const & a, MetricTime const & b)
{
	return a._milliseconds > b._milliseconds;
}

bool
sub::operator< (MetricTime const & a, MetricTime const & b)
{
	return a._milliseconds < b._milliseconds;
}

ostream&
sub::operator<< (ostream& st, MetricTime const & t)
{
	int64_t ms = t._milliseconds;
	int const h = ms / (3600 * 1000);
	ms -= h * 3600 * 1000;
	int const m = ms / (60 * 1000);
	ms -= m * 60 * 1000;
	int const s = ms / 1000;
	ms -= s * 1000;

	st << h << ":" << m << ":" << s << ":" << ms;
	return st;
}
