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

#include <gtkglmm.h>
#include "math.hpp"
#include "opengl.hpp"

void set_gl_texture_content(const Glib::RefPtr<const Gdk::Pixbuf>& pixbuf_, TextureHint hint_, Real& coord_max_u, Real& coord_max_v)
{
  if(!pixbuf_)
    throw std::invalid_argument("**set_gl_texture_content** invalid argument \"pixbuf_\"");

  g_assert(blog_int(16)==4);
  g_assert(blog_int(32)==5);
  g_assert(blog_int(1)==0);
  g_assert(blog_int(0)==G_MAXUINT);
  g_assert(blog_int(2)==1);
  g_assert(blog_int(4)==2);
  g_assert(blog_int(1024)==10);

  Glib::RefPtr<Gdk::Pixbuf> private_pixbuf;
  Glib::RefPtr<const Gdk::Pixbuf> pixbuf = pixbuf_;
  GLint internal_format;
  GLsizei tex_width = pixbuf->get_width(), tex_height = pixbuf->get_height();
  GLenum format, type;
  bool recalc = false;

  type  = GL_UNSIGNED_BYTE;

  coord_max_u = 1.f;
  coord_max_v = 1.f;

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

  if((hint_&TEXTURE_HINT_TILABLE) || (hint_&TEXTURE_HINT_MIPMAPS) || (TEXTURE_HINT_FIT_SIZE&hint_))
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
    if(src_height>0)
      src_line  -= scr_rowstride; // There, as the last "src_line  += scr_rowstride;" brings the line out of range
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

  if(hint_&TEXTURE_HINT_MIPMAPS)
  {
    min_filter  = GL_LINEAR_MIPMAP_LINEAR;
    mag_filter  = GL_LINEAR_MIPMAP_LINEAR;
  }else if(hint_&TEXTURE_HINT_SIZEABLE)
  {
    min_filter  = GL_LINEAR;
    mag_filter  = GL_LINEAR;
  }else
  {
    min_filter  = GL_NEAREST;
    mag_filter  = GL_NEAREST;
  }

  if(hint_&TEXTURE_HINT_TILABLE)
  {
    wrap_u  = GL_REPEAT;
    wrap_v  = GL_REPEAT;
  }else
  {
    wrap_u  = GL_CLAMP;
    wrap_v  = GL_CLAMP;
  }

  if(hint_ & TEXTURE_HINT_MIPMAPS)
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
}