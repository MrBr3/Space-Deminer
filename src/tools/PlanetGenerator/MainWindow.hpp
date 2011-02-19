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

#include "./Raytracer/Raytracer.hpp"

#include <cairomm/cairomm.h>

#include "SphereMeshQuads.hpp"
#include "SphereMeshTriangles.hpp"

using TriangleVersion::SphereMesh;

#include "RingMesh.hpp"
#include "Process.hpp"
#include "Texture.hpp"
#include "WarningListDialog.hpp"
#include "3DView.hpp"
#include "ImageFileSettings.hpp"
#include "ViewSettings.hpp"
#include "LayerView.hpp"
#include "UVMeshExporter.hpp"
#include "GradientButton.hpp"

const gint LENGTH_BORDER_WIDTH = 4;
const gint LENGTH_SMALLSPACE = 3;

/** \brief Represents the MainWindow of the Tool
 *
 * see Gtkmm Documentation for more information
 * */
class MainWindow : public Gtk::Window
{
  View3D* view_3d;
  ViewSettings* view_settings;
  Glib::RefPtr<Raytracer::Manager> raytracer;
  Glib::RefPtr<Process> _process;

  bool _sensitive_for_changes;
  sigc::signal<void> _signal_sensitive_for_changes_changed;

  class MyMenuItem : public Gtk::MenuItem
  {
  public:
    void set_accel_key(const Glib::ustring& key)
    {
      Gtk::MenuItem::set_accel_key(Gtk::AccelKey(key));
    }
  };
  class MyCheckMenuItem : public Gtk::CheckMenuItem
  {
  public:
    void set_accel_key(const Glib::ustring& key)
    {
      Gtk::CheckMenuItem::set_accel_key(Gtk::AccelKey(key));
    }
  };

  class RenderResultView : public Gtk::DrawingArea
  {
  public:
    void prepare_render_in_viewer_size();
    bool on_expose_event(GdkEventExpose* e);
    void pixbuf_object_changed();
    RenderResultView();
  };

  MyMenuItem menu_file;
  Gtk::Menu  menu_file_menu;
    MyMenuItem menu_file_export;
    Gtk::Menu  menu_file_export_menu;
      Gtk::SeparatorMenuItem menu_file_export_sep1;
      MyMenuItem menu_file_export_uv_not_warped;
      MyMenuItem menu_file_export_uv_warped;
    Gtk::SeparatorMenuItem menu_file_sep1;
    MyMenuItem menu_file_quit;
  MyMenuItem menu_render;
  Gtk::Menu  menu_render_menu;
    MyMenuItem menu_render_render;
    MyMenuItem menu_render_render_view;
    Gtk::SeparatorMenuItem menu_render_sep1;
    MyMenuItem menu_render_close_preview;
    MyMenuItem menu_render_save_result;
    Gtk::SeparatorMenuItem menu_render_sep2;
    MyMenuItem menu_render_settings;
  MyMenuItem menu_view;
  Gtk::Menu  menu_view_menu;
    MyCheckMenuItem menu_view_show_sidebar;
    MyCheckMenuItem menu_view_wireframed;
    MyMenuItem menu_view_settings;

  Gtk::VBox _vbox;
    Gtk::MenuBar _menu_bar;
    Gtk::HPaned _hpaned;
      Gtk::VPaned _vpaned;
        Gtk::ScrolledWindow _layers_scrollbars;
          LayerView* _layers;
        Gtk::VBox _settings;
          int last_settings_size_request;
    Gtk::ScrolledWindow _render_preview_scrollbars;
      RenderResultView _render_preview;
    Gtk::HSeparator _statusbar_sep;
    Gtk::HBox _statusbar;
      int last_statusbar_size_request;
      Gtk::Label _status_label;
      Gtk::ProgressBar _status_progressbar;
      Gtk::Button _status_abortbutton;

  void draw_wireframed_toggled()
  {
    view_3d->set_draw_wireframed(menu_view_wireframed.get_active());
  }

  void toggle_show_sidebar()
  {
    set_show_sidebar(!get_show_sidebar());
  }
  bool get_show_sidebar()const{return menu_view_show_sidebar.get_active();}
  void set_show_sidebar(bool j)
  {
    menu_view_show_sidebar.set_active(j);
  }

  void _adapt_show_sidebar();

  void adapt_statusbar_size_request(Gtk::Requisition*);
  void adapt_settings_size_request(Gtk::Requisition*);

  void on_realize();
  void on_show();
  void update_statusbar();

  void invalidate_render_preview();

public:
  MainWindow();
  ~MainWindow()throw();

  void get_render_view_size(guint& width, guint& height);
  void prepare_render(bool viewer_size);

  bool get_sensitive_for_changes()const
  {
    g_assert(this);
    return _sensitive_for_changes;
  }
  void set_sensitive_for_changes(bool sc)
  {
    g_assert(this);
    _sensitive_for_changes = sc;
    signal_sensitive_for_changes_changed().emit();
  }
  sigc::signal<void>& signal_sensitive_for_changes_changed()
  {
    g_assert(this);
    return _signal_sensitive_for_changes_changed;
  }

  View3D& get_view_3d(){g_assert(this);g_assert(view_3d);return *view_3d;}
  ViewSettings& get_view_settings(){g_assert(this);g_assert(view_settings);return *view_settings;}

  void append_settings_widget(Gtk::Widget& w);
};

/** \brief As long as the MainWindow is created, this pointer will point towards it
 *
 * \note NEVER! change this manually
 * */
extern MainWindow* main_window;
