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
#include "./window.hpp"
#include <gtkglmm.h>

namespace Framework
{
  class ImageGL : public Image
  {
    void init()
    {
      glGenTextures(1, &_texture);
      glBindTexture(GL_TEXTURE_2D, _texture);
    }

    void init(const Glib::RefPtr<const Gdk::Pixbuf>& pixbuf_)
    {
      deinit();

      g_assert(blog_int(16)==4);
      g_assert(blog_int(32)==5);
      g_assert(blog_int(1)==0);
      g_assert(blog_int(0)==G_MAXUINT);
      g_assert(blog_int(2)==1);
      g_assert(blog_int(4)==2);
      g_assert(blog_int(1024)==10);

      if(pixbuf_->get_width()<1)
        return;
      if(pixbuf_->get_height()<1)
        return;

      init();

      Glib::RefPtr<Gdk::Pixbuf> private_pixbuf;
      Glib::RefPtr<const Gdk::Pixbuf> pixbuf = pixbuf_;
      GLint internal_format;
      GLsizei tex_width = pixbuf->get_width(), tex_height = pixbuf->get_height();
      GLenum format, type;
      bool recalc = false;

      _tex_width  = tex_width;
      _tex_height  = tex_height;

      set_width(_tex_width);
      set_height(_tex_height);

      coord_max_u = 1.f;
      coord_max_v = 1.f;

      type  = GL_UNSIGNED_BYTE;

      int n_dest_components ;
      if(pixbuf->get_has_alpha() || pixbuf->get_n_channels()==4)
      {
        internal_format = GL_RGBA;
        format = GL_RGBA;

        n_dest_components = 4;
      }else
      {
        if(pixbuf->get_n_channels()!=3)
          throw std::invalid_argument("'pixbuf' should have 3 or 4 channels!");

        internal_format = GL_RGB;
        format = GL_RGB;

        n_dest_components = 3;
      }

      if((_hint&HINT_TILABLE) || (_hint&HINT_MIPMAPS))
      {
        tex_width = 1<<(blog_int(tex_width)+1);
        tex_height = 1<<(blog_int(tex_height)+1);

        if(tex_width!=pixbuf->get_width() || tex_width!=pixbuf->get_height())
        {
          private_pixbuf  = pixbuf->scale_simple(tex_width, tex_height, Gdk::INTERP_HYPER);
          pixbuf  = private_pixbuf;
        }
      }else
      {
        if(tex_width!=blog_int(tex_width))
        {
          tex_width  = 1<<(blog_int(tex_width)+1);
          coord_max_u = Real(pixbuf->get_width())/Real(tex_width);
          recalc  = true;
        }
        if(tex_height!=blog_int(tex_height))
        {
          tex_height = 1<<(blog_int(tex_height)+1);
          coord_max_v = Real(pixbuf->get_height())/Real(tex_height);
          recalc  = true;
        }
      }

      bool delete_tex_pixels  = false;
      guint8* text_pixels  = pixbuf->get_pixels();

      if(recalc)
      {
        text_pixels = new guint8[tex_width*tex_height*n_dest_components];
        delete_tex_pixels = true;

        const guint8* src_line  = pixbuf->get_pixels();
              guint8* dest_line  = text_pixels;
        const guint8* src_px;
              guint8* dest_px;

        const int scr_rowstride  = pixbuf->get_rowstride();
        const int dest_rowstride  = tex_width*n_dest_components;
        const int src_width  = pixbuf->get_width();
        const int src_height  = pixbuf->get_height();

        const int n_src_components  = pixbuf->get_n_channels();

        GLuint y=0;
        GLuint x;
        for(; y<src_height; ++y)
        {
          for(x=0; x<src_width; ++x)
          {
            src_px  = &src_line[n_src_components*x];
            dest_px  = &dest_line[n_dest_components*x];

            dest_px[0]  = src_px[0];
            dest_px[1]  = src_px[1];
            dest_px[2]  = src_px[2];
            if(n_dest_components==4)
              dest_px[3]  = src_px[3];
          }
          for(; x<tex_width; ++x)
          {
            dest_px  = &dest_line[n_dest_components*x];

            dest_px[0]  = src_px[0];
            dest_px[1]  = src_px[1];
            dest_px[2]  = src_px[2];
            if(n_dest_components==4)
              dest_px[3]  = src_px[3];
          }

          src_line  += scr_rowstride;
          dest_line  += dest_rowstride;
        }
        for(; y<tex_height; ++y)
        {
          for(x=0; x<src_width; ++x)
          {
            src_px  = &src_line[n_src_components*x];
            dest_px  = &dest_line[n_dest_components*x];

            dest_px[0]  = src_px[0];
            dest_px[1]  = src_px[1];
            dest_px[2]  = src_px[2];
            if(n_dest_components==4)
              dest_px[3]  = src_px[3];
          }
          for(; x<tex_width; ++x)
          {
            dest_px  = &dest_line[n_dest_components*x];

            dest_px[0]  = src_px[0];
            dest_px[1]  = src_px[1];
            dest_px[2]  = src_px[2];
            if(n_dest_components==4)
              dest_px[3]  = src_px[3];
          }

          dest_line  += dest_rowstride;
        }
      }

      GLint min_filter, mag_filter;
      GLint wrap_u, wrap_v;

      if(_hint&HINT_MIPMAPS)
      {
        min_filter  = GL_LINEAR_MIPMAP_LINEAR;
        mag_filter  = GL_LINEAR_MIPMAP_LINEAR;
      }else if(_hint&HINT_SIZEABLE)
      {
        min_filter  = GL_LINEAR;
        mag_filter  = GL_LINEAR;
      }else
      {
        min_filter  = GL_NEAREST;
        mag_filter  = GL_NEAREST;
      }

      if(_hint&HINT_TILABLE)
      {
        wrap_u  = GL_REPEAT;
        wrap_v  = GL_REPEAT;
      }else
      {
        wrap_u  = GL_CLAMP;
        wrap_v  = GL_CLAMP;
      }

      if(_hint & HINT_MIPMAPS)
      {
        GLint level = 0;
        while(tex_width>1 && tex_height>>1)
        {
          glTexImage2D(GL_TEXTURE_2D, level, internal_format, tex_width, tex_height, 0/*border*/, format, type, pixbuf->get_pixels());

          tex_height  = MAX(1, tex_height>>1);
          tex_width  = MAX(1, tex_width>>1);
          ++level;

          private_pixbuf  = pixbuf->scale_simple(tex_width, tex_height, Gdk::INTERP_HYPER);
          pixbuf  = private_pixbuf;
        }
      }else
      {
        glTexImage2D(GL_TEXTURE_2D, 0/*mipmaplevel*/, internal_format, tex_width, tex_height, 0/*border*/, format, type, text_pixels);
      }

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_u);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_v);

