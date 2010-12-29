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

class PanelLabelWidget : public Gtk::HBox
{
public:
  typedef Gtk::HBox ParentClass;

  class DoubleSep : public Gtk::Alignment
  {
    Gtk::VBox vbox;
    Gtk::HSeparator s1, s2;

  public:
  DoubleSep();
  };

private:
  Gtk::Label _caption;
  DoubleSep s1, s2;
  Glib::ustring _main_caption, _sub_caption;

  void _update_widget();

public:
  void set_main_caption(const Glib::ustring& str);
  void set_sub_caption(const Glib::ustring& str);

  PanelLabelWidget();
  ~PanelLabelWidget()throw();
};

class SettingsWidget : public Gtk::VBox
{
  typedef Gtk::VBox ParentClass;

  static SettingsWidget* visible_settings;

  Gtk::Table table;
  Gtk::ScrolledWindow scroll;
  guint n_entries;
public:
  PanelLabelWidget caption;

  void set_main_caption(const Glib::ustring& str){caption.set_main_caption(str);}
  void set_sub_caption(const Glib::ustring& str){caption.set_sub_caption(str);}

  void bring_to_front();

public:
  void append_color_widget(const Glib::ustring& name, const Glib::ustring& label, const Glib::ustring& tooltip, const sigc::slot<Gdk::Color>& getter, const sigc::slot<void, const Gdk::Color&>& setter, sigc::signal<void>& signal_changed)
  {
    append_color_widget(table, n_entries, name, label, tooltip, getter, setter, signal_changed);
  }
  void append_boolean_widget(const Glib::ustring& name, const Glib::ustring& label, const Glib::ustring& tooltip, const sigc::slot<bool>& getter, const sigc::slot<void, bool>& setter, sigc::signal<void>& signal_changed)
  {
    append_boolean_widget(table, n_entries, name, label, tooltip, getter, setter, signal_changed);
  }
  void append_int_widget(const Glib::ustring& name, const Glib::ustring& label, const Glib::ustring& tooltip, const sigc::slot<int>& getter, const sigc::slot<void, int>& setter, sigc::signal<void>& signal_changed)
  {
    append_int_widget(table, n_entries, name, label, tooltip, getter, setter, signal_changed);
  }
  void append_imagefile_widget(const Glib::ustring& name, const Glib::ustring& label, const Glib::ustring& tooltip, const Glib::RefPtr<ImageFile>& imagefile)
  {
    append_imagefile_widget(table, n_entries, name, label, tooltip, imagefile);
  }
  void append_filename_widget(const Glib::ustring& name, const Glib::ustring& label, const Glib::ustring& tooltip, const sigc::slot<Glib::ustring>& getter, const sigc::slot<void, const Glib::ustring&>& setter, sigc::signal<void>& signal_changed)
  {
    append_filename_widget(table, n_entries, name, label, tooltip, getter, setter, signal_changed);
  }

  static void append_color_widget(Gtk::Table& table, guint& n_entries, const Glib::ustring& name, const Glib::ustring& label, const Glib::ustring& tooltip, const sigc::slot<Gdk::Color>& getter, const sigc::slot<void, const Gdk::Color&>& setter, sigc::signal<void>& signal_changed);
  static void append_boolean_widget(Gtk::Table& table, guint& n_entries, const Glib::ustring& name, const Glib::ustring& label, const Glib::ustring& tooltip, const sigc::slot<bool>& getter, const sigc::slot<void, bool>& setter, sigc::signal<void>& signal_changed);
  static void append_int_widget(Gtk::Table& table, guint& n_entries, const Glib::ustring& name, const Glib::ustring& label, const Glib::ustring& tooltip, const sigc::slot<int>& getter, const sigc::slot<void, int>& setter, sigc::signal<void>& signal_changed);
  static void append_imagefile_widget(Gtk::Table& table, guint& n_entries, const Glib::ustring& name, const Glib::ustring& label, const Glib::ustring& tooltip, const Glib::RefPtr<ImageFile>& imagefile);
  static void append_filename_widget(Gtk::Table& table, guint& n_entries, const Glib::ustring& name, const Glib::ustring& label, const Glib::ustring& tooltip, const sigc::slot<Glib::ustring>& getter, const sigc::slot<void, const Glib::ustring&>& setter, sigc::signal<void>& signal_changed);
public:
  SettingsWidget();
  ~SettingsWidget()throw();

private:
  static guint n_sw;
  static std::list<Glib::RefPtr<Refable> > delete_with_last;

  template<class T_get, class T_set>
  class Update : public Refable
  {
    Update(const Update<T_get, T_set>&);
  public:
    sigc::slot<T_get> getter;
    sigc::slot<void, T_set> setter;

    void do_update()
    {
      setter(getter());
    }

    sigc::slot<void> update_slot(){return sigc::mem_fun(*this, &Update<T_get, T_set>::do_update);}

    Update(const sigc::slot<T_get>& getter, const sigc::slot<void, T_set>& setter)
    {
      this->getter  = getter;
      this->setter  = setter;
    }
  };

  template<class T_get, class T_set>
  static sigc::slot<void> create_updater(const sigc::slot<T_get>& getter, const sigc::slot<void, T_set>& setter)
  {
    Glib::RefPtr<Update<T_get, T_set> > updater = Glib::RefPtr<Update<T_get, T_set> >(new Update<T_get, T_set>(getter, setter));

    delete_with_last.push_back(updater);

    return updater->update_slot();
  }
};