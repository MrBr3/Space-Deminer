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

namespace Private
{
  const GLchar** load_shader(const Glib::ustring& filename)
  {
    const gsize buffer_size = 8192;
    static char buffer[buffer_size+1];
    static char* buffer_ptr;

    std::string fullpath = Glib::filename_from_utf8(apply_filename_macros("$(exe-share)/shader/330/"+filename));

    try
    {
      Glib::RefPtr<Gio::File> shader_file = Gio::File::create_for_path(fullpath);

      Glib::RefPtr<Gio::FileInputStream> stream = shader_file->read();

      gsize bytes_read = 0;
      if(!stream->read_all(buffer, buffer_size, bytes_read) || bytes_read==buffer_size)
        throw Gio::Error(Gio::Error::Code::FAILED, _("Temp buffer is too small. :-("));
      buffer[bytes_read] = 0;

      buffer_ptr = &buffer[0];
      return (const GLchar**)&buffer_ptr;
    }catch(Glib::Exception& e)
    {
      Gtk::MessageDialog dlg(_("Couldn't load shader File"), false, Gtk::MESSAGE_ERROR);
      dlg.set_secondary_text("Couldn't load Shader-File <"+fullpath+">\n\n"+e.what());
      dlg.run();
      exit(-1);
    }
  }

  void possible_shader_error(GLuint shader, const Glib::ustring& type, const Glib::ustring& name)
  {
    GLint r = GL_FALSE;
    if(!shader)
      throw std::logic_error("sorry, uninitializezed Shader :-(");

    glGetShaderiv(shader, GL_COMPILE_STATUS, &r);

    GLchar info[2048];

    glGetShaderInfoLog(shader, 2048, 0, info);

    if(r==GL_TRUE)
    {
      if(info[0]!=0)
        std::cout<<"Warning(s) while compiling "<<type<<"-Shader \""<<name<<"\":\n"<<info<<"\n";
      return;
    }

    throw std::logic_error(Glib::ustring::compose("Couldn't compile %1-Shader \"%2\"\n%3", type, name, info).c_str());
  }

  void possible_program_error(GLuint program, const Glib::ustring& name)
  {
    GLint r = GL_FALSE;
    if(!program)
      throw std::logic_error("sorry, uninitializezed Shader-Program :-(");

    glGetProgramiv(program, GL_LINK_STATUS, &r);

    GLchar info[2048];

    glGetProgramInfoLog(program, 2048, 0, info);

    if(r==GL_TRUE)
    {
      if(info[0]!=0)
        std::cout<<"Warning(s) while linking ShaderProgram \""<<name<<"\":\n"<<info<<"\n";
      return;
    }

    throw std::logic_error(Glib::ustring::compose("Couldn't link ShaderProgram \"%1\"\n%2", name, info).c_str());
  }

  inline GLint locate_uniform(GLuint program, const std::string& uniform, const std::string& program_name)
  {
    GLint location = glGetUniformLocation(program, uniform.c_str());

    if(location<0)
      std::cout<<Glib::ustring::compose("The Uniform Name \"%1\" doesn't represent an active uniform in the program \"%2\"\n", uniform, program_name).c_str();

    return location;
  }
#define LOCATE_UNIFORM(program, uniform) locate_uniform(program, uniform, #program)
}

using namespace Private;

