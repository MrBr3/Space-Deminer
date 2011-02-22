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
  _dont_update = 0;
  _n_gradients_needed = 0;

  request_no_updates();
    defcolor.set(0.f, 0.f, 0.f, 0.f);
    color1.set(1.f, 1.f, 1.f);

    curve1 = Curve::create();
    curve2 = Curve::create();
    curve3 = Curve::create();
    curve4 = Curve::create();

    curve2->load_present(Curve::PRESENT_EMPTY);
    curve3->load_present(Curve::PRESENT_EMPTY);
    curve4->load_present(Curve::PRESENT_EMPTY);
  unrequest_no_updates();

  curve1->signal_changed().connect(sigc::mem_fun(*this, &Gradient::invalidate_and_update));
  curve2->signal_changed().connect(sigc::mem_fun(*this, &Gradient::invalidate_and_update));
  curve3->signal_changed().connect(sigc::mem_fun(*this, &Gradient::invalidate_and_update));
  curve4->signal_changed().connect(sigc::mem_fun(*this, &Gradient::invalidate_and_update));

  set_n_samples(256);
}

Gradient::~Gradient()throw()
{
}

//---- Samples ----

void Gradient::set_n_samples(gsize s)
{
  s = MAX(16, s);

  request_no_updates();
    curve1->set_n_samples(s);
    curve2->set_n_samples(s);
    curve3->set_n_samples(s);
    curve4->set_n_samples(s);
  unrequest_no_updates();

  _samples.resize(s, DONT_INIT);

  invalidate();
  update_samples();
}

void Gradient::update_samples()
{
  if(!_invalidated||_dont_update>0)
    return;
  _invalidated = false;

  gsize n_samples = _samples.size();
  gdouble inv_n_samples = 1.f/gdouble(n_samples);


  for(gsize i=0; i<n_samples; ++i)
  {
    gdouble offset = gdouble(i)*inv_n_samples;
  //TODO remap

    gdouble c1 = curve1->get_value(offset);
    gdouble c2 = curve2->get_value(offset);
    gdouble c3 = curve3->get_value(offset);
    gdouble c4 = curve4->get_value(offset);
    gdouble c_def = 1. - CLAMP(c1+c2+c3+c4, 0., 1.);
    gdouble inv_c = c1+c2+c3+c4+c_def;
    g_assert(inv_c>0.);
    inv_c = 1./inv_c;

    _samples[i].set(( defcolor.r*c_def
                     +color1.r*c1
                     +color2.r*c2
                     +color3.r*c3
                     +color4.r*c4)*inv_c,
                     ( defcolor.g*c_def
                     +color1.g*c1
                     +color2.g*c2
                     +color3.g*c3
                     +color4.g*c4)*inv_c,
                     ( defcolor.b*c_def
                     +color1.b*c1
                     +color2.b*c2
                     +color3.b*c3
                     +color4.b*c4)*inv_c,
                     ( defcolor.a*c_def
                     +color1.a*c1
                     +color2.a*c2
                     +color3.a*c3
                     +color4.a*c4)*inv_c);

    _samples[i].set(c1, c1, c1);
  }

  update_cairo_gradient();

  signal_changed().emit();
}

//---- Cairo:Gradient ----

void Gradient::reference_cairo_gradient()
{
  ++_n_gradients_needed;

  update_cairo_gradient();
}

void Gradient::unreference_cairo_gradient()
{
  --_n_gradients_needed;

  update_cairo_gradient();
}

void Gradient::update_cairo_gradient()
{
  if(!_n_gradients_needed)
  {
    _cairo_gradient.clear();
    return;
  }

  _cairo_gradient = Cairo::LinearGradient::create(0., 0., 1., 0.);

  gsize n_samples = _samples.size();
  gdouble inv_n_samples = 1.f/gdouble(n_samples);

  for(gsize i=0; i<n_samples; ++i)
  {
    gdouble offset = gdouble(i)*inv_n_samples;

    _cairo_gradient->add_color_stop_rgba(offset, _samples[i].r, _samples[i].g, _samples[i].b, _samples[i].a);
  }
}
