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

class Planet : public Refable
{
  gfloat _rotation_x, _rotation_y, _rotation_z;
  gfloat _x_speed, _y_speed, _z_speed;
  bool _rotating;

  sigc::signal<void> _signal_invalidated;

  void _invalidate(){_signal_invalidated.emit();}

  static Planet* _planet;

  Planet();
  Planet(const Planet&);

  void _rotate();

public:
  sigc::signal<void> signal_invalidated(){return _signal_invalidated;}

  gfloat get_x_rotation()const throw(){return _rotation_x;}
  gfloat get_y_rotation()const throw(){return _rotation_y;}
  gfloat get_z_rotation()const throw(){return _rotation_z;}

  /** \brief Increments the sphere's rotation along the x-axis
   *
   * It's guarantes the sphere's x-rotation won't exceed 90° oder fall below -90°
   *
   * \param dx - the angle in degrees to be added to
   * */
  void rotate_x(gfloat dx)
  {
    _rotation_x = CLAMP(_rotation_x+dx, -90.f, 90.f);
    _invalidate();
  }

  /** \brief Increments the sphere's rotation along the y-axis
   *
   * It's guarantes the sphere's y-rotation won't exceed 90° oder fall below -90°
   *
   * \param dx - the angle in degrees to be added to
   * */
  void rotate_y(gfloat dy)
  {
    _rotation_y = CLAMP(_rotation_y+dy, -90.f, 90.f);
    _invalidate();
  }
  void rotate_z(gfloat dz){_rotation_z+=dz;_invalidate();}

  void set_rotation_x_speed(gfloat x_speed);
  void set_rotation_z_speed(gfloat z_speed);

  ~Planet()throw();
  static Glib::RefPtr<Planet> create()
  {
    if(!_planet)
      return Glib::RefPtr<Planet>(new Planet);
    _planet->reference();
    return Glib::RefPtr<Planet>(_planet);
  }
};