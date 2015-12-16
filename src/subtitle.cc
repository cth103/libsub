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

#include "subtitle.h"

using namespace sub;

Subtitle::Subtitle (RawSubtitle s)
	: from (s.from)
	, to (s.to)
	, fade_up (s.fade_up)
	, fade_down (s.fade_down)
{
	lines.push_back (Line (s));
}

bool
Subtitle::same_metadata (RawSubtitle s) const
{
	return from == s.from && to == s.to && fade_up == s.fade_up && fade_down == s.fade_down;
}

Line::Line (RawSubtitle s)
	: vertical_position (s.vertical_position)
{
	blocks.push_back (Block (s));
}

bool
Line::same_metadata (RawSubtitle s) const
{
	return vertical_position == s.vertical_position;
}

Block::Block (RawSubtitle s)
	: text (s.text)
	, font (s.font)
	, font_size (s.font_size)
	, effect (s.effect)
	, effect_colour (s.effect_colour)
	, colour (s.colour)
	, bold (s.bold)
	, italic (s.italic)
	, underline (s.underline)
{

}
