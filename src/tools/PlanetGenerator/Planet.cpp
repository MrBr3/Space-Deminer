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

#include "Model.hpp"

const gfloat dt = 0.05f;

Planet* Planet::_planet = nullptr;

Planet::Planet()
{
  g_assert(!_planet);

  _rotation_x  = 0.f;
  _rotation_y  = 0.f;
  _rotation_z  = 90.f;
  _x_speed  = 0.f;
  _y_speed  = 0.f;
  _z_speed  = 0.f;
  _rotating = false;

  _planet  = this;
}

Planet::~Planet()throw()
{
  g_assert(_planet);

  _planet  = nullptr;
}

void Planet::set_rotation_x_speed(gfloat x_speed)
{
  _x_speed  = x_speed;

  if(!_rotating && (_x_speed || _y_speed || _z_speed))
  {
    _rotating = true;
    Glib::signal_timeout().connect_once(sigc::mem_fun(*this, &Planet::_rotate),
                                        1000*dt);
  }
}

void Planet::set_rotation_z_speed(gfloat z_speed)
{
  _z_speed  = z_speed;

  if(!_rotating && (_x_speed || _y_speed || _z_speed))
  {
    _rotating = true;
    Glib::signal_timeout().connect_once(sigc::mem_fun(*this, &Planet::_rotate),
                                        1000*dt);
  }
}

void Planet::_rotate()
{
  _rotating = _x_speed || _y_speed || _z_speed;

  if(_rotating)
  {
    Glib::signal_timeout().connect_once(sigc::mem_fun(*this, &Planet::_rotate),
                                        1000*dt);

    rotate_x(_x_speed * dt);
    rotate_y(_y_speed * dt);
    rotate_z(_z_speed * dt);
  }
}