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

#include "./CurveButton.hpp"

class ColorCurvePreview : public Gtk::DrawingArea
{
protected:
  ColorCurvePtr _colorcurve;

  sigc::signal<void>::iterator colorcurve_changed_signal_iter;

  void draw_curve(const Cairo::RefPtr<Cairo::Context>& cairo_context, const ConstCurvePtr& curve);

public:
  const ColorCurvePtr& get_colorcurve()const{return _colorcurve;}
  void set_colorcurve(const ColorCurvePtr& g);

  bool on_expose_event(GdkEventExpose* ee);
  void on_size_request(Gtk::Requisition* r);

  ColorCurvePreview();
  ~ColorCurvePreview()throw();
};

class ColorCurveButton : public Gtk::Button
{
  ColorCurvePtr _colorcurve;
  sigc::signal<void> _signal_changed;

  Gtk::Frame _frame;
  ColorCurvePreview _colorcurve_preview;

  Gtk::Menu context_menu;
    Gtk::MenuItem _reset;

  void reset(){g_assert(_colorcurve);_colorcurve->reset();}

  bool on_button_press_event(GdkEventButton* eb);

public:
  typedef Gtk::Button ParentClass;

  void set_colorcurve(const ColorCurvePtr& g);
  const ColorCurvePtr& get_colorcurve()const{return _colorcurve;}

  void on_clicked();

  sigc::signal<void>& signal_changed(){return _signal_changed;}

  ColorCurveButton();
  ~ColorCurveButton()throw();
};
