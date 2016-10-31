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

/** @file  src/stl_binary_writer.cc
 *  @brief Writer for STL binary files.
 */

#include "stl_binary_writer.h"
#include "subtitle.h"
#include "iso6937.h"
#include "stl_util.h"
#include "compose.hpp"
#include "sub_assert.h"
#include <boost/locale.hpp>
#include <list>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <set>

using std::list;
using std::set;
using std::ofstream;
using std::string;
using std::setw;
using std::setfill;
using std::max;
using std::cout;
using boost::locale::conv::utf_to_utf;
using namespace sub;

/** Arbitrary number which to divide the screen into rows; e.g.
 *  64 here would mean that there are 64 addressable vertical positions
 *  on the screen, each 1/64th of the screen height tall.
 *
 *  The magic 23 makes our output agree more closely with
 *  AnnotationEdit, which makes life easier when testing.
 */
static int const ROWS = 23;

static void
put_string (char* p, string s)
{
	memcpy (p, s.c_str (), s.length ());
}

static void
put_string (char* p, unsigned int n, string s)
{
	SUB_ASSERT (s.length() <= n);

	memcpy (p, s.c_str (), s.length ());
	memset (p + s.length(), ' ', n - s.length ());
}

static void
put_int_as_string (char* p, int v, unsigned int n)
{
	locked_stringstream s;
	/* Be careful to ensure we get no thousands separators */
	s.imbue (std::locale::classic ());
	s << setw (n) << setfill ('0');
	s << v;
	SUB_ASSERT (s.str().length() == n);
	put_string (p, s.str ());
}

static void
put_int_as_int (char* p, int v, unsigned int n)
{
	for (unsigned int i = 0; i < n; ++i) {
		*p++ = (v & ((1 << ((i + 1) * 8)) - 1)) >> (i * 8);
	}
}

