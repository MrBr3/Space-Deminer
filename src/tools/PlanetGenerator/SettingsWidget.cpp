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

PanelLabelWidget::DoubleSep::DoubleSep()
{
  add(vbox);
  vbox.pack_start(s1, false, false, 1);
  vbox.pack_start(s2, false, false, 1);

  set(0.5, 0.6, 1., 0.);

  set_size_request(16, -1);

  vbox.show();
  s1.show();
  s2.show();
}

PanelLabelWidget::PanelLabelWidget()
{
  pack_start(s1);
  pack_start(_caption, false, false);
  pack_start(s2);
  s1.show();
  _caption.show();
  s2.show();
  set_spacing(LENGTH_SMALLSPACE);
}

PanelLabelWidget::~PanelLabelWidget()throw()
{
}

void PanelLabelWidget::set_main_caption(const Glib::ustring& str)
{
  _main_caption = str;
  _update_widget();
}

void PanelLabelWidget::set_sub_caption(const Glib::ustring& str)
{
  _sub_caption = str;
  _update_widget();
}

void PanelLabelWidget::_update_widget()
{
  if(_sub_caption.empty())
    _caption.set_markup(Glib::ustring::compose("<b>%1</b>", _main_caption));
  else
    _caption.set_markup(Glib::ustring::compose("<b>%1</b> [%2]", _main_caption, _sub_caption));
}

//========================================
//========================================

SettingsWidget* SettingsWidget::visible_settings  = nullptr;
guint SettingsWidget::n_sw  = 0;
std::list<Glib::RefPtr<Refable> > SettingsWidget::delete_with_last;

SettingsWidget::SettingsWidget()
{
  n_sw++;

  pack_start(caption, false, false);
  caption.show();

  pack_start(scroll, true, true);
  scroll.show();
  scroll.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_AUTOMATIC);

  scroll.add(table);
  table.show();
  table.set_border_width(LENGTH_SMALLSPACE);
  table.set_spacings(LENGTH_SMALLSPACE);
  n_entries = 0;

  set_spacing(LENGTH_SMALLSPACE);

  Gtk::manage(this);

  if(main_window)
    main_window->append_settings_widget(*this);
}

SettingsWidget::~SettingsWidget()throw()
{
  --n_sw;

  if(visible_settings==this)
    visible_settings  = nullptr;

  if(n_sw==0)
    delete_with_last.clear();
}

void SettingsWidget::bring_to_front()
{
  g_assert(this);

  if(visible_settings==this)
    return;
  if(visible_settings)
    visible_settings->hide();

  show();

  visible_settings  = this;
}

Gtk::Widget& SettingsWidget::append_color_widget(Gtk::Table& table, guint& n_entries, const Glib::ustring& name, const Glib::ustring& label, const Glib::ustring& tooltip, const sigc::slot<Gdk::Color>& getter, const sigc::slot<void, const Gdk::Color&>& setter, sigc::signal<void>& signal_changed)
{
  Gtk::Label* wlabel  = Gtk::manage(new Gtk::Label(label));
  Gtk::ColorButton* color_button  = Gtk::manage(new Gtk::ColorButton);

  wlabel->set_alignment(0., 0.5);
  wlabel->show();

  sigc::slot<Gdk::Color> w_getter = sigc::mem_fun(*color_button, &Gtk::ColorButton::get_color);
  sigc::slot<void, const Gdk::Color&> w_setter = sigc::mem_fun(*color_button, &Gtk::ColorButton::set_color);

  w_setter(getter());

  color_button->show();
  color_button->set_tooltip_text(tooltip);
  color_button->set_use_alpha(false);
  color_button->signal_color_set().connect(create_updater(w_getter, setter));
  signal_changed.connect(create_updater(getter, w_setter));

  table.attach(*wlabel, 0, 1, n_entries, n_entries+1, Gtk::FILL, Gtk::FILL);
  table.attach(*color_button, 1, 2, n_entries, n_entries+1, Gtk::EXPAND|Gtk::FILL, Gtk::FILL);
  ++n_entries;

  return *color_button;
}

Gtk::Widget& SettingsWidget::append_boolean_widget(Gtk::Table& table, guint& n_entries, const Glib::ustring& name, const Glib::ustring& label, const Glib::ustring& tooltip, const sigc::slot<bool>& getter, const sigc::slot<void, bool>& setter, sigc::signal<void>& signal_changed)
{
  Gtk::CheckButton* check_button  = Gtk::manage(new Gtk::CheckButton);

  sigc::slot<bool> w_getter = sigc::mem_fun(*check_button, &Gtk::CheckButton::get_active);
  sigc::slot<void, bool> w_setter = sigc::mem_fun(*check_button, &Gtk::CheckButton::set_active);

  w_setter(getter());

  check_button->show();
  check_button->set_label(label);
  check_button->set_tooltip_text(tooltip);
  check_button->signal_toggled().connect(create_updater(w_getter, setter));
  signal_changed.connect(create_updater(getter, w_setter));

  table.attach(*check_button, 0, 2, n_entries, n_entries+1, Gtk::EXPAND|Gtk::FILL, Gtk::FILL);
  ++n_entries;

  return *check_button;
}

