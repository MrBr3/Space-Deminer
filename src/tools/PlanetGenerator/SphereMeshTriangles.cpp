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

namespace TriangleVersion
{
  namespace Private_SphereMesh_
  {
    class Circle
    {
    private:
      Vector3* _circle_vertices;
      gsize _n_circle_vertices;

      gsize i_triangle;

    public:
      typedef SphereMesh::Polygon Polygon;
      typedef SphereMesh::UV UV;
      typedef Vector3 Normal;

      gsize n_triangles;
      Polygon* triangles;
      UV* uv_rectangle;
      UV* uv_warped;

      void triangle_pushed(gfloat upper_radius, gfloat lower_radius, bool a_upper, bool b_upper, bool c_upper);
      Polygon& curr_triangle()
      {
        g_assert(i_triangle>=0 && i_triangle<n_triangles);
        return triangles[i_triangle];
      }
      UV& curr_rect_uv()
      {
        g_assert(i_triangle>=0 && i_triangle<n_triangles);
        return uv_rectangle[i_triangle];
      }

      Vector3& point(gsize i)const throw()
      {
        g_assert(_circle_vertices);
        g_assert(_n_circle_vertices>=12);
        return _circle_vertices[i%_n_circle_vertices];
      }

      void create_polgons_cap(gfloat angle_a, gfloat angle_b);
      void create_polgons_between(gfloat angle_a, gfloat angle_b);

      Circle(gsize n_circle_vertices, gsize n_triangles);
      ~Circle()throw();
    };
  }

  using namespace Private_SphereMesh_;

  SphereMesh::SphereMesh()
  {
    _vertex_buffer_triangles  = 0;
    _vertex_buffer_uv_warped  = 0;
    _vertex_buffer_uv_rectangular  = 0;

    _n_triangles  = 0;
    _use_gl_buffers = !UVMeshExporter::initialized();

    _initialized  = false;
  }

  void SphereMesh::init(gsize latitude_segments)
  {
    g_assert(!_initialized);

    set_segment_division(latitude_segments);

    _initialized  = _use_gl_buffers;
  }

  SphereMesh::~SphereMesh()throw()
  {
    deinit();
  }

