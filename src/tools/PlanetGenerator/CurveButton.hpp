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

#ifndef _PLANET_GENERATOR_CURVE_BUTTON_HPP_
#define _PLANET_GENERATOR_CURVE_BUTTON_HPP_

#include <gtkmm/frame.h>

class CurvePreview : public Gtk::DrawingArea
{
protected:
  CurvePtr _curve;

  sigc::signal<void>::iterator curve_changed_signal_iter;

public:
  const CurvePtr& get_curve()const{return _curve;}
  void set_curve(const CurvePtr& g);

  bool on_expose_event(GdkEventExpose* ee);
  void on_size_request(Gtk::Requisition* r);

  ColorRGBA curve_color;

  CurvePreview();
  ~CurvePreview()throw();
};

class CurveEditView : public CurvePreview
{
  gsize focused_point;
  gdouble removed_point_last_x;

  enum State
  {
    STATE_NONE,
    STATE_CREATING,
    STATE_POINTING,
    STATE_MOVING,
    STATE_MOVING_REMOVING,
  }state;

  Gdk::Cursor cursor_creating, cursor_moving;

  void set_state(State state);

  void set_state_pointing_or_creating(int x, int y);

  void delete_or_move_point(gsize i_prev, gsize i_next, gdouble old_x, int widget_space_new_x, gdouble new_x, bool& allow_to_move, bool& should_remove);

  virtual void draw_back(Cairo::RefPtr<Cairo::Context>& cc){}

public:
  bool on_expose_event(GdkEventExpose* ee);
  void on_size_request(Gtk::Requisition* r);

  bool on_enter_notify_event(GdkEventCrossing* eb);
  bool on_leave_notify_event(GdkEventCrossing* eb);
  bool on_button_press_event(GdkEventButton* eb);
  bool on_button_release_event(GdkEventButton* eb);
  bool on_motion_notify_event(GdkEventMotion* eb);

  CurveEditView();
  ~CurveEditView()throw();
};

class CurveDialog : public Gtk::Dialog
{
  CurvePtr _curve;

  Gtk::VBox vbox;
  Gtk::Frame _frame;
  CurveEditView view;
  Gtk::CheckButton cbtn;

  void set_linear();

public:
  void set_curve(const CurvePtr& c);
  const CurvePtr& get_curve()const{return _curve;}

  CurveDialog();
};

class CurveButton : public Gtk::Button
{
  CurvePtr _curve;
  sigc::signal<void> _signal_changed;

  Gtk::Frame _frame;
  CurvePreview _curve_preview;

  Gtk::Menu context_menu;
    Gtk::MenuItem _flip_h;
    Gtk::MenuItem _flip_v;
    Gtk::SeparatorMenuItem sep;
    Gtk::MenuItem _load_present;
      Gtk::Menu load_present_menu;
      Gtk::MenuItem load_present_full;
      Gtk::MenuItem load_present_empty;
      Gtk::MenuItem load_present_linear;
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

  void flip_h(){g_assert(_curve);_curve->flip_h();}
  void flip_v(){g_assert(_curve);_curve->flip_v();}
  void load_present(Curve::Present p){g_assert(_curve);_curve->load_present(p);}
  void load_slot(guint i){g_assert(_curve);_curve->load_slot(i);}
  void save_slot(guint i){g_assert(_curve);_curve->save_slot(i);}

  bool on_button_press_event(GdkEventButton* eb);

public:
  typedef Gtk::Button ParentClass;

  void set_curve(const CurvePtr& g);
  const CurvePtr& get_curve()const{return _curve;}

  void on_clicked();

  sigc::signal<void>& signal_changed(){return _signal_changed;}

  CurveButton();
  ~CurveButton()throw();
};

#endif
