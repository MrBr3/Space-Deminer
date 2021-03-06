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

#include "./theme.hpp"

namespace Framework
{
  bool Theme::dont_destroy = true;
  sigc::signal<void> Theme::signal_can_destroy_now;

  Theme::Theme()
  {
    if(dont_destroy)
    {
      reference();
      signal_can_destroy_now.connect(sigc::mem_fun(*this, &Theme::unreference));
    }
  }
  Theme::~Theme()throw()
  {
  }

  void Theme::can_destroy_now()
  {
    if(dont_destroy)
      signal_can_destroy_now.emit();
    dont_destroy = false;
  }

  bool Theme::is_ment(Glib::ustring a, Glib::ustring b)
  {
    a = a.lowercase();
    b = b.lowercase();

    str_replace_all_with(a, '_', '-');
    str_replace_all_with(b, '_', '-');

    if(a.length()==b.length())
      return a==b;
    if(a.length()>b.length())
      exchange(a, b);

    if(a!=b.substr(0, a.length()))
      return false;

    if(a[a.length()-1]!='/' && b[a.length()]!='/')
      return false;

    return true;
  }
}