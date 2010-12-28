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
    Gtk::TreeModelColumn<Layer* > _layer;

    Columns()
    {
      add(name);
      add(visibility);
      add(_layer);
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
  static void add_layer(const Glib::RefPtr<Layer>& layer);

  ~LayerModel()throw();

  static Glib::RefPtr<LayerModel> create(){return Glib::RefPtr<LayerModel>(new LayerModel);}
};

class SettingsWidget;
class Layer : public Refable
{
  sigc::signal<void> _signal_activated;
  sigc::signal<void> _signal_visibililty_changed;
  sigc::signal<void> _signal_something_changed;

protected:
  void prepare_settings(const Glib::ustring& pre, SettingsWidget* sw);

public:
  Gtk::TreeRow row;

  bool get_visible()const{return LayerModel::get_singletonA()->pb_visible==row[LayerModel::columns().visibility];}
  void set_visible(bool v);
  Glib::ustring get_name()const{return row[LayerModel::columns().name];}

  sigc::signal<void>& signal_activated(){return _signal_activated;}
  sigc::signal<void>& signal_visibililty_changed(){return _signal_visibililty_changed;}
  sigc::signal<void>& signal_something_changed(){return _signal_something_changed;}

  Layer(const Glib::ustring& name, bool visible);
  ~Layer()throw();
};

/**
 * \note T_layer must be a child class of SingletonLayer<T_layer>
 * */
template<class T_layer>
class SingletonLayer : public Layer
{
public:
  typedef Layer ParentClass;

private:
  static T_layer* _singleton;

public:
  static T_layer* get_singleton(){return _singleton;}
  static T_layer* get_singletonA(){g_assert(_singleton);return _singleton;}

  SingletonLayer(const Glib::ustring& name, bool visible) : ParentClass(name, visible)
  {
    g_assert(!_singleton);
    _singleton  = (T_layer*)this;
  }

  ~SingletonLayer()throw()
  {
    g_assert(_singleton);
    _singleton  = nullptr;
  }
};

template<class T_layer>
T_layer* SingletonLayer<T_layer>::_singleton = nullptr;

//====

/**
 * \note T_layer must be a child class of ImageLayer<T_layer>
 * */
template<class T_layer>
class ImageLayer : public SingletonLayer<T_layer>
{
public:
  typedef SingletonLayer<T_layer> ParentClass;

private:

  Glib::RefPtr<ImageFile> _imagefile;

  sigc::signal<void> _signal_imagefile_changed;

public:

  static const Glib::RefPtr<ImageFile>& get_imagefile(){return ParentClass::get_singletonA()->_imagefile;}

  sigc::signal<void>& signal_imagefile_changed(){return _signal_imagefile_changed;}

  ImageLayer(const Glib::ustring& name, bool visible) : ParentClass(name, visible)
  {
    _imagefile  = ImageFile::create();

    _imagefile->signal_something_changed().connect(sigc::mem_fun(signal_imagefile_changed(), &sigc::signal<void>::emit));
    signal_imagefile_changed().connect(sigc::mem_fun(ParentClass::signal_something_changed(), &sigc::signal<void>::emit));
  }

  ~ImageLayer()throw()
  {
  }
};

void register_base_texture_layer();

#include "BaseTextureLayer.hpp"