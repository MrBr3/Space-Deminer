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

  sigc::signal<void> _signal_something_changed;

public:
  typedef std::list<Glib::RefPtr<Layer> > LayerList;

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

  static sigc::signal<void>& signal_something_changed(){return get_singletonA()->_signal_something_changed;}

  LayerList layers;

  /** \brief Gets the only visible Layer
   *
   * \return A valid Pointer if there's just ine visible, otherwise <tt>nullptr</tt>
   * */
  static Glib::RefPtr<Layer> just_one_texture_layer_visible();

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
  bool is_texture_layer;

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
 * \note T_layer must be a child class of MultiLayer<T_layer>
 * */
template<class T_layer>
class MultiLayer : public Layer
{
public:
  typedef Layer ParentClass;

private:
  typedef typename std::vector<T_layer*>::iterator VecIter;

  static std::vector<T_layer*> _singletons;

public:
  static T_layer* get_singleton(gsize i){return _singletons[i];}
  static T_layer* get_singletonA(gsize i){g_assert(i<_singletons.size());return _singletons[i];}

  MultiLayer(const Glib::ustring& name, bool visible) : ParentClass(name, visible)
  {
    _singletons.push_back((T_layer*)this);
  }

  ~MultiLayer()throw()
  {
    for(VecIter iter=_singletons.begin(); iter!=_singletons.end(); ++iter)
    {
      if(*iter == this)
      {
        iter = _singletons.erase(iter);
        return;
      }

      g_assert_not_reached();
    }
  }
};

template<class T_layer>
std::vector<T_layer*> MultiLayer<T_layer>::_singletons;

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
    this->is_texture_layer = true;

    _imagefile->signal_something_changed().connect(sigc::mem_fun(signal_imagefile_changed(), &sigc::signal<void>::emit));
    signal_imagefile_changed().connect(sigc::mem_fun(ParentClass::signal_something_changed(), &sigc::signal<void>::emit));
  }

  ~ImageLayer()throw()
  {
  }
};

void register_base_texture_layer();

#include "BaseTextureLayer.hpp"
#include "CloudTextureLayer.hpp"
#include "NightTextureLayer.hpp"
#include "WeightTextureLayer.hpp"

#include "RingLayer.hpp"
#include "LightLayer.hpp"
