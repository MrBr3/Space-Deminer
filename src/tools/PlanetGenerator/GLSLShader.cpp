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
  void possible_shader_error(GLuint shader, const Glib::ustring& type)
  {
    GLint r;
    if(!shader)
      throw std::logic_error("sorry, uninitializezed Shader :-(");

    glGetShaderiv(shader, GL_COMPILE_STATUS, &r);

    if(r==GL_TRUE)
      return;

    GLchar info[2048];

    glGetShaderInfoLog(shader, 2048, 0, info);

    throw std::logic_error(Glib::ustring::compose("Couldn't compile %1-Shader:\n%2", type, info).c_str());
  }

  void possible_program_error(GLuint program)
  {
    GLint r;
    if(!program)
      throw std::logic_error("sorry, uninitializezed Shader-Program :-(");

    glGetProgramiv(program, GL_COMPILE_STATUS, &r);

    if(r==GL_TRUE)
      return;

    GLchar info[2048];

    glGetProgramInfoLog(program, 2048, 0, info);

    throw std::logic_error(Glib::ustring::compose("Couldn't link Shader-Programs:\n%1", info).c_str());
  }
}

using namespace Private;

void View3D::init_shaders()
{
  try
  {
    {
      const char* planet_vs_src =
        "#version 330 compatibility\n"
        "\n"
        "uniform mat4 matrix_M;\n"
        "uniform mat4 matrix_PV;\n"
        "\n"
        "void main()\n"
        "{\n"
        "  gl_Position = matrix_PV * matrix_M * gl_Vertex;\n"
        "}\n";
      const char* planet_ps_src =
        "#version 330 compatibility\n"
        "\n"
        "void main()\n"
        "{\n"
        "  gl_FragColor = vec4(1., 0.5, 0., 1.);"
        "}\n";

      GLuint vs = glCreateShader(GL_VERTEX_SHADER);
      //gluint gs = glCreateShader(GL_GEOMETRY_SHADER);
      GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

      glShaderSource(vs, 1, &planet_vs_src, 0);

      glShaderSource(fs, 1, &planet_ps_src, 0);

      glCompileShader(vs);
      glCompileShader(fs);

      possible_shader_error(vs, "Vertex");
      possible_shader_error(fs, "Fragment");

      planet_program = glCreateProgram();

      glAttachShader(planet_program, vs);
      glAttachShader(planet_program, fs);

      glDeleteShader(vs);
      glDeleteShader(fs);

      glLinkProgram(planet_program);

      possible_program_error(planet_program);

      planet_program_unified.matrix_PV = glGetUniformLocation(planet_program, "matrix_PV");
      planet_program_unified.matrix_M  = glGetUniformLocation(planet_program, "matrix_M");
    }
  }catch(const std::exception& e)
  {
    deinit_shaders();

    Gtk::MessageDialog dlg(_("Couldn't init Shaders!"), false, Gtk::MESSAGE_ERROR);
    dlg.set_secondary_text(e.what());
    dlg.run();
    exit(-1);
  }

  ring_program = dummy_program = planet_program;
}

void View3D::deinit_shaders()
{
  if(planet_program!=dummy_program && planet_program)
    glDeleteProgram(planet_program);
  if(ring_program!=dummy_program && ring_program)
    glDeleteProgram(ring_program);
  if(dummy_program)
    glDeleteProgram(dummy_program);

  planet_program = ring_program = dummy_program = 0;
}
