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

namespace QuadVersion
{
  namespace Private_SphereMesh_
  {
    class Circle
    {
    private:
      Vector3* _circle_vertices;
      gsize _n_circle_vertices;

      gsize i_quad;

    public:
      typedef SphereMesh::Polygon Polygon;
      typedef SphereMesh::UV UV;
      typedef Vector3 Normal;

      gsize n_quads;
      Polygon* quads;
      UV* uv_rectangle;
      UV* uv_warped;
      Normal* normals;

      void quad_pushed(gfloat upper_radius, gfloat lower_radius, bool a_upper, bool b_upper, bool c_upper, bool d_upper);
      Polygon& curr_quad()
      {
        g_assert(i_quad>=0 && i_quad<n_quads);
        return quads[i_quad];
      }
      UV& curr_rect_uv()
      {
        g_assert(i_quad>=0 && i_quad<n_quads);
        return uv_rectangle[i_quad];
      }

      Vector3& point(gsize i)const throw()
      {
        g_assert(_circle_vertices);
        g_assert(_n_circle_vertices>=12);
        return _circle_vertices[i%_n_circle_vertices];
      }

      void create_polgons_between(gfloat angle_a, gfloat angle_b);

      Circle(gsize n_circle_vertices, gsize n_quads);
      ~Circle()throw();
    };
  }

  using namespace Private_SphereMesh_;

  SphereMesh::SphereMesh()
  {
    _vertex_buffer_quads  = 0;
    _vertex_buffer_normals  = 0;
    _vertex_buffer_uv_warped  = 0;
    _vertex_buffer_uv_rectangular  = 0;

    _n_quads  = 0;

    _initialized  = false;
  }

  void SphereMesh::init(gsize latitude_segments)
  {
    g_assert(!_initialized);

    set_segment_division(latitude_segments);

    _initialized  = true;
  }

  SphereMesh::~SphereMesh()throw()
  {
    deinit();
  }

  void SphereMesh::render(bool use_warped_uv)
  {
    if(!_initialized)
      return;

    g_assert(_vertex_buffer_quads);
    g_assert(_vertex_buffer_normals);
    g_assert(_vertex_buffer_uv_warped);
    g_assert(_vertex_buffer_uv_rectangular);
    g_assert(_n_quads>12);

    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB,_vertex_buffer_quads);
    glVertexPointer(3,GL_FLOAT,0,0);

