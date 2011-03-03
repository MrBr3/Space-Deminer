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

#include "math.hpp"
#include "opengl.hpp"

void set_gl_texture_content(const Glib::RefPtr<const Gdk::Pixbuf>& pixbuf_, TextureHint hint_)
{
  if(!pixbuf_)
    throw std::invalid_argument("**set_gl_texture_content** invalid argument \"pixbuf_\"");

  glEnable(GL_TEXTURE_2D);

  g_assert(blog_int(16)==4);
  g_assert(blog_int(32)==5);
  g_assert(blog_int(1)==0);
  g_assert(blog_int(0)==G_MAXUINT);
  g_assert(blog_int(2)==1);
  g_assert(blog_int(4)==2);
  g_assert(blog_int(1024)==10);

  Glib::RefPtr<const Gdk::Pixbuf> pixbuf = pixbuf_;
  GLint internal_format;
  GLsizei tex_width = pixbuf->get_width(), tex_height = pixbuf->get_height();
  GLenum format, type;

  type  = GL_UNSIGNED_BYTE;

  if(pixbuf->get_n_channels()!=3 && pixbuf->get_n_channels()!=4)
  {
    g_critical("illegal Image: n channels!=3 && n channels!=4!!");
    exit(-1);
  }

  int n_dest_components ;
  if(pixbuf->get_n_channels()==3)
  {
    Glib::RefPtr<Gdk::Pixbuf> private_pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, tex_width, tex_height);

    const guint8* const src_pixels = pixbuf->get_pixels();
    guint8* const dest_pixels = private_pixbuf->get_pixels();

    gsize src_stride = pixbuf->get_rowstride();
    gsize dest_stride = private_pixbuf->get_rowstride();

    const guint8 *src_line, *src_px;
    guint8 *dest_line, *dest_px;

    dest_line = dest_pixels;
    src_line = src_pixels;

    for(gsize y=0; y<tex_height; ++y)
    {
      dest_px = dest_line;
      src_px = src_line;

      for(gsize x=0; x<tex_width; ++x)
      {
        dest_px[0]  = src_px[0];
        dest_px[1]  = src_px[1];
        dest_px[2]  = src_px[2];
        dest_px[3]  = 255;

        dest_px  += 4;
        src_px  += 3;
      }

      dest_line += dest_stride;
      src_line += src_stride;
    }

    pixbuf = private_pixbuf;
  }

  g_assert(pixbuf->get_n_channels()==4);

  internal_format = GL_RGBA8;
  format = GL_RGBA;
  n_dest_components = 4;

  guint8* text_pixels  = pixbuf->get_pixels();
  bool delete_text_pixels = false;

  if(tex_width*4 != pixbuf->get_rowstride())
  {
    delete_text_pixels = true;
    text_pixels = new guint8[tex_width*tex_height*4];

    const guint8* const src_pixels = pixbuf->get_pixels();
    guint8* const dest_pixels = text_pixels;

    gsize src_stride = pixbuf->get_rowstride();
    gsize dest_stride = tex_width*4;

    const guint8 *src_line, *src_px;
    guint8 *dest_line, *dest_px;

    dest_line = dest_pixels;
    src_line = src_pixels;

    for(gsize y=0; y<tex_height; ++y)
    {
      dest_px = dest_line;
      src_px = src_line;

      for(gsize x=0; x<tex_width; ++x)
      {
        dest_px[0]  = src_px[0];
        dest_px[1]  = src_px[1];
        dest_px[2]  = src_px[2];
        dest_px[3]  = src_px[3];

        dest_px  += 4;
        src_px  += 4;
      }

      dest_line += dest_stride;
      src_line += src_stride;
    }
  }

  GLint min_filter, mag_filter;
  GLint wrap_u, wrap_v;

  if(hint_&TEXTURE_HINT_MIPMAPS)
  {
    min_filter  = GL_LINEAR_MIPMAP_LINEAR;
    mag_filter  = GL_LINEAR;
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
    wrap_u  = GL_CLAMP_TO_EDGE;
    wrap_v  = GL_CLAMP_TO_EDGE;
  }

  glTexImage2D(GL_TEXTURE_2D, 0/*mipmaplevel*/, internal_format, tex_width, tex_height, 0/*border*/, format, type, text_pixels);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_u);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_v);

  if(hint_ & TEXTURE_HINT_MIPMAPS)
  {
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  if(delete_text_pixels)
    delete[] text_pixels;
}
