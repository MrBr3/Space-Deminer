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
public:
  typedef Glib::RefPtr<Curve> CurvePtr;
  typedef Glib::RefPtr<const Curve> ConstCurvePtr;

  enum Present
  {
    PRESENT_FULL,
    PRESENT_LINEAR,
  };

  void flip_h(){std::cout<<"Curve::flip_h\n";}
  void flip_v(){std::cout<<"Curve::flip_v\n";}
  void load_present(Present p){std::cout<<"Curve::load_present("<<p<<")\n";}
  void save_slot(guint i){std::cout<<"Curve::save_slot("<<i<<")\n";}
  void load_slot(guint i){std::cout<<"Curve::load_slot("<<i<<")\n";}

public:
  static CurvePtr create(){return CurvePtr(new Curve);}

private:
  Curve();
  ~Curve()throw();
};


typedef Curve::CurvePtr CurvePtr;
typedef Curve::ConstCurvePtr ConstCurvePtr;

#endif
