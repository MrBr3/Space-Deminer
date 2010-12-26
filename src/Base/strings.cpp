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

#include "./base.hpp"

void str_replace_all_with(Glib::ustring& str, const Glib::ustring& replace, const Glib::ustring& with)
{
  if(replace.empty())
    throw std::invalid_argument("**str_replace_all_with** cannot search for an empty string");

  Glib::ustring::size_type old_i=0;
  bool first_loop=true;
  while(1)
  {
    Glib::ustring::size_type i  = str.find(replace, old_i+(first_loop?0:with.length()));
    old_i = i;
    first_loop  = false;

    if(i==Glib::ustring::npos)
      return;

    str = str.replace(i, replace.length(), with);
  }
}

void str_replace_all_with(Glib::ustring& str, const Glib::ustring::value_type& replace, const Glib::ustring& with)
{
  if(replace==0)
  {
    str+=with;
    return;
  }

  Glib::ustring::size_type old_i=0;
  bool first_loop=true;
  while(1)
  {
    Glib::ustring::size_type i  = str.find(replace, old_i+(first_loop?0:with.length()));
    old_i = i;
    first_loop  = false;

    if(i==Glib::ustring::npos)
      return;

    str = str.replace(i, 1, with);
  }
}

void str_replace_all_with(Glib::ustring& str, const Glib::ustring::value_type& replace, const Glib::ustring::value_type& with)
{
  if(replace==0)
  {
    str+=Glib::ustring(1, with);
    return;
  }
  if(with==0)
  {
    Glib::ustring::size_type f = str.find(replace);

    if(f!=Glib::ustring::npos)
      str = str.substr(0, f);
    return;
  }

  for(Glib::ustring::iterator s_iter=str.begin(); s_iter!=str.end(); ++s_iter)
  {
    if(*s_iter==replace)
    {
      Glib::ustring::iterator end = s_iter;
      ++end;
      str.replace(s_iter, end, 1, with);
    }
  }
}