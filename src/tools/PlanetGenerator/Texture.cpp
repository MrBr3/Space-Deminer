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

Texture::~Texture()throw()
{
  deinit();
}

void Texture::bind()
{
  if(!_initialized)
    return;

  glBindTexture(GL_TEXTURE_2D, _texture);
}

void Texture::unbind()
{
  if(!_initialized)
    return;

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

  Glib::RefPtr<Gdk::Pixbuf> pb  = imagefile->create_pixbuf();

  if(!pb)
    return;

  glGenTextures(1, &_texture);
  glBindTexture(GL_TEXTURE_2D, _texture);
  _initialized  = true;

  set_gl_texture_content(pb, TEXTURE_HINT_SIZEABLE|TEXTURE_HINT_FIT_SIZE);
}