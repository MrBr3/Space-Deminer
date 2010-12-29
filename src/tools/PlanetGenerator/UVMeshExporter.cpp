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

#include "./MainWindow.hpp"

const gdouble full_width  = 1024.0;
const gdouble full_height  = 512.0;

UVMeshExporter* UVMeshExporter::_singleton  = nullptr;

UVMeshExporter::UVMeshExporter(const Glib::ustring& filename, bool draw_warped, int n_segments)
{
  g_assert(!_singleton);
  g_assert(n_segments>0);
  _singleton  = this;

  Cairo::RefPtr<Cairo::SvgSurface> svg_surface = Cairo::SvgSurface::create(Glib::filename_from_utf8(filename), full_width, full_height);
  _cc = Cairo::Context::create(svg_surface);

  _cc->set_source_rgb(0., 0., 0.);
  _cc->set_line_width(0.05*full_height/n_segments);

  _draw_warped  = draw_warped;
}

UVMeshExporter::~UVMeshExporter()throw()
{
  _cc->stroke();

  g_assert(_singleton);
  _singleton  = nullptr;
}

void UVMeshExporter::add_triangle(const Vector2& a, const Vector2& b, const Vector2& c, bool is_warped)
{
  if(!_singleton || is_warped!=_singleton->_draw_warped)
    return;

  _singleton->_cc->move_to(a.x*full_width, a.y*full_height);
  _singleton->_cc->line_to(b.x*full_width, b.y*full_height);
  _singleton->_cc->line_to(c.x*full_width, c.y*full_height);
  _singleton->_cc->close_path();
}

void UVMeshExporter::add_quad(const Vector2& a, const Vector2& b, const Vector2& c, const Vector2& d, bool is_warped)
{
  if(!_singleton || is_warped!=_singleton->_draw_warped)
    return;

  _singleton->_cc->move_to(a.x*full_width, a.y*full_height);
  _singleton->_cc->line_to(b.x*full_width, b.y*full_height);
  _singleton->_cc->line_to(c.x*full_width, c.y*full_height);
  _singleton->_cc->line_to(d.x*full_width, d.y*full_height);
  _singleton->_cc->close_path();
}

void UVMeshExporter::export_uv_mesh(bool warped)
{
  Glib::ustring filename;
  {
    Gtk::FileFilter file_filter;
    Gtk::FileChooserDialog dlg(warped ? _("Export warped UV Mesh as SVG-File") : _("Export UV Mesh as SVG-File"), Gtk::FILE_CHOOSER_ACTION_SAVE);

    file_filter.set_name(_("Svg Imagefile"));
    file_filter.add_mime_type("image/svg+xml");

    if(warped)
      dlg.set_current_name("sphere-uv-warped.svg");
    else
      dlg.set_current_name("sphere-uv.svg");

    dlg.add_filter(file_filter);

    dlg.add_button(Gtk::Stock::CANCEL, 1);
    dlg.add_button(Gtk::Stock::SAVE, 0);

    if(dlg.run()!=0)
      return;

    filename  = dlg.get_filename();
  }

  int n_segments  = main_window->get_view_settings().get_n_sphere_segments(); // TODO use the subdivision of the viewsettings

  UVMeshExporter exporter(filename, warped, n_segments);
  {
    SphereMesh sphere_mesh;
    sphere_mesh.set_segment_division(n_segments);
  }
}