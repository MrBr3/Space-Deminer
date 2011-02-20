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

#include "./Model.hpp"

Gradient::Gradient()
{
  _puffer = new ColorRGBA[2];
  _puffer_size = 2;
  _puffer[0].set(0.f, 0.f, 0.f, 1.f);
  _puffer[1].set(1.f, 1.f, 1.f, 1.f);

  _n_gradients_needed = 0;
}

Gradient::~Gradient()throw()
{
  delete[] _puffer;
}

void Gradient::require_puffer_size(gsize s)
{
  s = MAX(256, s);

  if(s<=_puffer_size)
    return;

  _puffer_size = s;
  delete[] _puffer;
  _puffer = new ColorRGBA[_puffer_size];

  invalidate();
}

void Gradient::clear_puffer()
{
  if(_puffer_size<=265)
    return;
  _puffer_size = 0;
  delete[] _puffer;
  require_puffer_size(256);
}

void Gradient::require_cairo_gradient(gsize s)
{
  ++_n_gradients_needed;

  if(_cairo_gradient&&_puffer_size==s)
    return;

  _cairo_gradient = Cairo::LinearGradient::create(0., 0., 1., 0.);

  if(s>_puffer_size)
    require_puffer_size(s);
  else
    invalidate_cairo_gradient();
}

void Gradient::clear_cairo_gradient()
{
  --_n_gradients_needed;

  if(_n_gradients_needed==0)
    _cairo_gradient.clear();
}

void Gradient::invalidate()
{
  gdouble inv_puffersize = 1.f/gdouble(_puffer_size);

  for(gsize i=0; i<_puffer_size; ++i)
  {
    gdouble offset = gdouble(i)*inv_puffersize;

    _puffer[i].set(offset, offset, offset, 1.f);//TODO change
  }

  invalidate_cairo_gradient();

  signal_changed().emit();
}

void Gradient::invalidate_cairo_gradient()
{
  if(!_cairo_gradient)
    return;

  _cairo_gradient = Cairo::LinearGradient::create(0., 0., 1., 0.);

  gdouble inv_puffersize = 1.f/gdouble(_puffer_size);

  for(gsize i=0; i<_puffer_size; ++i)
  {
    gdouble offset = gdouble(i)*inv_puffersize;

    _cairo_gradient->add_color_stop_rgba(offset, _puffer[i].r, _puffer[i].g, _puffer[i].b, _puffer[i].a);
  }
}
