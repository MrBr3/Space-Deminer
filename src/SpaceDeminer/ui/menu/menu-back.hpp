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

#ifndef _SPACE_DEMINER_MENU_BACK_H_
#define _SPACE_DEMINER_MENU_BACK_H_

#include "./../framework.hpp"

class MenuBack : public Framework::FullscreenWindow
{
  Glib::Mutex _using_tex_mutex;
  Glib::ustring _current_filename;

  Glib::RefPtr<Gdk::Pixbuf> load_image_();
  void reload_image();
public:
  MenuBack();
  ~MenuBack()throw();

  void on_expose(Framework::Widget::EventExpose& ee);

  void load_image(const Glib::ustring& filename);
  //void load_random_image();

  void on_size_allocate();

public:
  /*std::list<Glib::ustring> _background_folders;
  std::list<Glib::ustring> _files;*/
  Framework::ResPtr<Framework::Image> _curr_back_image;
};

#endif