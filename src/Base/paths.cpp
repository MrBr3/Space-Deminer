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

#include "./paths.hpp"
#include "./../extern/binreloc/binreloc.h"
#include <exception>

namespace Private
{
  namespace Base
  {
    Glib::ustring path_appdata  = "$(home)";
    Glib::ustring path_home;
    Glib::ustring path_exe_prefix;
    Glib::ustring path_exe_filename;
    Glib::ustring path_exe_path;
  }
}

void init_paths()
{
  using namespace Private::Base;

  gchar* s;

#ifdef G_OS_WIN32
  const guint32 len = 2048;
  guint32 result, last_slash=0;
  s = new gchar[len];

  result = GetModuleFileNameA(0, s, len);

  if(result==len || result==0)
    return false;

  for(guint32 i=0; i<result; i++)
  {
    if(s[i]=='\\')
      s[i]  = '/';

    if(s[i]=='/')
    {
      last_slash  = i;
    }
  }
  r_assert(last_slash<result);
  if(last_slash>result)
    return false;

  path_exe_filename  = Glib::filename_to_utf8(s);

  s[last_slash] = 0;
  for(gssize i=last_slash; i>=0; i--)
  {
    if(s[i]=='/')
    {
      s[i]  = 0;
      break;
    }
  }
  path_exe_prefix  = Glib::filename_to_utf8(s);

  path_home = get_specialdir(CSIDL_PERSONAL);
  path_appdata = get_specialdir(CSIDL_APPDATA);

  delete[] s;
#else
  BrInitError e;
  if(!br_init(&e))
  {
    const gchar* s;
    switch(e)
    {
    case BR_INIT_ERROR_NOMEM:
      s = "BR_INIT_ERROR_NOMEM";
      break;
    case BR_INIT_ERROR_OPEN_MAPS:
      s = "BR_INIT_ERROR_OPEN_MAPS";
      break;
    case BR_INIT_ERROR_READ_MAPS:
      s = "BR_INIT_ERROR_READ_MAPS";
      break;
    case BR_INIT_ERROR_INVALID_MAPS:
      s = "BR_INIT_ERROR_INVALID_MAPS";
      break;
    case BR_INIT_ERROR_DISABLED:
      s = "BR_INIT_ERROR_DISABLED";
      break;
    default:
      g_assert_not_reached();
    }
    throw std::runtime_error(Glib::ustring::compose("Couldn't init binreloc: %1\n", s).data());
  }

  s = br_find_exe("");
  path_exe_filename = Glib::filename_to_utf8(s);
  free(s);

  s = br_find_prefix("");
  path_exe_prefix = Glib::filename_to_utf8(s);
  free(s);

  s = getenv("HOME");
  g_assert(s!=0);
  g_assert(s[0]!=0);
  if(!s)throw std::runtime_error("couldn't init paths. error-code 0x01");
  if(!s[0])throw std::runtime_error("couldn't init paths. error-code 0x02");
  path_home = Glib::filename_to_utf8(s);
  path_appdata  = path_home;
#endif
  g_assert(path_exe_filename.length());
  g_assert(path_exe_prefix.length());
  g_assert(path_home.length());

  if(!path_exe_filename.length())
    throw std::runtime_error("couldn't init paths. error-code 0x03");
  if(!path_exe_prefix.length())
    throw std::runtime_error("couldn't init paths. error-code 0x04");
  if(!path_home.length())
    throw std::runtime_error("couldn't init paths. error-code 0x05");

  path_exe_prefix  = clip_last_slash(path_exe_prefix);
  path_home  = clip_last_slash(path_home);

  path_exe_path  = path_exe_filename;
  for(gsize i=path_exe_path.length()-1; i>=0; --i)
  {
    if(path_exe_path[i]=='/')
    {
      path_exe_path  = path_exe_path.substr(0, i);
      break;
    }
  }
}