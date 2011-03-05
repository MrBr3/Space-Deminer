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
  void throw_ColorRGBA_parser_error(const std::string& s)
  {
    throw std::runtime_error("ColorRGBA: error while parsing: \""+s+"\"");
  }

  void throw_Gradient_parser_error(const std::string& s)
  {
    throw std::runtime_error("Gradient: error while parsing: \""+s+"\"");
  }

  void bool_parser_error(const std::string& s)
  {
    throw std::runtime_error("boolean: error while parsing: \""+s+"\"");
  }

  Glib::ustring ColorRGBA2string_schemestyle(const ColorRGBA& c)
  {
    return  "(color "+
            Options::real2string(c.r)+" "+
            Options::real2string(c.g)+" "+
            Options::real2string(c.b)+" "+
            Options::real2string(c.a)+")";
  }

  void boolfromstring_schemestyle(bool& value, Glib::ustring::const_iterator& begin, Glib::ustring::const_iterator end)
  {
    if(begin==end)
      bool_parser_error("expected '0' or '1'");

    if(*begin!='1' && *begin!='0')
      bool_parser_error("expected '0' or '1'");

    value = *begin=='1';
    ++begin;
  }

  void ColorRGBAfromstring_schemestyle(ColorRGBA& color, Glib::ustring::const_iterator& begin, Glib::ustring::const_iterator end)
  {
    bool success;
    if(!compare(begin, end, "(color "))
      throw_ColorRGBA_parser_error("missing beginning '(color '");

    for(int i=0; i<3; ++i)
    {
      Glib::ustring::const_iterator tmp = begin;

      while(begin!=end)
      {
        if(*begin==' ')
          break;
        ++begin;
      }
      if(begin==end)
        throw_ColorRGBA_parser_error("(color ... ...) needs two real numbers!");

      gfloat* channel = nullptr;
      switch(i)
      {
      case 0:
        channel = &color.r;
        break;
      case 1:
        channel = &color.g;
        break;
      case 2:
        channel = &color.b;
        break;
      default:
        g_assert_not_reached();
        break;
      }
      *channel = str_to_real(Glib::ustring(tmp, begin), 0., &success);
      if(!success)
        throw_ColorRGBA_parser_error("(color ... ...) needs four real numbers!");

      if(*begin != ' ')
        throw_ColorRGBA_parser_error("(color ... ...) needs four real numbers divided by a space character");

      ++begin;
    }

    Glib::ustring::const_iterator tmp = begin;
    while(begin!=end)
    {
      if(*begin==')')
        break;
      ++begin;
    }
    if(begin==end)
      throw_ColorRGBA_parser_error("(color ... ...) needs four real numbers!");

    color.a = str_to_real(Glib::ustring(tmp, begin), 0., &success);
    if(!success)
      throw_ColorRGBA_parser_error("(color ... ...) needs four real numbers!");

    if(*begin != ')')
      throw_ColorRGBA_parser_error("missing ')'");
    ++begin;
  }
}

using namespace Private;

Glib::ustring Gradient::save_to_string()const
{
  return "(gradient "+
          ColorRGBA2string_schemestyle(get_defcolor())+" "+
          ColorRGBA2string_schemestyle(get_color1())+" "+
          ColorRGBA2string_schemestyle(get_color2())+" "+
          ColorRGBA2string_schemestyle(get_color3())+" "+
          ColorRGBA2string_schemestyle(get_color4())+" "+
          get_curve1()->save_to_string()+" "+
          get_curve2()->save_to_string()+" "+
          get_curve3()->save_to_string()+" "+
          get_curve4()->save_to_string()+" "+
          Options::boolean2string(get_use_alpha())+" "+
          Options::real2string(get_remap_a())+" "+
          Options::real2string(get_remap_b())+")";
}

