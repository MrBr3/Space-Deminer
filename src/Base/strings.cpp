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

Glib::ustring time_val_to_str_hms(Glib::TimeVal tv, gchar sep)
{
  GTimeVal& time_val  = *((GTimeVal*)&tv);
  g_assert(sizeof(tv)==sizeof(time_val));

  int sec = time_val.tv_sec % 60;
  time_val.tv_sec = (time_val.tv_sec-sec)/60;
  int min = time_val.tv_sec % 60;
  time_val.tv_sec = (time_val.tv_sec-min)/60;
  int hour  = time_val.tv_sec ;

  if(sep!=0)
    return Glib::ustring::compose("%1%4%2%4%3", hour, Glib::ustring::format(SET_FILL_0, std::setw(2), min), Glib::ustring::format(SET_FILL_0, std::setw(2), sec), sep);
  else
    return Glib::ustring::compose(_("%1h %2min %3sec"), hour, Glib::ustring::format(SET_FILL_0, std::setw(2), min), Glib::ustring::format(SET_FILL_0, std::setw(2), sec));
}

void str_replace_last_with(Glib::ustring& str, Glib::ustring::value_type replace, const Glib::ustring& with)
{
  if(!replace)
  {
    str += with;
    return;
  }

  Glib::ustring::size_type i  = str.find_last_of(replace);

  if(i!=Glib::ustring::npos)
  {
    str.replace(i, 1, with);
  }
}

void str_replace_all_with(Glib::ustring& str, const Glib::ustring& replace, const Glib::ustring& with)
{
  if(replace.empty())
  {
    str += with;
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