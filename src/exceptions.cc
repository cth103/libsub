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

#include "compose.hpp"
#include "exceptions.h"

using std::list;
using std::string;
using namespace sub;

ProgrammingError::ProgrammingError (string file, int line)
	: runtime_error (String::compose ("Programming error at %1:%2", file, line))
{

}


SubripError::SubripError (string saw, string expecting, list<string> context)
	: runtime_error ("Error in SubRip file: saw " + (saw.empty() ? "an empty string" : saw) + " when expecting " + expecting)
	, _context (context)
{

}


WebVTTError::WebVTTError(string saw, string expecting, list<string> context)
	: runtime_error("Error in WebVTT file: saw " + (saw.empty() ? "an empty string" : saw) + " when expecting " + expecting)
	, _context(context)
{

}
