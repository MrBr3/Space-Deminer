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

#include "./MainWindow.hpp"

GLint View3D::CurveTexture::curr_curve_texture_stage = 8;

View3D::CurveTexture::CurveTexture()
{
  texture = 0;
  texture_stage = curr_curve_texture_stage;
  ++curr_curve_texture_stage;
}

View3D::CurveTexture::~CurveTexture()throw()
{
  if(texture)
  {
    glDeleteTextures(1, &texture);
    texture = 0;
  }
}

void View3D::CurveTexture::set(ConstGradientPtr g)
{
  if(gradient)
  {
    g_assert(g==gradient);
    return;
  }
  g_assert(!curve);

  gradient = g;
  gradient->signal_changed().connect(sigc::mem_fun(*this, &View3D::CurveTexture::fill_texture));

  init();
}

void View3D::CurveTexture::set(ConstCurvePtr c)
{
  if(curve)
  {
    g_assert(c==curve);
    return;
  }
  g_assert(!gradient);

  curve = c;
  curve->signal_changed().connect(sigc::mem_fun(*this, &View3D::CurveTexture::fill_texture));

  init();
}

void View3D::CurveTexture::bind()
{
  g_assert(XOR(gradient, curve));
  g_assert(texture);

  glActiveTexture(GL_TEXTURE0+texture_stage);
  glEnable(GL_TEXTURE_1D);
  glBindTexture(GL_TEXTURE_1D, texture);
}

void View3D::CurveTexture::fill_texture()
{
  g_assert(XOR(gradient, curve));

  bind();

  GLint internal_format;
  GLsizei width;
  GLenum format;
  GLfloat* data = nullptr;

  if(gradient)
  {
    internal_format = GL_RGBA32F;
    width = gradient->get_n_samples();
    format = GL_RGBA;

    g_assert(gradient->get_curve1()->get_n_samples()==width);
    g_assert(gradient->get_curve2()->get_n_samples()==width);
    g_assert(gradient->get_curve3()->get_n_samples()==width);
    g_assert(gradient->get_curve4()->get_n_samples()==width);

    const std::vector<ColorRGBA>& src = gradient->get_samples();

    data = new GLfloat[width*4];
    for(gsize i=0; i<width; i++)
    {
      data[i*4+0] = src[i].r;
      data[i*4+1] = src[i].g;
      data[i*4+2] = src[i].b;
      data[i*4+3] = src[i].a;
    }
  }else
  {
    g_assert(curve);

    internal_format = GL_RED;
    width = curve->get_n_samples();
    format = GL_RED;
    const gdouble* const src = curve->get_samples();

    data = new GLfloat[width];
    for(gsize i=0; i<width; ++i)
    {
      data[i] = src[i];
    }
  }

  glTexImage1D(GL_TEXTURE_1D, 0, internal_format, width, 0, format, GL_FLOAT, data);

  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  delete[] data;

  if(!main_window)
    return;
  View3D* view = main_window->get_view_3d();
  if(!view)
    return;
  view->invalidate();
}

void View3D::CurveTexture::init()
{
  g_assert(!texture);

  glGenTextures(1, &texture);

  fill_texture();
}
