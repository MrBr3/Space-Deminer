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

#include "./files.hpp"

bool exist_file_or_dir(const Glib::ustring& filename)
{
  if(!filename.length())
    return false;

  Glib::RefPtr<Gio::File> file = Gio::File::create_for_path(Glib::filename_from_utf8(filename));

  return file && file->query_exists();
}

bool exist_directory(const Glib::ustring& filename)
{
  if(!filename.length())
    return false;

  Glib::RefPtr<Gio::File> file = Gio::File::create_for_path(Glib::filename_from_utf8(filename));

  if(!file || !file->query_exists())
  {
    return false;
  }
  if(file->query_file_type()!=Gio::FILE_TYPE_DIRECTORY)
    return false;
  return true;
}

bool exist_regular_file(const Glib::ustring& filename)
{
  if(!filename.length())
    return false;

  Glib::RefPtr<Gio::File> file = Gio::File::create_for_path(Glib::filename_from_utf8(filename));

  if(!file || !file->query_exists())
  {
    return false;
  }
  if(file->query_file_type()!=Gio::FILE_TYPE_REGULAR)
    return false;
  return true;
}

bool make_all_parent_directories(const Glib::RefPtr<Gio::File>& file)
{
  if(!file)
    throw std::invalid_argument("file");

  Glib::RefPtr<Gio::File> folder  = file->get_parent();
  std::stack<Glib::RefPtr<Gio::File> > parents;
  while(!folder->query_exists())
  {
    parents.push(folder);
    folder  = folder->get_parent();
  }
  while(parents.size())
  {
    if(!parents.top()->make_directory())
      return false;
    parents.pop();
  }
  return true;
}

bool make_all_parent_directories(const Glib::ustring& filename)
{
  Glib::RefPtr<Gio::File> file = Gio::File::create_for_path(Glib::filename_from_utf8(filename));
  return make_all_parent_directories(file);
}