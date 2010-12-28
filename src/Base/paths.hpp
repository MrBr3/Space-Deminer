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
    extern Glib::ustring app_name;
    extern Glib::ustring local_folder;
  }
}
#endif

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
  str_replace_all_with(filename, "$(ui-menu-icon-path)", "$(exe-share)/ui/icons/48");
  str_replace_all_with(filename, "$(ui-icon-path)", "$(exe-share)/ui/icons");
  str_replace_all_with(filename, "$(exe-share)", "$(exe-prefix)/share/$(app-name)");
  str_replace_all_with(filename, "$(test-references)", "$(local-folder)/test-references");
  str_replace_all_with(filename, "$(tmp)", "$(local-folder)/temp");
  str_replace_all_with(filename, "$(local-folder)", Private::Base::local_folder);
  str_replace_all_with(filename, "$(appdata)", Private::Base::path_appdata);
  str_replace_all_with(filename, "$(home)", Private::Base::path_home);
  str_replace_all_with(filename, "$(exe-prefix)", Private::Base::path_exe_prefix);
  str_replace_all_with(filename, "$(exe-filename)", Private::Base::path_exe_filename);
  str_replace_all_with(filename, "$(app-name)", Private::Base::app_name);

  return filename;
}

void init_paths(const Glib::ustring& app_name);

#endif