      if(delete_tex_pixels)
      {
        delete[] text_pixels;
      }

      set_is_loaded(true);
    }

    void deinit()
    {
      set_is_loaded(false);
      if(_texture==0)
        return;

      _tex_width  = 0;
      _tex_height  = 0;

      set_width(0.f);
      set_height(0.f);

      coord_max_u = 0.f;
      coord_max_v = 0.f;

      glDeleteTextures(1, &_texture);
      _texture  = 0;
    }
  public:

    enum Type
    {
      TYPE_PIXBUF
    };

    void bind()
    {
      glBindTexture(GL_TEXTURE_2D, _texture);
    }

    void unbind()
    {
      glBindTexture(GL_TEXTURE_2D, 0);
    }

    void v_purge_resource()
    {
      deinit();
    }

    void v_reload_resource()
    {
      deinit();

      g_assert(_texture==0);

      switch(_type)
      {
      case TYPE_PIXBUF:
        g_assert(_slot_recreate);
        init(_slot_recreate());
        break;
      default:
        g_assert_not_reached();
      }
    }

    ImageGL(const Glib::RefPtr<Gdk::Pixbuf>& pixbuf, Hint hint, const Image::SlotRecreateFromPixbuf& slot_recreate) : _hint(hint), _type(TYPE_PIXBUF), _slot_recreate(slot_recreate)
    {
      if(!pixbuf)
        throw std::invalid_argument("pixbuf");
      if(!_slot_recreate)
        throw std::invalid_argument("_slot_recreate");

      _texture  = 0;
      _slot_recreate  = slot_recreate;
      init(pixbuf);
    }
    ~ImageGL()throw()
    {
      deinit();
    }

    Image::SlotRecreateFromPixbuf _slot_recreate;
    GLuint _texture;
    Hint _hint;
    Type _type;

    int _tex_width, _tex_height;

    Real coord_max_u, coord_max_v;
  };

  class PaintToolGL : public PaintTool
  {
  public:
    int _full_area_width;
    int _full_area_height;

    void on_scissor_changed()
    {
      const ScissorRect& srect  = get_top_scissor_rect();

      //std::cout<<"srect.left "<<srect.left<<" srect.top "<<srect.top<<" srect.right "<<srect.right<<" srect.bottom "<<srect.bottom<<"\n";

      glScissor(srect.left, calc_rect_gl_pos_y(srect.top, srect.bottom-srect.top), srect.right-srect.left, srect.bottom-srect.top);
    }

    void on_offset_changed(int dx, int dy)
    {
      glTranslatef(dx, dy, 0.f);
    }

    int calc_rect_gl_pos_y(int y, int h)
    {
      return _full_area_height-(y+h);
    }
    void draw_image(const ResPtr<Image>& img, Real x, Real y, Real width, Real height)
    {
      glEnable(GL_TEXTURE_2D);
      glColor4f(1.f, 1.f, 1.f, 1.f);

      ImageGL* gl_tex = dynamic_cast<ImageGL*>(img.operator->());

      g_assert(gl_tex);

      gl_tex->bind();

      glBegin(GL_QUADS);
        glTexCoord2f(0.f, 0.f);
        glVertex2f(x, y);

        glTexCoord2f(0.f, gl_tex->coord_max_v);
        glVertex2f(x, y+height);

        glTexCoord2f(gl_tex->coord_max_u, gl_tex->coord_max_v);
        glVertex2f(x+width, y+height);

        glTexCoord2f(gl_tex->coord_max_u, 0.f);
        glVertex2f(x+width, y);
      glEnd();

      gl_tex->unbind();
    }
    void draw_image(const ResPtr<Image>& img, Real x, Real y, Real width, Real height, Real sub_img_x, Real sub_img_y, Real sub_img_width, Real sub_img_height)
    {
      glEnable(GL_TEXTURE_2D);
      glColor4f(1.f, 1.f, 1.f, 1.f);

      ImageGL* gl_tex = dynamic_cast<ImageGL*>(img.operator->());

      g_assert(gl_tex);
      g_assert(gl_tex->_tex_width);
      g_assert(gl_tex->_tex_height);
      g_assert(gl_tex->get_width());
      g_assert(gl_tex->get_height());

      gl_tex->bind();

      Real min_u  = gl_tex->coord_max_u*Real(sub_img_x)/Real(gl_tex->get_width());
      Real min_v  = gl_tex->coord_max_v*Real(sub_img_y)/Real(gl_tex->get_height());

      Real max_u  = gl_tex->coord_max_u*Real(sub_img_x+sub_img_width) /Real(gl_tex->get_width());
      Real max_v  = gl_tex->coord_max_v*Real(sub_img_y+sub_img_height)/Real(gl_tex->get_height());

      glBegin(GL_QUADS);
        glTexCoord2f(min_u, min_v);
        glVertex2f(x, y);

        glTexCoord2f(min_u, max_v);
        glVertex2f(x, y+height);

        glTexCoord2f(max_u, max_v);
        glVertex2f(x+width, y+height);

        glTexCoord2f(max_u, min_v);
        glVertex2f(x+width, y);
      glEnd();

      gl_tex->unbind();
    }
    void draw_color_rect(Real x, Real y, Real width, Real height, Real r, Real g, Real b, Real a)
    {
      if(width==0 || height==0)
        return;

      glDisable(GL_TEXTURE_2D);

      glColor4f(r, g, b, a);
      glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x, y+height);
        glVertex2f(x+width, y+height);
        glVertex2f(x+width, y);
      glEnd();
    }
    PaintToolGL(const Gdk::Region& invalid_region, const WindowManager& wm) : PaintTool(invalid_region, wm)
    {
      _full_area_height  = wm.get_height();
      _full_area_width   = wm.get_width();

      glViewport(0.f, 0.f, _full_area_width, _full_area_height);

      glDisable(GL_SCISSOR_TEST);
      Gdk::Rectangle area;
      /*get_invalid_region().get_clipbox(area);*/area  = Gdk::Rectangle(0, 0, _full_area_width, _full_area_height);
      g_assert(get_scissor_stack_depth()==0);
      push_scissor(area);

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(0.f, _full_area_width, _full_area_height, 0.f, -1.f, 1.f);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

      glClearStencil(0);
      glClear(GL_STENCIL_BUFFER_BIT);
      glDisable(GL_STENCIL_TEST);
      glStencilFunc(GL_EQUAL, 1, 1);
      glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

      Glib::ArrayHandle<Gdk::Rectangle> invalid_rectangles  = invalid_region.get_rectangles();

      glBegin(GL_QUADS);
        for(Glib::ArrayHandle<Gdk::Rectangle>::const_iterator r_iter  = invalid_rectangles.begin(); r_iter  != invalid_rectangles.end(); ++r_iter)
        {
          const Gdk::Rectangle& rect  = *r_iter;
          gfloat x  = rect.get_x();
          gfloat y  = rect.get_y();
          gfloat x2  = x+rect.get_width();
          gfloat y2  = y+rect.get_height();

          glVertex3f(x , y , 0.f);
          glVertex3f(x , y2, 0.f);
          glVertex3f(x2, y2, 0.f);
          glVertex3f(x2, y , 0.f);
        }
      glEnd();

      glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
      glEnable(GL_STENCIL_TEST);

      glClearColor(0.f, 0.f, 0.f, 1.f);
      glClear(GL_COLOR_BUFFER_BIT);

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      glEnable(GL_SCISSOR_TEST);
      glDisable(GL_STENCIL_TEST);
    }
    ~PaintToolGL()throw()
    {
    }
  };

  class EngineGL : public Engine
  {
  public:
    EngineGL()
    {
    }
    ~EngineGL()throw()
    {
    }

    ResPtr<Image> v_create_image(const Glib::RefPtr<Gdk::Pixbuf>& pixbuf, Image::Hint hint, const Image::SlotRecreateFromPixbuf& slot_recreate)
    {
      return ResPtr<ImageGL>(new ImageGL(pixbuf, hint, slot_recreate));
    }

    Glib::RefPtr<PaintTool> create_paint_tool(const Gdk::Region& invalid_region, const WindowManager& wm)
    {
      return Glib::RefPtr<PaintToolGL>(new PaintToolGL(invalid_region, wm));
    }
  };

  Glib::RefPtr<Engine> Engine::create_gl_engine(const Glib::RefPtr<Gdk::GL::Context>& context)
  {
    return Glib::RefPtr<EngineGL>(new EngineGL);
  }
}