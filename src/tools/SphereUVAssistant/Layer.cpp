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

#include "Model.hpp"

LayerModel* LayerModel::_singleton = nullptr;

LayerModel::LayerModel()
{
  g_assert(!_singleton);
  _singleton  = this;

  try
  {
    pb_visible   = Gdk::Pixbuf::create_from_file(apply_filename_macros("$(exe-share)/icons/scalable/visible-layer.svg"));
    pb_invisible = Gdk::Pixbuf::create_from_file(apply_filename_macros("$(exe-share)/icons/scalable/invisible-layer.svg"));
  }CATCH_ALL("**LayerModel::LayerModel**", NOTHING_MACRO)

  gtk_tree_model  = Gtk::ListStore::create(columns());

  add_layer(Glib::RefPtr<Layer>(new BaseTextureLayer));
}

LayerModel::~LayerModel()throw()
{
  g_assert(_singleton);
  _singleton  = nullptr;
}

void LayerModel::add_layer(const Glib::RefPtr<Layer>& layer)
{
  layers.push_back(layer);
}

//=====================================

Layer::Layer(const Glib::ustring& name, bool visible)
{
  row  = *LayerModel::model()->append();
  row[LayerModel::columns().name]  = name;
  row[LayerModel::columns().visibility]  = visible ? LayerModel::get_singletonA()->pb_visible : LayerModel::get_singletonA()->pb_invisible;
}

Layer::~Layer()throw()
{
}