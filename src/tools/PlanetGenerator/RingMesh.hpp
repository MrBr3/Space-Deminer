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

/** \brief Represents the Ring when rendering in OpenGL
 *
 * This class encapsulates a OpenGL StaticMesh and does everything you need for you:
 * \li It renders the sphere
 * \li It generates the sphere
 * \li You can choose size and width of the ring
 *
 * */
class RingMesh : public Refable
{
public:
  class Polygon
  {
  public:
    Vector3 a;
    gfloat dummy_a;
    Vector3 b;
    gfloat dummy_b;
    Vector3 c;
    gfloat dummy_c;

    Polygon()
    {
      dummy_a = 1.f;
      dummy_b = 1.f;
      dummy_c = 1.f;
    }
  };
  class UV
  {
  public:
    Vector2 a,b,c;
  };

private:
  bool _initialized;

  gsize _n_triangles;

  sigc::signal<void> _signal_changed;

  void deinit();

  GLuint _vertex_buffer_triangles;
  GLuint _vertex_buffer_uv;

public:
  void init(gsize n_segments);

  /** \brief Sets the number of polygons, the Sphere will have.
   *
   * The default value ist \todo set default value here
   *
   * \param subdiv the number of latitude segments - the number of longitude segments will be simply twice as
   *        large - values smaller than 12 are illegal and will be interpreted as 12.
   * */
  void set_segment_division(gsize n_segments);

  /** \brief Signal getting called, when something on the sphere has changed and the sphere needs to be redrawn
   * */
  sigc::signal<void>& signal_changed(){return _signal_changed;}


  /** \brief Renders the OpenGL buffers
   *
   * This Function won't change anything in lightning nor any texture settings - just the polygons, the uvmesh and the normals.
   * */
  void render(gfloat width, gfloat outer_radius);

  GLuint ring_uv_factor;

  RingMesh();
  ~RingMesh()throw();
};
