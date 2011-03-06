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

ColorCurve::ColorCurve()
{
  _hide_alpha = true;
  _n_samples_forced = false;
  _hide_alpha_forced = false;

  _invalidated = true;
  _dont_update = 0;

  _samples.resize(512);

  value_curve = Curve::create(Curve::PRESENT_LINEAR);
  red_curve = Curve::create(Curve::PRESENT_LINEAR);
  green_curve = Curve::create(Curve::PRESENT_LINEAR);
  blue_curve = Curve::create(Curve::PRESENT_LINEAR);
  alpha_curve = Curve::create(Curve::PRESENT_LINEAR);

  value_curve->signal_changed().connect(sigc::mem_fun(*this, &ColorCurve::invalidate_and_update));
  red_curve->signal_changed().connect(sigc::mem_fun(*this, &ColorCurve::invalidate_and_update));
  green_curve->signal_changed().connect(sigc::mem_fun(*this, &ColorCurve::invalidate_and_update));
  blue_curve->signal_changed().connect(sigc::mem_fun(*this, &ColorCurve::invalidate_and_update));
  alpha_curve->signal_changed().connect(sigc::mem_fun(*this, &ColorCurve::invalidate_and_update));

  invalidate_and_update();
}

ColorCurve::~ColorCurve()throw()
{
}

ColorCurvePtr ColorCurve::create()
{
  return ColorCurvePtr(new ColorCurve);
}

void ColorCurve::reset()
{
  request_no_updates();
    value_curve->load_present(Curve::PRESENT_LINEAR);
    red_curve->load_present(Curve::PRESENT_LINEAR);
    green_curve->load_present(Curve::PRESENT_LINEAR);
    blue_curve->load_present(Curve::PRESENT_LINEAR);
    alpha_curve->load_present(Curve::PRESENT_LINEAR);
  unrequest_no_updates();

  invalidate_and_update();
}

void ColorCurve::set(const ColorCurve& cc)
{
  request_no_updates();
    value_curve->set(cc.get_value_curve());
    red_curve->set(cc.get_red_curve());
    green_curve->set(cc.get_green_curve());
    blue_curve->set(cc.get_blue_curve());
    alpha_curve->set(cc.get_alpha_curve());

    set_hide_alpha(cc.get_hide_alpha());
    set_n_samples(cc.get_n_samples());
  unrequest_no_updates();

  invalidate_and_update();
}

void ColorCurve::set(const ConstColorCurvePtr cc)
{
  set(*cc.operator->());
}

void ColorCurve::set_hide_alpha(bool a)
{
  if(_hide_alpha_forced)
    return;

  _hide_alpha = a;

  invalidate_and_update();
}

void ColorCurve::set_n_samples(gsize s)
{
  if(_n_samples_forced)
    return;

  s = MAX(16, s);

  if(s==get_n_samples())
    return;

  request_no_updates();
    value_curve->set_n_samples(s);
    red_curve->set_n_samples(s);
    green_curve->set_n_samples(s);
    blue_curve->set_n_samples(s);
    alpha_curve->set_n_samples(s);
  unrequest_no_updates();

  _samples.resize(s, DONT_INIT);

  g_assert(s==get_n_samples());

  invalidate_and_update();
}

void ColorCurve::update_samples()
{
  if(!_invalidated||_dont_update>0)
    return;
  _invalidated = false;

  gsize n_samples = _samples.size();
  gdouble inv_n_samples = 1.f/gdouble(n_samples);

  for(gsize i=0; i<n_samples; ++i)
  {
    gdouble offset = gdouble(i)*inv_n_samples;

    gdouble r = value_curve->get_value(red_curve->get_value(offset));
    gdouble g = value_curve->get_value(green_curve->get_value(offset));
    gdouble b = value_curve->get_value(blue_curve->get_value(offset));
    gdouble a = alpha_curve->get_value(offset);

    _samples[i].set(r, g, b, a);
  }

  signal_changed().emit();
}
