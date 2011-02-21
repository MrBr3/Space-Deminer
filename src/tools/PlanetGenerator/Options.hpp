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

#include <base.hpp>

#include "./Gradient.hpp"

class Options : public sigc::trackable
{
  typedef std::map<Glib::ustring, Glib::ustring> OptionMap;

  OptionMap _options;

  static bool option_exists(const Glib::ustring& name)
  {
    g_assert(_singleton);
    return _singleton->_options.find(name)!=_singleton->_options.end();
  }

  static Options* _singleton;

  static guint8 char_to_uint8(Glib::ustring::value_type u, Glib::ustring::value_type v)
  {
    return 16*CLAMP(u-'0', 0, 255)+CLAMP(v-'0', 0, 255);
  }
public:

  static Glib::ustring get_string(const Glib::ustring& name, const Glib::ustring&  def_value);
  static void set_string(const Glib::ustring& name, const Glib::ustring& value);
  static bool get_boolean(const Glib::ustring& name, bool def_value);
  static void set_boolean(const Glib::ustring& name, bool value);
  static gint get_integer(const Glib::ustring& name, gint def_value);
  static void set_integer(const Glib::ustring& name, gint value);
  static gfloat get_real(const Glib::ustring& name, gfloat def_value);
  static void set_real(const Glib::ustring& name, gfloat value);
  static Gdk::Color get_color(const Glib::ustring& name, const Gdk::Color& def_value);
  static void set_color(const Glib::ustring& name, const Gdk::Color&  value);
  static void get_gradient(const Glib::ustring& name, const GradientPtr& gradient);
  static void set_gradient(const Glib::ustring& name, const ConstGradientPtr&  value);
  static void get_curve(const Glib::ustring& name, const CurvePtr& curve);
  static void set_curve(const Glib::ustring& name, const ConstCurvePtr& value);

  static Glib::ustring integer2string(gint value){return Glib::ustring::compose("%1", value);}
  static Glib::ustring integer2string(guint value){return Glib::ustring::compose("%1", value);}
  static Glib::ustring size2string(gsize value){return Glib::ustring::compose("%1", value);}
  static Glib::ustring real2string(gdouble value){return Glib::ustring::format(std::scientific, std::setprecision(32), value);}
  static Glib::ustring real2string(gfloat value){return Glib::ustring::format(std::scientific, std::setprecision(32), value);}
  static Glib::ustring boolean2string(bool value){return value ? "1" : "0";}

  template<typename T=Glib::ustring>
  static void set(const Glib::ustring& name, const Glib::ustring& value)
  {
    set_string(name, value);
  }

  template<typename T=bool>
  static void set(const Glib::ustring& name, bool value)
  {
    set_boolean(name, value);
  }

  template<typename T=gint>
  static void set(const Glib::ustring& name, gint value)
  {
    set_integer(name, value);
  }

  template<typename T=gfloat>
  static void set(const Glib::ustring& name, gfloat value)
  {
    set_real(name, value);
  }

  template<typename T=Gdk::Color>
  static void set(const Glib::ustring& name, const Gdk::Color& value)
  {
    set_color(name, value);
  }

  template<typename T>
  static void update_option(const Glib::ustring& name, const sigc::slot<T>& getter)
  {
    set(name, getter());
  }

  template<typename T>
  static sigc::slot<void> update_option_slot(const Glib::ustring& name, const sigc::slot<T>& getter)
  {
    return sigc::bind(sigc::ptr_fun(&Options::update_option<T>), name, getter);
  }

  static Glib::RefPtr<Gio::File> get_file();

  void load_from_stream(const Glib::RefPtr<Gio::InputStream>& os);
  void write_to_stream(const Glib::RefPtr<Gio::OutputStream>& os);

  void load_options();
  bool save_options();

  Options();
  ~Options()throw();
};
