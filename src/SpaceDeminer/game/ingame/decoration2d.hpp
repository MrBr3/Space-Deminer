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

#ifndef _SPACE_DEMINER_INGAME_DECORATION_H_
#define _SPACE_DEMINER_INGAME_DECORATION_H_

#include "./ingame.hpp"

namespace InGame
{
  class Decoration2D : public Refable
  {
  public:
    /** \brief Defines the order of the decoration to render.
     *
     * \note 0 is set to be the image of the ship, negative numbers are shown behind and positive number are shown above the ship.
    */
    gint render_order;

    bool visible;

    bool never_show_again;

    //virtual void render()=0;

    Decoration2D();
    ~Decoration2D()throw();
  };
}

#endif