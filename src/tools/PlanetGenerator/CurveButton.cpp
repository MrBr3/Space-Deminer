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

#include "./MainWindow.hpp"

CurvePreview::CurvePreview()
{
  _curve = Curve::create();
}

CurvePreview::~CurvePreview()throw()
{
}

void CurvePreview::set_curve(const CurvePtr& g)
{
  _curve = g;
  invalidate(this);

  //TODO when the Gradient has changed the Widget should be redrawn
}

bool CurvePreview::on_expose_event(GdkEventExpose* ee)
{
  Glib::RefPtr<Gdk::Window> w = get_window();

  if(!w || get_width()<=0)
    return false;

  Cairo::RefPtr<Cairo::Context> cc = w->create_cairo_context();

  if(!cc)
    return false;

  if(get_curve()->get_n_samples()>0)
  {
    gdouble w = get_width();
    gdouble h = get_height();

    gdouble x = 0.;
    gdouble dx = 1./MAX(1., get_curve()->get_n_samples()-1);

    cc->move_to(0., h-get_curve()->get_sample(0)*h);
    for(gsize i=1; i<get_curve()->get_n_samples(); ++i)
    {
      x+=dx;
      cc->line_to(w*x, h-get_curve()->get_sample(i)*h);
    }

    cc->stroke();
  }
  return true;
}

void CurvePreview::on_size_request(Gtk::Requisition* r)
{
  if(r)
  {
    r->width = 35;
    r->height = 15;
  }
}

// ------------

CurveEditView::CurveEditView() : cursor_creating(Gdk::CROSSHAIR), cursor_moving(Gdk::HAND1)
{
  focused_point = G_MAXSIZE;

  state = STATE_NONE;

  add_events(Gdk::BUTTON_PRESS_MASK|Gdk::BUTTON_RELEASE_MASK|Gdk::POINTER_MOTION_MASK|Gdk::LEAVE_NOTIFY_MASK|Gdk::ENTER_NOTIFY_MASK);
}

CurveEditView::~CurveEditView()throw()
{
}

bool CurveEditView::on_expose_event(GdkEventExpose* ee)
{
  Glib::RefPtr<Gdk::Window> win = get_window();

  if(!win)
    return false;

  Cairo::RefPtr<Cairo::Context> cc = win->create_cairo_context();

  if(!cc)
    return false;

  gdouble w = get_width();
  gdouble h = get_height();

  cc->set_source_rgba(0., 0., 0., 0.25); // TODO: use TextColor

  cc->rectangle(0.5, 0.5, w-1., h-1.);
  cc->move_to(1.5,   0.50*h);
  cc->line_to(w-1.5, 0.50*h);

  cc->move_to(0.5*w,  1.5);
  cc->line_to(0.5*w,  h-1.5);

  cc->stroke();

  cc->set_source_rgba(0., 0., 0., 0.125); // TODO: use TextColor

  cc->move_to(1.5,   0.25*h);
  cc->line_to(w-1.5, 0.25*h);
  cc->move_to(1.5,   0.75*h);
  cc->line_to(w-1.5, 0.75*h);

  cc->move_to(0.75*w, 1.5);
  cc->line_to(0.75*w, h-1.5);
  cc->move_to(0.25*w, 1.5);
  cc->line_to(0.25*w, h-1.5);

  cc->stroke();

  cc->set_source_rgb(0., 0., 0.);

  for(gsize i=0; i<get_curve()->get_n_points(); ++i)
  {
    const Curve::Point& p = get_curve()->get_point(i);

    if(i==focused_point && (state==STATE_POINTING||state==STATE_MOVING))
    {
      cc->set_source_rgb(1.0, 0.5, 0.); // TODO: use Highlight Color
    }else
      cc->set_source_rgb(0., 0., 0.); // TODO: use TextColor

    cc->arc(p.x*w, h-p.y*h, 4., 0., PI2);
    cc->fill();
  }

  return CurvePreview::on_expose_event(ee);
}

void CurveEditView::on_size_request(Gtk::Requisition* r)
{
  if(r)
  {
    r->width = 256;
    r->height = 256;
  }
}