void View3D::init_shaders()
{
  try
  {
    {
      GLuint vs = glCreateShader(GL_VERTEX_SHADER);
      //gluint gs = glCreateShader(GL_GEOMETRY_SHADER);
      GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

      glShaderSource(vs, 1, load_shader("planet.vert"), 0);
      glShaderSource(fs, 1, load_shader("planet.frag"), 0);

      glCompileShader(vs);
      glCompileShader(fs);

      possible_shader_error(vs, "Vertex", "Planet");
      possible_shader_error(fs, "Fragment", "Planet");

      planet_program = glCreateProgram();

      glAttachShader(planet_program, vs);
      glAttachShader(planet_program, fs);

      glDeleteShader(vs);
      glDeleteShader(fs);

      glBindAttribLocation(planet_program, 0, "vertex");
      glBindAttribLocation(planet_program, 1, "att_tex_coord");
      glBindAttribLocation(planet_program, 2, "att_tex_coord_warped");

      glBindFragDataLocation(planet_program, 0, "resulting_color");

      glLinkProgram(planet_program);

      possible_program_error(planet_program, "Planet");

      planet_program_uniform.matrix_PV = LOCATE_UNIFORM(planet_program, "matrix_PV");
      planet_program_uniform.matrix_M  = LOCATE_UNIFORM(planet_program, "matrix_M");
      planet_program_uniform.base_texture  = LOCATE_UNIFORM(planet_program, "uni_base_texture");
      planet_program_uniform.night_texture  = LOCATE_UNIFORM(planet_program, "uni_night_texture");
      planet_program_uniform.cloud_texture  = LOCATE_UNIFORM(planet_program, "uni_cloud_texture");
      planet_program_uniform.weight_texture  = LOCATE_UNIFORM(planet_program, "uni_weight_texture");
      planet_program_uniform.uni_just_one_texture_visible  = LOCATE_UNIFORM(planet_program, "uni_just_one_texture_visible");
      planet_program_uniform.uni_base_texture_visible  = LOCATE_UNIFORM(planet_program, "uni_base_texture_visible");
      planet_program_uniform.uni_base_texture_warped  = LOCATE_UNIFORM(planet_program, "uni_base_texture_warped");
      planet_program_uniform.uni_night_texture_visible  = LOCATE_UNIFORM(planet_program, "uni_night_texture_visible");
      planet_program_uniform.uni_night_texture_warped  = LOCATE_UNIFORM(planet_program, "uni_night_texture_warped");
      planet_program_uniform.uni_cloud_texture_visible  = LOCATE_UNIFORM(planet_program, "uni_cloud_texture_visible");
      planet_program_uniform.uni_cloud_texture_warped  = LOCATE_UNIFORM(planet_program, "uni_cloud_texture_warped");
      planet_program_uniform.uni_weight_texture_visible  = LOCATE_UNIFORM(planet_program, "uni_weight_texture_visible");
      planet_program_uniform.uni_weight_texture_warped  = LOCATE_UNIFORM(planet_program, "uni_weight_texture_warped");
      planet_program_uniform.uni_no_lightning = LOCATE_UNIFORM(planet_program, "uni_no_lightning");
      planet_program_uniform.uni_no_nighttexture = LOCATE_UNIFORM(planet_program, "uni_no_nighttexture");
      planet_program_uniform.light[0].get_uniform_locations(planet_program, "light[0].");
      planet_program_uniform.light[1].get_uniform_locations(planet_program, "light[1].");
      planet_program_uniform.light[2].get_uniform_locations(planet_program, "light[2].");
      planet_program_uniform.light[3].get_uniform_locations(planet_program, "light[3].");
    }
    {
      GLuint vs = glCreateShader(GL_VERTEX_SHADER);
      //gluint gs = glCreateShader(GL_GEOMETRY_SHADER);
      GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

      glShaderSource(vs, 1, load_shader("ring.vert"), 0);
      glShaderSource(fs, 1, load_shader("ring.frag"), 0);

      glCompileShader(vs);
      glCompileShader(fs);

      possible_shader_error(vs, "Vertex", "Ring");
      possible_shader_error(fs, "Fragment", "Ring");

      ring_program = glCreateProgram();

      glAttachShader(ring_program, vs);
      glAttachShader(ring_program, fs);

      glDeleteShader(vs);
      glDeleteShader(fs);

      glBindAttribLocation(ring_program, 0, "att_vertex");
      glBindAttribLocation(ring_program, 1, "att_tex_coord");

      glBindFragDataLocation(ring_program, 0, "resulting_color");

      glLinkProgram(ring_program);

      possible_program_error(ring_program, "Ring");

      ring_program_uniform.matrix_PV = LOCATE_UNIFORM(ring_program, "matrix_PV");
      ring_program_uniform.matrix_M  = LOCATE_UNIFORM(ring_program, "matrix_M");
      ring_program_uniform.ring_texture  = LOCATE_UNIFORM(ring_program, "uni_ring_texture");
    }
    {
      GLuint vs = glCreateShader(GL_VERTEX_SHADER);
      //gluint gs = glCreateShader(GL_GEOMETRY_SHADER);
      GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

      glShaderSource(vs, 1, load_shader("simple.vert"), 0);
      glShaderSource(fs, 1, load_shader("simple.frag"), 0);

      glCompileShader(vs);
      glCompileShader(fs);

      possible_shader_error(vs, "Vertex", "Simple");
      possible_shader_error(fs, "Fragment", "Simple");

      simple_program = glCreateProgram();

      glAttachShader(simple_program, vs);
      glAttachShader(simple_program, fs);

      glDeleteShader(vs);
      glDeleteShader(fs);

      glBindAttribLocation(simple_program, 0, "att_vertex");
      glBindAttribLocation(simple_program, 1, "att_color");

      glBindFragDataLocation(simple_program, 0, "resulting_color");

      glLinkProgram(simple_program);

      possible_program_error(simple_program, "Simple");

      simple_program_uniform.matrix_PV = LOCATE_UNIFORM(simple_program, "matrix_PV");
      simple_program_uniform.matrix_M  = LOCATE_UNIFORM(simple_program, "matrix_M");
    }
  }catch(const std::exception& e)
  {
    deinit_shaders();

    Gtk::MessageDialog dlg(_("Couldn't init Shaders!"), false, Gtk::MESSAGE_ERROR);
    dlg.set_secondary_text(e.what());
    dlg.run();
    exit(-1);
  }
}

