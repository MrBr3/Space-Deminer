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

ColorCurvePointerHolder::ColorCurvePointerHolder()
{
  set_colorcurve(ColorCurve::create());
}

ColorCurvePointerHolder::~ColorCurvePointerHolder()throw()
{
  _colorcurve->signal_changed().slots().erase(colorcurve_changed_signal_iter);
}

void ColorCurvePointerHolder::set_colorcurve(const ColorCurvePtr& c)
{
  if(c==get_colorcurve())
    return;

  if(_colorcurve)
    _colorcurve->signal_changed().slots().erase(colorcurve_changed_signal_iter);

  _colorcurve = c;
  update.emit();

  colorcurve_changed_signal_iter = _colorcurve->signal_changed().connect(sigc::mem_fun(update, &sigc::signal<void>::emit));
}

// ------------

ColorCurvePreview::ColorCurvePreview()
{
  update.connect(sigc::bind(sigc::ptr_fun(::invalidate), this));
}

ColorCurvePreview::~ColorCurvePreview()throw()
{
}

void ColorCurvePreview::draw_curve(const Cairo::RefPtr<Cairo::Context>& cc, int w, int h, const ConstCurvePtr& curve)
{
  g_assert(curve);
  if(w<=0)
    return;
  if(h<=0)
    return;

  if(curve->get_n_samples()>0)
  {
    gdouble x = 0.;
    gdouble dx = 1./MAX(1., curve->get_n_samples()-1);

    cc->move_to(0., h-curve->get_sample(0)*h);
    for(gsize i=1; i<curve->get_n_samples(); ++i)
    {
      x+=dx;
      cc->line_to(w*x, h-curve->get_sample(i)*h);
    }

    cc->stroke();
  }
}

bool ColorCurvePreview::on_expose_event(GdkEventExpose* ee)
{
  Glib::RefPtr<Gdk::Window> w = get_window();

  if(!w || get_width()<=0)
    return false;

  Cairo::RefPtr<Cairo::Context> cc = w->create_cairo_context();

  if(!cc)
    return false;

  cc->set_line_width(1.);

  if(!get_colorcurve())
    return false;

  int width = get_width();
  int height = get_height();

  cc->set_source_rgba(0.5, 0.5, 0.5, 0.5);
  if(!get_colorcurve()->get_alpha_curve())
    draw_curve(cc, width, height, get_colorcurve()->get_alpha_curve());
  cc->set_source_rgba(0.0, 0.0, 0.5, 0.5);
  draw_curve(cc, width, height, get_colorcurve()->get_blue_curve());
  cc->set_source_rgba(0.0, 0.5, 0.0, 0.5);
  draw_curve(cc, width, height, get_colorcurve()->get_green_curve());
  cc->set_source_rgba(0.5, 0.0, 0.0, 0.5);
  draw_curve(cc, width, height, get_colorcurve()->get_red_curve());
  cc->set_source_rgba(0.0, 0.0, 0.0, 0.5);
  draw_curve(cc, width, height, get_colorcurve()->get_value_curve());

  return true;
}

void ColorCurvePreview::on_size_request(Gtk::Requisition* r)
{
  if(r)
  {
    r->width = 35;
    r->height = 15;
  }
}

// ------------

ColorCurveEditView::ColorCurveEditView()
{
  update.connect(sigc::mem_fun(*this, &ColorCurveEditView::color_curve_changed));
}

ColorCurveEditView::~ColorCurveEditView()throw()
{
}

void ColorCurveEditView::color_curve_changed()
{
  ::invalidate(this);
}

void ColorCurveEditView::draw_back(Cairo::RefPtr<Cairo::Context>& cc)
{
  if(get_curve()!=get_colorcurve()->get_value_curve())
  {
    cc->set_source_rgba(0., 0., 0., 0.33);
    ColorCurvePreview::draw_curve(cc, get_width(), get_height(), get_colorcurve()->get_value_curve());
  }

  if(get_curve()!=get_colorcurve()->get_red_curve())
  {
    cc->set_source_rgba(0.5, 0., 0., 0.33);
    ColorCurvePreview::draw_curve(cc, get_width(), get_height(), get_colorcurve()->get_red_curve());
  }

  if(get_curve()!=get_colorcurve()->get_green_curve())
  {
    cc->set_source_rgba(0., 0.5, 0., 0.33);
    ColorCurvePreview::draw_curve(cc, get_width(), get_height(), get_colorcurve()->get_green_curve());
  }

  if(get_curve()!=get_colorcurve()->get_blue_curve())
  {
    cc->set_source_rgba(0., 0., 0.5, 0.33);
    ColorCurvePreview::draw_curve(cc, get_width(), get_height(), get_colorcurve()->get_blue_curve());
  }

  if(get_curve()!=get_colorcurve()->get_alpha_curve() && !get_colorcurve()->get_hide_alpha())
  {
    cc->set_source_rgba(0.5, 0.5, 0.5, 0.33);
    ColorCurvePreview::draw_curve(cc, get_width(), get_height(), get_colorcurve()->get_alpha_curve());
  }
}

// ------------

