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

 #include "MainWindow.hpp"

Texture::Texture(const Glib::RefPtr<ImageFile>& file)
{
  _texture  = 0;
  _initialized  = false;

  imagefile = file;
  imagefile->signal_imagefile_changed().connect(sigc::mem_fun(*this, &Texture::init));
}

Texture::Texture(const Glib::RefPtr<Gdk::Pixbuf>& pixbuf)
{
  _texture  = 0;
  _initialized  = false;

  this->pixbuf = pixbuf;
}

Texture::~Texture()throw()
{
  deinit();
}

void Texture::bind(guint i, bool do_bind)
{
  g_assert(i<ENSURED_N_TEXTURE_STAGES);

  if(!do_bind)
  {
    unbind(i);
    return;
  }

  if(!_initialized)
    return;

  glActiveTexture(GL_TEXTURE0+i);
  glBindTexture(GL_TEXTURE_2D, _texture);
}

void Texture::set_wrapping(WrapMode wm_u, WrapMode wm_v, int texture_stage)
{
  if(!_initialized)
    return;

  GLint n_texture_stages;
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &n_texture_stages);

  g_assert(n_texture_stages>8);

  if(texture_stage<0 || texture_stage>n_texture_stages-1)
    texture_stage = n_texture_stages-1;

  GLint p_u, p_v;

  switch(wm_u)
  {
  case CLAMP:
    p_u = GL_CLAMP_TO_EDGE;
   break;
  case REPEAT:
  default:
    p_u = GL_REPEAT;
  }
  switch(wm_v)
  {
  case CLAMP:
    p_v = GL_CLAMP_TO_EDGE;
   break;
  case REPEAT:
  default:
    p_v = GL_REPEAT;
  }

  glActiveTexture(GL_TEXTURE0+texture_stage);
  glBindTexture(GL_TEXTURE_2D, _texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, p_u);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, p_v);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::unbind(guint i)
{
  g_assert(i<ENSURED_N_TEXTURE_STAGES);

  glActiveTexture(GL_TEXTURE0+i);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::deinit()
{
  if(!_initialized)
    return;

  glDeleteTextures(1, &_texture);
  _texture  = 0;
  _initialized  = false;
}

void Texture::init()
{
  deinit();

  Glib::RefPtr<Gdk::Pixbuf> pb = pixbuf;

  if(!pb)
    pb = imagefile->create_pixbuf();

  if(!pb)
    return;

  glGenTextures(1, &_texture);
  glBindTexture(GL_TEXTURE_2D, _texture);
  _initialized  = true;

  set_gl_texture_content(pb, TEXTURE_HINT_MIPMAPS);
}
