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
  _gradient->require_cairo_gradient(2);
}

GradientPreview::~GradientPreview()throw()
{
}

void GradientPreview::set_gradient(const GradientPtr& g)
{
  _gradient->clear_cairo_gradient();
  _gradient = g;
  _gradient->require_cairo_gradient(256);
  invalidate(this);

  //TODO when the Gradient has changed the Widget should be redrawn
}

bool GradientPreview::on_expose_event(GdkEventExpose* ee)
{
  Glib::RefPtr<Gdk::Window> w = get_window();

  if(!w || get_width()<=0)
    return false;

  Cairo::RefPtr<Cairo::Context> cc = w->create_cairo_context();

  if(!cc)
    return false;

  Cairo::RefPtr<Cairo::Gradient> cg = _gradient->get_cairo_gradient();

  if(!cg)
    return false;

  // TODO handle alpha

  cc->scale(get_width(), 1.);
  cc->set_source(cg);
  cc->paint();
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


  context_menu.append(_flip_h);
    _flip_h.set_label(_("Flip _Horizontal"));
    _flip_h.set_use_underline(true);
    _flip_h.signal_activate().connect(sigc::mem_fun(*this, &GradientButton::flip_h));
  context_menu.append(sep);
  context_menu.append(_load_present);
    _load_present.set_label(_("Load _Present"));
    _load_present.set_use_underline(true);
    _load_present.set_submenu(load_present_menu);
    load_present_menu.append(load_present_black2white);
      load_present_black2white.set_label("_Black -> White");
      load_present_black2white.set_use_underline(true);
      load_present_black2white.signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &GradientButton::load_present), Gradient::PRESENT_BLACK_2_WHITE));
    load_present_menu.append(load_present_transparent2white);
      load_present_transparent2white.set_label("_Transparent -> White");
      load_present_transparent2white.set_use_underline(true);
      load_present_transparent2white.signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &GradientButton::load_present), Gradient::PRESENT_TRANSPARENT_2_WHITE));
  context_menu.append(_load_slot);
    _load_slot.set_label(_("_Load From Slot"));
    _load_slot.set_use_underline(true);
    _load_slot.set_submenu(load_slot_menu);
    load_slot_menu.append(load_slot0);
      load_slot0.set_label("Slot_0");
      load_slot0.set_use_underline(true);
      load_slot0.signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &GradientButton::load_slot), 0));
    load_slot_menu.append(load_slot1);
      load_slot1.set_label("Slot_1");
      load_slot1.set_use_underline(true);
      load_slot1.signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &GradientButton::load_slot), 1));
    load_slot_menu.append(load_slot2);
      load_slot2.set_label("Slot_2");
      load_slot2.set_use_underline(true);
      load_slot2.signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &GradientButton::load_slot), 2));
    load_slot_menu.append(load_slot3);
      load_slot3.set_label("Slot_3");
      load_slot3.set_use_underline(true);
      load_slot3.signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &GradientButton::load_slot), 3));
  context_menu.append(_save_slot);
    _save_slot.set_label(_("_Save to Slot"));
    _save_slot.set_use_underline(true);
    _save_slot.set_submenu(save_slot_menu);
    save_slot_menu.append(save_slot0);
      save_slot0.set_label("Slot_0");
      save_slot0.set_use_underline(true);
      save_slot0.signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &GradientButton::save_slot), 0));
    save_slot_menu.append(save_slot1);
      save_slot1.set_label("Slot_1");
      save_slot1.set_use_underline(true);
      save_slot1.signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &GradientButton::save_slot), 1));
    save_slot_menu.append(save_slot2);
      save_slot2.set_label("Slot_2");
      save_slot2.set_use_underline(true);
      save_slot2.signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &GradientButton::save_slot), 2));
    save_slot_menu.append(save_slot3);
      save_slot3.set_label("Slot_3");
      save_slot3.set_use_underline(true);
      save_slot3.signal_activate().connect(sigc::bind(sigc::mem_fun(*this, &GradientButton::save_slot), 3));

  context_menu.show_all_children();
  context_menu.show();
}

GradientButton::~GradientButton()throw()
{
}

bool GradientButton::on_button_press_event(GdkEventButton* eb)
{
  g_assert(eb);
  if(eb->button==3)
  {
    context_menu.popup(eb->button, eb->time);
    return true;
  }

  return ParentClass::on_button_press_event(eb);
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
