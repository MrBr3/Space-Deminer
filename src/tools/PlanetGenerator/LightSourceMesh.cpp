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

LightSourceMesh::LightSourceMesh()
{
}

LightSourceMesh::~LightSourceMesh()throw()
{
}

void LightSourceMesh::init()
{
  gfloat point_r = 0.05f;
  gsize n_circle_steps = 64;
  gfloat a;
  gfloat step=PI2/n_circle_steps;

  point_mesh.BeginBatch();
    point_mesh.Attribute2f("attr_TexCoord", 0.f, 0.f);
    point_mesh.Attribute4ub("attr_Color", 255, 255, 32);

    point_mesh.Begin(NS_GLIM::GLIM_LINES);
      point_mesh.Vertex(-point_r, 0.f, 0.f);
      point_mesh.Vertex( point_r, 0.f, 0.f);

      point_mesh.Vertex(0.f, -point_r, 0.f);
      point_mesh.Vertex(0.f,  point_r, 0.f);

      point_mesh.Vertex(0.f, 0.f, -point_r);
      point_mesh.Vertex(0.f, 0.f,  point_r);
    point_mesh.End();
    point_mesh.Begin(NS_GLIM::GLIM_LINE_LOOP);
      a=0.f;
      for(gsize i=0; i<n_circle_steps; ++i, a+=step)
        point_mesh.Vertex(point_r*cos(a), point_r*sin(a), 0.f);
    point_mesh.End();
    point_mesh.Begin(NS_GLIM::GLIM_LINE_LOOP);
      a=0.f;
      for(gsize i=0; i<n_circle_steps; ++i, a+=step)
        point_mesh.Vertex(0.f, point_r*sin(a), point_r*cos(a));
    point_mesh.End();
    point_mesh.Begin(NS_GLIM::GLIM_LINE_LOOP);
      a=0.f;
      for(gsize i=0; i<n_circle_steps; ++i, a+=step)
        point_mesh.Vertex(point_r*sin(a), 0.f, point_r*cos(a));
    point_mesh.End();
  point_mesh.EndBatch();
  directional_light.BeginBatch();
    directional_light.Begin(NS_GLIM::GLIM_LINES);
      directional_light.Vertex(0.f, 0.f, 0.f);
      directional_light.Vertex(0.f, 1.f, 0.f);

      directional_light.Vertex(0.f, 1.f, 0.f);
      directional_light.Vertex(0.1f, 0.9f, 0.f);

      directional_light.Vertex(0.f, 1.f, 0.f);
      directional_light.Vertex(-0.1f, 0.9f, 0.f);

      directional_light.Vertex(0.f, 1.f, 0.f);
      directional_light.Vertex(0.0f, 0.9f, 0.1f);

      directional_light.Vertex(0.f, 1.f, 0.f);
      directional_light.Vertex(0.0f, 0.9f,-0.1f);
    directional_light.End();
  directional_light.EndBatch();
  area_light.BeginBatch();
    area_light.Begin(NS_GLIM::GLIM_LINE_LOOP);
      a=0.f;
      for(gsize i=0; i<n_circle_steps; ++i, a+=step)
        area_light.Vertex(sin(a), 0.f, cos(a));
    area_light.End();
  area_light.EndBatch();
}
