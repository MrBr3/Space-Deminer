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

#include "dependencies.hpp"
#include <GL/glew.h>
#include <gtkglmm.h>
#include "./../extern/glim/Main.h"

#ifndef _SPACE_DEMINER_BASE_OGL_H_
#define _SPACE_DEMINER_BASE_OGL_H_

enum TextureHint
{
  TEXTURE_HINT_NOT_TILABLE = 0,
  TEXTURE_HINT_NOT_SIZEABLE = 0,
  TEXTURE_HINT_NO_MIPMAPPING = 0,
  TEXTURE_HINT_DEFAULT = 0, ///> Identical to <tt>TEXTURE_HINT_NOT_TILABLE | TEXTURE_HINT_NOT_SIZEABLE | TEXTURE_HINT_NO_MIPMAPPING</tt>
  TEXTURE_HINT_TILABLE = 1,
  TEXTURE_HINT_SIZEABLE = 2,
  TEXTURE_HINT_MIPMAPS = 4,
  TEXTURE_HINT_FIT_SIZE = 8,///> When activated, the Texture's size will be resized to a 2^n x 2^m size (like Mipmaps and Tilable Textures)
};

inline TextureHint operator|(TextureHint a, TextureHint b){return TextureHint(int(a)|int(b));}
inline TextureHint operator&(TextureHint a, TextureHint b){return TextureHint(int(a)&int(b));}
inline TextureHint operator^(TextureHint a, TextureHint b){return TextureHint(int(a)^int(b));}
inline TextureHint operator~(TextureHint a){return TextureHint(~int(a));}
inline TextureHint& operator|=(TextureHint& a, TextureHint b){return a = a|b;}
inline TextureHint& operator&=(TextureHint& a, TextureHint b){return a = a&b;}
inline TextureHint& operator^=(TextureHint& a, TextureHint b){return a = a^b;}
//inline TextureHint& operator~=(TextureHint& a){return a = ~a;}

/** \brief Sets the content of an OpenGL Texture based on a Pixbuf
 *
 * \note The OpenGL Texture won't be generated. You must call <tt>glGenTextures(...)</tt> and  <tt>glBindTexture(GL_TEXTURE_2D, ..);</tt> by yourself before calling this function
 * */
void set_gl_texture_content(const Glib::RefPtr<const Gdk::Pixbuf>& pixbuf_, TextureHint hint_, Real& coord_max_u, Real& coord_max_v);

/** \brief Overloads and Wraps  around the set_gl_texture_content function.
 * */
inline void set_gl_texture_content(const Glib::RefPtr<const Gdk::Pixbuf>& pixbuf_, TextureHint hint_=TEXTURE_HINT_DEFAULT)
{
  Real dummy_a, dummy_b;
  set_gl_texture_content(pixbuf_, hint_, dummy_a, dummy_b);
}

#endif
