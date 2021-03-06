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
  srand(time(0));
  try
  {
    init_paths("space-deminer");

    Gtk::Main kit(argc, argv);
    Gtk::GL::init(argc, argv);

    Glib::RefPtr<Options> options(new Options(argc, argv));

    //----
    MainWindow window;

    kit.run(window);

    Framework::Theme::can_destroy_now();
  }CATCH_ALL("**main** ", return 42;)

	return 0;
}
