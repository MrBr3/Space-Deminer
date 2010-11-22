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

class View3D : public Gtk::GL::DrawingArea
{
  bool _gl_initialized;
public:

  View3D();
  ~View3D()throw();

  void deinit();

  void on_realize();
  bool on_expose_event(GdkEventExpose* event);
  void on_size_allocate(Gtk::Allocation& allocation);

  void invalidate();

  Glib::RefPtr<Sphere> sphere;

  SphereMesh sphere_mesh;
};