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

#include "./paned.hpp"

namespace Framework
{
  Paned::Paned(bool h) : _horizontal(h)
  {
    set_name("paned");
  }

  Paned::~Paned()throw()
  {
  }

  //--------

  HPaned::HPaned() : Paned(true)
  {
    set_name("hpaned");
  }

  HPaned::~HPaned()throw()
  {
  }

  //--------

  VPaned::VPaned() : Paned(false)
  {
    set_name("vpaned");
  }

  VPaned::~VPaned()throw()
  {
  }
}