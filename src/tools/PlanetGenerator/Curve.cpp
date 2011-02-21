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

CurvePtr Curve::slots[4];

Curve::Curve()
{
  points = new Point[2];
  n_points = 2;
  _interpolate_linear = false;
  _invalidated = false;

  points[0].x = 0.;
  points[0].y = 0.;
  points[1].x = 1.;
  points[1].y = 1.;

  n_samples = 0;
  samples = nullptr;
  set_n_samples(256);
}

Curve::~Curve()throw()
{
  delete[] points;
  delete[] samples;
}

void Curve::save_slot(guint i)
{
  if(i>=4)
    throw std::invalid_argument("Curve::load_slot: rule 0<=i<4 hurt");

  init_slots();

  slots[i]->set(*this);
}

void Curve::load_slot(guint i)
{
  if(i>=4)
    throw std::invalid_argument("Curve::load_slot: rule 0<=i<4 hurt");

  init_slots();

  set(slots[i]);
}

void Curve::init_slots()
{
  if(slots[0])
    return;

  slots[0] = Curve::create();
  slots[1] = Curve::create();
  slots[2] = Curve::create();
  slots[3] = Curve::create();

  for(gsize i=0; i<4; ++i)
  {
    //TODO Load value from options
    slots[i]->signal_changed().connect(sigc::bind(sigc::ptr_fun(slot_changed), i));
  }
}

void Curve::slot_changed(gsize i)
{
  //TODO save to options
}

void Curve::set(const Curve& other_c)
{
  if(!&other_c || &other_c==this)
    return;

  set_n_points(other_c.get_n_points());

  for(gsize i=0; i<get_n_points(); ++i)
    points[i] = other_c.points[i];

  invalidate();

  set_n_samples(other_c.get_n_samples());
  update_all_samples();
}

void Curve::set(const ConstCurvePtr& c)
{
  const Curve& other_c = *c.operator->();

  set(other_c);
}

void Curve::load_present(Present p)
{
  switch(p)
  {
  case PRESENT_FULL:
    {
      set_n_points(1);
      points[0].x = 0.5;
      points[0].y = 1.0;

      invalidate();
      update_all_samples();
    }
    break;
  case PRESENT_LINEAR:
    {
      set_n_points(2);
      points[0].x = 0.0;
      points[0].y = 0.0;
      points[1].x = 1.0;
      points[1].y = 1.0;

      invalidate();
      update_all_samples();
    }
    break;
  default:
    throw std::invalid_argument("**Curve::load_present** illegal argument");
  }
}

void Curve::flip_h()
{
  update_all_samples();

  for(gsize i=0, j=get_n_points()-1; i<j; ++i, --j)
  {
    exchange(points[i], points[j]);
    points[i].x = 1.-points[i].x;
    points[j].x = 1.-points[j].x;
  }

  if(get_n_points()%2)
  {
    gsize m = get_n_points()>>1;
    points[m].x = 1.-points[m].x;
  }

  for(gsize i=0, j=get_n_samples()-1; i<j; ++i, --j)
    exchange(samples[i], samples[j]);

  _invalidated = false;

  signal_changed().emit();
}

void Curve::flip_v()
{
  update_all_samples();

  for(gsize i=0; i<get_n_points(); ++i)
    points[i].y = 1.-points[i].y;

  for(gsize i=0; i<n_samples; ++i)
    samples[i] = 1.-samples[i];

  _invalidated = false;

  signal_changed().emit();
}

void Curve::throw_parser_error(const std::string& s)
{
  throw std::runtime_error("error while parsing: \""+s+"\"");
}

void Curve::load_from_string(Glib::ustring::const_iterator begin, Glib::ustring::const_iterator end)
{
}

Glib::ustring Curve::save_to_string()const
{
  Glib::ustring str;

  for(gsize i=0; i<get_n_points(); ++i)
    str += "(pt "+Options::real2string(points[i].x)+" "+Options::real2string(points[i].x)+")";

  return "(curve "+boolean2string(get_interpolate_linear())+" "+str+")";
}

void Curve::set_interpolate_linear(bool linear)
{
  if(_interpolate_linear==linear)
    return;
  _interpolate_linear = linear;
  invalidate();
  update_all_samples();
}

