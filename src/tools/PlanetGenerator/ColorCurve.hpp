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

 #ifndef TOOLS_MODEL_COLOR_CURVE_HPP_
 #define TOOLS_MODEL_COLOR_CURVE_HPP_

#include <base.hpp>
 #include "./Curve.hpp"

class ColorCurve : public Refable
{
  bool operator==(const ColorCurve& g)const;
  bool operator!=(const ColorCurve& g)const;
  ColorCurve(const ColorCurve&);

  sigc::signal<void> _signal_changed;
public:
  typedef Glib::RefPtr<ColorCurve> ColorCurvePtr;
  typedef Glib::RefPtr<const ColorCurve> ConstColorCurvePtr;

  void set(const ColorCurve& cc);
  void set(const ConstColorCurvePtr cc);

  static ColorCurvePtr create();

  sigc::signal<void>& signal_changed(){return _signal_changed;}
  sigc::signal<void>& signal_changed()const{return const_cast<sigc::signal<void>& >(_signal_changed);}

  Glib::ustring save_to_string()const;
  void load_from_string(Glib::ustring::const_iterator& begin, Glib::ustring::const_iterator end);

///@{
///@name Components
private:
  CurvePtr value_curve, red_curve, green_curve, blue_curve, alpha_curve;
  bool _hide_alpha;

public:
  const CurvePtr& get_value_curve(){return value_curve;}
  ConstCurvePtr get_value_curve()const{return value_curve;}
  const CurvePtr& get_red_curve(){return red_curve;}
  ConstCurvePtr get_red_curve()const{return red_curve;}
  const CurvePtr& get_green_curve(){return green_curve;}
  ConstCurvePtr get_green_curve()const{return green_curve;}
  const CurvePtr& get_blue_curve(){return blue_curve;}
  ConstCurvePtr get_blue_curve()const{return blue_curve;}
  const CurvePtr& get_alpha_curve(){return alpha_curve;}
  ConstCurvePtr get_alpha_curve()const{return alpha_curve;}

  void set_hide_alpha(bool a);
  bool get_hide_alpha()const{return _hide_alpha;}
///@}

///@{
///@name Samples Stuff
public:
  typedef std::vector<ColorRGBA> SamplesVector;

private:
  SamplesVector _samples;
  bool _n_samples_forced;
  bool _hide_alpha_forced;

  bool _invalidated;
  gsize _dont_update;
public:
  gsize get_n_samples()const{return _samples.size();}
  void set_n_samples(gsize s);

  void request_no_updates(){_dont_update++;}
  void unrequest_no_updates(){g_assert(_dont_update>0);_dont_update--;}

  /**
   * \note if you try to force the number of samples a second time to another number of samples, an assertion will be thrown
   * */
  ColorCurvePtr force_n_samples(gsize n)
  {
    g_assert(!_n_samples_forced || n==get_n_samples());
    value_curve->force_n_samples(n);
    red_curve->force_n_samples(n);
    green_curve->force_n_samples(n);
    blue_curve->force_n_samples(n);
    alpha_curve->force_n_samples(n);
    set_n_samples(n);
    _n_samples_forced = true;

    reference();
    return ColorCurvePtr(this);
  }

  /**
   * \note if you try to force the number of samples a second time to another number of samples, an assertion will be thrown
   * */
  ColorCurvePtr force_hide_alpha(bool ha)
  {
    g_assert(!_hide_alpha_forced || ha==get_hide_alpha());
    set_hide_alpha(ha);
    _hide_alpha_forced = true;

    reference();
    return ColorCurvePtr(this);
  }

  /**
   *
   * \note the samples are \b not remapped
   * */
  const SamplesVector& get_samples()const{return _samples;}

  void invalidate(){_invalidated=true;}
  void update_samples();

  void invalidate_and_update()
  {
    invalidate();
    update_samples();
  }

  bool is_n_samples_forced()const{return _n_samples_forced;}
  bool is_use_alpha_forced()const{return _hide_alpha_forced;}
///@}

private:
  ColorCurve();
  ~ColorCurve()throw();
};

typedef ColorCurve::ColorCurvePtr ColorCurvePtr;
typedef ColorCurve::ConstColorCurvePtr ConstColorCurvePtr;

 #endif
