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

#ifndef _SPACEDEMINER_UI_FRAMEWORK_THEME_H_
#define _SPACEDEMINER_UI_FRAMEWORK_THEME_H_

#include <gdkmm/rectangle.h>
#include "./image.hpp"
#include "./font.hpp"
#include "./segmentedimage.hpp"

namespace Framework
{
  class PaintTool;
  class Theme : public Refable
  {
  public:
    struct Metrics
    {
      gint x1, x2, y1, y2;

      void set(gint x)throw(){x1=x2=y1=y2=x;}
    };
    
    enum Spacing
    {
      SPACING_SMALL,
      SPACING_NORMAL,
      SPACING_LARGE
    };
    
    virtual int get_spacing(Spacing spacing)const=0;

    virtual void draw(PaintTool& ee, const Glib::ustring& what, DrawPass pass, guint32 param, const Gdk::Rectangle& where)const=0;

    virtual void get_metrics(const Glib::ustring& what, Metrics& metrics)const=0;

    virtual ResPtr<Font> create_font(const Glib::ustring& what)const=0;

    static bool is_ment(Glib::ustring a, Glib::ustring b);

    virtual void on_init()=0;
    virtual void on_deinit()=0;
    
    static Glib::RefPtr<Theme> create_default_theme();

  protected:
    Theme(){}
    ~Theme()throw(){}
  };
}

#endif