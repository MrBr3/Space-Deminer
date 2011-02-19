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

class GradientPreview : public Gtk::DrawingArea
{
  GradientPtr _gradient;

public:
  const GradientPtr& get_gradient()const{return _gradient;}
  void set_gradient(const GradientPtr& g);

  bool on_expose_event(GdkEventExpose* ee);
  void on_size_request(Gtk::Requisition* r);

  GradientPreview();
  ~GradientPreview()throw();
};

class GradientDialog : public Gtk::Dialog
{
  GradientPtr _private_gradient;

  Gtk::Table table_;
  Gtk::ColorButton def_color;
  Gtk::ColorButton color1, color2, color3, color4;
  Gtk::ColorButton curve1, curve2, curve3, curve4;//TODO replace with CurveButton
  GradientPreview preview_;
  Gtk::Frame preview_frame_;
  Gtk::Label label[5];
  Gtk::CheckButton use_alpha_;

public:
  void set_gradient(const GradientPtr& g);
  const GradientPtr& get_gradient()const{return _private_gradient;}

  GradientDialog();
  ~GradientDialog()throw();
};

class GradientButton : public Gtk::Button
{
  GradientPtr _gradient;
  sigc::signal<void> _signal_changed;

  GradientPreview preview_;
  Gtk::Frame frame_;

public:
  void set_gradient(const GradientPtr& g);
  const GradientPtr& get_gradient()const{return _gradient;}

  void on_clicked();

  sigc::signal<void>& signal_changed(){return _signal_changed;}

  GradientButton();
  ~GradientButton()throw();
};
