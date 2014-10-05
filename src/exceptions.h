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

#include <stdexcept>
#include <string>

namespace sub {

class MessageError : public std::exception
{
public:
	MessageError (std::string const & message)
		: _message (message) {}
	~MessageError () throw () {}

	/** @return error message */
	char const * what () const throw () {
		return _message.c_str ();
	}

private:
	/** error message */
	std::string _message;
};

/** @class XMLError
 *  @brief An error raised when reading an XML file.
 */
class XMLError : public MessageError
{
public:
	XMLError (std::string const & message)
		: MessageError (message)
	{}
};

/** @class STLError
 *  @brief An error raised when reading a binary STL file.
 */
class STLError : public MessageError
{
public:
	STLError (std::string const & message)
		: MessageError (message)
	{}
};

/** @class SubripError
 *  @brief An error raised when reading a Subrip file.
 */
class SubripError : public MessageError
{
public:
	SubripError (std::string saw, std::string expecting)
		: MessageError ("Error in SubRip file: saw " + saw + " while expecting " + expecting)
	{}
};

}
