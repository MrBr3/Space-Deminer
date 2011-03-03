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

#include "MainWindow.hpp"

MainWindow* main_window  = nullptr;

void invalidate(Gtk::Widget* w)
{
  if(!w)
    return;

  Glib::RefPtr<Gdk::Window> window = w->get_window();

  if(window)
    window->invalidate(true);
}

namespace Private
{
  bool add_unit_to_spinbutton(Gtk::SpinButton* sb, const Glib::ustring& str)
  {
    sb->set_text(Glib::ustring::compose("%1%2",
                                        Glib::ustring::format(std::fixed, std::setprecision(sb->get_digits()), sb->get_value()),
                                        str));
    return true;
  }
}

void set_unit(Gtk::SpinButton& sb, const Glib::ustring& str)
{
  sb.signal_output().connect(sigc::bind(sigc::ptr_fun(Private::add_unit_to_spinbutton), &sb, str));
}

bool negate(const sigc::slot<bool>& fun)
{
  return !fun();
}

void about_dlg()
{
  Gtk::AboutDialog dlg;
  Gtk::ScrolledWindow sw;
  Gtk::Label external;

  dlg.set_program_name("Planet Generator");
  dlg.set_name("Planet Generator");
  dlg.set_copyright("Copyright (C) 2010 the Space Deminer Development Team");
  dlg.set_license("This program is free software; you can redistribute it and/or\n"
                  "modify it under the terms of the GNU General Public License\n"
                  "as published by the Free Software Foundation; either version 3\n"
                  "of the License, or (at your option) any later version.\n"
                  "\n"
                  "This program is distributed in the hope that it will be useful,\n"
                  "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
                  "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU\n"
                  "General Public License for more details.\n"
                  "\n"
                  "You should have received a copy of the GNU General Public License\n"
                  "along with this program; if not, see <http://www.gnu.org/licenses/>.");

  g_assert(dlg.get_vbox());
  dlg.get_vbox()->pack_start(sw);
  sw.show();
  sw.add(external);
  sw.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_ALWAYS);
  sw.set_size_request(-1, 128);
  external.show();
  external.set_markup("The Planet Generator uses also source from\nother OpenSource Projects:\n\n"
                      "<b>The GIMP</b>\n"
                      "  We are using a part the GIMP's Curve Widget\n"
                      "  see www.gimp.org\n"
                      "    <i>(last visited 2011-02-20)</i>"
                      "\n\n"
                      "<b>binreloc</b>\n"
                      "  see http://autopackage.org/docs/binreloc/\n"
                      "  <i>(last visited 2011-02-20)</i>"
                      "\n\n"
                      "<b>GLIM</b>\n"
                      "  see http://www.artifactgames.de/FILES/START.PHP?where=GLIM.PHP\n"
                      "  <i>(last visited 2011-03-01)</i>");

  dlg.run();
}

