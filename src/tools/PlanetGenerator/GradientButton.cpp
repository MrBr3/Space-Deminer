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

GradientPreview::GradientPreview()
{
  _gradient = Gradient::create_black2white();
}

GradientPreview::~GradientPreview()throw()
{
}

void GradientPreview::set_gradient(const GradientPtr& g)
{
  _gradient = g;
  _gradient->require_puffer(256);
  invalidate(this);

  //TODO when the Gradient has changed the Widget should be redrawn
}

bool GradientPreview::on_expose_event(GdkEventExpose* ee)
{
  return true;
}

void GradientPreview::on_size_request(Gtk::Requisition* r)
{
  if(r)
  {
    r->width = 35;
    r->height = 15;
  }
}

// ------------

GradientDialog::GradientDialog()
{
  _private_gradient = Gradient::create_black2white();

  set_default_size(480, 320);

  g_assert(get_vbox());
  get_vbox()->pack_start(table_);
    table_.show();
    table_.set_spacings(LENGTH_SMALLSPACE);
    table_.set_border_width(LENGTH_SMALLSPACE);
    table_.attach(label[0], 0, 1, 0, 1, Gtk::SHRINK, Gtk::SHRINK);
      label[0].show();
      label[0].set_label(_("DefaultColor"));
    table_.attach(label[1], 0, 1, 1, 2, Gtk::SHRINK, Gtk::SHRINK);
      label[1].show();
      label[1].set_label(_("Color1"));
    table_.attach(label[2], 0, 1, 2, 3, Gtk::SHRINK, Gtk::SHRINK);
      label[2].show();
      label[2].set_label(_("Color2"));
    table_.attach(label[3], 0, 1, 3, 4, Gtk::SHRINK, Gtk::SHRINK);
      label[3].show();
      label[3].set_label(_("Color3"));
    table_.attach(label[4], 0, 1, 4, 5, Gtk::SHRINK, Gtk::SHRINK);
      label[4].show();
      label[4].set_label(_("Color4"));
    table_.attach(def_color, 1, 2, 0, 1, Gtk::SHRINK, Gtk::SHRINK);
      def_color.show();
    table_.attach(color1, 1, 2, 1, 2, Gtk::SHRINK, Gtk::SHRINK);
      color1.show();
    table_.attach(color2, 1, 2, 2, 3, Gtk::SHRINK, Gtk::SHRINK);
      color2.show();
    table_.attach(color3, 1, 2, 3, 4, Gtk::SHRINK, Gtk::SHRINK);
      color3.show();
    table_.attach(color4, 1, 2, 4, 5, Gtk::SHRINK, Gtk::SHRINK);
      color4.show();
    table_.attach(curve1, 2, 3, 1, 2, Gtk::EXPAND|Gtk::FILL, Gtk::SHRINK);
      curve1.show();
    table_.attach(curve2, 2, 3, 2, 3, Gtk::EXPAND|Gtk::FILL, Gtk::SHRINK);
      curve2.show();
    table_.attach(curve3, 2, 3, 3, 4, Gtk::EXPAND|Gtk::FILL, Gtk::SHRINK);
      curve3.show();
    table_.attach(curve4, 2, 3, 4, 5, Gtk::EXPAND|Gtk::FILL, Gtk::SHRINK);
      curve4.show();
    table_.attach(use_alpha_, 0, 3, 5, 6, Gtk::EXPAND|Gtk::FILL, Gtk::SHRINK);
      use_alpha_.show();
      use_alpha_.set_label(_("UseAlpha"));
      //TODO die ganzen anderen widgets sollten refreshen, wenn das hier verändert wurde
    table_.attach(preview_frame_, 0, 3, 6, 7);
      preview_frame_.show();
      preview_frame_.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
      preview_frame_.add(preview_);
        preview_.show();
        preview_.set_gradient(_private_gradient);

  set_has_separator(true);
  set_default_response(Gtk::RESPONSE_OK);

  add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
  add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
}

GradientDialog::~GradientDialog()throw()
{
}

void GradientDialog::set_gradient(const GradientPtr& g)
{
  _private_gradient->set(g);

  //TODO
  // def_color.set_color(_private_gradient->get_def_color);
  // same for input widgets....
}

// ------------

GradientButton::GradientButton()
{
  _gradient = Gradient::create_black2white();

  preview_.show();
  preview_.set_gradient(_gradient);
  frame_.add(preview_);

  frame_.show();
  frame_.unset_label();
  frame_.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);

  add(frame_);
}

GradientButton::~GradientButton()throw()
{
}

void GradientButton::set_gradient(const GradientPtr& g)
{
  _gradient = g;

  preview_.set_gradient(_gradient);
  signal_changed().emit();
}

void GradientButton::on_clicked()
{
  GradientDialog dlg;

  dlg.set_gradient(get_gradient());

  if(dlg.run()==Gtk::RESPONSE_OK)
    _gradient->set(dlg.get_gradient());
}
