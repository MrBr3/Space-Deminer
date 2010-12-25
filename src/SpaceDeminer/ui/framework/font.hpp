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

#ifndef _SPACEDEMINER_UI_FRAMEWORK_FONT_H_
#define _SPACEDEMINER_UI_FRAMEWORK_FONT_H_

#include "./resources.hpp"
#include <pangomm.h>

namespace Framework
{
  class PaintTool;
  class Font : public Resource
  {
  public:
    Real get_r()const{return _r;}
    Real get_g()const{return _g;}
    Real get_b()const{return _b;}
    Real get_a()const{return _a;}

    const Glib::ustring& get_family()const{return _family;}
    bool get_italic()const{return _italic;}
    bool get_bold()const{return _bold;}

    gdouble get_height()const{return _height;}

    static ResPtr<Font> create(const Glib::ustring& family, gdouble height, bool italic=false, bool bold=false, bool use_memnotic=true, Real r=0.5f, Real g=0.5f, Real b=0.5f, Real a=1.f);

    /**
     * \param text_width_ if you already known the size of the text, you can use text_width_ and text_height_ to increase performance - in that case you are responsible to recalculate the size if something has changed. If <0 the width will be recalculated and stored in text_width_
     * \param text_height_ (see text_width_). If <0 the width height be recalculated and stored in text_height_
     * */
    virtual void draw_simple(PaintTool& paint_tool, const Glib::ustring& text, int x, int y, Real x_align=0.5f, Real y_align=0.5f, int* text_width_=nullptr, int* text_height_=nullptr)=0;
    virtual void get_size(const Glib::ustring& text, int& width, int& height)=0;

  protected:
    virtual void on_init()=0;
    virtual void on_deinit()=0;

    void init();
    void init(const Glib::ustring& family, gdouble height, bool italic=false, bool bold=false, bool use_memnotic=true, Real r=0.5f, Real g=0.5f, Real b=0.5f, Real a=1.f);
    void deinit();

    Font();
    ~Font()throw()
    {
      g_assert(!get_is_loaded());
    }

  private:
    Real _r, _g, _b, _a;
    Glib::ustring _family;
    bool _italic, _bold, _use_memnotic;
    gdouble _height;

    void v_purge_resource();
    void v_reload_resource();
  };

  class Engine;
  class Image;
  class ImageFont : public Font
  {
  public:
    typedef Glib::ustring::value_type unichar;

    ImageFont();

    void draw_simple(PaintTool& paint_tool, const Glib::ustring& text, int x, int y, Real x_align, Real y_align, int* text_width_, int* text_height_);
    void get_size(const Glib::ustring& text, int& width, int& height);

    void on_init();
    void on_deinit();

  private:
    ObsLink<Engine> _engine;

    class Character : public Refable
    {
    public:
      ResPtr<Image> img;
      int width;

      ~Character()throw(){}
    };

    int calc_line_width(const Glib::ustring::const_iterator& s_from, const Glib::ustring& text);

    typedef std::map<unichar,  Glib::RefPtr<Character> > MapCharacters;

    MapCharacters _characters;

  protected:
    ResPtr<Image> dummy_img;
    int _real_height;

    /** Return's a valid reference to the character c
     * */
    const Character& get_char(unichar c)
    {
      ensure_character(c);
      g_assert(_characters.find(c)!=_characters.end());
      g_assert(_characters.find(c)->second);
      return *_characters.find(c)->second.operator->();
    }

    void ensure_characters(const Glib::ustring& str);

    void ensure_character(unichar c)
    {
      if(_characters.find(c)==_characters.end())
        generate_character(c);
    }

    void generate_character(unichar c)
    {
      ensure_characters(Glib::ustring(2,c));
    }

    ~ImageFont()throw();
  };
}

#endif