MainWindow::MainWindow()
{
  g_assert(!main_window);
  main_window = this;

  _process  = Process::create();

  _sensitive_for_changes = true;

  _layers = Gtk::manage(new LayerView);
  view_3d = Gtk::manage(new View3D);
  view_settings = new ViewSettings;

  view_settings->signal_something_changed().connect(sigc::mem_fun(view_3d, &View3D::invalidate));
  view_settings->bring_to_front();

  raytracer = Raytracer::Manager::create();

  signal_hide().connect(sigc::ptr_fun(&Process::abort_all));

  add(_vbox);
    _vbox.show();
    _vbox.pack_start(_menu_bar, false, false);
      _menu_bar.show();
    //_vbox.pack_start(_tool_bar, false, false);
    //  _tool_bar.show();
    _vbox.pack_start(_hpaned);
      signal_sensitive_for_changes_changed().connect(SettingsWidget::create_updater<bool, bool>(sigc::mem_fun(*this, &MainWindow::get_sensitive_for_changes), sigc::mem_fun(_hpaned, &Gtk::HPaned::set_sensitive)));
      signal_sensitive_for_changes_changed().connect(SettingsWidget::create_updater<bool, bool>(sigc::mem_fun(*this, &MainWindow::get_sensitive_for_changes), sigc::mem_fun(_hpaned, &Gtk::HPaned::set_visible)));
      _hpaned.show();
      _hpaned.set_border_width(LENGTH_BORDER_WIDTH);
      _hpaned.pack1(*view_3d);
      _hpaned.pack2(_vpaned);
        _vpaned.show();
        _vpaned.pack1(_layers_scrollbars);
          _layers_scrollbars.show();
          _layers_scrollbars.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
          _layers_scrollbars.add(*_layers);
          _layers_scrollbars.set_shadow_type(Gtk::SHADOW_IN);
            _layers->show();
        _vpaned.pack2(_settings);
          last_settings_size_request  = 0;
          _settings.signal_size_request().connect(sigc::mem_fun(*this, &MainWindow::adapt_settings_size_request));
          _settings.show();
        _vpaned.set_position(96);
      _vbox.pack_start(_render_preview_scrollbars);
         _render_preview_scrollbars.set_border_width(LENGTH_BORDER_WIDTH);
         signal_sensitive_for_changes_changed().connect(SettingsWidget::create_updater<bool, bool>(sigc::bind(sigc::ptr_fun(negate), sigc::mem_fun(*this, &MainWindow::get_sensitive_for_changes)), sigc::mem_fun(_render_preview_scrollbars, &Gtk::ScrolledWindow::set_visible)));
        _render_preview_scrollbars.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
        _render_preview_scrollbars.add(_render_preview);
          _render_preview.show();
          Raytracer::Manager::get_resulting_image().on_invalidated = sigc::mem_fun(*this, &MainWindow::invalidate_render_preview);
          Raytracer::Manager::get_resulting_image().signal_new_pixbuf_created().connect(sigc::mem_fun(_render_preview, &RenderResultView::pixbuf_object_changed));
      _vbox.pack_end(_statusbar, false, false);
      _vbox.pack_end(_statusbar_sep, false, false, LENGTH_SMALLSPACE);
        _statusbar_sep.show();
        _statusbar.show();
          last_statusbar_size_request  = 0;
          _statusbar.signal_size_request().connect(sigc::mem_fun(*this, &MainWindow::adapt_statusbar_size_request));
          _statusbar.set_spacing(LENGTH_BORDER_WIDTH);
          _statusbar.pack_start(_status_label, false, false);
          _statusbar.pack_start(_status_progressbar);
          _statusbar.pack_start(_status_abortbutton, false, false);
            _status_abortbutton.set_label(_("Abort"));
            _status_abortbutton.signal_clicked().connect(sigc::ptr_fun(&Process::abort_process));
        update_statusbar();
        Process::signal_something_changed().connect(sigc::mem_fun(*this, &MainWindow::update_statusbar));

  _menu_bar.append(menu_file);
    menu_file.set_label(_("_File"));
    menu_file.set_use_underline();
    menu_file.set_submenu(menu_file_menu);
      menu_file_menu.append(menu_file_export);
        menu_file_export.set_label(_("_Export"));
        menu_file_export.set_use_underline();
        menu_file_export.show();
        menu_file_export.set_submenu(menu_file_export_menu);
          menu_file_export_menu.append(menu_file_export_sep1);
          menu_file_export_menu.append(menu_file_export_uv_not_warped);
            menu_file_export_uv_not_warped.set_label(_("Export not warped Sphere-UV as SVG"));
            menu_file_export_uv_not_warped.show();
            menu_file_export_uv_not_warped.signal_activate().connect(sigc::bind(sigc::ptr_fun(&UVMeshExporter::export_uv_mesh), false));
          menu_file_export_menu.append(menu_file_export_uv_warped);
            menu_file_export_uv_warped.set_label(_("Export warped Sphere-UV as SVG"));
            menu_file_export_uv_warped.show();
            menu_file_export_uv_warped.signal_activate().connect(sigc::bind(sigc::ptr_fun(&UVMeshExporter::export_uv_mesh), true));
      menu_file_menu.append(menu_file_sep1);
        menu_file_sep1.show();
      menu_file_menu.append(menu_file_quit);
        menu_file_quit.set_accel_key("<control>q");
        menu_file_quit.set_label(_("_Quit"));
        menu_file_quit.set_use_underline();
        menu_file_quit.signal_activate().connect(sigc::mem_fun(*this, &Gtk::Window::hide));
  _menu_bar.append(menu_render);
    menu_render.set_label(_("_Render"));
    menu_render.set_submenu(menu_render_menu);
    menu_render.set_use_underline();
      menu_render_menu.append(menu_render_render_view);
        menu_render_render_view.set_label(_("Render View"));
        menu_render_render_view.set_accel_key("F5");
        menu_render_render_view.set_use_underline();
        menu_render_render_view.signal_activate().connect(sigc::bind(sigc::ptr_fun(&Raytracer::Manager::render), true));
        signal_sensitive_for_changes_changed().connect(SettingsWidget::create_updater<bool, bool>(sigc::mem_fun(*this, &MainWindow::get_sensitive_for_changes), sigc::mem_fun(menu_render_render_view, &MyMenuItem::set_sensitive)));
      menu_render_menu.append(menu_render_render);
        menu_render_render.set_label(_("Render"));
        menu_render_render.set_accel_key("<control>F5");
        menu_render_render.set_use_underline();
        menu_render_render.signal_activate().connect(sigc::bind(sigc::ptr_fun(&Raytracer::Manager::render), false));
        signal_sensitive_for_changes_changed().connect(SettingsWidget::create_updater<bool, bool>(sigc::mem_fun(*this, &MainWindow::get_sensitive_for_changes), sigc::mem_fun(menu_render_render, &MyMenuItem::set_sensitive)));
      menu_render_menu.append(menu_render_sep1);
      menu_render_menu.append(menu_render_close_preview);
        menu_render_close_preview.set_sensitive(false);
        menu_render_close_preview.set_label(_("_Close View"));
        menu_render_close_preview.set_accel_key("Escape");
        menu_render_close_preview.set_use_underline();
        menu_render_close_preview.signal_activate().connect(sigc::bind(sigc::ptr_fun(&Raytracer::Manager::set_doing_preview), false));
        Raytracer::Manager::signal_doing_preview().connect(SettingsWidget::create_updater<bool, bool>(sigc::ptr_fun(&Raytracer::Manager::get_doing_preview), sigc::mem_fun(menu_render_close_preview, &MyMenuItem::set_sensitive)));
      menu_render_menu.append(menu_render_save_result);
        menu_render_save_result.set_sensitive(false);
        menu_render_save_result.set_label(_("_SaveResult"));
        menu_render_save_result.set_use_underline();
      menu_render_menu.append(menu_render_sep2);
      menu_render_menu.append(menu_render_settings);
        menu_render_settings.set_label(_("Render _Settings"));
        menu_render_settings.set_use_underline();
        menu_render_settings.signal_activate().connect(sigc::ptr_fun(&Raytracer::Manager::open_settings));
  _menu_bar.append(menu_view);
    menu_view.set_label(_("_View"));
    menu_view.set_use_underline();
    menu_view.set_submenu(menu_view_menu);
    signal_sensitive_for_changes_changed().connect(SettingsWidget::create_updater<bool, bool>(sigc::mem_fun(*this, &MainWindow::get_sensitive_for_changes), sigc::mem_fun(menu_view, &MyMenuItem::set_sensitive)));
      menu_view_menu.append(menu_view_show_sidebar);
        menu_view_show_sidebar.set_accel_key("F9");
        menu_view_show_sidebar.set_label(_("Show _Sidebar"));
        menu_view_show_sidebar.set_use_underline();
        menu_view_show_sidebar.signal_toggled().connect(sigc::mem_fun(*this, &MainWindow::_adapt_show_sidebar));
        menu_view_show_sidebar.set_active(true);
      menu_view_menu.append(menu_view_wireframed);
        menu_view_wireframed.set_accel_key("w");
        menu_view_wireframed.set_label(_("_Wireframed"));
        menu_view_wireframed.set_use_underline();
        menu_view_wireframed.set_active(view_3d->get_draw_wireframed());
        menu_view_wireframed.signal_toggled().connect(sigc::mem_fun(*this, &MainWindow::draw_wireframed_toggled));
        view_3d->signal_wireframed_changed().connect(sigc::mem_fun(menu_view_wireframed, &MyCheckMenuItem::set_active));
      menu_view_menu.append(menu_view_settings);
        menu_view_settings.set_label(_("View _Settings"));
        menu_view_settings.set_use_underline();
        menu_view_settings.signal_activate().connect(sigc::mem_fun(*view_settings, &SettingsWidget::bring_to_front));
      menu_view_menu.append(menu_view_reload_shader);
        menu_view_reload_shader.set_label(_("_Reload Shader"));
        menu_view_reload_shader.set_use_underline();
        menu_view_reload_shader.set_accel_key("<control>F12");
        menu_view_reload_shader.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::reload_shader));
  _menu_bar.append(menu_info);
    menu_info.set_label(_("_Info"));
    menu_info.set_use_underline();
    menu_info.set_submenu(menu_info_menu);
    menu_info_menu.append(menu_info_about);
      menu_info_about.set_label(_("_About"));
      menu_info_about.set_use_underline();
      menu_info_about.signal_activate().connect(sigc::ptr_fun(about_dlg));
  _menu_bar.show_all_children();

  set_title(_("Planet Generator"));

  show();
}

