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

ImageFileSettings::ImageFileSettings(const Glib::RefPtr<ImageFile>& imagefile, const Glib::ustring& name, const Glib::ustring& label, const Glib::ustring& tooltip)
{
  set_label_widget(_label);

  image_file  = imagefile;

  set_expanded(true);

  _n_entries  = 0;

  _label.show();
  _hbox.show();
  table.show();

  _label.set_label(label);
  set_tooltip_text(tooltip);

  add(table);

  Gtk::FileFilter img_filter;
  img_filter.set_name(_("Image Files"));
  img_filter.add_pixbuf_formats();

  SettingsWidget::append_filename_widget(table, _n_entries, name, label, tooltip, img_filter,
                                         sigc::mem_fun(*image_file.operator->(), &ImageFile::get_filename),
                                         sigc::mem_fun(*image_file.operator->(), &ImageFile::set_filename),
                                         image_file->signal_imagefile_changed());
  SettingsWidget::append_boolean_widget(table, _n_entries, name+"-needs-to-be-warped", "Warp?", "Decides, whether the Image needs to be warped before beeing used",
                                       sigc::mem_fun(*image_file.operator->(), &ImageFile::get_needs_to_be_warped),
                                       sigc::mem_fun(*image_file.operator->(), &ImageFile::set_needs_to_be_warped),
                                       image_file->signal_needs_to_be_warped_changed());

  if(imagefile->correction_curve_type==ImageFile::COLOR_CURVE)
    SettingsWidget::append_colorcurve_widget(table, _n_entries, name+"-color-curve", _("ColorCurve"), _("Allows you to modify the Rexture's Contrast, Brightness, Color ..."), imagefile->get_color_curve());
  else if(imagefile->correction_curve_type==ImageFile::CONTRAST_CURVE)
    SettingsWidget::append_curve_widget(table, _n_entries, name+"-contrast-curve", _("ConstrastCurve"), _("Allows you to modify the Rexture's Contrast, Brightness, ..."), imagefile->get_contrast_curve());
}

ImageFileSettings::~ImageFileSettings()throw()
{
}
