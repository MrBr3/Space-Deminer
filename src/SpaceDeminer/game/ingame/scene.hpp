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

#ifndef _SPACE_DEMINER_INGAME_SCENE_H_
#define _SPACE_DEMINER_INGAME_SCENE_H_

#include "./ingame.hpp"

namespace InGame
{
  class Thinkable;
  class Player;
  class Mine;
  class Object3D;
  class Scene : public Refable
  {
  public:
    std::list<Glib::RefPtr<Thinkable> > thinkables;
    std::list<Glib::RefPtr<Object3D> > background_objects, foreground_objects;

    std::list<Glib::RefPtr<Mine> > mines;
    Glib::RefPtr<Player> player;

    /*void render()
    {
      forall(background_objects)iter->render();
      for(std::list<Glib::RefPtr<Mine> >::iterator iter = mines.begin(); iter!=mines.end(); ++iter)
        iter->render();
      player->render();
      forall(foreground_objects)iter->render();
    }*/

  public:
    Scene();
    ~Scene()throw();
  };
}

#endif