/* Space Deminer is a small 2D Arcade Game. Your task ist to eliminate
 * intelligent Mines created in the last galactic war.
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

SphereMesh::SphereMesh()
{
  _vertex_buffer_triangles  = 0;
  _vertex_buffer_normals  = 0;
  _vertex_buffer_uv_warped  = 0;
  _vertex_buffer_uv_rectangular  = 0;

  _n_triangles  = 0;

  _use_warped_uv  = false;

  set_segment_division(42);
}

SphereMesh::~SphereMesh()throw()
{
  deinit();
}

void SphereMesh::render()
{
  g_assert(_vertex_buffer_triangles);
  g_assert(_vertex_buffer_normals);
  g_assert(_vertex_buffer_uv_warped);
  g_assert(_vertex_buffer_uv_rectangular);
  g_assert(_n_triangles>12);

  glEnableClientState(GL_VERTEX_ARRAY);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB,_vertex_buffer_triangles);
  glVertexPointer(3,GL_FLOAT,0,0);

  glEnableClientState(GL_NORMAL_ARRAY);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB,_vertex_buffer_normals);
  glNormalPointer(GL_FLOAT,0,0);

  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glClientActiveTextureARB(GL_TEXTURE0_ARB+0);
  if(_use_warped_uv)
    glBindBufferARB(GL_ARRAY_BUFFER_ARB,_vertex_buffer_uv_warped);
  else
    glBindBufferARB(GL_ARRAY_BUFFER_ARB,_vertex_buffer_uv_rectangular);
  glNormalPointer(GL_FLOAT,0,0);

  glDrawArrays(GL_TRIANGLES,0,_n_triangles*3);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);

  glActiveTextureARB(GL_TEXTURE0+0);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void SphereMesh::deinit()
{
  if(_vertex_buffer_triangles)
    glDeleteBuffersARB(1, &_vertex_buffer_triangles);
  if(_vertex_buffer_normals)
    glDeleteBuffersARB(1, &_vertex_buffer_normals);
  if(_vertex_buffer_uv_warped)
    glDeleteBuffersARB(1, &_vertex_buffer_uv_warped);
  if(_vertex_buffer_uv_rectangular)
    glDeleteBuffersARB(1, &_vertex_buffer_uv_rectangular);

  _vertex_buffer_triangles  = 0;
  _vertex_buffer_normals  = 0;
  _vertex_buffer_uv_warped  = 0;
  _vertex_buffer_uv_rectangular  = 0;
}

void SphereMesh::set_segment_division(gint subdiv)
{
  deinit();
}