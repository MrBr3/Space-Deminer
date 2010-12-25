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

#ifndef _SPACEDEMINER_UI_FRAMEWORK_BUTTON_H_
#define _SPACEDEMINER_UI_FRAMEWORK_BUTTON_H_

#include "./bin.hpp"

namespace Framework
{
  class Button : public Bin
  {
    typedef Bin ParentClass;
  public:

    enum DrawParam
    {
      DRAW_PARAM_RELIEF_NORMAL,
      DRAW_PARAM_RELIEF_PUSHED,
      DRAW_PARAM_RELIEF_MOUSEOVER,
    };

    void set_relief(ReliefType rt);
    ReliefType get_relief()const throw(){return _relief_type;}

    void set_theme(const Glib::RefPtr<Theme>& theme);

    void set_image_widget(Framework::Widget& w){add(w);}

    Button();
    Button(Glib::ustring& label, bool use_memnotic=true);
    ~Button()throw();

          sigc::signal<void> signal_clicked()      throw(){return _signal_clicked;}
    const sigc::signal<void> signal_clicked()const throw(){return _signal_clicked;}

    void on_size_request(int& w, int& h);

    void on_expose(EventExpose& paint_tool);
    void v_rearrange_children();

    bool on_mouse_move(MouseEvent& mouse_event);
    bool on_mouse_enter(MouseEvent& mouse_event);
    bool on_mouse_leave(MouseEvent& mouse_event);
    bool on_button_press(MouseButtonEvent& mouse_event);
    bool on_button_release(MouseButtonEvent& mouse_event);

  private:
    ReliefType _relief_type;
    DrawParam _state;

    sigc::signal<void> _signal_clicked;

  protected:
    Theme::Metrics child_dist;
  };

  class ToggleButton : public Button
  {
  public:
    void set_theme(const Glib::RefPtr<Theme>& theme);

    ToggleButton();
  };
}

#endif