void CurveEditView::set_state(State state)
{
  Glib::RefPtr<Gdk::Window> win = get_window();

  this->state = state;

  if(!win)
    return;

  switch(state)
  {
  case STATE_CREATING:
    win->set_cursor(cursor_creating);
    break;
  case STATE_MOVING:
  case STATE_POINTING:
    win->set_cursor(cursor_moving);
    break;
  }
}

bool CurveEditView::on_enter_notify_event(GdkEventCrossing* eb)
{
  switch(state)
  {
  case STATE_MOVING:
    break;
  case STATE_NONE:
  default:
    set_state(STATE_CREATING);
  }

  return true;
}

bool CurveEditView::on_leave_notify_event(GdkEventCrossing* eb)
{
  switch(state)
  {
  case STATE_POINTING:
  case STATE_CREATING:
    state = STATE_NONE;
  default:
    break;
  }
  return true;
}

bool CurveEditView::on_button_press_event(GdkEventButton* eb)
{
  if(!get_width()||!get_height())
    return false;

  gdouble inv_w = 1./get_width();
  gdouble inv_h = 1./get_height();

  switch(state)
  {
  case STATE_CREATING:
    if(eb->button==1)
    {
      focused_point = get_curve()->add_point(eb->x*inv_w, 1.-eb->y*inv_h);

      set_state(STATE_POINTING);
      invalidate(this);
    }
    break;
  case STATE_POINTING:
    if(eb->button==1)
    {
      set_state(STATE_MOVING);
    }
    break;
  }

  return true;
}

bool CurveEditView::on_button_release_event(GdkEventButton* eb)
{
  if(!get_width()||!get_height())
    return false;

  gdouble inv_w = 1./get_width();
  gdouble inv_h = 1./get_height();

  switch(state)
  {
  case STATE_MOVING:
    if(eb->button==1)
    {
      set_state_pointing_or_creating(eb->x, eb->y);
      invalidate(this);
    }
    break;
  }
  return true;
}

bool CurveEditView::on_motion_notify_event(GdkEventMotion* eb)
{
  if(!get_width()||!get_height())
    return false;

  gdouble inv_w = 1./get_width();
  gdouble inv_h = 1./get_height();

  switch(state)
  {
  case STATE_CREATING:
  case STATE_POINTING:
  {
    gsize old_focused_point = focused_point;
    set_state_pointing_or_creating(eb->x, eb->y);

    if(focused_point!=old_focused_point || state==STATE_CREATING)
      invalidate(this);
    break;
  }
  case STATE_MOVING:
    if(!get_curve()->move_point(focused_point, eb->x*inv_w, 1.-eb->y*inv_h))
      set_state_pointing_or_creating(eb->x, eb->y);
    invalidate(this);
    break;
  }

  return true;
}

void CurveEditView::set_state_pointing_or_creating(int x, int y)
{
  if(!get_width()||!get_height())
    return;

  gdouble inv_w = 1./get_width();
  gdouble inv_h = 1./get_height();

  focused_point = get_curve()->find_point(x*inv_w, 1.-y*inv_h, 8.*.5*(inv_w+inv_h));
  if(focused_point>=get_curve()->get_n_points())
    set_state(STATE_CREATING);
  else
    set_state(STATE_POINTING);
}

// ------------

CurveDialog::CurveDialog()
{
  _curve = Curve::create();

  set_title(_("EditCurve"));

  Gtk::VBox& dlg_vbox = *get_vbox();
  g_assert(&dlg_vbox);

  vbox.set_border_width(LENGTH_SMALLSPACE);
  vbox.set_spacing(LENGTH_SMALLSPACE);
  vbox.pack_start(_frame);
  vbox.pack_start(cbtn, false, false);

  _frame.add(view);

  cbtn.set_label(_("_Cubic-Spline-Interpolation"));
  cbtn.set_use_underline(true);

  dlg_vbox.pack_start(vbox);
  dlg_vbox.show_all_children();
  dlg_vbox.set_spacing(LENGTH_SMALLSPACE);

  set_has_separator(true);
  set_default_response(Gtk::RESPONSE_OK);

  add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
  add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
}

void CurveDialog::set_curve(const CurvePtr& c)
{
  _curve->set(c);
}

// ------------

