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

#ifndef _SPACEDEMINER_UI_FRAMEWORK_PANED_H_
#define _SPACEDEMINER_UI_FRAMEWORK_PANED_H_

#include "./container.hpp"

namespace Framework
{
  class Paned : public Container
  {
  public:
    Paned(bool h);
    ~Paned()throw();

  private:
    const bool _horizontal;
  };

  class VPaned : public Paned
  {
  public:
    VPaned();
    ~VPaned()throw();
  };

  class HPaned : public Paned
  {
  public:
    HPaned();
    ~HPaned()throw();
  };
}

#endif