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

class ViewSettings : public SettingsWidget
{
  gfloat _back_color_red;
  gfloat _back_color_green;
  gfloat _back_color_blue;

  int _n_sphere_segments;

  sigc::signal<void> _signal_back_color_changed;
  sigc::signal<void> _signal_n_sphere_segments_changed;
  sigc::signal<void> _signal_something_changed;

public:
  gfloat get_back_color_red()const{g_assert(this);return _back_color_red;}
  gfloat get_back_color_green()const{g_assert(this);return _back_color_green;}
  gfloat get_back_color_blue()const{g_assert(this);return _back_color_blue;}

  Gdk::Color get_back_color()const;
  void set_back_color(const Gdk::Color& color);

  int get_n_sphere_segments()const{return _n_sphere_segments;}
  void set_n_sphere_segments(int n);

  sigc::signal<void>& signal_back_color_changed(){return _signal_back_color_changed;}
  sigc::signal<void>& signal_something_changed(){return _signal_something_changed;}
  sigc::signal<void>& signal_n_sphere_segments_changed(){return _signal_n_sphere_segments_changed;}

public:
  ViewSettings();
  ~ViewSettings()throw();
};

extern ViewSettings* view_settings;