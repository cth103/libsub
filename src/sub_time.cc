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

#include "sub_time.h"
#include "exceptions.h"
#include <cmath>
#include <iomanip>
#include <iostream>

using std::ostream;
using std::cout;
using std::setw;
using std::setfill;
using boost::optional;
using namespace sub;

bool
sub::operator< (sub::Time const & a, sub::Time const & b)
{
	if (a._seconds != b._seconds) {
		return a._seconds < b._seconds;
	}

	if (!a._rate && !b._rate) {
		/* Can compare if neither has a specified frame rate */
		return a._frames < b._frames;
	}

	if ((a._rate && !b._rate) || (!a._rate && b._rate)) {
		throw UnknownFrameRateError ();
	}

	return (a._frames * a._rate.get().numerator * b._rate.get().denominator) < (b._frames * b._rate.get().numerator * a._rate.get().denominator);
}

bool
sub::operator> (sub::Time const & a, sub::Time const & b)
{
	if (a._seconds != b._seconds) {
		return a._seconds > b._seconds;
	}

	if (!a._rate && !b._rate) {
		/* Can compare if neither has a specified frame rate */
		return a._frames > b._frames;
	}

	if ((a._rate && !b._rate) || (!a._rate && b._rate)) {
		throw UnknownFrameRateError ();
	}

	return (a._frames * a._rate.get().numerator * b._rate.get().denominator) > (b._frames * b._rate.get().numerator * a._rate.get().denominator);
}

bool
sub::operator== (sub::Time const & a, sub::Time const & b)
{
	if (!a._rate && !b._rate) {
		/* Can compare if neither has a specified frame rate */
		return (a._seconds == b._seconds && a._frames == b._frames);
	}

	if ((a._rate && !b._rate) || (!a._rate && b._rate)) {
		throw UnknownFrameRateError ();
	}

	if (a._seconds != b._seconds) {
		return false;
	}

	return (a._frames * a._rate.get().numerator * b._rate.get().denominator) == (b._frames * b._rate.get().numerator * a._rate.get().denominator);
}

bool
sub::operator!= (sub::Time const & a, sub::Time const & b)
{
	return !(a == b);
}

ostream&
sub::operator<< (ostream& s, Time const & t)
{
	s << setw (2) << setfill('0') << t.hours() << ":"
	  << setw (2) << setfill('0') << t.minutes() << ":"
	  << setw (2) << setfill('0') << t.seconds() << ":"
	  << t._frames;

	if (t._rate) {
		s << " @ " << t._rate.get().numerator << "/" << t._rate.get().denominator;
	}

	return s;
}

int
Time::hours () const
{
	return _seconds / 3600;
}

int
Time::minutes () const
{
	return (_seconds - hours() * 3600) / 60;
}

int
Time::seconds () const
{
	return (_seconds - hours() * 3600 - minutes() * 60);
}

int
Time::frames_at (Rational rate) const
{
	if (!_rate) {
		throw UnknownFrameRateError ();
	}

	return rint (double (_frames) * _rate.get().denominator * rate.numerator / (_rate.get().numerator * rate.denominator));
}

int
Time::milliseconds () const
{
	return frames_at (Rational (1000, 1));
}

Time
Time::from_hmsf (int h, int m, int s, int f, optional<Rational> rate)
{
	return Time (h * 3600 + m * 60 + s, f, rate);
}

Time
Time::from_hms (int h, int m, int s, int ms)
{
	return Time (h * 3600 + m * 60 + s, ms, Rational (1000, 1));
}

double
Time::all_as_seconds () const
{
	return _seconds + double(milliseconds ()) / 1000;
}
