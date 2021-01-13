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

#ifndef LIBSUB_EXCEPTIONS_H
#define LIBSUB_EXCEPTIONS_H

#include <list>
#include <stdexcept>
#include <string>
#include <vector>

namespace sub {

/** @class XMLError
 *  @brief An error raised when reading an XML file.
 */
class XMLError : public std::runtime_error
{
public:
	XMLError (std::string const & message)
		: std::runtime_error (message)
	{}
};

/** @class STLError
 *  @brief An error raised when reading a binary STL file.
 */
class STLError : public std::runtime_error
{
public:
	STLError (std::string const & message)
		: std::runtime_error (message)
	{}
};

/** @class SubripError
 *  @brief An error raised when reading a Subrip file.
 */
class SubripError : public std::runtime_error
{
public:
	SubripError (std::string saw, std::string expecting, std::list<std::string> context);
	~SubripError () throw () {}

	std::list<std::string> context () const {
		return _context;
	}

private:
	std::list<std::string> _context;
};

class SSAError : public std::runtime_error
{
public:
	SSAError (std::string message)
		: std::runtime_error(message)
	{}
};

class MXFError : public std::runtime_error
{
public:
	MXFError (std::string const & message)
		: std::runtime_error (message)
	{}
};

class UnknownFrameRateError : public std::runtime_error
{
public:
	UnknownFrameRateError ()
		: std::runtime_error ("unknown frame rate")
	{}
};

class DCPError : public std::runtime_error
{
public:
	DCPError (std::string const & message)
		: std::runtime_error (message)
	{}
};

class ProgrammingError : public std::runtime_error
{
public:
	ProgrammingError (std::string file, int line);
};

}

#endif
