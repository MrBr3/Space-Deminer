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

RingMesh::RingMesh()
{
  _initialized  = false;
  _n_triangles  = 0;

  _vertex_buffer_triangles  = 0;
  _vertex_buffer_uv = 0;

  ring_uv_factor = 0;
}

RingMesh::~RingMesh()throw()
{
  deinit();
}

void RingMesh::init(gsize n_segments, GLuint ring_uv_factor)
{
  g_assert(!_initialized);

  set_segment_division(n_segments);

  _initialized  = true;

  this->ring_uv_factor = ring_uv_factor;
}

void RingMesh::deinit()
{
  if(_vertex_buffer_triangles)
    glDeleteBuffers(1, &_vertex_buffer_triangles);
  if(_vertex_buffer_uv)
    glDeleteBuffers(1, &_vertex_buffer_uv);

  _vertex_buffer_triangles  = 0;
  _vertex_buffer_uv  = 0;

  _n_triangles  = 0;
}

void RingMesh::render(gfloat width, gfloat outer_radius)
{
  std::cout<<"uncomment \"glGetUniformLocation(ring_program, \"ring_uv_factor\")\"\n";
  exit(0);

  if(!_initialized || width<=0.f || outer_radius<=1.f || !ring_uv_factor)
    return;

  glLoadIdentity();

  glUniform1f(ring_uv_factor, outer_radius/((outer_radius-1.f)*width));

  g_assert(_vertex_buffer_triangles);
  g_assert(_vertex_buffer_uv);
  g_assert(_n_triangles>=12);

  glBindBuffer(GL_ARRAY_BUFFER,_vertex_buffer_triangles);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER,_vertex_buffer_uv);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(1);

  glDrawArrays(GL_TRIANGLES, 0, _n_triangles*3);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

void RingMesh::set_segment_division(gsize n_segments)
{
  _n_triangles  = MAX(12, n_segments);

  Polygon* triangle_buffer  = new Polygon[_n_triangles];
  UV* uv_buffer  = new UV[_n_triangles];

  Vector3* points = new Vector3[_n_triangles];

  gfloat a  = 0.f;
  gfloat ia  = PI2/gfloat(_n_triangles);
  for(gsize i=0; i<_n_triangles; ++i)
  {
    points[i] = Vector3(cos(a), sin(a), 0.f);

    a += ia;
  }

  for(gsize i=0; i<_n_triangles; ++i)
  {
    uv_buffer[i].a.x  = 0.0f;
    uv_buffer[i].a.y  = 0.5f;
    uv_buffer[i].b.x  = 0.0f;
    uv_buffer[i].b.y  = 0.5f;
    uv_buffer[i].c.x  = 1.0f;
    uv_buffer[i].c.y  = 0.5f;

    triangle_buffer[i].a  = points[i];
    triangle_buffer[i].b  = points[(i+1)%_n_triangles];
    triangle_buffer[i].c  = Vector3(0.f, 0.f, 0.f);
  }

  glGenBuffers(1,&_vertex_buffer_triangles);
  glBindBuffer(GL_ARRAY_BUFFER,_vertex_buffer_triangles);
  glBufferData(GL_ARRAY_BUFFER,_n_triangles * sizeof(Polygon), triangle_buffer, GL_STATIC_DRAW);

  glGenBuffers(1,&_vertex_buffer_uv);
  glBindBuffer(GL_ARRAY_BUFFER,_vertex_buffer_uv);
  glBufferData(GL_ARRAY_BUFFER,_n_triangles * sizeof(UV), uv_buffer, GL_STATIC_DRAW);

  signal_changed().emit();

  delete[] points;
  delete[] triangle_buffer;
  delete[] uv_buffer;
}
