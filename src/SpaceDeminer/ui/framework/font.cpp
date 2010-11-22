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

#include "./engine.hpp"

namespace Framework
{
  ResPtr<Font> Font::create(const Glib::ustring& family, gdouble height, bool italic, bool bold, Real r, Real g, Real b, Real a)
  {
    ResPtr<Font> font = Engine::get_singleton()->create_font();

    font->init(family, height, italic, bold, r, g, b, a);

    return font;
  }

  Font::Font()
  {
    _r=_g=_b=0.5f;
    _a=1.f;
    _height = 10.;

    _family = "sans";
  }

  void Font::init()
  {
    on_init();

    set_is_loaded(true);
  }

  void Font::init(const Glib::ustring& family, gdouble height, bool italic, bool bold, Real r, Real g, Real b, Real a)
  {
    _r  = r;
    _g  = g;
    _b  = b;
    _a  = a;

    _italic = italic;
    _bold = bold;

    _height = height;

    _family = family;

    init();
  }

  void Font::deinit()
  {
    if(!get_is_loaded())
      return;

    set_is_loaded(false);

    on_deinit();
  }

  void Font::v_purge_resource()
  {
    deinit();
  }

  void Font::v_reload_resource()
  {
    init();
  }

  //-----------------------------

  ImageFont::ImageFont()
  {
    _real_height  = 0;
  }

  ImageFont::~ImageFont()throw()
  {
    deinit();
  }

  void ImageFont::draw_simple(PaintTool& paint_tool, const Glib::ustring& text, int x, int y, Real x_align, Real y_align, int* text_width_, int* text_height_)
  {
    int _tmp_w=-1, _tmp_h=-1;
    int line_dist  = _real_height;

    if(!text_width_)
      text_width_ = &_tmp_w;
    if(!text_height_)
      text_height_ = &_tmp_h;

    if((*text_width_<0 || *text_height_<0) && (y_align!=0.f))
      get_size(text, *text_width_, *text_height_);

    if(y_align!=0.f)
      y -= round(*text_height_ * y_align);

    int curr_x=x;

    if(x_align)
      curr_x -= round(calc_line_width(text.begin(), text)*x_align);

    for(Glib::ustring::const_iterator s_iter=text.begin(); s_iter!=text.end(); ++s_iter)
    {
      unichar c = *s_iter;

      if(c=='\n') // TODO: Tab
      {
        curr_x=x;
        y += line_dist;
        if(x_align)
        {
          Glib::ustring::const_iterator i = s_iter;
          ++i;
          curr_x -= round(calc_line_width(i, text)*x_align);
        }
      }else if(c<32)
      {
        continue;
      }else
      {
        const Character& _c = get_char(c);
        if(c!=32)
        {
          //paint_tool.draw_color_rect(curr_x, y, _c.width, _real_height, 0.f, 0.f, 0.f, 1.f);
          paint_tool.draw_image(_c.img, curr_x, y);
        }
        curr_x += _c.width;
      }
    }
  }

  void ImageFont::get_size(const Glib::ustring& text, int& width, int& height)
  {
    int line_dist  = _real_height;

    width = 0;
    height  = _real_height;

    int curr_line_width = 0;

    for(Glib::ustring::const_iterator s_iter=text.begin(); s_iter!=text.end(); ++s_iter)
    {
      unichar c = *s_iter;

      if(c=='\n') // TODO: Tab
      {
        width = MAX(curr_line_width, width);
        curr_line_width = 0;
        height  += line_dist;
      }else if(c<32)
      {
        continue;
      }else
      {
        curr_line_width += get_char(c).width;
      }
    }
    width = MAX(curr_line_width, width);
  }

