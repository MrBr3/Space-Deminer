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

#include <MainWindow.hpp>

LayerView::LayerView()
{
  layer_model = LayerModel::create();

  set_model(LayerModel::model());
  set_headers_visible(false);
  append_column(_("Visibility"), LayerModel::columns().visibility);
  append_column(_("Name"), LayerModel::columns().name);
}

LayerView::~LayerView()throw()
{
}

Layer* LayerView::get_layer_for_path(const Gtk::TreeModel::Path& path)
{
  if(path.empty())
    return nullptr;

  Gtk::TreeModel::iterator iter = get_model()->get_iter(path);

  if(!iter)
    return nullptr;

  Gtk::TreeModel::Row row = *iter;

  return row[LayerModel::columns()._layer];
}

bool LayerView::on_button_press_event(GdkEventButton* e)
{
  g_assert(e);

  bool b = ParentClass::on_button_press_event(e);

  if(e->type==GDK_BUTTON_PRESS)
  {
    Gtk::TreeModel::Path path;
    Gtk::TreeViewColumn* column;
    int cell_x, cell_y;

    get_path_at_pos(e->x, e->y, path, column, cell_x, cell_y);

    if(get_column(0)==column)
    {
      Layer* layer  = get_layer_for_path(path);

      if(layer)
      {
        layer->set_visible(!layer->get_visible());
      }
    }
  }

  return b;
}

void LayerView::on_cursor_changed()
{
  Gtk::TreeModel::Path path;
  Gtk::TreeViewColumn* column;

  get_cursor(path, column);

  Layer* layer = get_layer_for_path(path);

  if(!layer)
    return;

  layer->signal_activated().emit();
}