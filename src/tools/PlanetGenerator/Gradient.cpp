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

GradientPtr Gradient::slot[4];

Gradient::Gradient(Present p)
{
  _use_alpha = false;
  _dont_update = 0;
  _n_gradients_needed = 0;
  remap_a = 0.;
  remap_b = 1.;
  _samples.resize(512);

  request_no_updates();
    curve1 = Curve::create();
    curve2 = Curve::create();
    curve3 = Curve::create();
    curve4 = Curve::create();

    load_present(p);
  unrequest_no_updates();

  curve1->signal_changed().connect(sigc::mem_fun(*this, &Gradient::invalidate_and_update));
  curve2->signal_changed().connect(sigc::mem_fun(*this, &Gradient::invalidate_and_update));
  curve3->signal_changed().connect(sigc::mem_fun(*this, &Gradient::invalidate_and_update));
  curve4->signal_changed().connect(sigc::mem_fun(*this, &Gradient::invalidate_and_update));

  invalidate_and_update();
}

Gradient::~Gradient()throw()
{
}

void Gradient::flip_h()
{
  request_no_updates();
    curve1->flip_h();
    curve2->flip_h();
    curve3->flip_h();
    curve4->flip_h();
    exchange(remap_a, remap_b);
    remap_a = 1.-remap_a;
    remap_b = 1.-remap_b;
  unrequest_no_updates();

  invalidate_and_update();
}

void Gradient::load_present(Present p)
{
  bool illegal = false;

  request_no_updates();
  switch(p)
  {
  case PRESENT_BLACK_2_WHITE:
    {
      curve1->load_present(Curve::PRESENT_LINEAR);
      curve2->load_present(Curve::PRESENT_EMPTY);
      curve3->load_present(Curve::PRESENT_EMPTY);
      curve4->load_present(Curve::PRESENT_EMPTY);
      defcolor.set(0.f, 0.f, 0.f);
      color1.set(1.f, 1.f, 1.f);
      color2.set(1.f, 0.f, 0.f);
      color3.set(0.f, 1.f, 0.f);
      color4.set(0.f, 0.f, 1.f);
      _use_alpha = false;
      remap_a = 0.;
      remap_b = 1.;
      set_n_samples(512);
    }
    break;
  case PRESENT_TRANSPARENT_2_WHITE:
    {
      curve1->load_present(Curve::PRESENT_LINEAR);
      curve2->load_present(Curve::PRESENT_EMPTY);
      curve3->load_present(Curve::PRESENT_EMPTY);
      curve4->load_present(Curve::PRESENT_EMPTY);
      defcolor.set(1.f, 1.f, 1.f, 0.f);
      color1.set(1.f, 1.f, 1.f);
      color2.set(1.f, 0.f, 0.f);
      color3.set(0.f, 1.f, 0.f);
      color4.set(0.f, 0.f, 1.f);
      _use_alpha = true;
      remap_a = 0.;
      remap_b = 1.;
      set_n_samples(512);
    }
    break;
  default:
    illegal = true;
  }
  unrequest_no_updates();

  invalidate_and_update();

  if(illegal)
    throw std::invalid_argument("**Gradient::load_present** illegal argument");
}

void Gradient::set(const Gradient& g)
{
  request_no_updates();
    curve1->set(g.curve1);
    curve2->set(g.curve2);
    curve3->set(g.curve3);
    curve4->set(g.curve4);
    defcolor = g.defcolor;
    color1 = g.color1;
    color2 = g.color2;
    color3 = g.color3;
    color4 = g.color4;
    _use_alpha = g._use_alpha;
    remap_a = g.remap_a;
    remap_b = g.remap_b;
    set_n_samples(g.get_n_samples());
  unrequest_no_updates();

  invalidate_and_update();
}

void Gradient::set(GradientPtr g)
{
  set(*g.operator->());
}

void Gradient::init_slots()
{
  if(slot[0])
    return;

  for(gsize i=0; i<4; ++i)
  {
    slot[i] = Gradient::create();
    Options::get_gradient(Glib::ustring::compose("gradient-slot%1", i),  slot[i]);
    slot[i]->signal_changed().connect(sigc::bind(sigc::ptr_fun(slot_changed), i));
  }
}

