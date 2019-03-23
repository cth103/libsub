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

#include "locale_convert.h"

using std::string;

template<>
string
sub::locale_convert (int x, int, bool)
{
	char buffer[64];
	snprintf (buffer, sizeof(buffer), "%d", x);
	return buffer;
}

template<>
string
sub::locale_convert (long int x, int, bool)
{
	char buffer[64];
#ifdef LIBSUB_WINDOWS
	__mingw_snprintf (buffer, sizeof(buffer), "%ld", x);
#else
	snprintf (buffer, sizeof(buffer), "%ld", x);
#endif
	return buffer;
}

template<>
string
sub::locale_convert (unsigned long int x, int, bool)
{
	char buffer[64];
	snprintf (buffer, sizeof(buffer), "%lu", x);
	return buffer;
}

template<>
string
sub::locale_convert (unsigned long long x, int, bool)
{
	char buffer[64];
#ifdef LIBSUB_WINDOWS
	__mingw_snprintf (buffer, sizeof(buffer), "%lld", x);
#else
	snprintf (buffer, sizeof(buffer), "%lld", x);
#endif
	return buffer;
}

template<>
string
sub::locale_convert (string x, int, bool)
{
	return x;
}

template<>
int
sub::locale_convert (string x, int, bool)
{
	int y = 0;
	sscanf (x.c_str(), "%d", &y);
	return y;
}

template<>
float
sub::locale_convert (string x, int, bool)
{
	float y = 0;
	sscanf (x.c_str(), "%f", &y);
	return y;
}
