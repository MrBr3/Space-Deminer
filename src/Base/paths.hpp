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

#ifndef _SPACEDEMINER_BASE_PATHS_H_
#define _SPACEDEMINER_BASE_PATHS_H_

#include <glibmm.h>
#include "./dependencies.hpp"

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace Private
{
  namespace Base
  {
    extern Glib::ustring path_appdata;
    extern Glib::ustring path_home;
    extern Glib::ustring path_exe_prefix;
    extern Glib::ustring path_exe_filename;
  }
}
#endif

inline void replace(Glib::ustring& str, const Glib::ustring& what, const Glib::ustring& with)
{
  if(str.substr(0, what.length())==what)
    str = with  + str.substr(what.length(), str.length()-what.length());
}

/** \brief Makes sure the Path won't end with a slash.
 *
 * It simply clips the last character as long as the string's last character is slash ('/' oder '\')
 *
 * Examples:
 * \li <tt>"/abc"</tt> -> <tt>"/abc"</tt>
 * \li <tt>"/abc/"</tt> -> <tt>"/abc"</tt>
 * \li <tt>"/abc//\\//\\/\\"</tt> -> <tt>"/abc"</tt>
 * \li <tt>"//\\//\\/\\"</tt> -> <tt>"/"</tt>
 * \li <tt>"\\/\//\\/\\"</tt> -> <tt>"\\"</tt>
 * \li <tt>"/"</tt> -> <tt>"/"</tt>
 * \li <tt>""</tt> -> <tt>""</tt>
 * */
inline Glib::ustring clip_last_slash(const Glib::ustring& str)
{
  Glib::ustring result=str;
  while(result.length()>1 && (result[result.length()-1]=='/' || result[result.length()-1]=='\\'))
    result  = result.substr(0, result.length()-1);
  return result;
}

inline Glib::ustring apply_filename_macros(Glib::ustring filename)
{
  replace(filename, "$(ui-menu-icon-path)", "$(exe-share)/ui/icons/48");
  replace(filename, "$(ui-icon-path)", "$(exe-share)/ui/icons");
  replace(filename, "$(exe-share)", "$(exe-prefix)/share/space-deminer");
  replace(filename, "$(test-references)", "$(local-folder)/test-references");
  replace(filename, "$(tmp)", "$(local-folder)/temp");
  replace(filename, "$(local-folder)", "$(appdata)/.SpaceDeminer");
  replace(filename, "$(appdata)", Private::Base::path_appdata);
  replace(filename, "$(home)", Private::Base::path_home);
  replace(filename, "$(exe-prefix)", Private::Base::path_exe_prefix);
  replace(filename, "$(exe-filename)", Private::Base::path_exe_filename);

  return filename;
}

void init_paths();

#endif