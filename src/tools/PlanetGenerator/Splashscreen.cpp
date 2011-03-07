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

#include "MainWindow.hpp"

// see http://stackoverflow.com/questions/3908565/how-to-make-gtk-window-background-transparent
// see http://plan99.net/~mike/files/alphademo.c

Splashscreen::Splashscreen()
{
  support_alpha = false;

  Glib::RefPtr<Gdk::Screen> def_screen = Gdk::Screen::get_default();

  if(!def_screen || def_screen->get_width()<800 || def_screen->get_height()<600)
    return;

  try
  {
    img_surface = Cairo::ImageSurface::create_from_png(Glib::filename_from_utf8(apply_filename_macros("$(exe-share)/splash.png")));
    if(!img_surface)
      throw 0;
  }catch(...)
  {
    g_warning("Couldn't load SplashScreen");
    return;
  }

  set_title("PlanetGenerator starting...");
  set_default_size(img_surface->get_width(), img_surface->get_height());

  set_app_paintable(true);
  set_decorated(false);
  set_position(Gtk::WIN_POS_CENTER);

  on_screen_changed(Glib::RefPtr<Gdk::Screen>());

  show();

  while(Gtk::Main::events_pending())
    Gtk::Main::iteration(false);
}


void Splashscreen::on_screen_changed(const Glib::RefPtr<Gdk::Screen>& prev_screen)
{
  Glib::RefPtr<Gdk::Screen> screen = get_screen();

  if(!screen)
  {
    support_alpha = false;
    return;
  }

  Glib::RefPtr<Gdk::Colormap> cm = screen->get_rgba_colormap();

  if(!cm)
  {
    support_alpha = false;
    return;
  }

  support_alpha =true;

  set_colormap(cm);
}

bool Splashscreen::on_expose_event(GdkEventExpose*)
{
  Glib::RefPtr<Gdk::Window> w = get_window();

  if(!w)
    return true;

  Cairo::RefPtr<Cairo::Context> cc = w->create_cairo_context();

  g_assert(cc);

  if(support_alpha)
    cc->set_source_rgba(1., 1., 1., 0.);
  else
    cc->set_source_rgb(1., 1., 1.);

  cc->set_operator(Cairo::OPERATOR_SOURCE);
  cc->paint();

  if(img_surface)
  {
    cc->set_source(img_surface, 0., 0.);
    cc->paint();
  }

  return true;
}
