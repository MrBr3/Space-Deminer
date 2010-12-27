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

class BaseTextureLayer : public Layer
{
  Glib::ustring _filename;

  sigc::signal<void> _signal_filename_changed;

public:
  static BaseTextureLayer* singleton;
  BaseTextureLayer();
  ~BaseTextureLayer()throw();

  Glib::ustring get_filename()const{return _filename;}
  void set_filename(const Glib::ustring& fn){_filename = fn;}

  sigc::signal<void>& signal_filename_changed(){return _signal_filename_changed;}
};

void register_base_texture_layer();