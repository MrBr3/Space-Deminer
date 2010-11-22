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

#include "./ui/main-window.hpp"
#include <vector>

#include <gtkmm/main.h>

const int error_code_exception  = 256;

int main(int argc, char **argv)
{
  try
  {
    init_paths();

    //----Load the correct Gtk-Theme--------
    {
      std::vector<Glib::ustring> v;

      v.push_back(apply_filename_macros("$(exe-share)/themes/Clearlooks-0x333333/gtk-2.0/gtkrc")); // used for the test cases

      Gtk::RC::set_default_files(v);
    }

    Gtk::Main kit(argc, argv);
    Gtk::GL::init(argc, argv);

    Glib::RefPtr<Options> options(new Options(argc, argv));

    //----
    MainWindow window;

    kit.run(window);
  }catch(const Glib::Exception& e)
  {
    std::cout<<"**main** an Glib::Exception has been detected: '"<<e.what()<<"'\n";
    return 42;
  }catch(const std::exception& e)
  {
    std::cout<<"**main** an std::exception has been detected: '"<<e.what()<<"'\n";
    return 42;
  }catch(...)
  {
    std::cout<<"**main** an unkown exception has been detected!\n";
    return 42;
  }

	return 0;
}
