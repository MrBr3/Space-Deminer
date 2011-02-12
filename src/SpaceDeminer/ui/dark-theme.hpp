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

#ifndef _SPACE_DEMINER_DARK_THEME_H_
#define _SPACE_DEMINER_DARK_THEME_H_

#include "./ui/framework/theme.hpp"

class DarkTheme : public Framework::Theme
{
  static DarkTheme* _singleton;
public:
  void draw(Framework::PaintTool& ee, const Glib::ustring& what, Framework::DrawPass pass, guint32 param, const Gdk::Rectangle& where)const;
  void get_metrics(const Glib::ustring& what, Metrics& metrics)const;
  Framework::ResPtr<Framework::Font> create_font(const Glib::ustring& what)const;

  void on_init();
  void on_deinit();
  
  int get_spacing(Spacing spacing)const;

  static Glib::RefPtr<DarkTheme> create()
  {
    if(!_singleton)
      return Glib::RefPtr<DarkTheme>(new DarkTheme());
    _singleton->reference();
    return Glib::RefPtr<DarkTheme>(_singleton);
  }

protected:
  Glib::RefPtr<Framework::Font> default_font;
  Framework::ResPtr<Framework::SegmentedImage9> button_normal_simg, button_mouseover_simg, button_pushed_simg, button_focused_simg;
  Framework::ResPtr<Framework::SegmentedImage9> window_frame_simg;


  DarkTheme();
  ~DarkTheme()throw();
};

#endif