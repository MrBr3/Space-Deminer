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

#define CHECKER_WIDTH 8

Cairo::RefPtr<Cairo::SurfacePattern> GradientPreview::_alpha_checker_pattern;
gsize GradientPreview::_n_instances = 0;

GradientPreview::GradientPreview()
{
  set_gradient(Gradient::create());

  if(_n_instances==0)
  {
    _n_instances++;
    int w, h;
    w = h = CHECKER_WIDTH*2;
    Cairo::RefPtr<Cairo::ImageSurface> checker_img = Cairo::ImageSurface::create(Cairo::FORMAT_RGB24, w, h);;
    _alpha_checker_pattern = Cairo::SurfacePattern::create(checker_img);
    _alpha_checker_pattern->set_extend(Cairo::EXTEND_REPEAT);
    _alpha_checker_pattern->set_filter(Cairo::FILTER_NEAREST);

    int rowstride = checker_img->get_stride();
    guint8* data = (guint8*)checker_img->get_data();
    guint8* line = data;
    bool dark;

    for(int y=0; y<h; ++y)
    {
      guint8* pixel = line;
      for(int x=0; x<w; ++x)
      {
        dark = !XOR(x<CHECKER_WIDTH, y<CHECKER_WIDTH);

        guint8 g = 0x80;
        if(dark)
          g = 0x40;

        pixel[0] = pixel[1] = pixel[2] = g;

        pixel+=4;
      }
      line += rowstride;
    }
  }
}

GradientPreview::~GradientPreview()throw()
{
  _gradient->signal_changed().slots().erase(gradient_changed_signal_iter);

  _n_instances--;
  if(_n_instances==0)
  {
    _alpha_checker_pattern.clear();
  }
}

void GradientPreview::set_gradient(const GradientPtr& g)
{
  if(_gradient)
  {
    _gradient->signal_changed().slots().erase(gradient_changed_signal_iter);
    _gradient->unreference_cairo_gradient();
  }

  _gradient = g;
  _gradient->reference_cairo_gradient();
  invalidate(this);

  gradient_changed_signal_iter = _gradient->signal_changed().connect(sigc::bind(sigc::ptr_fun(::invalidate), this));
}

bool GradientPreview::on_expose_event(GdkEventExpose* ee)
{
  Glib::RefPtr<Gdk::Window> w = get_window();

  if(!w || get_width()<=0)
    return false;

  Cairo::RefPtr<Cairo::Context> cc = w->create_cairo_context();

  if(!cc)
    return false;

  Cairo::RefPtr<const Cairo::Gradient> cg = _gradient->get_cairo_gradient();

  if(!cg)
    return false;

  if(_gradient->get_use_alpha())
  {
    cc->set_source(_alpha_checker_pattern);
    cc->paint();
  }

  cc->scale(get_width(), 1.);
  cc->set_source(cg);
  cc->paint();
  return true;
}

void GradientPreview::on_size_request(Gtk::Requisition* r)
{
  if(r)
  {
    r->width = 48;
    r->height = 16;
  }
}

#undef CHECKER_WIDTH

// ------------

