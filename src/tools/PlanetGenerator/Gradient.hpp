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

 #include "./Curve.hpp"

class Gradient : public Refable
{
  sigc::signal<void> _signal_changed;
public:
  typedef Glib::RefPtr<Gradient> GradientPtr;
  typedef Glib::RefPtr<const Gradient> ConstGradientPtr;

  enum Present
  {
    PRESENT_BLACK_2_WHITE,
    PRESENT_TRANSPARENT_2_WHITE,
  };

  bool operator==(const Gradient& g)const{return true;/*TODO implement*/}
  bool operator!=(const Gradient& g)const
  {
    return !(*this==g);
  }

  void flip_h(){std::cout<<"Gradient::flip_h\n";}
  void load_present(Present p){std::cout<<"Gradient::load_present("<<p<<")\n";}
  void save_slot(guint i){std::cout<<"Gradient::save_slot("<<i<<")\n";}
  void load_slot(guint i){std::cout<<"Gradient::load_slot("<<i<<")\n";}

  void set(GradientPtr g){std::cout<<"Gradient::set\n";}

  static GradientPtr create_black2white(){return GradientPtr(new Gradient());}

  sigc::signal<void>& signal_changed(){return _signal_changed;}

///@{
///@name Puffer Stuff
private:
  ColorRGBA* _puffer;
  gsize _puffer_size;
  gsize _n_gradients_needed;
  Cairo::RefPtr<Cairo::Gradient> _cairo_gradient;

  void invalidate_cairo_gradient();

public:
  gsize get_puffer_size()const{return _puffer_size;}
  void require_puffer_size(gsize s);
  void clear_puffer();

  const ColorRGBA* get_puffer()const{return _puffer;}
  const Cairo::RefPtr<Cairo::Gradient>& get_cairo_gradient()const{return _cairo_gradient;}

  void require_cairo_gradient(gsize s);
  void clear_cairo_gradient();

  void invalidate();
///@}

private:
  Gradient();
  ~Gradient()throw();
};

typedef Gradient::GradientPtr GradientPtr;
typedef Gradient::ConstGradientPtr ConstGradientPtr;
#endif
