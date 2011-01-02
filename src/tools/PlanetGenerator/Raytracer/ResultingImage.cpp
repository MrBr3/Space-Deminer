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

#include "./../MainWindow.hpp"

namespace Raytracer
{
  ResultingImage::ResultingImage()
  {
    _width  = 64;
    _height  = 64;
  }

  ResultingImage::~ResultingImage()throw()
  {
  }

  void ResultingImage::prepare(bool preview)
  {
    if(preview)
    {
      main_window->get_render_view_size(_width, _height);
    }else
    {
      _width = Manager::get_settings().get_width();
      _height = Manager::get_settings().get_height();
    }

    _width  = Settings::correct_image_size(_width);
    _height  = Settings::correct_image_size(_height);
  }
}