Gtk::Widget& SettingsWidget::append_int_widget(Gtk::Table& table, guint& n_entries, const Glib::ustring& name, const Glib::ustring& label, const Glib::ustring& tooltip, const sigc::slot<int>& getter, const sigc::slot<void, int>& setter, sigc::signal<void>& signal_changed)
{
  Gtk::Label* wlabel  = Gtk::manage(new Gtk::Label(label));
  Gtk::SpinButton* spin_button  = Gtk::manage(new Gtk::SpinButton);

  wlabel->set_alignment(0., 0.5);
  wlabel->show();

  sigc::slot<gdouble> w_getter = sigc::mem_fun(*spin_button, &Gtk::SpinButton::get_value);
  sigc::slot<void, gdouble> w_setter = sigc::mem_fun(*spin_button, &Gtk::SpinButton::set_value);

  spin_button->set_range(G_MININT, G_MAXINT);

  w_setter(getter());

  spin_button->show();
  spin_button->set_tooltip_text(tooltip);
  spin_button->set_increments(1, 5);
  spin_button->signal_value_changed().connect(create_updater(w_getter, setter));
  signal_changed.connect(create_updater(getter, w_setter));

  table.attach(*wlabel, 0, 1, n_entries, n_entries+1, Gtk::FILL, Gtk::FILL);
  table.attach(*spin_button, 1, 2, n_entries, n_entries+1, Gtk::EXPAND|Gtk::FILL, Gtk::FILL);
  ++n_entries;

  return *spin_button;
}

Gtk::Widget& SettingsWidget::append_real_widget(Gtk::Table& table, guint& n_entries, const Glib::ustring& name, const Glib::ustring& label, const Glib::ustring& tooltip, const sigc::slot<gfloat>& getter, const sigc::slot<void, gfloat>& setter, sigc::signal<void>& signal_changed)
{
  Gtk::Label* wlabel  = Gtk::manage(new Gtk::Label(label));
  Gtk::SpinButton* spin_button  = Gtk::manage(new Gtk::SpinButton);

  wlabel->set_alignment(0., 0.5);
  wlabel->show();

  sigc::slot<gdouble> w_getter = sigc::mem_fun(*spin_button, &Gtk::SpinButton::get_value);
  sigc::slot<void, gdouble> w_setter = sigc::mem_fun(*spin_button, &Gtk::SpinButton::set_value);

  spin_button->set_range(G_MININT, G_MAXINT);

  w_setter(getter());

  spin_button->show();
  spin_button->set_tooltip_text(tooltip);
  spin_button->set_increments(1, 5);
  spin_button->set_digits(3);
  spin_button->signal_value_changed().connect(create_updater(w_getter, setter));
  signal_changed.connect(create_updater(getter, w_setter));

  table.attach(*wlabel, 0, 1, n_entries, n_entries+1, Gtk::FILL, Gtk::FILL);
  table.attach(*spin_button, 1, 2, n_entries, n_entries+1, Gtk::EXPAND|Gtk::FILL, Gtk::FILL);
  ++n_entries;

  return *spin_button;
}

namespace Private
{
  class MyComboBoxText : public Gtk::ComboBox
  {
    class MyColumnRecord : public Gtk::TreeModel::ColumnRecord
    {
    public:
      Gtk::TreeModelColumn<Glib::ustring> text;

      MyColumnRecord()
      {
        add(text);
      }

    }columns;

    Glib::RefPtr<Gtk::ListStore> _list_store;
  public:
    void set_active_row_number(int i)
    {
      g_assert(get_model());

      if(get_active_row_number()==i)
        return;
      if(i<0)
        Gtk::ComboBox::set_active(-1);

      int j=0;
      for(Gtk::TreeModel::iterator iter=_list_store->children().begin(); iter!=_list_store->children().end(); ++iter)
      {
        if(i==j)
        {
          set_active(iter);
          return;
        }

        ++j;
      }

      Gtk::ComboBox::set_active(-1);
    }

    void append_text(const Glib::ustring& txt)
    {
      Gtk::TreeModel::iterator iter  = _list_store->append();
      (*iter)[columns.text] = txt;
    }

    MyComboBoxText()
    {
      _list_store = Gtk::ListStore::create(columns);
      set_model(_list_store);

      pack_start(columns.text);
    }
  };
}

using Private::MyComboBoxText;

