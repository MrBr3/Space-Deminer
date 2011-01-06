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
  bool _draw_wireframed;

  bool _rotating_with_mouse;
  bool using_mouse()const throw()
  {
    return _rotating_with_mouse;
  }
  int mouse_drag_start_x, mouse_drag_start_y;

  gfloat distance;

  sigc::signal<void, bool> _signal_wireframed_changed;
  sigc::signal<void> _sig_wireframed_changed_noparam;

  void reinit_sphere_mesh();

public:

  View3D();
  ~View3D()throw();

  void deinit();

  void on_realize();
  bool on_expose_event(GdkEventExpose* event);
  void on_size_allocate(Gtk::Allocation& allocation);

  bool on_scroll_event(GdkEventScroll* event);
  bool on_button_press_event(GdkEventButton* event);
  bool on_button_release_event(GdkEventButton* event);
  bool on_motion_notify_event(GdkEventMotion* event);

  void invalidate();

  Glib::RefPtr<Sphere> sphere;

  /** \brief Gets, whether the Mesh should be drawn wireframed.
   * */
  bool get_draw_wireframed()const{return _draw_wireframed;}

  /** \brief Sets, whether the Mesh should be drawn wireframed.
   *
   * */
  void set_draw_wireframed(bool draw_wireframed=true)
  {
    if(_draw_wireframed==draw_wireframed)
      return;

    _draw_wireframed  = draw_wireframed;
    _signal_wireframed_changed.emit(draw_wireframed);
    invalidate();
  }

  sigc::signal<void, bool>& signal_wireframed_changed(){return _signal_wireframed_changed;}
  sigc::signal<void>& sig_wireframed_changed_noparam(){return _sig_wireframed_changed_noparam;}

  SphereMesh sphere_mesh;
  Glib::RefPtr<Texture> base_texture, cloud_texture, night_texture, weight_texture;

  Matrix44 model_matrix, view_matrix, projection_matrix;
};