CurveButton::CurveButton()
{
  _curve = Curve::create();

  _curve_preview.show();
  _curve_preview.set_curve(_curve);
  _frame.add(_curve_preview);

  _frame.show();
  _frame.unset_label();
  _frame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);

  add(_frame);

  context_menu.append(_flip_h);
    _flip_h.set_label(_("Flip _Horizontal"));
    _flip_h.set_use_underline(true);
    _flip_h.signal_activate().connect(sigc::mem_fun(*this, &CurveButton::flip_h));
  context_menu.append(_flip_v);
    _flip_v.set_label(_("Flip _Vertical"));
    _flip_v.set_use_underline(true);
    _flip_v.signal_activate().connect(sigc::mem_fun(*this, &CurveButton::flip_v));
  context_menu.append(sep);
  context_menu.append(_load_present);
    _load_present.set_label(_("Load _Present"));
    _load_present.set_use_underline(true);
    _load_present.set_submenu(load_present_menu);
    load_present_menu.append(load_present_full);
      load_present_full.set_label("100%");
      load_present_full.set_use_underline(true);
      load_present_full.signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &CurveButton::load_present), Curve::PRESENT_FULL));
    load_present_menu.append(load_present_linear);
      load_present_linear.set_label("0% -> 100% Linear");
      load_present_linear.set_use_underline(true);
      load_present_linear.signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &CurveButton::load_present), Curve::PRESENT_LINEAR));
  context_menu.append(_load_slot);
    _load_slot.set_label(_("_Load From Slot"));
    _load_slot.set_use_underline(true);
    _load_slot.set_submenu(load_slot_menu);
    load_slot_menu.append(load_slot0);
      load_slot0.set_label("Slot_0");
      load_slot0.set_use_underline(true);
      load_slot0.signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &CurveButton::load_slot), 0));
    load_slot_menu.append(load_slot1);
      load_slot1.set_label("Slot_1");
      load_slot1.set_use_underline(true);
      load_slot1.signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &CurveButton::load_slot), 1));
    load_slot_menu.append(load_slot2);
      load_slot2.set_label("Slot_2");
      load_slot2.set_use_underline(true);
      load_slot2.signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &CurveButton::load_slot), 2));
    load_slot_menu.append(load_slot3);
      load_slot3.set_label("Slot_3");
      load_slot3.set_use_underline(true);
      load_slot3.signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &CurveButton::load_slot), 3));
  context_menu.append(_save_slot);
    _save_slot.set_label(_("_Save to Slot"));
    _save_slot.set_use_underline(true);
    _save_slot.set_submenu(save_slot_menu);
    save_slot_menu.append(save_slot0);
      save_slot0.set_label("Slot_0");
      save_slot0.set_use_underline(true);
      save_slot0.signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &CurveButton::save_slot), 0));
    save_slot_menu.append(save_slot1);
      save_slot1.set_label("Slot_1");
      save_slot1.set_use_underline(true);
      save_slot1.signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &CurveButton::save_slot), 1));
    save_slot_menu.append(save_slot2);
      save_slot2.set_label("Slot_2");
      save_slot2.set_use_underline(true);
      save_slot2.signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &CurveButton::save_slot), 2));
    save_slot_menu.append(save_slot3);
      save_slot3.set_label("Slot_3");
      save_slot3.set_use_underline(true);
      save_slot3.signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &CurveButton::save_slot), 3));

  context_menu.show_all_children();
  context_menu.show();
}

CurveButton::~CurveButton()throw()
{
}

bool CurveButton::on_button_press_event(GdkEventButton* eb)
{
  g_assert(eb);
  if(eb->button==3)
  {
    context_menu.popup(eb->button, eb->time);
    return true;
  }

  return ParentClass::on_button_press_event(eb);
}

void CurveButton::set_curve(const CurvePtr& c)
{
  _curve = c;

  _curve_preview.set_curve(_curve);
  signal_changed().emit();
}

void CurveButton::on_clicked()
{
  CurveDialog dlg;

  dlg.set_curve(get_curve());

  if(dlg.run()==Gtk::RESPONSE_OK)
    _curve->set(dlg.get_curve());
}