  void SphereMesh::render()
  {
    if(!_initialized)
      return;

    g_assert(_vertex_buffer_triangles);
    g_assert(_vertex_buffer_uv_warped);
    g_assert(_vertex_buffer_uv_rectangular);
    g_assert(_n_triangles>12);

    glBindBuffer(GL_ARRAY_BUFFER,_vertex_buffer_triangles);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,_vertex_buffer_uv_rectangular);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER,_vertex_buffer_uv_warped);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glDrawArrays(GL_TRIANGLES, 0, _n_triangles*3);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
  }

  void SphereMesh::deinit()
  {
    if(_vertex_buffer_triangles)
      glDeleteBuffers(1, &_vertex_buffer_triangles);
    if(_vertex_buffer_uv_warped)
      glDeleteBuffers(1, &_vertex_buffer_uv_warped);
    if(_vertex_buffer_uv_rectangular)
      glDeleteBuffers(1, &_vertex_buffer_uv_rectangular);

    _vertex_buffer_triangles  = 0;
    _vertex_buffer_uv_warped  = 0;
    _vertex_buffer_uv_rectangular  = 0;
  }

  void SphereMesh::set_segment_division(gsize n_segments)
  {
    deinit();

    n_segments  = MAX(12, n_segments);

    gsize n_latitude_segments = n_segments;
    gsize n_longitude_segments = n_segments;

    _n_triangles  = n_longitude_segments +
                    n_longitude_segments*2*n_latitude_segments +
                    n_longitude_segments;

    Circle circle(n_longitude_segments, _n_triangles);

    g_assert(abs(M_PI-3.1415926) < 0.001);

    gfloat a  = 0.f;
    gfloat ia  = G_PI/gfloat(n_latitude_segments);
    gfloat min_a  = ia*0.25f;
    gfloat max_a  = G_PI - min_a;

    circle.create_polgons_cap(0.f, min_a);
    circle.create_polgons_cap(max_a, G_PI);

    for(gsize i=0; i<n_latitude_segments; ++i)
    {
      gfloat curr_a = CLAMP(a, min_a, max_a);
      a += ia;
      gfloat next_a = CLAMP(a, min_a, max_a);

      circle.create_polgons_between(curr_a, next_a);
    }

    if(_use_gl_buffers)
    {
      g_assert(&_vertex_buffer_triangles!=nullptr);

      glGenBuffers(1,&_vertex_buffer_triangles);
      glBindBuffer(GL_ARRAY_BUFFER,_vertex_buffer_triangles);
      glBufferData(GL_ARRAY_BUFFER,_n_triangles * sizeof(Polygon), circle.triangles, GL_STATIC_DRAW);

      glGenBuffers(1,&_vertex_buffer_uv_rectangular);
      glBindBuffer(GL_ARRAY_BUFFER,_vertex_buffer_uv_rectangular);
      glBufferData(GL_ARRAY_BUFFER,_n_triangles * sizeof(UV), circle.uv_rectangle, GL_STATIC_DRAW);

      glGenBuffers(1,&_vertex_buffer_uv_warped);
      glBindBuffer(GL_ARRAY_BUFFER,_vertex_buffer_uv_warped);
      glBufferData(GL_ARRAY_BUFFER,_n_triangles * sizeof(UV), circle.uv_warped, GL_STATIC_DRAW);
    }
  }

  namespace Private_SphereMesh_
  {
    Circle::Circle(gsize n_circle_vertices, gsize n_triangles)
    {
      g_assert(abs(PI2-6.2831) < 0.001);
      g_assert(n_circle_vertices>=12);

      //-- Allocate enough memory for the meshdata
      i_triangle = 0;
      this->n_triangles  = n_triangles;
      triangles  = new Polygon[n_triangles];
      uv_rectangle  = new UV[n_triangles*3];
      uv_warped  = new UV[n_triangles*3];

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

#define SPHERE_WARP_U(a, r) 0.5f + (uv_warped[i_triangle].a.x-0.5f)*r

    void Circle::triangle_pushed(gfloat upper_radius, gfloat lower_radius, bool a_upper, bool b_upper, bool c_upper)
    {
      g_assert(i_triangle<n_triangles);

      uv_warped[i_triangle].a = uv_rectangle[i_triangle].a;
      uv_warped[i_triangle].b = uv_rectangle[i_triangle].b;
      uv_warped[i_triangle].c = uv_rectangle[i_triangle].c;
      if(a_upper)
        uv_warped[i_triangle].a.x = SPHERE_WARP_U(a, upper_radius);
      else
        uv_warped[i_triangle].a.x = SPHERE_WARP_U(a, lower_radius);

      if(b_upper)
        uv_warped[i_triangle].b.x = SPHERE_WARP_U(b, upper_radius);
      else
        uv_warped[i_triangle].b.x = SPHERE_WARP_U(b, lower_radius);

      if(c_upper)
        uv_warped[i_triangle].c.x = SPHERE_WARP_U(c, upper_radius);
      else
        uv_warped[i_triangle].c.x = SPHERE_WARP_U(c, lower_radius);

      UVMeshExporter::add_triangle(uv_rectangle[i_triangle].a, uv_rectangle[i_triangle].b, uv_rectangle[i_triangle].c, false);
      UVMeshExporter::add_triangle(uv_warped[i_triangle].a, uv_warped[i_triangle].b, uv_warped[i_triangle].c, true);

      ++i_triangle;
    }

    void Circle::create_polgons_cap(gfloat angle_a, gfloat angle_b)
    {
      if(angle_a==0.f) // upper cap
      {
        g_assert(_n_circle_vertices>0);

        gfloat lower_radius = sin(angle_b);
        gfloat lower_z  = cos(angle_b);

        gfloat uv_x = 0.f;
        gfloat i_uv_x = 1.f/_n_circle_vertices;
        gfloat lower_uv_y = angle_b/G_PI;

        for(gsize i=0; i<_n_circle_vertices; ++i)
        {
          Vector3 lower_circle_b = point(i);
          Vector3 lower_circle_c = point(i+1);

          lower_circle_b  *= lower_radius;
          lower_circle_c  *= lower_radius;
          lower_circle_b.z = lower_z;
          lower_circle_c.z = lower_z;

          curr_triangle().a.set(0.f, 0.f, 1.f);
          curr_triangle().b.set(lower_circle_b);
          curr_triangle().c.set(lower_circle_c);

          curr_rect_uv().a.set(uv_x+0.5f*i_uv_x,
                               0.f);
          curr_rect_uv().b.set(uv_x,
                               lower_uv_y);
          curr_rect_uv().c.set(uv_x+i_uv_x,
                               lower_uv_y);

          uv_x  += i_uv_x;
          triangle_pushed(0.f, lower_radius, true, false, false);
        }
      }else // lower cap
      {
        g_assert(_n_circle_vertices>0);

        gfloat higher_radius = sin(angle_a);
        gfloat higher_z  = cos(angle_a);

        gfloat uv_x = 0.f;
        gfloat i_uv_x = 1.f/_n_circle_vertices;
        gfloat higher_uv_y = angle_a/G_PI;

        for(gsize i=0; i<_n_circle_vertices; ++i)
        {
          Vector3 higher_circle_c = point(i);
          Vector3 higher_circle_b = point(i+1);

          higher_circle_c  *= higher_radius;
          higher_circle_b  *= higher_radius;
          higher_circle_c.z = higher_z;
          higher_circle_b.z = higher_z;

          curr_triangle().a.set(0.f, 0.f, -1.f);
          curr_triangle().b.set(higher_circle_b);
          curr_triangle().c.set(higher_circle_c);

          curr_rect_uv().a.set(uv_x+0.5f*i_uv_x,
                               1.f);
          curr_rect_uv().b.set(uv_x+i_uv_x,
                               higher_uv_y);
          curr_rect_uv().c.set(uv_x,
                               higher_uv_y);

          uv_x  += i_uv_x;
          triangle_pushed(higher_radius, 0.f, false, true, true);
        }
      }
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

        lower_circle_a  *= lower_radius;
        lower_circle_b  *= lower_radius;
        lower_circle_a.z = lower_z;
        lower_circle_b.z = lower_z;

        higher_circle_c  *= higher_radius;
        higher_circle_d  *= higher_radius;
        higher_circle_c.z = higher_z;
        higher_circle_d.z = higher_z;

        curr_triangle().a.set(lower_circle_a);
        curr_triangle().b.set(lower_circle_b);
        curr_triangle().c.set(higher_circle_c);

        curr_rect_uv().a.set(uv_x,
                             lower_uv_y);
        curr_rect_uv().b.set(uv_x+i_uv_x,
                             lower_uv_y);
        curr_rect_uv().c.set(uv_x+i_uv_x,
                             higher_uv_y);

        triangle_pushed(higher_radius, lower_radius, false, false, true);

        curr_triangle().a.set(higher_circle_c);
        curr_triangle().b.set(higher_circle_d);
        curr_triangle().c.set(lower_circle_a);

        curr_rect_uv().a.set(uv_x+i_uv_x,
                             higher_uv_y);
        curr_rect_uv().b.set(uv_x,
                             higher_uv_y);
        curr_rect_uv().c.set(uv_x,
                             lower_uv_y);

        triangle_pushed(higher_radius, lower_radius, true, true, false);

        uv_x  += i_uv_x;
      }
    }

    Circle::~Circle()throw()
    {
      delete (int*)0; // Just to ensure this will work
      delete[] (int*)0; // Just to ensure this will work

      //g_assert(i_triangle==n_triangles);

      delete[] _circle_vertices;

      delete[] triangles;
      delete[] uv_rectangle;
      delete[] uv_warped;
    }
  }
}
