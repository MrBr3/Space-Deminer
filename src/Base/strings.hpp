/* Space Deminer is a small 2D Arcade Game. Your task ist to eliminate
 * intelligent Mines created in the past galactic war.
 *
 * Copyright (C) 2010 the Space Deminer Development Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _SPACE_DEMINER_BASE_STRINGS_H_
#define _SPACE_DEMINER_BASE_STRINGS_H_

#include <glibmm.h>

#ifdef G_OS_UNIX
#define SET_FILL_0 std::setfill(L'0')
#else
#define SET_FILL_0 std::setfill('0')
#endif

Glib::ustring time_val_to_str_hms(Glib::TimeVal tv, gchar sep=0);

/** \brief Checks, whether the first <tt>what.length()</tt> characters of the string specified by \c begin and \c end are equal (case sensitive) to \c what
 * */
bool compare(Glib::ustring::const_iterator& begin, Glib::ustring::const_iterator end, const Glib::ustring& what);

gfloat str_to_real(const Glib::ustring& num, gfloat defvalue=0.0f, bool* successful=nullptr);
gdouble str_to_eral(const Glib::ustring& num, gdouble defvalue=0.0, bool* successful=nullptr);
gint str_to_integer(const Glib::ustring& num, gint defvalue=0, gint base=10, bool* successful=nullptr);

/** \brief Replaces the only last appereance of the character <tt>replace</tt> with the string <tt>width</tt>.
*
* \param str - the string where to replace (as this is a reference, this string string itself will store the result afterwards)
* \param replace - the character to replace (if zero the resulting string with be equal to <tt>str+with</tt>)
* \param with - the string which will replace the character given by <tt>replace</tt>
 * */
void str_replace_last_with(Glib::ustring& str, Glib::ustring::value_type replace, const Glib::ustring& with);

/** \brief Replaces the only last appereance of the character <tt>replace</tt> with the string <tt>width</tt>.
*
* \param str - the string where to replace
* \param replace - the character to replace (if zero the resulting string with be equal to <tt>str+with</tt>)
* \param with - the string which will replace the character given by <tt>replace</tt>
*
* \return A string which each appereance of <tt>replace</tt> has been replaced.
* */
inline Glib::ustring str_copy_replace_last_with(Glib::ustring str, Glib::ustring::value_type replace, const Glib::ustring& with)
{
  str_replace_last_with(str, replace, with);
  return str;
}

/** \brief Replaces every appereance of the string <tt>replace</tt> within the string <tt>str</tt> with the string <tt>with</tt>.
*
* Uses internal a repeated call of Glib::ustring::replace.
*
* \param str - the string where to replace (as this is a reference, this string string itself will store the result afterwards)
* \param replace - the string to replace (if empty, the resulting string with be equal to <tt>str+with</tt>)
* \param with - the string which will replace the character given by <tt>replace</tt>
*/
void str_replace_all_with(Glib::ustring& str, const Glib::ustring& replace, const Glib::ustring& with);

/** \brief Replaces every appereance of the string <tt>replace</tt> within the string <tt>str</tt> with the string <tt>with</tt>.
*
* Uses internal a repeated call of Glib::ustring::replace.
*
* \param str - the string where to replace
* \param replace - the string to replace (if empty, the resulting string with be equal to <tt>str+with</tt>)
* \param with - the string which will replace the character given by <tt>replace</tt>
*
* \return A string which each appereance of <tt>replace</tt> has been replaced.
* */
inline Glib::ustring str_copy_replace_all_with(Glib::ustring str, const Glib::ustring& replace, const Glib::ustring& with)
{
  str_replace_all_with(str, replace, with);
  return str;
}

/** \brief Replaces every appereance of the character <tt>replace</tt> within the string <tt>str</tt> with the string <tt>with</tt>.
*
* Uses internal a repeated call of Glib::ustring::replace.
*
* \param str - the string where to replace (as this is a reference, this string string itself will store the result afterwards)
* \param replace - the character to replace (if zero the resulting string with be equal to <tt>str+with</tt>)
* \param with - the string which will replace the character given by <tt>replace</tt>
*/
void str_replace_all_with(Glib::ustring& str, const Glib::ustring::value_type& replace, const Glib::ustring& with);

/** \brief Replaces every appereance of the character <tt>replace</tt> within the string <tt>str</tt> with the string <tt>with</tt>.
*
* Uses internal a repeated call of Glib::ustring::replace.
*
* \param str - the string where to replace (as this isn't a reference, this string string itself won't be touched)
* \param replace - the character to replace (if zero the resulting string with be equal to <tt>str+with</tt>)
* \param with - the string which will replace the character given by <tt>replace</tt>
*
* \return the resulting string
*/
inline Glib::ustring str_copy_replace_all_with(Glib::ustring str, const Glib::ustring::value_type& replace, const Glib::ustring& with)
{
  str_replace_all_with(str, replace, with);
  return str;
}

/** \brief Replaces every appereance of the character <tt>replace</tt> within the string <tt>str</tt> with the string <tt>with</tt>.
*
* Uses internal a repeated call of Glib::ustring::replace.
*
* \param str - the string where to replace (as this is a reference, this string string itself will store the result afterwards)
* \param replace - the character to replace (if zero the resulting string with be equal to <tt>str+with</tt>)
* \param with - the character which will replace the character given by <tt>replace</tt>
*/
void str_replace_all_with(Glib::ustring& str, const Glib::ustring::value_type& replace, const Glib::ustring::value_type& with);

/** \brief Replaces every appereance of the character <tt>replace</tt> within the string <tt>str</tt> with the string <tt>with</tt>.
*
* Uses internal a repeated call of Glib::ustring::replace.
*
* \param str - the string where to replace (as this isn't a reference, this string string itself won't be touched)
* \param replace - the character to replace (if zero the resulting string with be equal to <tt>str+with</tt>)
* \param with - the character which will replace the character given by <tt>replace</tt>
*
* \return the resulting string
*/
inline Glib::ustring str_copy_replace_all_with(Glib::ustring str, const Glib::ustring::value_type& replace, const Glib::ustring::value_type& with)
{
  str_replace_all_with(str, replace, with);
  return str;
}

/** \brief Splits a string into a stringlist using a seperator.
 *
 * Example:
 * \code
 * std::list<Glib::ustring> l;
 * l.pack_start(":-)");
 * str_to_stringlist(l, "a,b,,c,d,", ',');
 * // Now the content of l is ":-)" "a" "b" "" "c" "d"
 * \endcode
 *
 * \param list the list, the result weill be stored in
 * \param str the string to split
 * \param seperator the character which will be used to split the string
 * */
void str_to_stringlist(std::list<Glib::ustring>& list, const Glib::ustring& str, Glib::ustring::value_type seperator);

/** \brief Splits a string into two string using a seperator.
 *
 * Example:
 * \code
 * Glib::ustring a, b;
 * split_string(a, b, "hihi=blabla=hoho", '=');
 * // Now a is "hihi"
 * // Now b is "blabla=hoho"
 * \endcode
 *
 * \param a the first part of the returned string (will be equal to \c str if \c seperator is not found)
 * \param b the second part of the returned string (will empty if \c seperator is the last char or not found)
 * \param str the string to split
 * \param seperator the character which will be used to split the string
 * */
void split_string(Glib::ustring& a, Glib::ustring& b, const Glib::ustring& str, Glib::ustring::value_type seperator);

#endif