MainWindow::~MainWindow()throw()
{
  view_3d->deinit();

  main_window = nullptr;
}

void MainWindow::on_realize()
{
  Gtk::Window::on_realize();
}

void MainWindow::on_show()
{
  Gtk::Window::on_show();
}

void MainWindow::update_statusbar()
{
  Process::State state;

  Process::fill_state(state);

  if(state.abortable)
    _status_abortbutton.show();
  else
    _status_abortbutton.hide();

  _status_abortbutton.set_sensitive(!state.is_aborted);

  if(state.has_progress)
  {
    _status_label.hide();
    _status_progressbar.set_text(state.state);
    _status_progressbar.show();
    _status_progressbar.set_fraction(state.p);
  }else
  {
    _status_label.set_label(state.state);
    _status_progressbar.hide();
    _status_label.show();
  }

  while(Gtk::Main::events_pending())
  {
    Gtk::Main::iteration();
  }
}

void MainWindow::get_render_view_size(guint& width, guint& height)
{
  g_assert(this);
  width = _render_preview.get_width();
  height = _render_preview.get_height();
  _render_preview.set_size_request(width, height);
  _render_preview_scrollbars.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
}

void MainWindow::reload_shader()
{
  View3D* v = get_view_3d();

  if(!v)
    return;

  try
  {
    v->deinit_shaders();
    v->init_shaders();
    v->invalidate();
  }catch(BadShader)
  {
    Gtk::MessageDialog dlg(_("Retry?"), false, Gtk::MESSAGE_QUESTION);
    dlg.add_button(_("_Yep"), 42);
    dlg.add_button(_("_Nope"), 0);
    if(dlg.run()==42)
      reload_shader();
  }
}

