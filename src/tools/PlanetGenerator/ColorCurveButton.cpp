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

ColorCurvePreview::ColorCurvePreview()
{
  set_colorcurve(ColorCurve::create());
}

ColorCurvePreview::~ColorCurvePreview()throw()
{
  _colorcurve->signal_changed().slots().erase(colorcurve_changed_signal_iter);
}

void ColorCurvePreview::set_colorcurve(const ColorCurvePtr& c)
{
  if(_colorcurve)
    _colorcurve->signal_changed().slots().erase(colorcurve_changed_signal_iter);

  _colorcurve = c;
  invalidate(this);

  colorcurve_changed_signal_iter = _colorcurve->signal_changed().connect(sigc::bind(sigc::ptr_fun(::invalidate), this));
}

void ColorCurvePreview::draw_curve(const Cairo::RefPtr<Cairo::Context>& cc, const ConstCurvePtr& curve)
{
  g_assert(curve);

  if(curve->get_n_samples()>0)
  {
    gdouble w = get_width();
    gdouble h = get_height();

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

  cc->set_source_rgba(0.5, 0.5, 0.5, 0.5);
  if(!get_colorcurve()->get_alpha_curve())
    draw_curve(cc, get_colorcurve()->get_alpha_curve());
  cc->set_source_rgb(0., 0., 0.5);
  draw_curve(cc, get_colorcurve()->get_blue_curve());
  cc->set_source_rgb(0., 0.5, 0.);
  draw_curve(cc, get_colorcurve()->get_green_curve());
  cc->set_source_rgb(0.5, 0., 0.);
  draw_curve(cc, get_colorcurve()->get_red_curve());
  cc->set_source_rgb(0., 0., 0.);
  draw_curve(cc, get_colorcurve()->get_value_curve());

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
    _reset.set_label(_("_Reset"));
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
  /*ColorCurveDialog dlg;
  ColorCurvePtr tmp_curve = dlg.get_colorcurve();

  dlg.set_colorcurve(get_curve());

  tmp_curve->set(get_colorcurve());

  if(dlg.run()==Gtk::RESPONSE_CANCEL)
    _colorcurve->set(tmp_curve);*/
}
// ------------
