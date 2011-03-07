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

#define N_TEXTURE_SLICES 42
#define N_SAMPLES 512

GLuint View3D::CurveTexture::next_texture_slize_to_create = 0;
GLuint View3D::CurveTexture::texture_id = 0;
gsize View3D::CurveTexture::n_referenced = 0;
GLfloat* View3D::CurveTexture::samples = nullptr;

View3D::CurveTexture::CurveTexture()
{
  g_assert(N_TEXTURE_SLICES > next_texture_slize_to_create);

  this_slice = next_texture_slize_to_create;
  ++next_texture_slize_to_create;

  initialized = false;
}

View3D::CurveTexture::~CurveTexture()throw()
{
  if(initialized)
  {
    g_assert(n_referenced>0);

    --n_referenced;

    if(n_referenced==0)
    {
      g_assert(samples);
      g_assert(texture_id);
      glDeleteTextures(1, &texture_id);
      texture_id = 0;

      delete[] samples;
    }
  }
}

void View3D::CurveTexture::init()
{
  if(n_referenced==0)
  {
    g_assert(!texture_id);

    glGenTextures(1, &texture_id);
    bind();

    glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    samples = new GLfloat[4*N_TEXTURE_SLICES*N_SAMPLES];
  }

  g_assert(texture_id);
  g_assert(samples);

  ++n_referenced;

  fill_texture();

  initialized = true;
}

void View3D::CurveTexture::set(GradientPtr g)
{
  if(gradient)
  {
    g_assert(gradient==g);
    return;
  }
  g_assert(!curve);
  g_assert(!colorcurve);

  g->force_n_samples(N_SAMPLES);
  gradient = g;
  gradient->signal_changed().connect(sigc::mem_fun(*this, &View3D::CurveTexture::fill_texture));

  init();
}

void View3D::CurveTexture::set(CurvePtr c)
{
  if(curve)
  {
    g_assert(curve==c);
    return;
  }
  g_assert(!gradient);
  g_assert(!colorcurve);

  c->force_n_samples(N_SAMPLES);
  curve = c;
  curve->signal_changed().connect(sigc::mem_fun(*this, &View3D::CurveTexture::fill_texture));

  init();
}

void View3D::CurveTexture::set(ColorCurvePtr c)
{
  if(colorcurve)
  {
    g_assert(colorcurve==c);
    return;
  }
  g_assert(!gradient);
  g_assert(!curve);

  c->force_n_samples(N_SAMPLES);
  colorcurve = c;
  colorcurve->signal_changed().connect(sigc::mem_fun(*this, &View3D::CurveTexture::fill_texture));

  init();
}

void View3D::CurveTexture::bind()
{
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_1D_ARRAY, texture_id);
}

void View3D::CurveTexture::unbind()
{
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_1D_ARRAY, 0);
}

void View3D::CurveTexture::fill_texture()
{
  g_assert(n_true_booleans(gradient, curve, colorcurve)==1);
  g_assert(texture_id);

  bind();

  GLfloat* data = &samples[N_SAMPLES*4*this_slice];

  if(gradient)
  {
    g_assert(gradient->get_curve1()->get_n_samples()==N_SAMPLES);
    g_assert(gradient->get_curve2()->get_n_samples()==N_SAMPLES);
    g_assert(gradient->get_curve3()->get_n_samples()==N_SAMPLES);
    g_assert(gradient->get_curve4()->get_n_samples()==N_SAMPLES);
    g_assert(N_SAMPLES==gradient->get_n_samples());

    const std::vector<ColorRGBA>& src = gradient->get_samples();

    for(gsize i=0; i<N_SAMPLES; i++)
    {
      data[i*4+0] = src[i].r;
      data[i*4+1] = src[i].g;
      data[i*4+2] = src[i].b;
      data[i*4+3] = src[i].a;
    }
  }else if(colorcurve)
  {
    g_assert(colorcurve->get_value_curve()->get_n_samples()==N_SAMPLES);
    g_assert(colorcurve->get_red_curve()->get_n_samples()==N_SAMPLES);
    g_assert(colorcurve->get_green_curve()->get_n_samples()==N_SAMPLES);
    g_assert(colorcurve->get_blue_curve()->get_n_samples()==N_SAMPLES);
    g_assert(colorcurve->get_alpha_curve()->get_n_samples()==N_SAMPLES);
    g_assert(N_SAMPLES==colorcurve->get_n_samples());

    const std::vector<ColorRGBA>& src = colorcurve->get_samples();

    for(gsize i=0; i<N_SAMPLES; i++)
    {
      data[i*4+0] = src[i].r;
      data[i*4+1] = src[i].g;
      data[i*4+2] = src[i].b;
      data[i*4+3] = src[i].a;
    }
  }else if(curve)
  {
    g_assert(curve);

    const gdouble* const src = curve->get_samples();
    g_assert(N_SAMPLES==curve->get_n_samples());

    for(gsize i=0; i<N_SAMPLES; ++i)
    {
      data[i*4] = src[i];
      data[i*4+0] = src[i];
      data[i*4+1] = src[i];
      data[i*4+2] = src[i];
    }
  }

  glTexImage2D(GL_TEXTURE_1D_ARRAY, 0, GL_RGBA32F, N_SAMPLES, N_TEXTURE_SLICES, 0, GL_RGBA, GL_FLOAT, samples);

  if(!main_window)
    return;
  View3D* view = main_window->get_view_3d();
  if(!view)
    return;
  view->invalidate();
}
