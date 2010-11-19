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

/** \brief Represents the Sphere
 *
 * This class encapsulates a OpenGL StaticMesh and does everything you need for you:
 * \li It renders the sphere
 * \li It generates the sphere
 * \li You can decide to use a warped UV-Mesh in order to get a fast preview
 *
 * */
class SphereMesh : public Refable
{
private:
  bool _use_warped_uv;

  gsize _n_triangles;

  sigc::signal<void> _signal_changed;

  void deinit();

  GLuint _vertex_buffer_triangles;
  GLuint _vertex_buffer_normals;
  GLuint _vertex_buffer_uv_warped;
  GLuint _vertex_buffer_uv_rectangular;

public:
  /** \brief Gets, whether the UV-Mesh ist warped.
   * */
  bool get_use_warped_uv()const{return _use_warped_uv;}

  /** \brief Sets, whether the UV-Mesh should be warped.
   *
   * */
  void set_use_warped_uv(bool warped=true);

  /** \brief Sets the number of polygons, the Sphere will have.
   *
   * The default value ist \todo set default value here
   *
   * \param subdiv the number of latitude subdivisions - the longitude subdivision will be simply twice as
   *        large - values smaller than 12 are illegal and will be interpreted as 12.
   * */
  void set_segment_division(gint subdiv);

  /** \brief Signal getting called, when something on the sphere has changed and the sphere needs to be redrawn
   * */
  sigc::signal<void>& signal_changed(){return _signal_changed;}


  /** \brief Renders the OpenGL buffers
   *
   * This Function won't change anything in lightning nor any texture settings - just the polygons, the uvmesh and the normals.
   * */
  void render();

  SphereMesh();
  ~SphereMesh()throw();
};