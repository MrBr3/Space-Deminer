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

 #ifndef TOOLS_MODEL_GRADIENT_HPP_
 #define TOOLS_MODEL_GRADIENT_HPP_

#include <base.hpp>
 #include "./Curve.hpp"

class Gradient : public Refable
{
  bool operator==(const Gradient& g)const;
  bool operator!=(const Gradient& g)const;
  Gradient(const Gradient&);

  sigc::signal<void> _signal_changed;
public:
  typedef Glib::RefPtr<Gradient> GradientPtr;
  typedef Glib::RefPtr<const Gradient> ConstGradientPtr;

  enum Present
  {
    PRESENT_BLACK_2_WHITE,
    PRESENT_TRANSPARENT_2_WHITE,
    PRESENT_FULL_WHITE,
  };

  void flip_h();
  void load_present(Present p);
  void save_slot(guint i);
  void load_slot(guint i);

  void set(const Gradient& g);
  void set(GradientPtr g);

  static GradientPtr create(Present p=PRESENT_BLACK_2_WHITE){return GradientPtr(new Gradient(p));}

  sigc::signal<void>& signal_changed(){return _signal_changed;}
  sigc::signal<void>& signal_changed()const{return const_cast<sigc::signal<void>& >(_signal_changed);}

  static void init_slots();

  Glib::ustring save_to_string()const;
  void load_from_string(Glib::ustring::const_iterator& begin, Glib::ustring::const_iterator end);

private:
  static GradientPtr slot[4];
  static void slot_changed(gsize i);

///@{
///@name Components
private:
  CurvePtr curve1, curve2, curve3, curve4;
  ColorRGBA defcolor, color1, color2, color3, color4;
  bool _use_alpha;
  gdouble remap_a, remap_b;

public:
  const CurvePtr& get_curve1(){return curve1;}
  ConstCurvePtr get_curve1()const{return curve1;}
  const CurvePtr& get_curve2(){return curve2;}
  ConstCurvePtr get_curve2()const{return curve2;}
  const CurvePtr& get_curve3(){return curve3;}
  ConstCurvePtr get_curve3()const{return curve3;}
  const CurvePtr& get_curve4(){return curve4;}
  ConstCurvePtr get_curve4()const{return curve4;}

  const ColorRGBA& get_defcolor()const{return defcolor;}
  const ColorRGBA& get_color1()const{return color1;}
  const ColorRGBA& get_color2()const{return color2;}
  const ColorRGBA& get_color3()const{return color3;}
  const ColorRGBA& get_color4()const{return color4;}
  void set_defcolor(const ColorRGBA& defcolor);
  void set_color1(const ColorRGBA& color1);
  void set_color2(const ColorRGBA& color2);
  void set_color3(const ColorRGBA& color3);
  void set_color4(const ColorRGBA& color4);

  void set_use_alpha(bool a);
  bool get_use_alpha()const{return _use_alpha;}

  void set_remap_a(gdouble a);
  void set_remap_b(gdouble b);
  gdouble get_remap_a()const{return remap_a;}
  gdouble get_remap_b()const{return remap_b;}

  gdouble get_remapped_offset(gdouble o)const{return remap_a+o*(remap_b-remap_a);}
///@}

///@{
///@name Samples Stuff
public:
  typedef std::vector<ColorRGBA> SamplesVector;

private:
  SamplesVector _samples;
  gsize _n_gradients_needed;
  Cairo::RefPtr<Cairo::Gradient> _cairo_gradient;
  bool _n_samples_forced;
  bool _use_alpha_forced;

  void update_cairo_gradient();

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
  GradientPtr force_n_samples(gsize n)
  {
    g_assert(!_n_samples_forced || n==get_n_samples());
    curve1->force_n_samples(n);
    curve2->force_n_samples(n);
    curve3->force_n_samples(n);
    curve4->force_n_samples(n);
    set_n_samples(n);
    _n_samples_forced = true;

    reference();
    return GradientPtr(this);
  }

  /**
   * \note if you try to force the number of samples a second time to another number of samples, an assertion will be thrown
   * */
  GradientPtr force_use_alpha(bool ua)
  {
    g_assert(!_use_alpha_forced || ua==get_use_alpha());
    set_use_alpha(ua);
    _use_alpha_forced = true;

    reference();
    return GradientPtr(this);
  }

  /**
   *
   * \note the samples are \b not remapped
   * */
  const SamplesVector& get_samples()const{return _samples;}
  /**
   *
   * \note the gradient is already remapped
   * */
  Cairo::RefPtr<const Cairo::Gradient> get_cairo_gradient()const{return _cairo_gradient;}

  void reference_cairo_gradient();
  void unreference_cairo_gradient();

  void invalidate(){_invalidated=true;}
  void update_samples();

  void invalidate_and_update()
  {
    invalidate();
    update_samples();
  }
  void invalidate_and_update_just_gradient()
  {
    if(_invalidated)
      update_samples();
    else
    {
      update_cairo_gradient();
      signal_changed().emit();
    }
  }

  bool is_n_samples_forced()const{return _n_samples_forced;}
  bool is_use_alpha_forced()const{return _use_alpha_forced;}
///@}

private:
  Gradient(Present p);
  ~Gradient()throw();
};

typedef Gradient::GradientPtr GradientPtr;
typedef Gradient::ConstGradientPtr ConstGradientPtr;
#endif
