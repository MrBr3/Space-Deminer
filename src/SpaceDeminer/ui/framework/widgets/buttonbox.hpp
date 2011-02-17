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

#ifndef _SPACEDEMINER_UI_FRAMEWORK_BUTTONBOX_H_
#define _SPACEDEMINER_UI_FRAMEWORK_BUTTONBOX_H_

#include "./box.hpp"

namespace Framework
{
  class ButtonBox : public Box
  {
  public:
    typedef Box ParentClass;

    void set_layout(ButtonBoxStyle layout);
    ButtonBoxStyle get_layout()const throw(){return _layout;}

    ButtonBox(bool h);
    ~ButtonBox()throw();

  protected:

    void on_size_request(int& w, int& h);

    void v_rearrange_children();

    void set_theme(const Glib::RefPtr<Theme>& theme);

  private:
    ButtonBoxStyle _layout;

    int _max_child_width, _max_child_height;
    int _min_max_child_width, _min_max_child_height;
    int _n_visible_children;
  };

  class VButtonBox : public ButtonBox
  {
  public:
    VButtonBox();
    ~VButtonBox()throw();
  };

  class HButtonBox : public ButtonBox
  {
  public:
    HButtonBox();
    ~HButtonBox()throw();
  };
}

#endif