GradientDialog::GradientDialog()
{
  _private_gradient = Gradient::create();

  set_default_size(300, -1);

  set_title(_("EditGradient"));

  g_assert(get_vbox());
  get_vbox()->pack_start(table_);
    table_.show();
    table_.set_spacings(LENGTH_SMALLSPACE);
    table_.set_border_width(LENGTH_SMALLSPACE);
    table_.attach(label[0], 0, 1, 0, 1, Gtk::FILL, Gtk::SHRINK);
      label[0].show();
      label[0].set_label(_("DefaultColor"));
      label[0].set_alignment(0.f, 0.5f);
    table_.attach(label[1], 0, 1, 1, 2, Gtk::FILL, Gtk::SHRINK);
      label[1].show();
      label[1].set_label(_("Color1"));
      label[1].set_alignment(0.f, 0.5f);
    table_.attach(label[2], 0, 1, 2, 3, Gtk::FILL, Gtk::SHRINK);
      label[2].show();
      label[2].set_label(_("Color2"));
      label[2].set_alignment(0.f, 0.5f);
    table_.attach(label[3], 0, 1, 3, 4, Gtk::FILL, Gtk::SHRINK);
      label[3].show();
      label[3].set_label(_("Color3"));
      label[3].set_alignment(0.f, 0.5f);
    table_.attach(label[4], 0, 1, 4, 5, Gtk::FILL, Gtk::SHRINK);
      label[4].show();
      label[4].set_label(_("Color4"));
      label[4].set_alignment(0.f, 0.5f);
    table2_.attach(label[5], 0, 1, 1, 2, Gtk::FILL, Gtk::SHRINK);
      label[5].show();
      label[5].set_label(_("nSamples"));
      label[5].set_alignment(0.f, 0.5f);
    table2_.attach(label[6], 0, 1, 2, 3, Gtk::FILL, Gtk::SHRINK);
      label[6].show();
      label[6].set_label(_("Remap"));
      label[6].set_alignment(0.f, 0.5f);
    table_.attach(def_color, 1, 2, 0, 1, Gtk::SHRINK, Gtk::SHRINK);
      def_color.show();
      def_color.set_color(_private_gradient->get_defcolor().get_gdk_color());
      def_color.set_alpha(_private_gradient->get_defcolor().a*0xffff);
      def_color.signal_color_set().connect(sigc::mem_fun(*this, &GradientDialog::set_defcolor_from_widget));
    table_.attach(color1, 1, 2, 1, 2, Gtk::SHRINK, Gtk::SHRINK);
      color1.show();
      color1.set_color(_private_gradient->get_color1().get_gdk_color());
      color1.set_alpha(_private_gradient->get_color1().a*0xffff);
      color1.signal_color_set().connect(sigc::mem_fun(*this, &GradientDialog::set_color1_from_widget));
    table_.attach(color2, 1, 2, 2, 3, Gtk::SHRINK, Gtk::SHRINK);
      color2.show();
      color2.set_color(_private_gradient->get_color2().get_gdk_color());
      color2.set_alpha(_private_gradient->get_color2().a*0xffff);
      color2.signal_color_set().connect(sigc::mem_fun(*this, &GradientDialog::set_color2_from_widget));
    table_.attach(color3, 1, 2, 3, 4, Gtk::SHRINK, Gtk::SHRINK);
      color3.show();
      color3.set_color(_private_gradient->get_color3().get_gdk_color());
      color3.set_alpha(_private_gradient->get_color3().a*0xffff);
      color3.signal_color_set().connect(sigc::mem_fun(*this, &GradientDialog::set_color3_from_widget));
    table_.attach(color4, 1, 2, 4, 5, Gtk::SHRINK, Gtk::SHRINK);
      color4.show();
      color4.set_color(_private_gradient->get_color4().get_gdk_color());
      color4.set_alpha(_private_gradient->get_color4().a*0xffff);
      color4.signal_color_set().connect(sigc::mem_fun(*this, &GradientDialog::set_color4_from_widget));
    table_.attach(curve1, 2, 3, 1, 2, Gtk::EXPAND|Gtk::FILL, Gtk::SHRINK);
      curve1.show();
      curve1.set_curve(_private_gradient->get_curve1());
    table_.attach(curve2, 2, 3, 2, 3, Gtk::EXPAND|Gtk::FILL, Gtk::SHRINK);
      curve2.show();
      curve2.set_curve(_private_gradient->get_curve2());
    table_.attach(curve3, 2, 3, 3, 4, Gtk::EXPAND|Gtk::FILL, Gtk::SHRINK);
      curve3.show();
      curve3.set_curve(_private_gradient->get_curve3());
    table_.attach(curve4, 2, 3, 4, 5, Gtk::EXPAND|Gtk::FILL, Gtk::SHRINK);
      curve4.show();
      curve4.set_curve(_private_gradient->get_curve4());
    table_.attach(table2_, 0, 3, 5, 6, Gtk::EXPAND|Gtk::FILL, Gtk::SHRINK);
    table2_.show();
      table2_.set_spacings(LENGTH_SMALLSPACE);
      table2_.attach(n_samples, 1, 3, 1, 2, Gtk::EXPAND|Gtk::FILL, Gtk::SHRINK);
        n_samples.show();
        n_samples.set_digits(0);
        n_samples.set_range(16., 2.5e5);
        n_samples.set_increments(1., 10.);
        n_samples.set_value(_private_gradient->get_n_samples());
        n_samples.signal_value_changed().connect(sigc::mem_fun(*this, &GradientDialog::set_n_samples));
      table2_.attach(remap_a, 1, 2, 2, 3, Gtk::EXPAND|Gtk::FILL, Gtk::SHRINK);
      table2_.attach(remap_b, 2, 3, 2, 3, Gtk::EXPAND|Gtk::FILL, Gtk::SHRINK);
        remap_a.show();
        remap_b.show();
        remap_a.set_digits(3);
        remap_b.set_digits(3);
        remap_a.set_increments(1., 5.);
        remap_b.set_increments(1., 5.);
        remap_a.set_range(G_MININT, G_MAXINT);
        remap_b.set_range(G_MININT, G_MAXINT);
        remap_b.set_value(100.);
        set_unit(remap_a, " %");
        set_unit(remap_b, " %");
        //TODO remapwerte vom Gradient Laden
        //remap_a.signal_value_changed().connect(sigc::mem_fun(*this, &GradientDialog::set_remap));
        //remap_b.signal_value_changed().connect(sigc::mem_fun(*this, &GradientDialog::set_remap));
      table2_.attach(use_alpha_, 0, 3, 0, 1, Gtk::EXPAND|Gtk::FILL, Gtk::SHRINK);
        use_alpha_.show();
        use_alpha_.set_label(_("UseAlpha"));
        use_alpha_.signal_toggled().connect(sigc::mem_fun(*this, &GradientDialog::set_use_alpha));
        use_alpha_.set_active(_private_gradient->get_use_alpha());
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
  _private_gradient = g;

  _private_gradient->request_no_updates();
    preview_.set_gradient(_private_gradient);
    use_alpha_.set_active(_private_gradient->get_use_alpha());
    //TODO remapwerte vom Gradient Laden
    n_samples.set_value(_private_gradient->get_n_samples());

    curve1.set_curve(_private_gradient->get_curve1());
    curve2.set_curve(_private_gradient->get_curve2());
    curve3.set_curve(_private_gradient->get_curve3());
    curve4.set_curve(_private_gradient->get_curve4());

    color1.set_color(_private_gradient->get_color1().get_gdk_color());
    color1.set_alpha(_private_gradient->get_color1().a*0xffff);
    color2.set_color(_private_gradient->get_color2().get_gdk_color());
    color2.set_alpha(_private_gradient->get_color2().a*0xffff);
    color3.set_color(_private_gradient->get_color3().get_gdk_color());
    color3.set_alpha(_private_gradient->get_color3().a*0xffff);
    color4.set_color(_private_gradient->get_color4().get_gdk_color());
    color4.set_alpha(_private_gradient->get_color4().a*0xffff);
    def_color.set_color(_private_gradient->get_defcolor().get_gdk_color());
    def_color.set_alpha(_private_gradient->get_defcolor().a*0xffff);
  _private_gradient->unrequest_no_updates();
}

// ------------

GradientButton::GradientButton()
{
  _gradient = Gradient::create();

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

  GradientPtr tmp = Gradient::create();
  tmp->set(get_gradient());

  dlg.set_gradient(get_gradient());

  if(dlg.run()!=Gtk::RESPONSE_OK)
    _gradient->set(tmp);
}
