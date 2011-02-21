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

#include <stdio.h>

#include "./MainWindow.hpp"

guint get_n_cores()
{
  static guint n_cores=0;

  if(!n_cores)
  {
    n_cores = MAX(boost::thread::hardware_concurrency(), 1);
  }

  return  n_cores;
}

int main(int argc, char **argv)
{
  srand(time(0));

  Gtk::Main kit(argc, argv);

  init_opengl_stuff(argc, argv);

  init_paths("planet-generator");

  Options options;

  options.load_options();
  Curve::init_slots();
  {
    MainWindow main_window;

    kit.run(main_window);
  }
  if(!options.save_options())
    g_warning("Couldn't write Config File");

	return 0;
}
// inspired by http://www.richardrosenman.com/software/downloads/ (last time visited at 2010-12-30 (see README in Root Project Folder))
