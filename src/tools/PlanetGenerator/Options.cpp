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

#include "./Options.hpp"

Options* Options::_singleton = nullptr;

Options::Options()
{
  g_assert(!_singleton);
  _singleton = this;
}

Options::~Options()throw()
{
  g_assert(_singleton);
  _singleton = nullptr;
}

Glib::ustring Options::get_string(const Glib::ustring& name, const Glib::ustring& def_value)
{
  g_assert(_singleton);
  OptionMap::iterator iter = _singleton->_options.find(name);

  if(iter !=_singleton-> _options.end())
    return iter->second;

  _singleton->_options[name]  = def_value;

  return def_value;
}

void Options::set_string(const Glib::ustring& name, const Glib::ustring& value)
{
  g_assert(_singleton);
  _singleton->_options[name]  = value;
}

bool Options::get_boolean(const Glib::ustring& name, bool def_value)
{
  return get_string(name, def_value ? "1" : "0")=="1";
}

void Options::set_boolean(const Glib::ustring& name, bool value)
{
  set_string(name, value ? "1" : "0");
}

gint Options::get_integer(const Glib::ustring& name, gint def_value)
{
  if(!option_exists(name))
  {
    set_integer(name, def_value);
    return def_value;
  }

  Glib::ustring str = get_string(name, "");

  return str_to_integer(str, def_value);
}

void Options::set_integer(const Glib::ustring& name, gint value)
{
  set_string(name, Glib::ustring::compose("%1", value));
}

gfloat Options::get_real(const Glib::ustring& name, gfloat def_value)
{
  if(!option_exists(name))
  {
    set_real(name, def_value);
    return def_value;
  }

  Glib::ustring str = get_string(name, "");

  return str_to_real(str, def_value);
}

void Options::set_real(const Glib::ustring& name, gfloat value)
{
  set_string(name, Glib::ustring::format(std::scientific, std::setprecision(32), value));
}

Gdk::Color Options::get_color(const Glib::ustring& name, const Gdk::Color& def_value)
{
  if(!option_exists(name))
  {
    set_color(name, def_value);
    return def_value;
  }

  Glib::ustring str = get_string(name, "");

  if(str.length()!=6)
  {
    g_warning("**Options::get_color** bad color representation");
    return def_value;
  }

  Gdk::Color col;

  col.set_rgb_p(char_to_uint8(str[0], str[1])/255.f,
                char_to_uint8(str[2], str[3])/255.f,
                char_to_uint8(str[4], str[5])/255.f);

  return col;
}

void Options::set_color(const Glib::ustring& name, const Gdk::Color& value)
{
  set_string(name, Glib::ustring::compose("%1%2%3",
                                          Glib::ustring::format(SET_FILL_0, std::setw(2), CLAMP(round(value.get_red_p()*255.), 0, 255)),
                                          Glib::ustring::format(SET_FILL_0, std::setw(2), CLAMP(round(value.get_green_p()*255.), 0, 255)),
                                          Glib::ustring::format(SET_FILL_0, std::setw(2), CLAMP(round(value.get_blue_p()*255.), 0, 255))));
}

Glib::RefPtr<Gio::File> Options::get_file()
{
  return Gio::File::create_for_path(Glib::filename_from_utf8(apply_filename_macros("$(local-folder)/config.xml")));
}

void Options::load_from_string(const Glib::RefPtr<Gio::InputStream>& os)
{
  if(!os)
    throw std::invalid_argument("**Options::write_to_stream** needs a valid stream");

  std::cout<<"Options loading\n";
}

void Options::write_to_stream(const Glib::RefPtr<Gio::OutputStream>& os)
{
  if(!os)
    throw std::invalid_argument("**Options::write_to_stream** needs a valid stream");

  std::cout<<"Options saving\n";

  os->write("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");
  os->write("<config>\n");
  for(OptionMap::const_iterator i=_options.begin(); i!=_options.end(); ++i)
    os->write(Glib::ustring::compose("  <%1 value=\"%2\">\n", i->first, i->second).c_str());
  os->write("</config>");
}

void Options::load_options()
{
  try
  {
    Glib::RefPtr<Gio::File> file = get_file();

    if(!file || !file->query_exists())
      return;

    load_from_string(file->read());
  }CATCH_ALL("**Options::load_options** ", return;)
}

bool Options::save_options()
{
  if(is_portable_version())
    return true;

  try
  {
    Glib::RefPtr<Gio::File> file = get_file();

    if(!file)
      return false;

    if(!make_all_parent_directories(file))
      return false;

    if(!file->query_exists())
    {
      write_to_stream(file->create_file());
    }else
    {
      Glib::RefPtr<Gio::FileIOStream> stream  = file->replace_readwrite(std::string(), true);
      if(!stream)
        return false;
      write_to_stream(stream->get_output_stream());
    }

  }CATCH_ALL("**Options::save_options** ", return false;)

  return true;
}