MainWindow::RenderResultView::RenderResultView()
{
}

void MainWindow::prepare_render(bool viewer_size)
{
  if(viewer_size)
  {
    _render_preview_scrollbars.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_NEVER);
    _render_preview.set_size_request(-1, -1);

  }else
  {
    _render_preview_scrollbars.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    _render_preview.set_size_request(Raytracer::Manager::get_settings().get_width(), Raytracer::Manager::get_settings().get_height());
  }

  while(Gtk::Main::events_pending())
    Gtk::Main::iteration();
}

void MainWindow::RenderResultView::pixbuf_object_changed()
{
  Glib::RefPtr<Gdk::Pixbuf> pb  = Raytracer::Manager::get_resulting_image().get_pixbuf();

  if(!pb)
    return;
}

bool MainWindow::RenderResultView::on_expose_event(GdkEventExpose* e)
{
  g_assert(e);

  Glib::RefPtr<Gdk::Window> window  = get_window();
  Glib::RefPtr<Gdk::Pixbuf> pb  = Raytracer::Manager::get_resulting_image().get_pixbuf();

  if(window)
  {
    Glib::RefPtr<Gdk::GC> gc = Gdk::GC::create(window);
    Gdk::Color black("000000");

    gc->set_foreground(black);
    if(pb)
    {
      int w = MIN(get_width(), pb->get_width());
      int h = MIN(get_height(), pb->get_height());
      window->draw_rectangle(gc, true, 0, 0, w, h);
      window->draw_pixbuf(gc, pb, 0, 0, 0, 0, w, h, Gdk::RGB_DITHER_NONE, 0, 0);
    }else
    {
      window->draw_rectangle(gc, true, 0, 0, get_width(), get_height());
    }
  }

  return true;
}

void MainWindow::invalidate_render_preview()
{
  Glib::RefPtr<Gdk::Window> w = _render_preview.get_window();

  if(w)
    w->invalidate(false);
}

void MainWindow::adapt_statusbar_size_request(Gtk::Requisition* r)
{
  r->height  = last_statusbar_size_request = MAX(r->height, last_statusbar_size_request);
}

void MainWindow::adapt_settings_size_request(Gtk::Requisition* r)
{
  r->width  = last_settings_size_request = MAX(r->width, last_settings_size_request);
}

void MainWindow::append_settings_widget(Gtk::Widget& w)
{
  _settings.pack_start(w);
}

void MainWindow::_adapt_show_sidebar()
{
  g_assert(_hpaned.get_child2());

  int side_width  = _hpaned.get_child2()->get_width();
  bool maximized  = false;

  if(get_window())
    maximized = get_window()->get_state() & Gdk::WINDOW_STATE_MAXIMIZED;

  if(!get_show_sidebar())
  {
    _hpaned.get_child2()->hide();
    //_menu_bar.hide();
    //_tool_bar.hide();

    if(!maximized)
      resize(get_width()-side_width, get_height());
  }else
  {
    _hpaned.get_child2()->show();
    //_menu_bar.show();
    //_tool_bar.show();

    if(!maximized)
      resize(get_width()+side_width, get_height());
  }
}

// ---- ---- ---- ----

Process* Process::_singleton  = nullptr;

void Process::_block_changes(bool unblock)
{
  main_window->set_sensitive_for_changes(unblock);
}