    glEnableClientState(GL_NORMAL_ARRAY);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB,_vertex_buffer_normals);
    glNormalPointer(GL_FLOAT,0,0);

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glClientActiveTextureARB(GL_TEXTURE0_ARB+0);
    if(use_warped_uv)
      glBindBufferARB(GL_ARRAY_BUFFER_ARB,_vertex_buffer_uv_warped);
    else
      glBindBufferARB(GL_ARRAY_BUFFER_ARB,_vertex_buffer_uv_rectangular);
    glTexCoordPointer(2,GL_FLOAT,0,0);

    glDrawArrays(GL_QUADS, 0, _n_quads*4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  }

  void SphereMesh::deinit()
  {
    if(_vertex_buffer_quads)
      glDeleteBuffersARB(1, &_vertex_buffer_quads);
    if(_vertex_buffer_normals)
      glDeleteBuffersARB(1, &_vertex_buffer_normals);
    if(_vertex_buffer_uv_warped)
      glDeleteBuffersARB(1, &_vertex_buffer_uv_warped);
    if(_vertex_buffer_uv_rectangular)
      glDeleteBuffersARB(1, &_vertex_buffer_uv_rectangular);

    _vertex_buffer_quads  = 0;
    _vertex_buffer_normals  = 0;
    _vertex_buffer_uv_warped  = 0;
    _vertex_buffer_uv_rectangular  = 0;
  }

  void SphereMesh::set_segment_division(gsize n_segments)
  {
    deinit();

    n_segments  = MAX(12, n_segments);

    gsize n_latitude_segments = n_segments;
    gsize n_longitude_segments = n_segments;

    _n_quads  = n_longitude_segments*n_latitude_segments;

    Circle circle(n_longitude_segments, _n_quads);

    g_assert(abs(M_PI-3.1415926) < 0.001);

    gfloat a  = 0.f;
    gfloat ia  = G_PI/gfloat(n_latitude_segments);
    gfloat min_a  = 0.f;
    gfloat max_a  = G_PI;

    for(gsize i=0; i<n_latitude_segments; ++i)
    {
      gfloat curr_a = CLAMP(a, min_a, max_a);
      a += ia;
      gfloat next_a = CLAMP(a, min_a, max_a);

      circle.create_polgons_between(curr_a, next_a);
    }

    g_assert(&_vertex_buffer_quads!=nullptr);

    glGenBuffersARB(1,&_vertex_buffer_quads);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB,_vertex_buffer_quads);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB,_n_quads * sizeof(Polygon), circle.quads, GL_STATIC_DRAW_ARB);

    glGenBuffersARB(1,&_vertex_buffer_normals);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB,_vertex_buffer_normals);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB,_n_quads * 4 * sizeof(Vector3), circle.normals, GL_STATIC_DRAW_ARB);

    glGenBuffersARB(1,&_vertex_buffer_uv_rectangular);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB,_vertex_buffer_uv_rectangular);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB,_n_quads * sizeof(UV), circle.uv_rectangle, GL_STATIC_DRAW_ARB);

    glGenBuffersARB(1,&_vertex_buffer_uv_warped);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB,_vertex_buffer_uv_warped);
    glBufferDataARB(GL_ARRAY_BUFFER_ARB,_n_quads * sizeof(UV), circle.uv_warped, GL_STATIC_DRAW_ARB);
  }

  namespace Private_SphereMesh_
  {
    Circle::Circle(gsize n_circle_vertices, gsize n_quads)
    {
      g_assert(abs(PI2-6.2831) < 0.001);
      g_assert(n_circle_vertices>=24);

      //-- Allocate enough memory for the meshdata
      i_quad = 0;
      this->n_quads  = n_quads;
      quads  = new Polygon[n_quads];
      uv_rectangle  = new UV[n_quads];
      uv_warped  = new UV[n_quads];
      normals  = new Normal[n_quads*4];

      //-- Generates the Circle
      _n_circle_vertices = n_circle_vertices;
      _circle_vertices = new Vector3[_n_circle_vertices];

      gfloat ia  = PI2/gfloat(_n_circle_vertices);
      gfloat a  = 0.f;

      for(gsize i=0; i<_n_circle_vertices; ++i)
      {
        _circle_vertices[i].x  = cos(a);
        _circle_vertices[i].y  = sin(a);
        _circle_vertices[i].z  = 0.f;

        a += ia;
      }
    }

    void Circle::quad_pushed(gfloat upper_radius, gfloat lower_radius, bool a_upper, bool b_upper, bool c_upper, bool d_upper)
    {
      g_assert(i_quad<n_quads);

      normals[4*i_quad+0] = quads[i_quad].a;
      normals[4*i_quad+1] = quads[i_quad].b;
      normals[4*i_quad+2] = quads[i_quad].c;
      normals[4*i_quad+3] = quads[i_quad].d;
      /*normals[i_quad+0].normalize(); shouldn't be needed, as our sphere has the radius 1
      normals[i_quad+1].normalize();
      normals[i_quad+2].normalize();
      normals[i_quad+3].normalize();*/

      uv_warped[i_quad].a = uv_rectangle[i_quad].a;
      uv_warped[i_quad].b = uv_rectangle[i_quad].b;
      uv_warped[i_quad].c = uv_rectangle[i_quad].c;
      uv_warped[i_quad].d = uv_rectangle[i_quad].d;
      if(a_upper)
        uv_warped[i_quad].a.x = 0.5f - uv_warped[i_quad].a.x*upper_radius * 0.5f;
      else
        uv_warped[i_quad].a.x = 0.5f - uv_warped[i_quad].a.x*lower_radius * 0.5f;

      if(b_upper)
        uv_warped[i_quad].b.x = 0.5f - uv_warped[i_quad].b.x*upper_radius * 0.5f;
      else
        uv_warped[i_quad].b.x = 0.5f - uv_warped[i_quad].b.x*lower_radius * 0.5f;

      if(c_upper)
        uv_warped[i_quad].c.x = 0.5f - uv_warped[i_quad].c.x*upper_radius * 0.5f;
      else
        uv_warped[i_quad].c.x = 0.5f - uv_warped[i_quad].c.x*lower_radius * 0.5f;

      if(d_upper)
        uv_warped[i_quad].d.x = 0.5f - uv_warped[i_quad].d.x*upper_radius * 0.5f;
      else
        uv_warped[i_quad].d.x = 0.5f - uv_warped[i_quad].d.x*lower_radius * 0.5f;

      ++i_quad;
    }

    void Circle::create_polgons_between(gfloat angle_a, gfloat angle_b)
    {
      g_assert(_n_circle_vertices>0);

      gfloat higher_radius = sin(angle_a);
      gfloat higher_z  = cos(angle_a);
      gfloat lower_radius = sin(angle_b);
      gfloat lower_z  = cos(angle_b);

      gfloat uv_x = 0.f;
      gfloat i_uv_x = 1.f/_n_circle_vertices;
      gfloat higher_uv_y = angle_a/G_PI;
      gfloat lower_uv_y = angle_b/G_PI;

      for(gsize i=0; i<_n_circle_vertices; ++i)
      {
        Vector3 lower_circle_a = point(i);
        Vector3 lower_circle_b = point(i+1);
        Vector3 higher_circle_c = point(i+1);
        Vector3 higher_circle_d = point(i);

        gfloat p  = 0.0f;
        gfloat ip = 1.f-p;

        lower_circle_a  *= p+lower_radius*ip;
        lower_circle_b  *= p+lower_radius*ip;
        lower_circle_a.z = lower_z;
        lower_circle_b.z = lower_z;

        higher_circle_c  *= p+higher_radius*ip;
        higher_circle_d  *= p+higher_radius*ip;
        higher_circle_c.z = higher_z;
        higher_circle_d.z = higher_z;

        curr_quad().a.set(lower_circle_a);
        curr_quad().b.set(lower_circle_b);
        curr_quad().c.set(higher_circle_c);
        curr_quad().d.set(higher_circle_d);

        curr_rect_uv().a.set(uv_x,
                             lower_uv_y);
        curr_rect_uv().b.set(uv_x+i_uv_x,
                             lower_uv_y);
        curr_rect_uv().c.set(uv_x+i_uv_x,
                             higher_uv_y);
        curr_rect_uv().d.set(uv_x,
                             higher_uv_y);

        quad_pushed(higher_radius, lower_radius, false, false, true, true);

        uv_x  += i_uv_x;
      }
    }

    Circle::~Circle()throw()
    {
      delete (int*)0; // Just to ensure this will work
      delete[] (int*)0; // Just to ensure this will work

      g_assert(i_quad==n_quads);

      delete[] _circle_vertices;

      delete[] quads;
      delete[] uv_rectangle;
      delete[] uv_warped;
      delete[] normals;
    }
  }
}