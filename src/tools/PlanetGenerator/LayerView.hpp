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

class LayerView : public Gtk::TreeView
{
public:
  typedef Gtk::TreeView ParentClass;

private:
  void on_cursor_changed();
  bool on_button_press_event(GdkEventButton*);

  Layer* get_layer_for_path(const Gtk::TreeModel::Path& path);

public:
  Glib::RefPtr<LayerModel> layer_model;

  LayerView();
  ~LayerView()throw();
};