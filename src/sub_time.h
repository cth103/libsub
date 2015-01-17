/*
    Copyright (C) 2014-2015 Carl Hetherington <cth@carlh.net>

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

#ifndef LIBSUB_SUB_TIME_H
#define LIBSUB_SUB_TIME_H

#include <boost/optional.hpp>

namespace sub {
	
class Rational
{
public:
	Rational (int numerator_, int denominator_)
		: numerator (numerator_)
		, denominator (denominator_)
	{}
	
	int numerator;
	int denominator;

	double fraction () const {
		return double (numerator) / denominator;
	}
};

class Time
{
public:
	Time ()
		: _seconds (0)
		, _frames (0)
	{}

	int hours () const;
	int minutes () const;
	int seconds () const;

	int frames_at (Rational rate) const;
	int milliseconds () const;

	double all_as_seconds () const;

	static Time from_hmsf (int h, int m, int s, int f, boost::optional<Rational> rate = boost::optional<Rational> ());
	static Time from_hms (int h, int m, int s, int ms);
	
private:
	friend bool operator< (Time const & a, Time const & b);
	friend bool operator> (Time const & a, Time const & b);
	friend bool operator== (Time const & a, Time const & b);
	friend std::ostream& operator<< (std::ostream& s, Time const & t);

	Time (int seconds, int frames, boost::optional<Rational> rate)
		: _seconds (seconds)
		, _frames (frames)
		, _rate (rate)
	{}
	
	int _seconds;
	int _frames;
	boost::optional<Rational> _rate;
};

bool operator< (Time const & a, Time const & b);
bool operator> (Time const & a, Time const & b);
bool operator== (Time const & a, Time const & b);
bool operator!= (Time const & a, Time const & b);
std::ostream& operator<< (std::ostream& s, Time const & t);
	
}

#endif
