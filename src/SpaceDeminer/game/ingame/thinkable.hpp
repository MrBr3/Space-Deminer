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

#ifndef _SPACE_DEMINER_INGAME_THINKABLE_H_
#define _SPACE_DEMINER_INGAME_THINKABLE_H_

#include "./ingame.hpp"

namespace InGame
{
  class Thinkable : public Refable
  {
  public:
    /** \brief The order of the thinkables to think.
    */
    guint think_order;

    /** \brief Gets called when the scene should refresh it's state.
     *
     * \param scene a reference to the scene.
    */
    virtual void on_think(Scene& scene) = 0;

  protected:
    Thinkable();
    ~Thinkable()throw();
  };
}

#endif