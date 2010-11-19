/* Space Deminer is a small 2D Arcade Game. Your task ist to eliminate
 * intelligent Mines created in the last galactic war.
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

#ifndef _SPACE_DEMINER_OPTIONS_H_
#define _SPACE_DEMINER_OPTIONS_H_

#include <base.hpp>

enum OptionType
{
  OPTIONTYPE_BOOL = 0x10000,

  OPTIONTYPE_MASK_  = 0xff0000,
  OPTIONTYPE_INVALID_ = 0,
};

enum OptionID{OPTION_UNKOWN=-1};

inline OptionID   operator|(OptionType t, int id )     {return   OptionID(int(t)|int(id));}
inline OptionID   operator|(OptionType t, OptionID id ){return   OptionID(int(t)|int(id));}
inline OptionType operator&(OptionID id,  OptionType t){return OptionType(int(t)&int(id));}
inline OptionID   operator&(OptionID id,  OptionID   b){return   OptionID(int(b)&int(id));}


// wenn ein neuer eintrag hinzugefügt wird, IDMap ergänzen!
const OptionID  OPTION_UI_TRUNK_BACK_IMAGE  = OPTIONTYPE_BOOL | 0x0001;

const OptionID  OPTION_VIDEO_FULLSCREEN     = OPTIONTYPE_BOOL | 0x0101;

class Options : public StaticManager<Options>
{
private:

  typedef std::map<OptionID, bool> BoolMap;

  BoolMap _bool_values;
  std::map<Glib::ustring, OptionID> option_id_name_map;
  std::map<OptionID, Glib::ustring> option_id_description_map;

  bool _dont_save_config, _dont_load_config;

  void load_config();

  template<class T> static const T& get_value(OptionID id, const T& def_value, std::map<OptionID, T>& map, OptionType t)
  {
    if((id&OPTIONTYPE_MASK_)!=t)
      throw std::invalid_argument("id");

    BoolMap::const_iterator iter  = map.find(id);
    if(iter==map.end())
    {
      map.insert(std::make_pair(id, def_value));
      return def_value;
    }

    return iter->second;
  }
  template<class T> static void set_value(OptionID id, const T& new_value, std::map<OptionID, T>& map, OptionType t)
  {
    if((id&OPTIONTYPE_MASK_)!=t)
      throw std::invalid_argument("id");

    map[id] = new_value;
  }

public:
  static bool get_bool(OptionID id, bool def_value)
  {
    return get_value(id, def_value, get_singleton()->_bool_values, OPTIONTYPE_BOOL);
  }
  static void set_bool(OptionID id, bool new_value)
  {
    return set_value(id, new_value, get_singleton()->_bool_values, OPTIONTYPE_BOOL);
  }

  static OptionID get_id_for_string(const Glib::ustring& name);
  static OptionType get_type(OptionID id){return id&OPTIONTYPE_MASK_;}

  void set_value(const Glib::ustring& id, const Glib::ustring& value);
  void set_value(OptionID id, const Glib::ustring& value);

  Options(int argc, char**argv);
};

#endif