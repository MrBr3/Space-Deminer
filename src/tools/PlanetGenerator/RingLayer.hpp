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

class RingLayer : public ImageLayer<RingLayer>
{
public:
  typedef ImageLayer<RingLayer> ParentClass;

private:
  gfloat x_rotation;
  gfloat z_rotation;

  gfloat outer_radius;
  gfloat inner_radius;

  sigc::signal<void> _signal_x_rotation_changed;
  sigc::signal<void> _signal_z_rotation_changed;
  sigc::signal<void> _signal_rotation_changed;
  sigc::signal<void> _signal_inner_radius_changed;
  sigc::signal<void> _signal_outer_radius_changed;

public:
  void rotate_z(gfloat a)
  {
    z_rotation += a;
    _signal_rotation_changed.emit();
  }
  void rotate_x(gfloat a)
  {
    x_rotation += a;
    _signal_rotation_changed.emit();
  }
  void set_z_rotation(gfloat a){z_rotation = a;_signal_rotation_changed.emit();}
  void set_x_rotation(gfloat a){x_rotation = a;_signal_rotation_changed.emit();}
  gfloat get_z_rotation()const{return z_rotation;}
  gfloat get_x_rotation()const{return x_rotation;}
  gfloat get_outer_radius()const{return outer_radius;}
  gfloat get_inner_radius()const{return inner_radius;}
  void set_outer_radius(gfloat r){outer_radius=CLAMP(r, 1.f, 100.f);_signal_outer_radius_changed.emit();}
  void set_inner_radius(gfloat r){inner_radius=CLAMP(r, 1.f, 100.f);_signal_inner_radius_changed.emit();}

  // TODO Subsurface scattering

  sigc::signal<void>& signal_x_rotation_changed(){return _signal_x_rotation_changed;}
  sigc::signal<void>& signal_z_rotation_changed(){return _signal_z_rotation_changed;}
  sigc::signal<void>& signal_rotation_changed(){return _signal_rotation_changed;}
  sigc::signal<void>& signal_inner_radius_changed(){return _signal_inner_radius_changed;}
  sigc::signal<void>& signal_outer_radius_changed(){return _signal_outer_radius_changed;}

  RingLayer();
  ~RingLayer()throw();
};

void register_ring_layer();