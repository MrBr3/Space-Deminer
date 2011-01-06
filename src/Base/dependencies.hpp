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

#ifndef _SPACE_DEMINER_BASE_DEPENDENCIES_H_
#define _SPACE_DEMINER_BASE_DEPENDENCIES_H_

#define nullptr 0

#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sigc++/trackable.h>
#include <glibmm.h>
#include <iostream>
#include <algorithm>
#include <gdkmm/rectangle.h>


inline Glib::ustring _(const Glib::ustring& str)
{
  if(str=="[common-characters]")
    return Glib::ustring();
  return str;
}

namespace Gdk
{
  namespace GL
  {
    class Context;
  }
  class Region;
}

inline bool operator==(const Gdk::Rectangle& a, const Gdk::Rectangle& b)
{
  return a.get_x()==b.get_x() && a.get_y()==b.get_y() && a.get_width()==b.get_width() && a.get_height()==b.get_height();
}
inline bool operator!=(const Gdk::Rectangle& a, const Gdk::Rectangle& b){return !operator==(a, b);}

typedef float Real;

class IllegalParam : public Glib::Exception
{
public:
  const Glib::ustring _text;

  Glib::ustring what()const{return _text;}

  IllegalParam(const Glib::ustring& param, const Glib::ustring& reason) : _text(Glib::ustring::compose("IllegalParam \"%1\": %2", param, reason)){}
  IllegalParam(const Glib::ustring& param) : _text(Glib::ustring::compose("IllegalParam \"%1\"", param)){}
  IllegalParam():_text("IllegalParam"){}

  ~IllegalParam()throw(){}
};
class NoInstance : public Glib::Exception
{
public:
  Glib::ustring what()const{return _("There's no instance of a desired object.");}

  ~NoInstance()throw(){}
};
class BadEnum : public Glib::Exception
{
public:
  Glib::ustring what()const{return _("Using illegal enumeration value.");}

  ~BadEnum()throw(){}
};

enum DontInit{DONT_INIT};

#endif