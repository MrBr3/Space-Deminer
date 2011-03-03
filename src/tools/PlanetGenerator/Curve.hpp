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

#ifndef TOOLS_MODEL_CURVE_HPP_
#define TOOLS_MODEL_CURVE_HPP_

class Curve : public Refable
{
  Curve(const Curve&);
public:
  typedef Glib::RefPtr<Curve> CurvePtr;
  typedef Glib::RefPtr<const Curve> ConstCurvePtr;

  enum Present
  {
    PRESENT_FULL,
    PRESENT_EMPTY,
    PRESENT_LINEAR,
  };

  struct Point
  {
    gdouble x, y;
  };

  void flip_h();
  void flip_v();
  void load_present(Present p);

  void set(const ConstCurvePtr& c);
  void set(const Curve& c);
  Curve& operator=(const Curve& c)
  {
    set(c);
    return *this;
  }

  bool get_interpolate_linear()const{return _interpolate_linear;}
  void set_interpolate_linear(bool linear=true);

  sigc::signal<void>& signal_changed(){return _signal_changed;}
  sigc::signal<void>& signal_changed()const{return const_cast<sigc::signal<void>& >(_signal_changed);}

  void invalidate()
  {
    _invalidated = true;
  }

private:
  bool _interpolate_linear;
  bool _invalidated;

  sigc::signal<void> _signal_changed;

public:
  static CurvePtr create(){return CurvePtr(new Curve);}

// ---- Slot stuff ----
public:
  static void init_slots();
  void save_slot(guint i);
  void load_slot(guint i);

private:
  static CurvePtr slots[4];
  static void slot_changed(gsize i);

// ---- Parsing stuff----
public:
  void load_from_string(Glib::ustring::const_iterator& begin, Glib::ustring::const_iterator end);
  Glib::ustring save_to_string()const;

private:
  void load_point_from_string(Point& pt, Glib::ustring::const_iterator& begin, Glib::ustring::const_iterator end);
  void throw_parser_error(const std::string& s);
  void throw_point_parser_error(const std::string& s);

// ----Sample/Point stuff----
private:
  Curve();
  ~Curve()throw();

public:
  void set_n_samples(gsize n);
  gsize get_n_samples()const{return n_samples;}
  const gdouble* get_samples()const{return samples;}
  void update_all_samples();
  const gdouble& get_sample(gsize i)const
  {
    if(i>=get_n_samples())
      throw std::out_of_range("Curve::get_sample: bad index");
    return get_samples()[i];
  }

  gdouble get_value(gdouble x)const
  {
    x = CLAMP(x, 0., 1.)*(get_n_samples()-1);

    gdouble a = get_frac_part(x);
    gdouble b = 1.-a;
    return get_sample(floor(x))*a + b*get_sample(ceil(x));
  }

  const Point* get_points()const{return points;}
  Point* get_points(){return points;}
  const Point& get_point(gsize i)const
  {
    if(i>=get_n_points())
      throw std::out_of_range("Curve::get_point: bad index");
    return get_points()[i];
  }
  Point& get_point(gsize i)
  {
    if(i>=get_n_points())
      throw std::out_of_range("Curve::get_point: bad index");
    return points[i];
  }
  gsize get_n_points()const{return n_points;}
  void set_n_points(gsize n);

  gsize add_point(gdouble x, gdouble y);
  void remove_point(gsize i);
  gsize find_point(gdouble x, gdouble y, gdouble max_diff);

  gdouble snap_to_samples(gdouble x)const
  {
    if(n_samples==0)
      throw std::out_of_range("Curve::snap_to_samples: bad number of samples");
    if(n_samples<16)
      return x;
    return round(x*n_samples-1)/(n_samples-1);
  }

  /**
   *
   * \return true if the point still exists after the moving operation
   **/
  bool move_point(gsize i, gdouble x, gdouble y);

private:
  // The following names and types are chosen for compatibility with the gimp_curve_plot procedure in gimpcurve.cpp
  static void gimp_curve_plot(Curve*, gsize, gsize, gsize, gsize);

  Point* points;
  gsize n_points;
  gsize n_samples;
  gdouble* samples;
};


typedef Curve::CurvePtr CurvePtr;
typedef Curve::ConstCurvePtr ConstCurvePtr;

#endif