void Gradient::slot_changed(gsize i)
{
  Options::set_gradient(Glib::ustring::compose("gradient-slot%1", i), slot[i]);
}

void Gradient::save_slot(guint i)
{
  init_slots();

  slot[i]->set(*this);
}

void Gradient::load_slot(guint i)
{
  init_slots();

  set(slot[i]);
}

//---- colors ----

void Gradient::set_defcolor(const ColorRGBA& defcolor)
{
  this->defcolor = defcolor;
  invalidate_and_update();
}

void Gradient::set_color1(const ColorRGBA& color1)
{
  this->color1 = color1;
  invalidate_and_update();
}

void Gradient::set_color2(const ColorRGBA& color2)
{
  this->color2 = color2;
  invalidate_and_update();
}

void Gradient::set_color3(const ColorRGBA& color3)
{
  this->color3 = color3;
  invalidate_and_update();
}

void Gradient::set_color4(const ColorRGBA& color4)
{
  this->color4 = color4;
  invalidate_and_update();
}

//--------

void Gradient::set_use_alpha(bool use_alpha)
{
  this->_use_alpha = use_alpha;
  invalidate_and_update();
}

void Gradient::set_remap_a(gdouble a)
{
  remap_a = a;
  invalidate_and_update_just_gradient();
}

void Gradient::set_remap_b(gdouble b)
{
  remap_b = b;
  invalidate_and_update_just_gradient();
}

//---- Samples ----

void Gradient::set_n_samples(gsize s)
{
  s = MAX(16, s);

  if(s==get_n_samples())
    return;

  request_no_updates();
    curve1->set_n_samples(s);
    curve2->set_n_samples(s);
    curve3->set_n_samples(s);
    curve4->set_n_samples(s);
  unrequest_no_updates();

  _samples.resize(s, DONT_INIT);

  g_assert(s==get_n_samples());

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

    gdouble c1 = curve1->get_value(offset);
    gdouble c2 = curve2->get_value(offset);
    gdouble c3 = curve3->get_value(offset);
    gdouble c4 = curve4->get_value(offset);
    gdouble c_def = 1. - CLAMP(c1+c2+c3+c4, 0., 1.);
    gdouble c_color_def = defcolor.a*c_def;
    gdouble inv_c = c1+c2+c3+c4+c_def;
    gdouble inv_col_c = c1+c2+c3+c4+c_color_def;
    if(inv_c>0.)
      inv_c = 1./inv_c;
    if(inv_col_c>0.)
      inv_col_c = 1./inv_col_c;

    gfloat r = (defcolor.r*c_color_def
               +color1.r*c1
               +color2.r*c2
               +color3.r*c3
               +color4.r*c4)*inv_col_c;
    gfloat g = (defcolor.g*c_color_def
               +color1.g*c1
               +color2.g*c2
               +color3.g*c3
               +color4.g*c4)*inv_col_c;
    gfloat b = (defcolor.b*c_color_def
               +color1.b*c1
               +color2.b*c2
               +color3.b*c3
               +color4.b*c4)*inv_col_c;
    gfloat a = get_use_alpha() ?
               (defcolor.a*c_def
               +color1.a*c1
               +color2.a*c2
               +color3.a*c3
               +color4.a*c4)*inv_c : 1.f;

    _samples[i].set(r, g, b, a);
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
}

void Gradient::update_cairo_gradient()
{
  if(!_n_gradients_needed)
  {
    _cairo_gradient.clear();
    signal_changed().emit();
    return;
  }

  _cairo_gradient = Cairo::LinearGradient::create(0., 0., 1., 0.);

  gsize n_samples = _samples.size();
  gdouble inv_n_samples = 1.f/gdouble(n_samples);

  for(gsize i=0; i<n_samples; ++i)
  {
    gdouble offset = get_remapped_offset(gdouble(i)*inv_n_samples);

    _cairo_gradient->add_color_stop_rgba(offset, _samples[i].r, _samples[i].g, _samples[i].b, _samples[i].a);
  }
}
