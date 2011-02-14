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

Glib::RefPtr<Theme> Theme::create_default_theme()
{
  g_assert_not_reached();
  return DarkTheme::create();
}

DarkTheme* DarkTheme::_singleton = nullptr;

DarkTheme::DarkTheme()
{
  g_assert(!_singleton);
  _singleton  = this;
}

DarkTheme::~DarkTheme()throw()
{
  g_assert(_singleton==this);
  _singleton  = nullptr;
}

void DarkTheme::on_init()
{
  default_font  = Font::create("sans", 12, false, false, true, 0.6f, 0.6f, 0.6f);

  button_normal_simg =    SegmentedImage9::create_from_file(apply_filename_macros("$(exe-share)/ui/themes/dark-theme/button-01.png"),  0, 25, 25, 25, 3, 3, 3, 3);
  button_mouseover_simg = SegmentedImage9::create_from_file(apply_filename_macros("$(exe-share)/ui/themes/dark-theme/button-01.png"), 25, 25, 25, 25, 3, 3, 3, 3);
  button_pushed_simg =    SegmentedImage9::create_from_file(apply_filename_macros("$(exe-share)/ui/themes/dark-theme/button-01.png"), 25,  0, 25, 25, 3, 3, 3, 3);
  button_focused_simg =   SegmentedImage9::create_from_file(apply_filename_macros("$(exe-share)/ui/themes/dark-theme/button-01.png"),  0,  0, 25, 25, 3, 3, 3, 3);

  window_frame_simg =   SegmentedImage9::create_from_file(apply_filename_macros("$(exe-share)/ui/themes/dark-theme/window-frame.png"),  0,  0, 64, 25, 10, 10, 10, 0);
}

void DarkTheme::on_deinit()
{
}

int DarkTheme::get_spacing(Spacing s)const
{
  switch(s)
  {
  case SPACING_SMALL:
    return 2;
  case SPACING_NORMAL:
  case SPACING_LARGE:
    return 4;
  default:
    g_assert_not_reached();
    return 0;
  }
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
  gdouble light_color_r = 1.0;
  gdouble light_color_g = 1.0;
  gdouble light_color_b = 1.0;
  gdouble light_color_a = 0.3;

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
  }else if(is_ment(what, "Window/Back"))
  {
    ee.draw_color_rect(x, y, w, h, back_color_r, back_color_g, back_color_b);
  }else if(is_ment(what, "Window/Frame"))
  {
    button_pushed_simg->draw(ee, x, y, w, 25);
    ee.draw_color_rect(x, y+25, w, MAX(0, h-25), back_color_r, back_color_g, back_color_b);
  }else if(is_ment(what, "Separator/HSeparator"))
  {
    y = y+((1+h)>>1);

    ee.draw_color_rect(x, y, w, 1, light_color_r, light_color_g, light_color_b, light_color_a);
    ee.draw_color_rect(x, y-1, w, 1, shadow_color_r, shadow_color_g, shadow_color_b, shadow_color_a);
  }else if(is_ment(what, "Separator/VSeparator"))
  {
    x = x+((1+w)>>1);

    ee.draw_color_rect(x, y, 1, h, light_color_r, light_color_g, light_color_b, light_color_a);
    ee.draw_color_rect(x-1, y, 1, h, shadow_color_r, shadow_color_g, shadow_color_b, shadow_color_a);
  }
}

void DarkTheme::get_metrics(const Glib::ustring& what, Metrics& metrics)const
{
  metrics.set(0);
  if(is_ment(what, "child_dist"))
  {
    if(is_ment(what, "child-dist/Button"))
    {
      metrics.set(6);
    }else if(is_ment(what, "child-dist/SHADOW_IN"))
    {
      metrics.set(1);
    }else if(is_ment(what, "child-dist/Window/Frame"))
    {
      metrics.x1 = 0;
      metrics.x2 = 0;
      metrics.y1 = 25;
      metrics.y2 = 0;
    }
  }else if(is_ment(what, "width"))
  {
    if(is_ment(what, "width/Separator/VSeparator"))
    {
      metrics.x1 = 0;
      metrics.x2 = 2;
      metrics.y1 = 0;
      metrics.y2 = 0;
    }else if(is_ment(what, "width/Separator/HSeparator"))
    {
      metrics.x1 = 0;
      metrics.x2 = 0;
      metrics.y1 = 0;
      metrics.y2 = 2;
    }
  }
}

Framework::ResPtr<Framework::Font> DarkTheme::create_font(const Glib::ustring& what)const
{
  return default_font;
}

Glib::RefPtr<Framework::Theme> get_menu_main_theme()
{
  return DarkTheme::create();
}