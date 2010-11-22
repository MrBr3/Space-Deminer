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

#include "./options.hpp"

Options::Options(int argc, char**argv)
{
  bool any_option_set = false;

  _dont_save_config = false;
  _dont_load_config = false;

#define MAP_ENTRY(e, d) option_id_name_map[#e]  = e;option_id_description_map[e] = _(d);

  MAP_ENTRY(OPTION_UI_TRUNK_BACK_IMAGE, "If set, the background image will be trunkated in order to avoid letterboxes. The initial value is true.");
  MAP_ENTRY(OPTION_VIDEO_FULLSCREEN, "If set, the game will try to be displayed in fullscreen");

#undef MAP_ENTRY

  for(int i=1; i<argc; ++i)
  {
    Glib::ustring arg = argv[i];
    if(arg[0]=='-' && arg[1]=='o')
    {
      if(!any_option_set && !_dont_load_config)
      {
        any_option_set  = true;
        load_config();
      }

      const gsize prefix_len  = 2;

      Glib::ustring::size_type eq = arg.find('=');
      set_value("OPTION_"+str_copy_replace_all_with(arg.substr(prefix_len, eq-prefix_len).uppercase(), '-', '_'), arg.substr(eq+1, arg.length()-eq-1));

    }else if(arg.substr(0, 14)=="--config-file=")
    {
      if(any_option_set)
      {
        g_warning("The Config File has been specified using the '--config-file' argument, after some options has been set! so it will be ignored!");
      }else
        arg.substr(14, arg.length()-14);
    }else if(arg=="--dont_save_config")
    {
      _dont_save_config = true;
    }else if(arg=="--dont_load_config")
    {
      if(any_option_set)
      {
        g_warning("The '--dont_load_config' argument has been set, after some options has been set, so it will be ignored!");
      }else
        _dont_load_config = true;
    }else if(arg=="--help")
    {
      std::cout<<
"Argument-Help\n"
"\n"
"Options\n"
"--dont_load_config                If this argument is used, no config data will\n"
"                                    be loaded from on your harddisc - ideal for\n"
"                                    CDs aor in order to recover te game.\n"
"                                    Must be set before any -oOPTION=VALUE\n"
"                                    argument\n"
"--dont_save_config                If this argument is used, no config data will\n"
"                                    be save on your harddisc - ideal for CDs.\n"
"--config-file=FILE                Sets the place, where the Config File should\n"
"                                    be loaded and saved to. FILE is the full\n"
"                                    path to the config file.\n"
"                                    Must be set before any -oOPTION=VALUE\n"
"                                    argument\n"
"-oOPTION=VALUE                    Sets the value of an single option. Valid\n"
"                                    Values for OPTION are:\n";
      for(std::map<Glib::ustring, OptionID>::iterator iter=option_id_name_map.begin(); iter!=option_id_name_map.end(); ++iter)
      {
        std::cout << " "<<str_copy_replace_all_with(iter->first.substr(7, iter->first.length()-7).lowercase(), '_', '-').c_str()<<"\n    "<<option_id_description_map[iter->second].c_str()<<"\n";
      }

      std::cout<<
"\n";
    }else
    {
      std::cout<<"Use '--help' to receive information about valid arguments\n";
    }
  }

  if(!any_option_set && !_dont_load_config)
  {
    load_config();
  }
}

void Options::load_config()
{
}

void Options::set_value(const Glib::ustring& id, const Glib::ustring& value)
{
  OptionID id_ = get_id_for_string(id);

  if(OPTION_UNKOWN==id_)
  {
    std::cout<<"unkown option \""<<id.c_str()<<"\"\n";
    return;
  }

  set_value(id_, value);
}

void Options::set_value(OptionID id, const Glib::ustring& value)
{
  switch(id&OPTIONTYPE_MASK_)
  {
  case OPTIONTYPE_BOOL:
    set_bool(id, !value.empty() && value!="0" && value.lowercase()!="false" && value.lowercase()!="off");
    break;
  default:
    std::cout<<"unkown option type \""<<int(id&OPTIONTYPE_MASK_)<<"\"\n";
  }
}

OptionID Options::get_id_for_string(const Glib::ustring& name)
{
  std::map<Glib::ustring, OptionID>::iterator iter  = get_singleton()->option_id_name_map.find(name);
  if(iter==get_singleton()->option_id_name_map.end())
    return OPTION_UNKOWN;
  return iter->second;
}