/** @param language ISO 3-character country code for the language of the subtitles */
void
sub::write_stl_binary (
	list<Subtitle> subtitles,
	float frames_per_second,
	Language language,
	string original_programme_title,
	string original_episode_title,
	string translated_programme_title,
	string translated_episode_title,
	string translator_name,
	string translator_contact_details,
	string creation_date,
	string revision_date,
	int revision_number,
	string country_of_origin,
	string publisher,
	string editor_name,
	string editor_contact_details,
	boost::filesystem::path file_name
	)
{
	SUB_ASSERT (original_programme_title.size() <= 32);
	SUB_ASSERT (original_episode_title.size() <= 32);
	SUB_ASSERT (translated_programme_title.size() <= 32);
	SUB_ASSERT (translated_episode_title.size() <= 32);
	SUB_ASSERT (translator_name.size() <= 32);
	SUB_ASSERT (translator_contact_details.size() <= 32);
	SUB_ASSERT (creation_date.size() == 6);
	SUB_ASSERT (revision_date.size() == 6);
	SUB_ASSERT (revision_number <= 99);
	SUB_ASSERT (country_of_origin.size() == 3);
	SUB_ASSERT (publisher.size() <= 32);
	SUB_ASSERT (editor_name.size() <= 32);
	SUB_ASSERT (editor_contact_details.size() <= 32);

	char* buffer = new char[1024];
	memset (buffer, 0, 1024);
	ofstream output (file_name.string().c_str ());
	STLBinaryTables tables;

	/* Find the longest subtitle in characters and the number of lines */

	int longest = 0;
	int lines = 0;

	for (list<Subtitle>::const_iterator i = subtitles.begin(); i != subtitles.end(); ++i) {
		for (list<Line>::const_iterator j = i->lines.begin(); j != i->lines.end(); ++j) {
			int t = 0;
			for (list<Block>::const_iterator k = j->blocks.begin(); k != j->blocks.end(); ++k) {
				t += k->text.size ();
			}
			longest = max (longest, t);
			++lines;
		}
	}

	/* Code page: 850 */
	put_string (buffer + 0, "850");
	/* Disk format code */
	put_string (buffer + 3, stl_frame_rate_to_dfc (frames_per_second));
	/* Display standard code: open subtitling */
	put_string (buffer + 11, "0");
	/* Character code table: Latin (ISO 6937) */
	put_string (buffer + 12, "00");
	put_string (buffer + 14, tables.language_enum_to_file (language));
	put_string (buffer + 16, 32, original_programme_title);
	put_string (buffer + 48, 32, original_episode_title);
	put_string (buffer + 80, 32, translated_programme_title);
	put_string (buffer + 112, 32, translated_episode_title);
	put_string (buffer + 144, 32, translator_name);
	put_string (buffer + 176, 32, translator_contact_details);
	/* Subtitle list reference code */
	put_string (buffer + 208, "0000000000000000");
	put_string (buffer + 224, creation_date);
	put_string (buffer + 230, revision_date);
	put_int_as_string (buffer + 236, revision_number, 2);
	/* TTI blocks */
	put_int_as_string (buffer + 238, lines, 5);
	/* Total number of subtitles */
	put_int_as_string (buffer + 243, lines, 5);
	/* Total number of subtitle groups */
	put_string (buffer + 248, "001");
	/* Maximum number of displayable characters in any text row */
	put_int_as_string (buffer + 251, longest, 2);
	/* Maximum number of displayable rows */
	put_int_as_string (buffer + 253, ROWS, 2);
	/* Time code status */
	put_string (buffer + 255, "1");
	/* Start-of-programme time code */
	put_string (buffer + 256, "00000000");
	/* First-in-cue time code */
	put_string (buffer + 264, "00000000");
	/* Total number of disks */
	put_string (buffer + 272, "1");
	/* Disk sequence number */
	put_string (buffer + 273, "1");
	put_string (buffer + 274, 3, country_of_origin);
	put_string (buffer + 277, 32, publisher);
	put_string (buffer + 309, 32, editor_name);
	put_string (buffer + 341, 32, editor_contact_details);

	output.write (buffer, 1024);

	for (list<Subtitle>::const_iterator i = subtitles.begin(); i != subtitles.end(); ++i) {
		int N = 0;
		for (list<Line>::const_iterator j = i->lines.begin(); j != i->lines.end(); ++j) {

			memset (buffer, 0, 1024);

			/* Subtitle group number */
			put_int_as_int (buffer + 0, 1, 1);
			/* Subtitle number */
			put_int_as_int (buffer + 1, N, 2);
			/* Extension block number.  Use 0xff here to indicate that it is the last TTI
			   block in this subtitle "set", as we only ever use one.
			*/
			put_int_as_int (buffer + 3, 255, 1);
			/* Cumulative status */
			put_int_as_int (buffer + 4, tables.cumulative_status_enum_to_file (CUMULATIVE_STATUS_NOT_CUMULATIVE), 1);
			/* Time code in */
			put_int_as_int (buffer + 5, i->from.hours (), 1);
			put_int_as_int (buffer + 6, i->from.minutes (), 1);
			put_int_as_int (buffer + 7, i->from.seconds (), 1);
			put_int_as_int (buffer + 8, i->from.frames_at (sub::Rational (frames_per_second * 1000, 1000)), 1);
			/* Time code out */
			put_int_as_int (buffer + 9, i->to.hours (), 1);
			put_int_as_int (buffer + 10, i->to.minutes (), 1);
			put_int_as_int (buffer + 11, i->to.seconds (), 1);
			put_int_as_int (buffer + 12, i->to.frames_at (sub::Rational (frames_per_second * 1000, 1000)), 1);
			/* Vertical position */
			int vp = 0;
			if (j->vertical_position.proportional) {
				switch (j->vertical_position.reference.get_value_or (TOP_OF_SCREEN)) {
				case TOP_OF_SCREEN:
					vp = rint (j->vertical_position.proportional.get() * ROWS);
					break;
				case CENTRE_OF_SCREEN:
					vp = rint (j->vertical_position.proportional.get() * ROWS + (ROWS / 2.0));
					break;
				case BOTTOM_OF_SCREEN:
					vp = rint (ROWS - (j->vertical_position.proportional.get() * ROWS));
					break;
				default:
					break;
				}
			} else if (j->vertical_position.line) {
				float const prop = float (j->vertical_position.line.get()) / j->vertical_position.lines.get ();
				switch (j->vertical_position.reference.get_value_or (TOP_OF_SCREEN)) {
				case TOP_OF_SCREEN:
					vp = prop * ROWS;
					break;
				case CENTRE_OF_SCREEN:
					vp = (prop + 0.5) * ROWS;
					break;
				case BOTTOM_OF_SCREEN:
					vp = (1 - prop) * ROWS;
					break;
				default:
					break;
				}
			}
			put_int_as_int (buffer + 13, vp, 1);
			/* Justification code */
			/* XXX */
			put_int_as_int (buffer + 14, tables.justification_enum_to_file (JUSTIFICATION_NONE), 1);
			/* Comment flag */
			put_int_as_int (buffer + 15, tables.comment_enum_to_file (COMMENT_NO), 1);

			/* Text */
			string text;
			bool italic = false;
			bool underline = false;

			for (list<Block>::const_iterator k = j->blocks.begin(); k != j->blocks.end(); ++k) {
				if (k->underline && !underline) {
					text += "\x82";
					underline = true;
				} else if (underline && !k->underline) {
					text += "\x83";
					underline = false;
				}
				if (k->italic && !italic) {
					text += "\x80";
					italic = true;
				} else if (italic && !k->italic) {
					text += "\x81";
					italic = false;
				}

				text += utf16_to_iso6937 (utf_to_utf<wchar_t> (k->text));
			}

			/* Turn italic/underline off before the end of this subtitle */

			if (underline) {
				text += "\x83";
			}

			if (italic) {
				text += "\x81";
			}

			text += "\x8A";

			if (text.length() > 111) {
				text = text.substr (111);
			}

			while (text.length() < 112) {
				text += "\x8F";
			}

			put_string (buffer + 16, text);
			output.write (buffer, 128);

			++N;
		}
	}

	delete[] buffer;
}