void Gradient::load_from_string(Glib::ustring::const_iterator& begin, Glib::ustring::const_iterator end)
{
  request_no_updates();

  try
  {
    if(!compare(begin, end, "(gradient "))
      throw_Gradient_parser_error("missing beginning '(gradient '");

    if(begin==end)
      throw_Gradient_parser_error("expected '(color '");

    ColorRGBAfromstring_schemestyle(defcolor, begin, end);

    if(begin==end)
      throw_Gradient_parser_error("expected '(color '");

    if(*begin!=' ')
      throw_Gradient_parser_error("expected ' '");
    ++begin;

    if(begin==end)
      throw_Gradient_parser_error("expected '(color '");

    ColorRGBAfromstring_schemestyle(color1, begin, end);


    if(begin==end)
      throw_Gradient_parser_error("expected '(color '");

    if(*begin!=' ')
      throw_Gradient_parser_error("expected ' '");
    ++begin;

    if(begin==end)
      throw_Gradient_parser_error("expected '(color '");

    ColorRGBAfromstring_schemestyle(color2, begin, end);


    if(begin==end)
      throw_Gradient_parser_error("expected '(color '");

    if(*begin!=' ')
      throw_Gradient_parser_error("expected ' '");
    ++begin;

    if(begin==end)
      throw_Gradient_parser_error("expected '(color '");

    ColorRGBAfromstring_schemestyle(color3, begin, end);


    if(begin==end)
      throw_Gradient_parser_error("expected '(color '");

    if(*begin!=' ')
      throw_Gradient_parser_error("expected ' '");
    ++begin;

    if(begin==end)
      throw_Gradient_parser_error("expected '(color '");

    ColorRGBAfromstring_schemestyle(color4, begin, end);

    // curves

    if(begin==end)
      throw_Gradient_parser_error("expected '(curve '");

    if(*begin!=' ')
      throw_Gradient_parser_error("expected ' '");
    ++begin;

    if(begin==end)
      throw_Gradient_parser_error("expected '(curve '");

    curve1->load_from_string(begin, end);

    if(begin==end)
      throw_Gradient_parser_error("expected '(curve '");

    if(*begin!=' ')
      throw_Gradient_parser_error("expected ' '");
    ++begin;

    if(begin==end)
      throw_Gradient_parser_error("expected '(curve '");

    curve2->load_from_string(begin, end);

    if(begin==end)
      throw_Gradient_parser_error("expected '(curve '");

    if(*begin!=' ')
      throw_Gradient_parser_error("expected ' '");
    ++begin;

    if(begin==end)
      throw_Gradient_parser_error("expected '(curve '");

    curve3->load_from_string(begin, end);

    if(begin==end)
      throw_Gradient_parser_error("expected '(curve '");

    if(*begin!=' ')
      throw_Gradient_parser_error("expected ' '");
    ++begin;

    if(begin==end)
      throw_Gradient_parser_error("expected '(curve '");

    curve4->load_from_string(begin, end);

    //Use Alpha?

    if(*begin!=' ')
      throw_Gradient_parser_error("expected ' '");
    ++begin;

    bool temp_bool;
    boolfromstring_schemestyle(temp_bool, begin, end);
    set_use_alpha(temp_bool);

    if(*begin!=' ')
      throw_Gradient_parser_error("expected ' '");
    ++begin;

    // remap

    Glib::ustring::const_iterator tmp = begin;
    while(begin!=end)
    {
      if(*begin==' ')
        break;
      ++begin;
    }
    if(begin==end)
      throw_Gradient_parser_error("(gradient ... ) needs two remap values!");

    bool success;
    remap_a = str_to_real(Glib::ustring(tmp, begin), 0., &success);
    if(!success)
      throw_Gradient_parser_error("(gradient ... ) needs two remap values!");

    if(*begin != ' ')
      throw_Gradient_parser_error("(gradient ... ) needs two remap values divided by a space character!");

    ++begin;
    tmp = begin;
    while(begin!=end)
    {
      if(*begin==')')
        break;
      ++begin;
    }
    if(begin==end)
      throw_Gradient_parser_error("(gradient ... ) needs two remap values!");

    remap_b = str_to_real(Glib::ustring(tmp, begin), 0., &success);
    if(!success)
      throw_Gradient_parser_error("(gradient ... ) needs two remap values!");

    if(*begin != ')')
      throw_Gradient_parser_error("missing ')'");
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