void View3D::deinit_shaders()
{
  if(planet_program)
    glDeleteProgram(planet_program);
  if(ring_program)
    glDeleteProgram(ring_program);
  if(simple_program)
    glDeleteProgram(simple_program);

  planet_program = ring_program = simple_program = 0;
}

//--------

void View3D::GradientUniform::get_uniform_locations(GLuint planet_program, const std::string& prefix)
{
  curves = LOCATE_UNIFORM(planet_program, (prefix+"curves").c_str());
  defcolor = LOCATE_UNIFORM(planet_program, (prefix+"defcolor").c_str());
  col[0] = LOCATE_UNIFORM(planet_program, (prefix+"col[0]").c_str());
  col[1] = LOCATE_UNIFORM(planet_program, (prefix+"col[1]").c_str());
  col[2] = LOCATE_UNIFORM(planet_program, (prefix+"col[2]").c_str());
  col[3] = LOCATE_UNIFORM(planet_program, (prefix+"col[3]").c_str());
  remap[0] = LOCATE_UNIFORM(planet_program, (prefix+"remap[0]").c_str());
  remap[1] = LOCATE_UNIFORM(planet_program, (prefix+"remap[0]").c_str());
}

void View3D::PlanetProgramUniform::Light::get_uniform_locations(GLuint planet_program, const std::string& prefix)
{
  visible = LOCATE_UNIFORM(planet_program, (prefix+"visible").c_str());
  dir = LOCATE_UNIFORM(planet_program, (prefix+"dir").c_str());
  pos = LOCATE_UNIFORM(planet_program, (prefix+"pos").c_str());
  color = LOCATE_UNIFORM(planet_program, (prefix+"color").c_str());
  type = LOCATE_UNIFORM(planet_program, (prefix+"type").c_str());
  influence_night = LOCATE_UNIFORM(planet_program, (prefix+"influence_night").c_str());
  light_on_planet = LOCATE_UNIFORM(planet_program, (prefix+"light_on_planet").c_str());
  //light_on_ring = LOCATE_UNIFORM(planet_program, (prefix+"light_on_ring").c_str());
  specular_factor = LOCATE_UNIFORM(planet_program, (prefix+"specular_factor").c_str());
  ring_shadow = LOCATE_UNIFORM(planet_program, (prefix+"ring_shadow").c_str());
  cloud_shadow = LOCATE_UNIFORM(planet_program, (prefix+"cloud_shadow").c_str());
  //planet_shadow = LOCATE_UNIFORM(planet_program, (prefix+"planet_shadow").c_str());
  just_shadows = LOCATE_UNIFORM(planet_program, (prefix+"just_shadows").c_str());
  shade_gradient.get_uniform_locations(planet_program, (prefix+"shade_gradient.").c_str());
  gradient[0].get_uniform_locations(planet_program, prefix+"gradient[0].");
  gradient[1].get_uniform_locations(planet_program, prefix+"gradient[1].");
  gradient[2].get_uniform_locations(planet_program, prefix+"gradient[2].");
  gradient[3].get_uniform_locations(planet_program, prefix+"gradient[3].");
}

