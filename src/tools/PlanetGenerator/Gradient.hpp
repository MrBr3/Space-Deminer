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
  };

  void flip_h(){std::cout<<"Gradient::flip_h\n";}
  void load_present(Present p){std::cout<<"Gradient::load_present("<<p<<")\n";}
  void save_slot(guint i){std::cout<<"Gradient::save_slot("<<i<<")\n";}
  void load_slot(guint i){std::cout<<"Gradient::load_slot("<<i<<")\n";}

  void set(GradientPtr g){std::cout<<"Gradient::set\n";}

  static GradientPtr create(){return GradientPtr(new Gradient());}

  sigc::signal<void>& signal_changed(){return _signal_changed;}

///@{
///@name Components
private:
  CurvePtr curve1, curve2, curve3, curve4;
  ColorRGBA defcolor, color1, color2, color3, color4;
  bool _use_alpha;

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
///@}

///@{
///@name Samples Stuff
public:
  typedef std::vector<ColorRGBA> SamplesVector;

private:
  SamplesVector _samples;
  gsize _n_gradients_needed;
  Cairo::RefPtr<Cairo::Gradient> _cairo_gradient;

  void update_cairo_gradient();

  bool _invalidated;
  gsize _dont_update;

  void request_no_updates(){_dont_update++;}
  void unrequest_no_updates(){g_assert(_dont_update>0);_dont_update--;}

public:
  gsize get_n_samples()const{return _samples.size();}
  void set_n_samples(gsize s);

  const SamplesVector& get_samples()const{return _samples;}
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
///@}

private:
  Gradient();
  ~Gradient()throw();
};

typedef Gradient::GradientPtr GradientPtr;
typedef Gradient::ConstGradientPtr ConstGradientPtr;
#endif
