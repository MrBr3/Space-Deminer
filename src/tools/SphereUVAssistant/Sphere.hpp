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

class Sphere : public Refable
{
  gfloat _rotation_x, _rotation_y, _rotation_z;
  gfloat _x_speed, _y_speed, _z_speed;
  gfloat _scale;
  bool _rotating;

  sigc::signal<void> _signal_invalidated;

  static Sphere* _sphere;

  Sphere();

  void _invalidate(){_signal_invalidated.emit();}
  void _rotate();

public:
  sigc::signal<void> signal_invalidated(){return _signal_invalidated;}

  gfloat get_x_rotation()const throw(){return _rotation_x;}
  gfloat get_y_rotation()const throw(){return _rotation_y;}
  gfloat get_z_rotation()const throw(){return _rotation_z;}
  void rotate_x(gfloat dx){_rotation_x+=dx;_invalidate();}
  void rotate_y(gfloat dy){_rotation_y+=dy;_invalidate();}
  void rotate_z(gfloat dz){_rotation_z+=dz;_invalidate();}
  gfloat get_scale()const{return _scale;}
  void set_scale(gfloat new_scale){_scale = CLAMP(new_scale, 0.001f, 16.f);_invalidate();}

  void set_rotation_speed(gfloat x_speed, gfloat y_speed, gfloat z_speed);

  ~Sphere()throw();
  static Glib::RefPtr<Sphere> create()
  {
    if(!_sphere)
      return Glib::RefPtr<Sphere>(new Sphere);
    _sphere->reference();
    return Glib::RefPtr<Sphere>(_sphere);
  }
};