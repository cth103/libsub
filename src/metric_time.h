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

#ifndef LIBSUB_METRIC_TIME_H
#define LIBSUB_METRIC_TIME_H

#include <stdint.h>
#include <iostream>

namespace sub {

/** @class MetricTime
 *  @brief A time held in milliseconds.
 */
class MetricTime
{
public:
	MetricTime ()
		: _ms (0)
	{}

	MetricTime (int h, int m, int s, int ms);

	int hours () const;
	int minutes () const;
	int seconds () const;
	int milliseconds () const;
			  
private:
	void split (int& h, int& m, int& s, int& ms) const;
	
	friend bool operator== (MetricTime const & a, MetricTime const & b);
	friend bool operator> (MetricTime const & a, MetricTime const & b);
	friend bool operator< (MetricTime const & a, MetricTime const & b);
	friend std::ostream& operator<< (std::ostream&, MetricTime const & t);
	
	int64_t _ms;
};

bool operator== (MetricTime const & a, MetricTime const & b);
bool operator> (MetricTime const & a, MetricTime const & b);	
bool operator< (MetricTime const & a, MetricTime const & b);	
std::ostream& operator<< (std::ostream&, MetricTime const & t);
	
}

#endif
