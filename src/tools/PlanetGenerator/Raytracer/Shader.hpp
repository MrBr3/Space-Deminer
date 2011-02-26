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

namespace Raytracer
{
  class RenderParam;
  namespace Shader
  {
    enum ObjectShadeID
    {
      NO_ID = 0,
      PLANET_SURFACE_ID = 1,
      PLANET_NIGHT_ID = 2,
      PLANET_CLOUDS_ID = 3,
      PLANET_ATMOSPHERE_ID = 5,
      ATMOSPHERE_ID = 256, // The Planet's Glow
      RING_ID = 1024,
    };

    inline ObjectShadeID operator|(ObjectShadeID a, ObjectShadeID b){return ObjectShadeID(int(a)|int(b));}
    inline ObjectShadeID operator&(ObjectShadeID a, ObjectShadeID b){return ObjectShadeID(int(a)&int(b));}
    inline ObjectShadeID operator^(ObjectShadeID a, ObjectShadeID b){return ObjectShadeID(int(a)^int(b));}
    inline ObjectShadeID operator~(ObjectShadeID a){return ObjectShadeID(~int(a));}
    inline ObjectShadeID& operator|=(ObjectShadeID& a, ObjectShadeID b){return a = a|b;}
    inline ObjectShadeID& operator&=(ObjectShadeID& a, ObjectShadeID b){return a = a&b;}
    inline ObjectShadeID& operator^=(ObjectShadeID& a, ObjectShadeID b){return a = a^b;}

    class Param
    {
    public:
      ColorRGBA result;
      ColorRGBA diffuse;
      ColorRGBA specular;
      gfloat specular_width, specular_strength;
      Vector3 view;
      Vector3 normal;
      Vector3 geometry_normal; // just needed for planet_surface
      Vector2 screen_pos;
      gfloat day_light; // just needed to differ between day and night Texture
      gint n_day_lights; // just needed to differ between day and night Texture
      ObjectShadeID object_id;

      Param(ObjectShadeID oid) : object_id(oid)
      {
        result.set(0.f, 0.f, 0.f);
        day_light = 0.f;
        n_day_lights = 0;
      }
    };

    class Shader : public Refable
    {
    public:
      ObjectShadeID mask;
      virtual void shade(Param& shader_param)const=0;
    };

    typedef Glib::RefPtr<Shader> ShaderPtr;
    typedef std::list<ShaderPtr> ShaderList;

    extern ShaderList all_shaders;

    void shade(Param& shader_param);
  }
  typedef Shader::Param Param;
  typedef Glib::RefPtr<Shader::Shader> ShaderPtr;
  typedef std::list<ShaderPtr> ShaderList;
}
