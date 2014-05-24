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
#include <cmath>

using std::ostream;
using std::string;
using std::cout;
using namespace sub;

MetricTime::MetricTime (int h, int m, int s, int ms)
        /* cast up to int64_t to force a 64-bit calculation */
	: _ms ((int64_t (h) * 3600 + m * 60 + s) * 1000 + ms)
{

}

void
MetricTime::split (int& h, int &m, int& s, int& ms) const
{
	int64_t w = _ms;
	h = floor (w / (3600 * 1000));
	/* this multiply could overflow 32 bits so cast to make sure it is done as 64-bit */
	w -= int64_t (h) * (3600 * 1000);
	m = floor (w / (60 * 1000));
	w -= m * (60 * 1000);
	s = floor (w / 1000);
	w -= s * 1000;
	ms = w;
}

int
MetricTime::hours () const
{
	int h, m, s, ms;
	split (h, m, s, ms);
	return h;
}

int
MetricTime::minutes () const
{
	int h, m, s, ms;
	split (h, m, s, ms);
	return m;
}

int
MetricTime::seconds () const
{
	int h, m, s, ms;
	split (h, m, s, ms);
	return s;
}

int
MetricTime::milliseconds () const
{
	int h, m, s, ms;
	split (h, m, s, ms);
	return ms;
}

bool
sub::operator== (MetricTime const & a, MetricTime const & b)
{
	return a._ms == b._ms;
}

bool
sub::operator> (MetricTime const & a, MetricTime const & b)
{
	return a._ms > b._ms;
}

bool
sub::operator< (MetricTime const & a, MetricTime const & b)
{
	return a._ms < b._ms;
}

ostream&
sub::operator<< (ostream& st, MetricTime const & t)
{
	int h, m, s, ms;
	t.split (h, m, s, ms);
	st << h << ":" << m << ":" << s << ":" << ms;
	return st;
}
