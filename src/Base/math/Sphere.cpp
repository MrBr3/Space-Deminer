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
 
#include <base.hpp>

namespace Math
{
  void Sphere::screen_circle_world(Vector3& circle_center, Vector3& circle_point, const Matrix44& view_maxtrix)
  {
    Matrix44 inv_camera = view_maxtrix;
    inv_camera.invert();
    
    Vector3 camera_pos  =  inv_camera * Vector3(0.f, 0.f, 0.f);
    Vector3 sphere_pos  =  get_position();
    
    if(radius==0.f)
    {
      circle_point = circle_center = sphere_pos;
      return;
    }
    
    // formula by http://de.wikipedia.org/wiki/Schnittebene (last visited 2011-02-08) used
    
    gfloat d  = (camera_pos-sphere_pos).get_length()*0.5f;
    if(d==0.f)
    {
      circle_center = sphere_pos;
      circle_point = circle_center+Vector3(0.f, 0.f, 1.f)*radius;
      return;
    }
    gfloat r1 = radius;
    gfloat r2 = d;
    Vector3& m1 = sphere_pos;
    Vector3 m2 = camera_pos+(sphere_pos-camera_pos)*0.5f;
    Vector3& m3 = circle_center;
    
    m3 = m1 + (m2-m1)*((r1*r1-r2*r2)/(2.f*d*d)+0.5f);
    
    // Bestimme Schnittkreisradius
    
    gfloat cut_r = r1 * sin(acos((m1-m3).get_length()/r1));
    
    circle_point = m3 + (m1-m2).get_ortho().normalize() * cut_r;
  }
  
  void Sphere::screen_circle_pixelscreen(Vector2& circle_center, gfloat& circle_radius, const Matrix44& projection_maxtrix, const Matrix44& view_maxtrix, gfloat screen_width, gfloat screen_height)
  {
    Vector3 circle_center_3d;
    Vector3 circle_point_3d;
    
    screen_circle_world(circle_center_3d, circle_point_3d, view_maxtrix);
    
    Matrix44 m = projection_maxtrix*view_maxtrix;
    
    circle_center_3d  = m*circle_center_3d;
    circle_point_3d  = m*circle_point_3d;
    
    circle_center.x  = circle_center_3d.x = screen_width  * (circle_center_3d.x*0.5f+0.5f);
    circle_center.y  = circle_center_3d.y = screen_height * (0.5f-circle_center_3d.y*0.5f);
    circle_center_3d.z = 0.f;
    circle_point_3d.x = screen_width  * (circle_point_3d.x*0.5f+0.5f);
    circle_point_3d.y = screen_height * (0.5f-circle_point_3d.y*0.5f);
    circle_point_3d.z = 0.f;
    
    circle_radius = (circle_point_3d-circle_center_3d).get_length();
  }
}