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

#include <base.hpp>
#include <gtkmm.h>
#include "opengl.hpp"

#include "SphereMesh.hpp"
#include "3DView.hpp"
#include "FileSettings.hpp"
#include "ViewSettings.hpp"

/** \brief Represents the MainWindow of the Tool
 *
 * see Gtkmm Documentation for more information
 * */
class MainWindow : public Gtk::Window
{
  View3D view_3d;
  ViewSettings view_settings;

  Gtk::HBox _view_hbox;
  Gtk::VBox _vbox;
public:
  MainWindow();
  ~MainWindow()throw();
};

/** \brief As long as the MainWindow is created, this pointer will point towards it
 *
 * \note NEVER! change this manually
 * */
extern MainWindow* main_window;