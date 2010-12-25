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

#include "./dark-theme.hpp"
#include "./framework.hpp"

using namespace Framework;

DarkTheme::DarkTheme()
{
  default_font  = Font::create("sans", 12, false, false, true, 0.6f, 0.6f, 0.6f);

  button_normal_simg =    SegmentedImage9::create_from_file(apply_filename_macros("$(exe-share)/ui/themes/dark-theme/button-01.png"),  0, 25, 25, 25, 3, 3, 3, 3);
  button_mouseover_simg = SegmentedImage9::create_from_file(apply_filename_macros("$(exe-share)/ui/themes/dark-theme/button-01.png"), 25, 25, 25, 25, 3, 3, 3, 3);
  button_pushed_simg =    SegmentedImage9::create_from_file(apply_filename_macros("$(exe-share)/ui/themes/dark-theme/button-01.png"), 25,  0, 25, 25, 3, 3, 3, 3);
  button_focused_simg =   SegmentedImage9::create_from_file(apply_filename_macros("$(exe-share)/ui/themes/dark-theme/button-01.png"),  0,  0, 25, 25, 3, 3, 3, 3);
}

DarkTheme::~DarkTheme()throw()
{
}

void DarkTheme::draw(Framework::PaintTool& ee, const Glib::ustring& what, Framework::DrawPass pass, guint32 param, const Gdk::Rectangle& where)const
{
  int x = where.get_x();
  int y = where.get_y();
  int w = where.get_width();
  int h = where.get_height();

  gdouble front_color_r = 0.6;
  gdouble front_color_g = 0.6;
  gdouble front_color_b = 0.6;
  gdouble front_color_a = 1.;
  gdouble back_color_r = 0.2;
  gdouble back_color_g = 0.2;
  gdouble back_color_b = 0.2;
  gdouble back_color_a = 1.0;
  gdouble shadow_color_r = 0.0;
  gdouble shadow_color_g = 0.0;
  gdouble shadow_color_b = 0.0;
  gdouble shadow_color_a = 0.3;

  back_color_r = 0.0;
  back_color_g = 0.2;
  back_color_b = 0.0;
  back_color_a = 1.0;

  if(is_ment(what, "Button/Dummy"))
  {
    g_assert_not_reached();
  }

  if(is_ment(what, "Button"))
  {
    if(pass==DRAWPASS_BACK)
    {
      int offset=0;

      switch(param)
      {
      case Framework::Button::DRAW_PARAM_RELIEF_PUSHED:
        button_pushed_simg->draw(ee, where);
        break;
      case Framework::Button::DRAW_PARAM_RELIEF_MOUSEOVER:
        button_mouseover_simg->draw(ee, where);
        break;
      case Framework::Button::DRAW_PARAM_RELIEF_NORMAL:
      default:
        button_normal_simg->draw(ee, where);
      }
    }
  }
}

void DarkTheme::get_metrics(const Glib::ustring& what, Metrics& metrics)const
{
  metrics.set(0);
  if(is_ment(what, "child-dist/Button"))
  {
    metrics.set(6);
  }else if(is_ment(what, "child-dist/SHADOW_IN"))
  {
    metrics.set(1);
  }
}

Framework::ResPtr<Framework::Font> DarkTheme::create_font(const Glib::ustring& what)const
{
  return default_font;
}

void DarkTheme::on_init()
{
}

void DarkTheme::on_deinit()
{
}