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

#include "./Model.hpp"

namespace Private
{
  void throw_ColorCurve_parser_error(const std::string& s)
  {
    throw std::runtime_error("ColorCurve: error while parsing: \""+s+"\"");
  }

  void boolfromstring_schemestyle(bool& value, Glib::ustring::const_iterator& begin, Glib::ustring::const_iterator end);
}

using namespace Private;

Glib::ustring ColorCurve::save_to_string()const
{
  return "(colorcurve "+
          get_value_curve()->save_to_string()+" "+
          get_red_curve()->save_to_string()+" "+
          get_green_curve()->save_to_string()+" "+
          get_blue_curve()->save_to_string()+" "+
          get_alpha_curve()->save_to_string()+" "+
          Options::boolean2string(get_hide_alpha())+")";
}

void ColorCurve::load_from_string(Glib::ustring::const_iterator& begin, Glib::ustring::const_iterator end)
{
  request_no_updates();
  try
  {
    if(!compare(begin, end, "(colorcurve "))
      throw_ColorCurve_parser_error("missing beginning '(colorcurve '");

    if(begin==end)
      throw_ColorCurve_parser_error("expected '(curve '");

    value_curve->load_from_string(begin, end);

    if(begin==end)
      throw_ColorCurve_parser_error("expected '(curve '");

    if(*begin!=' ')
      throw_ColorCurve_parser_error("expected ' '");
    ++begin;

    if(begin==end)
      throw_ColorCurve_parser_error("expected '(curve '");

    red_curve->load_from_string(begin, end);

    if(begin==end)
      throw_ColorCurve_parser_error("expected '(curve '");

    if(*begin!=' ')
      throw_ColorCurve_parser_error("expected ' '");
    ++begin;

    if(begin==end)
      throw_ColorCurve_parser_error("expected '(curve '");

    green_curve->load_from_string(begin, end);

    if(begin==end)
      throw_ColorCurve_parser_error("expected '(curve '");

    if(*begin!=' ')
      throw_ColorCurve_parser_error("expected ' '");
    ++begin;

    if(begin==end)
      throw_ColorCurve_parser_error("expected '(curve '");

    blue_curve->load_from_string(begin, end);

    if(begin==end)
      throw_ColorCurve_parser_error("expected '(curve '");

    if(*begin!=' ')
      throw_ColorCurve_parser_error("expected ' '");
    ++begin;

    if(begin==end)
      throw_ColorCurve_parser_error("expected '(curve '");

    alpha_curve->load_from_string(begin, end);

    if(*begin!=' ')
      throw_ColorCurve_parser_error("expected ' '");
    ++begin;

    bool temp_bool;
    boolfromstring_schemestyle(temp_bool, begin, end);
    set_hide_alpha(temp_bool);

    if(*begin != ')')
      throw_ColorCurve_parser_error("missing ')'");
    ++begin;
  }catch(...)
  {
    unrequest_no_updates();
    invalidate_and_update();
    throw;
  }
  unrequest_no_updates();
  invalidate_and_update();
}
