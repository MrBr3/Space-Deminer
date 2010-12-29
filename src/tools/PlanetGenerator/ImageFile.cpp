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

#include "./Model.hpp"

ImageFile::ImageFile()
{
  signal_imagefile_changed().connect(sigc::mem_fun(_signal_something_changed, &sigc::signal<void>::emit));
  signal_needs_to_be_warped_changed().connect(sigc::mem_fun(_signal_something_changed, &sigc::signal<void>::emit));
}

ImageFile::~ImageFile()throw()
{
}

Glib::RefPtr<Gdk::Pixbuf> ImageFile::create_pixbuf()
{
  try
  {
    return Gdk::Pixbuf::create_from_file(Glib::filename_from_utf8(_filename));
  }catch(...)
  {
  }
  return Glib::RefPtr<Gdk::Pixbuf>();
}

void ImageFile::set_filename(const Glib::ustring& filename)
{
  //Glib::RefPtr<Gio::File> file  = Gio::File::create_for_path(Glib::filename_from_utf8(filename));

  _filename = filename;

  _signal_imagefile_changed.emit();
}

void ImageFile::set_needs_to_be_warped(bool w)
{
  _needs_to_be_warped = w;

  signal_needs_to_be_warped_changed().emit();
}