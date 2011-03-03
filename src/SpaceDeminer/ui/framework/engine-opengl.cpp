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

      if(pixbuf_->get_width()<1)
        return;
      if(pixbuf_->get_height()<1)
        return;

      init();

      _tex_width = pixbuf_->get_width();
      _tex_height = pixbuf_->get_height();

      set_width(_tex_width);
      set_height(_tex_height);

      set_gl_texture_content(pixbuf_, _hint);

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

        glTexCoord2f(0.f, 1.f);
        glVertex2f(x, y+height);

        glTexCoord2f(1.f, 1.f);
        glVertex2f(x+width, y+height);

        glTexCoord2f(1.f, 0.f);
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

      Real min_u  = Real(sub_img_x)/Real(gl_tex->get_width());
      Real min_v  = Real(sub_img_y)/Real(gl_tex->get_height());

      Real max_u  = Real(sub_img_x+sub_img_width) /Real(gl_tex->get_width());
      Real max_v  = Real(sub_img_y+sub_img_height)/Real(gl_tex->get_height());

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
