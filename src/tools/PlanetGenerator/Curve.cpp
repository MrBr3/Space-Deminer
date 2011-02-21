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

Curve::Curve()
{
  points = new Point[1];
  n_points = 1;
  n_samples = 2;
  samples = new gdouble[2];
  _interpolare_linear = false;

  points[0].x = 0.;
  points[0].y = 0.;
  samples[0] = 0.;
  samples[1] = 0.;
}

Curve::~Curve()throw()
{
  delete[] points;
  delete[] samples;
}

void Curve::set_n_samples(gsize n)
{
  n = MAX(2, n);
  if(n_samples==n)
    return;

  delete[] samples;

  n_samples = n;
  samples = new gdouble[n_samples];

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
  return !remove;
}

void Curve::update_all_samples()
{
  g_assert(get_n_points()>0);

  gsize first_interval_sample = round(points[0].x*(n_samples-1)); // TODO genau Testen
  gsize last_interval_sample = round(points[get_n_points()-1].x*(n_samples-1)); // TODO genau Testen
  for(gsize i=0; i<first_interval_sample; ++i) // TODO genau Testen
    samples[i] = points[0].y;
  for(gsize i=last_interval_sample; i<n_samples; ++i) // TODO genau Testen
    samples[i] = points[get_n_points()-1].y;

  const gsize max_i = get_n_points()-1;

  /*for(gsize i=0; i<max_i; i+=2)
    if(i+1==max_i)
      gimp_curve_plot(this, i, i, max_i, max_i);
    else
      gimp_curve_plot(this, i, i+1), MIN(max_i, i+2), MIN(max_i, i+3));*/

  signal_changed().emit();
}
