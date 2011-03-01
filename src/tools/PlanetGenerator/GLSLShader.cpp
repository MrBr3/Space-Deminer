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
    const gsize buffer_size = 4096;
    static char buffer[buffer_size+1];
    static char* buffer_ptr;

    std::string fullpath = Glib::filename_from_utf8(apply_filename_macros("$(exe-share)/shader/330/"+filename));

    try
    {
      Glib::RefPtr<Gio::File> shader_file = Gio::File::create_for_path(fullpath);

      Glib::RefPtr<Gio::FileInputStream> stream = shader_file->read();

      gsize bytes_read = 0;
      if(!stream->read_all(buffer, buffer_size, bytes_read))
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
    GLint r;
    if(!shader)
      throw std::logic_error("sorry, uninitializezed Shader :-(");

    glGetShaderiv(shader, GL_COMPILE_STATUS, &r);

    if(r==GL_TRUE)
      return;

    GLchar info[2048];

    glGetShaderInfoLog(shader, 2048, 0, info);

    throw std::logic_error(Glib::ustring::compose("Couldn't compile %1-Shader \"%2\"\n%3", type, name, info).c_str());
  }

  void possible_program_error(GLuint program, const Glib::ustring& name)
  {
    GLint r;
    if(!program)
      throw std::logic_error("sorry, uninitializezed Shader-Program :-(");

    glGetProgramiv(program, GL_COMPILE_STATUS, &r);

    if(r==GL_TRUE)
      return;

    GLchar info[2048];

    glGetProgramInfoLog(program, 2048, 0, info);

    throw std::logic_error(Glib::ustring::compose("Couldn't link Shader-Programs \"%1\"\n%2", name, info).c_str());
  }
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

      planet_program_uniform.matrix_PV = glGetUniformLocation(planet_program, "matrix_PV");
      planet_program_uniform.matrix_M  = glGetUniformLocation(planet_program, "matrix_M");
      planet_program_uniform.base_texture  = glGetUniformLocation(planet_program, "uni_base_texture");
      planet_program_uniform.night_texture  = glGetUniformLocation(planet_program, "uni_night_texture");
      planet_program_uniform.cloud_texture  = glGetUniformLocation(planet_program, "uni_cloud_texture");
      planet_program_uniform.weight_texture  = glGetUniformLocation(planet_program, "uni_weight_texture");
      planet_program_uniform.uni_just_one_texture_visible  = glGetUniformLocation(planet_program, "uni_just_one_texture_visible");
      planet_program_uniform.uni_base_texture_visible  = glGetUniformLocation(planet_program, "uni_base_texture_visible");
      planet_program_uniform.uni_base_texture_warped  = glGetUniformLocation(planet_program, "uni_base_texture_warped");
      planet_program_uniform.uni_night_texture_visible  = glGetUniformLocation(planet_program, "uni_night_texture_visible");
      planet_program_uniform.uni_night_texture_warped  = glGetUniformLocation(planet_program, "uni_night_texture_warped");
      planet_program_uniform.uni_cloud_texture_visible  = glGetUniformLocation(planet_program, "uni_cloud_texture_visible");
      planet_program_uniform.uni_cloud_texture_warped  = glGetUniformLocation(planet_program, "uni_cloud_texture_warped");
      planet_program_uniform.uni_weight_texture_visible  = glGetUniformLocation(planet_program, "uni_weight_texture_visible");
      planet_program_uniform.uni_weight_texture_warped  = glGetUniformLocation(planet_program, "uni_weight_texture_warped");
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

      glLinkProgram(ring_program);

      possible_program_error(ring_program, "Ring");

      ring_program_uniform.matrix_PV = glGetUniformLocation(ring_program, "matrix_PV");
      ring_program_uniform.matrix_M  = glGetUniformLocation(ring_program, "matrix_M");
      ring_program_uniform.ring_texture  = glGetUniformLocation(ring_program, "uni_ring_texture");
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

      glLinkProgram(simple_program);

      possible_program_error(simple_program, "Simple");

      simple_program_uniform.matrix_PV = glGetUniformLocation(simple_program, "matrix_PV");
      simple_program_uniform.matrix_M  = glGetUniformLocation(simple_program, "matrix_M");
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
