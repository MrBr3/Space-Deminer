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

class Layer;
class LayerModel : public Refable
{
  static LayerModel* _singleton;
  LayerModel(const LayerModel&);

  LayerModel();
public:
  class Columns : public Gtk::TreeModel::ColumnRecord
  {
  public:
    Gtk::TreeModelColumn<Glib::ustring> name;
    Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > visibility;

    Columns()
    {
      add(name);
      add(visibility);
    }
  }gtk_tree_model_columns;

  Glib::RefPtr<Gdk::Pixbuf> pb_visible;
  Glib::RefPtr<Gdk::Pixbuf> pb_invisible;

  Glib::RefPtr<Gtk::ListStore> gtk_tree_model;
  static Glib::RefPtr<Gtk::ListStore> model(){return get_singletonA()->gtk_tree_model;}

  static Columns& columns(){return get_singletonA()->gtk_tree_model_columns;}

  static LayerModel* get_singletonA(){g_assert(_singleton);return _singleton;}

  std::list<Glib::RefPtr<Layer> > layers;

public:
  void add_layer(const Glib::RefPtr<Layer>& layer);

  ~LayerModel()throw();

  static Glib::RefPtr<LayerModel> create(){return Glib::RefPtr<LayerModel>(new LayerModel);}
};

class Layer : public Refable
{
public:
  Gtk::TreeRow row;

  bool get_visible()const{return LayerModel::get_singletonA()->pb_visible==row[LayerModel::columns().visibility];}
  Glib::ustring get_name()const{return row[LayerModel::columns().name];}

  Layer(const Glib::ustring& name, bool visible);
  ~Layer()throw();
};

class BaseTextureLayer : public Layer
{
public:
  BaseTextureLayer() : Layer(_("Base Texture"), true){}
  ~BaseTextureLayer()throw(){}
};