Gtk::Widget& SettingsWidget::append_enum_widget(Gtk::Table& table, guint& n_entries, const Glib::ustring& name, const Glib::ustring& label, const Glib::ustring& tooltip, const std::vector<Glib::ustring>& items, const sigc::slot<int>& getter, const sigc::slot<void, int>& setter, sigc::signal<void>& signal_changed)
{
  Gtk::Label* wlabel  = Gtk::manage(new Gtk::Label(label));
  MyComboBoxText* cb_text  = Gtk::manage(new MyComboBoxText());

  wlabel->set_alignment(0., 0.5);
  wlabel->show();

  sigc::slot<int> w_getter = sigc::mem_fun(*cb_text, &MyComboBoxText::get_active_row_number);
  sigc::slot<void, int> w_setter = sigc::mem_fun(*cb_text, &MyComboBoxText::set_active_row_number);

  for(std::vector<Glib::ustring>::const_iterator i = items.begin(); i!=items.end(); ++i)
  {
    cb_text->append_text(*i);
  }

  w_setter(getter());

  cb_text->show();
  cb_text->set_tooltip_text(tooltip);
  cb_text->signal_changed().connect(create_updater(w_getter, setter));
  signal_changed.connect(create_updater(getter, w_setter));

  table.attach(*wlabel, 0, 1, n_entries, n_entries+1, Gtk::FILL, Gtk::FILL);
  table.attach(*cb_text, 1, 2, n_entries, n_entries+1, Gtk::EXPAND|Gtk::FILL, Gtk::FILL);
  ++n_entries;

  return *cb_text;
}

Gtk::Widget& SettingsWidget::append_imagefile_widget(Gtk::Table& table, guint& n_entries, const Glib::ustring& name, const Glib::ustring& label, const Glib::ustring& tooltip, const Glib::RefPtr<ImageFile>& imagefile)
{
  ImageFileSettings* image_file_widget  = Gtk::manage(new ImageFileSettings(imagefile, name, label, tooltip));

  image_file_widget->show();

  table.attach(*image_file_widget, 0, 2, n_entries, n_entries+1, Gtk::EXPAND|Gtk::FILL, Gtk::FILL);
  ++n_entries;

  return *image_file_widget;
}

Gtk::Widget& SettingsWidget::append_filename_widget(Gtk::Table& table, guint& n_entries, const Glib::ustring& name, const Glib::ustring& label, const Glib::ustring& tooltip, const Gtk::FileFilter& filter, const sigc::slot<Glib::ustring>& getter, const sigc::slot<void, const Glib::ustring&>& setter, sigc::signal<void>& signal_changed)
{
  Gtk::FileChooserButton* filechooser = Gtk::manage(new Gtk::FileChooserButton);

  sigc::slot<Glib::ustring> w_getter = sigc::mem_fun(*filechooser, &Gtk::FileChooserButton::get_filename);
  sigc::slot<void, const Glib::ustring&> w_setter = sigc::hide_return(sigc::mem_fun(*filechooser, &Gtk::FileChooserButton::set_filename));

  w_setter(getter());

  filechooser->show();
  filechooser->add_filter(filter);
  filechooser->set_tooltip_text(tooltip);
  filechooser->set_width_chars(12);
  filechooser->set_action(Gtk::FILE_CHOOSER_ACTION_OPEN);
  filechooser->signal_file_set().connect(create_updater(w_getter, setter));
  signal_changed.connect(create_updater(getter, w_setter));

  table.attach(*filechooser, 0, 2, n_entries, n_entries+1, Gtk::EXPAND|Gtk::FILL, Gtk::FILL);
  ++n_entries;

  return *filechooser;
}

Gtk::Widget& SettingsWidget::append_string_widget(Gtk::Table& table, guint& n_entries, const Glib::ustring& name, const Glib::ustring& label, const Glib::ustring& tooltip, const sigc::slot<Glib::ustring>& getter, const sigc::slot<void, const Glib::ustring&>& setter, sigc::signal<void>& signal_changed)
{
  Gtk::Label* wlabel  = Gtk::manage(new Gtk::Label(label));
  Gtk::Entry* entry  = Gtk::manage(new Gtk::Entry);

  wlabel->set_alignment(0., 0.5);
  wlabel->show();

  sigc::slot<Glib::ustring> w_getter = sigc::mem_fun(*entry, &Gtk::Entry::get_text);
  sigc::slot<void, const Glib::ustring&> w_setter = sigc::mem_fun(*entry, &Gtk::Entry::set_text);

  w_setter(getter());

  entry->show();
  entry->set_tooltip_text(tooltip);
  entry->signal_changed().connect(create_updater(w_getter, setter));
  signal_changed.connect(create_updater(getter, w_setter));

  table.attach(*wlabel, 0, 1, n_entries, n_entries+1, Gtk::FILL, Gtk::FILL);
  table.attach(*entry, 1, 2, n_entries, n_entries+1, Gtk::EXPAND|Gtk::FILL, Gtk::FILL);
  ++n_entries;

  return *entry;
}