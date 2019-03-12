/*
    Copyright (C) 2014-2019 Carl Hetherington <cth@carlh.net>

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

#include "raw_convert.h"
#include "locale_convert.h"
#include <boost/algorithm/string.hpp>

using std::string;

static
string
make_local (string v)
{
	struct lconv* lc = localeconv ();
	boost::algorithm::replace_all (v, ".", lc->decimal_point);
	/* We hope it's ok not to add in thousands separators here */
	return v;
}

/** @param v Numeric value as an ASCII string */
static
string
make_raw (string v)
{
	struct lconv* lc = localeconv ();
	/* thousands_sep may be . so remove them before changing decimal points */
	boost::algorithm::replace_all (v, lc->thousands_sep, "");
	boost::algorithm::replace_all (v, lc->decimal_point, ".");
	return v;
}

template <>
int
sub::raw_convert (string v, int precision)
{
	return locale_convert<int> (make_local(v), precision);
}

template <>
float
sub::raw_convert (string v, int precision)
{
	return locale_convert<float> (make_local(v), precision);
}

template <>
string
sub::raw_convert (unsigned long v, int precision)
{
	return make_raw (locale_convert<string>(v, precision));
}
