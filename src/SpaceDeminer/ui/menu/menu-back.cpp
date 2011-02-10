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

#include "./../menu.hpp"

const gdouble reloading_timeout = 1.0;

MenuBack::MenuBack()
{
  g_assert(Framework::ResourceManager::get_n_resources()==0);

  //load_random_image();
  load_image("$(exe-share)/ui/background-images/scrap-001.png");  // TODO put this into an init() function?
  
  show();
}

MenuBack::~MenuBack()throw()
{
}

void MenuBack::on_expose(Framework::Widget::EventExpose& ee)
{
  g_assert(&ee);

  Glib::Mutex::Lock _lock(_using_tex_mutex);

  if(_curr_back_image)
    ee.draw_resized_image(_curr_back_image, 0., 0., get_width(), get_height(), Options::get_bool(OPTION_UI_TRUNK_BACK_IMAGE, true));
}

void MenuBack::reload_image()
{
  Framework::ResPtr<Framework::Image> new_img = Framework::Image::create(load_image_(), TEXTURE_HINT_SIZEABLE, sigc::mem_fun(*this, &MenuBack::load_image_));

  {
    Glib::Mutex::Lock _lock(_using_tex_mutex);

    _curr_back_image  = new_img;
  }
  invalidate();
}

void MenuBack::on_size_allocate()
{
  FullscreenWindow::on_size_allocate();
  
  Glib::signal_timeout().connect_once(sigc::mem_fun(*this, &MenuBack::reload_image), reloading_timeout*1000.);
}

Glib::RefPtr<Gdk::Pixbuf> MenuBack::load_image_()
{
  Glib::RefPtr<Gdk::Pixbuf> pb = Gdk::Pixbuf::create_from_file(Glib::filename_from_utf8(apply_filename_macros(_current_filename)));

  if(!pb)
    return pb;

  int dest_width  = get_width();
  int dest_height = get_height();

  if(dest_width==0)
    return pb;

  g_assert(pb->get_width()>0);
  g_assert(dest_width>0);

  Real dest_aspect  = Real(dest_height)/Real(dest_width);
  Real img_aspect  = Real(pb->get_height())/Real(pb->get_width());

  g_assert(dest_aspect>0);
  g_assert(img_aspect>0);

  bool scale_height_based;

  if(dest_aspect>img_aspect)
  {
    //if(pb->get_height()<=dest_height)
    //  return pb;

    scale_height_based  = true;
  }else
  {
    //if(pb->get_width()<=dest_width)
    //  return pb;

    scale_height_based  = false;
  }

  if(!Options::get_bool(OPTION_UI_TRUNK_BACK_IMAGE, true))
    scale_height_based  = !scale_height_based;

  if(scale_height_based)
    return pb->scale_simple(dest_height/img_aspect, dest_height, Gdk::INTERP_HYPER);
  else
    return pb->scale_simple(dest_width, img_aspect*dest_width, Gdk::INTERP_HYPER);
}

void MenuBack::load_image(const Glib::ustring& filename)
{
  _current_filename = filename;
  _curr_back_image  = Framework::Image::create(load_image_(), TEXTURE_HINT_SIZEABLE, sigc::mem_fun(*this, &MenuBack::load_image_));
}