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

class Gradient : public Refable
{
public:
  typedef Glib::RefPtr<Gradient> GradientPtr;
  typedef Glib::RefPtr<const Gradient> ConstGradientPtr;

  bool operator==(const Gradient& g)const{return true;/*TODO implement*/}
  bool operator!=(const Gradient& g)const
  {
    return !(*this==g);
  }

  void require_puffer(gsize s){/*TODO*/}
  void clear_puffer(){/*TODO*/}

  void set(GradientPtr g){/*TODO*/}

  static GradientPtr create_black2white(){return GradientPtr(new Gradient());}

private:
  Gradient();
  ~Gradient()throw();
};

typedef Gradient::GradientPtr GradientPtr;
typedef Gradient::ConstGradientPtr ConstGradientPtr;
#endif
