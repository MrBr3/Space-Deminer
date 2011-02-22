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

 #include <gtkmm/frame.h>

 #include "./CurveButton.hpp"

class GradientPreview : public Gtk::DrawingArea
{
  GradientPtr _gradient;

  sigc::signal<void>::iterator gradient_changed_signal_iter;

public:
  ConstGradientPtr get_gradient()const{return _gradient;}
  const GradientPtr& get_gradient(){return _gradient;}
  void set_gradient(const GradientPtr& g);

  bool on_expose_event(GdkEventExpose* ee);
  void on_size_request(Gtk::Requisition* r);

  GradientPreview();
  ~GradientPreview()throw();
};

class GradientDialog : public Gtk::Dialog
{
  GradientPtr _private_gradient;

  Gtk::Table table_, table2_;
  Gtk::ColorButton def_color;
  Gtk::ColorButton color1, color2, color3, color4;
  CurveButton curve1, curve2, curve3, curve4;
  GradientPreview preview_;
  Gtk::Frame preview_frame_;
  Gtk::Label label[7];
  Gtk::SpinButton remap_a, remap_b, n_samples;
  Gtk::CheckButton use_alpha_;

public:
  void set_gradient(const GradientPtr& g);
  ConstGradientPtr get_gradient()const{return _private_gradient;}
  const GradientPtr& get_gradient(){return _private_gradient;}

  GradientDialog();
  ~GradientDialog()throw();
};

class GradientButton : public Gtk::Button
{
  GradientPtr _gradient;
  sigc::signal<void> _signal_changed;

  GradientPreview preview_;
  Gtk::Frame frame_;

  Gtk::Menu context_menu;
    Gtk::MenuItem _flip_h;
    Gtk::SeparatorMenuItem sep;
    Gtk::MenuItem _load_present;
      Gtk::Menu load_present_menu;
      Gtk::MenuItem load_present_black2white;
      Gtk::MenuItem load_present_transparent2white;
    Gtk::MenuItem _load_slot;
      Gtk::Menu load_slot_menu;
      Gtk::MenuItem load_slot0;
      Gtk::MenuItem load_slot1;
      Gtk::MenuItem load_slot2;
      Gtk::MenuItem load_slot3;
    Gtk::MenuItem _save_slot;
      Gtk::Menu save_slot_menu;
      Gtk::MenuItem save_slot0;
      Gtk::MenuItem save_slot1;
      Gtk::MenuItem save_slot2;
      Gtk::MenuItem save_slot3;

  void flip_h(){g_assert(_gradient);_gradient->flip_h();}
  void load_present(Gradient::Present p){g_assert(_gradient);_gradient->load_present(p);}
  void load_slot(guint i){g_assert(_gradient);_gradient->load_slot(i);}
  void save_slot(guint i){g_assert(_gradient);_gradient->save_slot(i);}

  bool on_button_press_event(GdkEventButton* eb);

public:
  typedef Gtk::Button ParentClass;

  void set_gradient(const GradientPtr& g);
  GradientPtr& get_gradient(){return _gradient;}
  ConstGradientPtr get_gradient()const{return _gradient;}

  void on_clicked();

  sigc::signal<void>& signal_changed(){return _signal_changed;}

  GradientButton();
  ~GradientButton()throw();
};