void View3D::PlanetProgramUniform::Light::GradientLight::get_uniform_locations(GLuint planet_program, const std::string& prefix)
{
  use = LOCATE_UNIFORM(planet_program, (prefix+"use").c_str());
  multiply_gradient_color_with_light_color = LOCATE_UNIFORM(planet_program, (prefix+"multiply_gradient_color_with_light_color").c_str());
  //add_x_rotation = LOCATE_UNIFORM(planet_program, (prefix+"add_x_rotation").c_str());
  //add_z_rotation = LOCATE_UNIFORM(planet_program, (prefix+"add_z_rotation").c_str());
  //radius = LOCATE_UNIFORM(planet_program, (prefix+"radius").c_str());
  inside_planet = LOCATE_UNIFORM(planet_program, (prefix+"inside_planet").c_str());
  //outside_planet = LOCATE_UNIFORM(planet_program, (prefix+"outside_planet").c_str());
  modulate_type = LOCATE_UNIFORM(planet_program, (prefix+"modulate_type").c_str());
  light_gradient.get_uniform_locations(planet_program, prefix+"light_gradient.");
}

void View3D::PlanetProgramUniform::Light::feed_data(guint i)
{
  LightLayer& layer = *LightLayer::get_singleton(i);

  glUniform1i(visible, layer.get_visible());
  glUniform1i(type, layer.get_light_type());
  layer.get_light_direction().glUniform4(dir, 0.f);
  layer.get_light_position().glUniform4(pos, 1.f);
  ColorRGBA::rgb_mult(layer.get_light_color(), layer.get_light_intensity()).glUniformRGB(color);
  glUniform1f(influence_night, layer.get_influence_night());
  glUniform1f(light_on_planet, layer.get_light_on_planet());
  //glUniform1f(light_on_ring, layer.get_light_on_ring());
  glUniform1f(specular_factor, layer.get_specular_factor());
  glUniform1f(ring_shadow, layer.get_ring_shadow());
  glUniform1f(cloud_shadow, layer.get_cloud_shadow());
  //glUniform1f(cloud_shadow, layer.get_planet_shadow());
  glUniform1i(just_shadows, layer.get_just_shadows());
  shade_gradient.feed_data(layer.get_shading_gradient());
  gradient[0].feed_data(layer.gradient0);
  gradient[1].feed_data(layer.gradient1);
  gradient[2].feed_data(layer.gradient2);
  gradient[3].feed_data(layer.gradient3);
}

void View3D::PlanetProgramUniform::Light::GradientLight::feed_data(const LightLayer::GradientSetting& gradient)
{
  glUniform1i(use, gradient.get_use());
  glUniform1f(multiply_gradient_color_with_light_color, gradient.get_multiply_gradient_color_with_light_color());
  //glUniform1f(add_x_rotation, gradient.get_add_x_rotation());
  //glUniform1f(add_z_rotation, gradient.get_add_z_rotation());
  //glUniform1f(radius, gradient.get_radius());
  glUniform1f(inside_planet, gradient.get_inside_planet());
  //glUniform1f(outside_planet, gradient.get_outside_planet());
  glUniform1f(modulate_type, gradient.get_modulate_type());
  light_gradient.feed_data(gradient.get_light_gradient());
}

void View3D::GradientUniform::feed_data(const ConstGradientPtr& gradient)
{
  static bool l=false;
  if(!l)
  {
    l=true;
    std::cout<<"glUniform1i(curves, gradient.);\n";
  }

  if(gradient->get_use_alpha())
  {
    gradient->get_defcolor().glUniformRGBA(defcolor);
    gradient->get_color1().glUniformRGBA(col[0]);
    gradient->get_color2().glUniformRGBA(col[1]);
    gradient->get_color3().glUniformRGBA(col[2]);
    gradient->get_color4().glUniformRGBA(col[3]);
  }else
  {
    gradient->get_defcolor().glUniformRGB(defcolor);
    gradient->get_color1().glUniformRGB(col[0]);
    gradient->get_color2().glUniformRGB(col[1]);
    gradient->get_color3().glUniformRGB(col[2]);
    gradient->get_color4().glUniformRGB(col[3]);
  }
  gfloat remap_a = gradient->get_remap_a();
  gfloat remap_b = gradient->get_remap_b();
  if(remap_a==remap_b)
  {
    remap_a = 2.f;
    remap_b = 3.f;
  }
  glUniform1f(remap[0], remap_a);
  glUniform1f(remap[1], remap_b);
}