ColorCurveDialog::ColorCurveDialog()
{
  update.connect(sigc::mem_fun(*this, &ColorCurveDialog::color_curve_changed));

  set_title(_("Color Curves"));

  g_assert(get_vbox());
  get_vbox()->set_spacing(LENGTH_SMALLSPACE);
  get_vbox()->pack_start(_my_vbox, true, true);
    _my_vbox.set_border_width(LENGTH_SMALLSPACE);
    _my_vbox.set_spacing(LENGTH_SMALLSPACE);
    _my_vbox.pack_start(_channels_hbox, false, false);
      _channels_hbox.set_spacing(LENGTH_SMALLSPACE);
      _channels_hbox.pack_start(value_btn);
        value_btn.set_label("Intensity");
        value_btn.set_active();
        value_btn.signal_toggled().connect(sigc::bind(sigc::mem_fun(*this, &ColorCurveDialog::set_active_channel), 0));
      _channels_hbox.pack_start(red_btn);
        red_btn.set_label("Red");
        red_btn.signal_toggled().connect(sigc::bind(sigc::mem_fun(*this, &ColorCurveDialog::set_active_channel), 1));
      _channels_hbox.pack_start(green_btn);
        green_btn.signal_toggled().connect(sigc::bind(sigc::mem_fun(*this, &ColorCurveDialog::set_active_channel), 2));
        green_btn.set_label("Green");
      _channels_hbox.pack_start(blue_btn);
        blue_btn.signal_toggled().connect(sigc::bind(sigc::mem_fun(*this, &ColorCurveDialog::set_active_channel), 3));
        blue_btn.set_label("Blue");
      _channels_hbox.pack_start(alpha_btn);
        alpha_btn.signal_toggled().connect(sigc::bind(sigc::mem_fun(*this, &ColorCurveDialog::set_active_channel), 4));
        alpha_btn.set_label("Alpha");
    _my_vbox.pack_start(edit_view);

  get_action_area()->pack_start(reset_btn);
    reset_btn.signal_clicked().connect(sigc::mem_fun(*this, &ColorCurveDialog::reset));
    reset_btn.set_label(_("Reset"));

  get_vbox()->show_all_children();

  set_has_separator(true);
  set_default_response(Gtk::RESPONSE_OK);

  add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
  add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
}

ColorCurveDialog::~ColorCurveDialog()throw()
{
}

void ColorCurveDialog::set_active_channel(int c)
{
  static gsize ignore = 0;

  if(!get_colorcurve())
    return;

  if(ignore>0)
    return;
  ++ignore;

  value_btn.set_active(c==0 || (c>4&&!get_colorcurve()->get_hide_alpha()) || (c>3&&get_colorcurve()->get_hide_alpha()));
  red_btn.set_active(c==1);
  green_btn.set_active(c==2);
  blue_btn.set_active(c==3);
  alpha_btn.set_active(c==4 && !get_colorcurve()->get_hide_alpha());

  switch(c)
  {
  case 0:
    edit_view.curve_color.set(0.f, 0.f, 0.f);
    edit_view.set_curve(get_colorcurve()->get_value_curve());
    break;
  case 1:
    edit_view.curve_color.set(0.5f, 0.f, 0.f);
    edit_view.set_curve(get_colorcurve()->get_red_curve());
    break;
  case 2:
    edit_view.curve_color.set(0.f, 0.5f, 0.f);
    edit_view.set_curve(get_colorcurve()->get_green_curve());
    break;
  case 3:
    edit_view.curve_color.set(0.f, 0.f, 05.f);
    edit_view.set_curve(get_colorcurve()->get_blue_curve());
    break;
  case 4:
    edit_view.curve_color.set(0.5f, 0.5f, 0.5f);
    edit_view.set_curve(get_colorcurve()->get_alpha_curve());
    break;
  }
  --ignore;
}

void ColorCurveDialog::color_curve_changed()
{
  alpha_btn.set_visible(!get_colorcurve()->get_hide_alpha());

  if(get_colorcurve()==edit_view.get_colorcurve())
    return;
  edit_view.set_colorcurve(get_colorcurve());
  set_active_channel(0);
}

// ------------

ColorCurveButton::ColorCurveButton()
{
  _colorcurve = ColorCurve::create();

  _colorcurve_preview.show();
  _colorcurve_preview.set_colorcurve(_colorcurve);
  _frame.add(_colorcurve_preview);

  _frame.show();
  _frame.unset_label();
  _frame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);

  add(_frame);

  context_menu.append(_reset);
    _reset.set_label(_("Set _Linear"));
    _reset.set_use_underline(true);
    _reset.signal_activate().connect(sigc::mem_fun(*this, &ColorCurveButton::reset));

  context_menu.show_all_children();
  context_menu.show();
}

ColorCurveButton::~ColorCurveButton()throw()
{
}

bool ColorCurveButton::on_button_press_event(GdkEventButton* eb)
{
  g_assert(eb);
  if(eb->button==3)
  {
    context_menu.popup(eb->button, eb->time);
    return true;
  }

  return ParentClass::on_button_press_event(eb);
}

void ColorCurveButton::set_colorcurve(const ColorCurvePtr& c)
{
  _colorcurve = c;

  _colorcurve_preview.set_colorcurve(_colorcurve);
  signal_changed().emit();
  ::invalidate(this);
}

void ColorCurveButton::on_clicked()
{
  ColorCurveDialog dlg;
  ColorCurvePtr tmp_curve = dlg.get_colorcurve();

  dlg.set_colorcurve(get_colorcurve());

  tmp_curve->set(get_colorcurve());
  dlg.prev_curve = tmp_curve;

  if(dlg.run()!=Gtk::RESPONSE_OK)
    _colorcurve->set(tmp_curve);
}
// ------------