  int ImageFont::calc_line_width(const Glib::ustring::const_iterator& s_from, const Glib::ustring& text)
  {
    int w=0;

    for(Glib::ustring::const_iterator s_iter=text.begin(); s_iter!=text.end(); ++s_iter)
    {
      unichar c = *s_iter;

      if(c=='\n') // TODO: Tab
      {
        break;
      }else if(c<32)
      {
        continue;
      }else
      {
        w += get_char(c).width;
      }
    }

    return w;
  }

  void ImageFont::on_init()
  {
    ensure_characters("\x20\x7f");
    ensure_characters(_("[common-characters]"));
  }

  void ImageFont::on_deinit()
  {
    _characters.clear();
  }

  void ImageFont::ensure_characters(const Glib::ustring& str)
  {
    if(str.length()<2)
      return;

    int height  = ceil(get_height());

    int img_size  = height;

    if(img_size<=0)
    {
      g_warning("**ImageFont::ensure_characters** img_size<=0\n");
      return;
    }

    Cairo::FontExtents font_extents;
    Cairo::RefPtr<Cairo::ImageSurface> cairo_img = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, img_size<<2, img_size<<2);
    Cairo::RefPtr<Cairo::Context> cairo_context = Cairo::Context::create(cairo_img);
    std::vector<Cairo::Glyph> glyph_vector;
    glyph_vector.resize(1);
    Cairo::Glyph& glyph = glyph_vector[0];

    cairo_context->select_font_face(get_family(), get_italic() ? Cairo::FONT_SLANT_ITALIC : Cairo::FONT_SLANT_NORMAL, get_bold() ? Cairo::FONT_WEIGHT_BOLD : Cairo::FONT_WEIGHT_NORMAL);
    cairo_context->set_antialias(Cairo::ANTIALIAS_SUBPIXEL);

    cairo_context->set_font_size(get_height());
    cairo_context->get_font_extents(font_extents);
    //cairo_context->set_font_size(gdouble(get_height()) * gdouble(get_height()) / gdouble(font_extents.ascent+font_extents.descent));
    //cairo_context->get_font_extents(font_extents);
    cairo_context->translate(0., font_extents.height-font_extents.descent);

    _real_height  = ceil(font_extents.height);

    Glib::ustring::const_iterator s_iter=str.begin();

    while(s_iter!=str.end())
    {
      unichar a = *s_iter;

      ++s_iter;

      if(s_iter==str.end())
        return;

      unichar b = *s_iter;

      for(;a<=b; ++a)
      {
        if(_characters.find(a)==_characters.end())
        {
          Cairo::TextExtents text_extents;

          cairo_context->save();
          clear_cairo_img(cairo_img, get_r(), get_g(), get_b(), 0.);
          cairo_context->set_source_rgba(get_r(), get_g(), get_b(), get_a());
          glyph.index = a-29;
          glyph.x = 0;
          glyph.y = 0;
          cairo_context->get_glyph_extents(glyph_vector, text_extents);
          cairo_context->show_glyphs(glyph_vector);
          cairo_context->restore();

          int width = ceil(text_extents.x_bearing + text_extents.width);

          Glib::RefPtr<Character> character(new Character);
          Glib::RefPtr<Gdk::Pixbuf> img = create_cairo_imagesurface_from_pixbuf(cairo_img, 0, 0, width, _real_height);

          //create_cairo_imagesurface_from_pixbuf(cairo_img)->save(Glib::ustring::compose("/home/robert/Bilder/GD/font/%1.png", int(a)), "png");

          if(a>32)
          {
            if(img)
            {
              character->img  = Image::create(img);
              character->width  = width;
            }else
            {
              if(!dummy_img)
                dummy_img = Image::create(Gdk::Pixbuf::create_from_file(apply_filename_macros("$(ui-icon-path)/16/image-missing.png"), height, height, false));
              character->img  = dummy_img;
              character->width=height;
            }

            g_assert(character->img);
          }else if(a==32)
            character->width  = width;
          else
            character->width  = 0;

          _characters.insert(std::make_pair(a, character));
        }
      }

      ++s_iter;
    }
  }
}