void Curve::set_n_samples(gsize n)
{
  n = MAX(2, n);
  if(n_samples==n)
    return;

  delete[] samples;

  n_samples = n;
  samples = new gdouble[n_samples];

  invalidate();
  update_all_samples();
}

void Curve::set_n_points(gsize n)
{
  n = MAX(1, n);
  if(n_points==n)
    return;

  delete[] points;

  n_points = n;
  points = new Point[n_points];
}

gsize Curve::find_point(gdouble x, gdouble y, gdouble max_diff)
{
  gsize found = G_MAXSIZE;
  gdouble found_sdist = G_MAXDOUBLE;

  max_diff = max_diff*max_diff;

  for(gsize i=0; i<get_n_points(); ++i)
  {
    gdouble sdist = square(points[i].x-x)+square(points[i].y-y);

    if(max_diff>=sdist && found_sdist>sdist)
    {
      found_sdist = sdist;
      found = i;
    }
  }

  return found;
}

gsize Curve::add_point(gdouble x, gdouble y) // TODO genau Testen
{
  gsize new_index=G_MAXSIZE;

  x = CLAMP(x, 0., 1.);
  y = CLAMP(y, 0., 1.);

  n_points++;
  Point* new_points = new Point[n_points];

  bool found = false;

  for(gsize i=0; i<get_n_points(); ++i)
  {
    Point& p = points[i];

    if(p.x==x)
    {
      p.y = y;
      return i;
    }
  }

  gsize j=0;
  for(gsize i=0; i<get_n_points(); ++i)
  {
    Point& p = points[j];

    if(p.x==x)
      throw std::invalid_argument("**Curve::add_point** two points with the same x position are not allowed");

    new_points[i] = p;

    if(!found && (p.x>x || i+1==get_n_points()))
    {
      found = true;
      new_points[i].x=x;
      new_points[i].y=y;
      new_index = i;
    }else
      ++j;
  }

  delete[] points;

  points = new_points;

  invalidate();
  update_all_samples();

  return new_index;
}

void Curve::remove_point(gsize i)
{
  if(i>=n_points)
    throw std::out_of_range("**Curve::remove_point** i is out of range");

  for(gsize j=0, k=0; j<n_points; ++j)
  {
    if(i!=j)
    {
      if(j!=k)
        points[k]=points[j];
      ++k;
    }
  }

  --n_points;

  invalidate();
  update_all_samples();
}

bool Curve::move_point(gsize i, gdouble x, gdouble y)
{
  if(i>=n_points)
    throw std::out_of_range("**Curve::move_point** i is out of range");

  x = CLAMP(x, 0., 1.);
  y = CLAMP(y, 0., 1.);

  bool remove = false;

  if(i>0 && x<=points[i-1].x)
    remove = true;
  if(i+1<n_points && x>=points[i+1].x)
    remove = true;

  if(remove)
  {
    remove_point(i);
  }else
  {
    points[i].x = x;
    points[i].y = y;
  }

  invalidate();

  update_all_samples();

  return !remove;
}

void Curve::update_all_samples()
{
  if(!_invalidated)
    return;

  g_assert(get_n_points()>0);

  gsize first_interval_sample = round(points[0].x*(n_samples-1));
  gsize last_interval_sample = round(points[get_n_points()-1].x*(n_samples-1));
  for(gsize i=0; i<first_interval_sample; ++i)
    samples[i] = points[0].y;
  for(gsize i=last_interval_sample; i<n_samples; ++i)
    samples[i] = points[get_n_points()-1].y;

  if(get_n_points()>1)
  {
    const gsize max_i = get_n_points()-1;

    if(get_interpolate_linear())
      for(gsize i=0; i<max_i; i+=1)
        gimp_curve_plot(this, i, i, MIN(i+1, max_i), MIN(i+1, max_i));
    else
      for(gsize i=0; i<max_i; i+=1)
        gimp_curve_plot(this, MAX(1, i)-1, i, MIN(i+1, max_i), MIN(i+2, max_i));
  }

  _invalidated = false;

  signal_changed().emit();
}
