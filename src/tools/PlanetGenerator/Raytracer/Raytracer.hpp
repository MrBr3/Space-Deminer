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

// see http://wiki.delphigl.com/index.php/Tutorial_Raytracing_-_Grundlagen_I (last time visited at 2010-12-30 (see README in Root Project Folder))

#include "./../Model.hpp"

#include "./RaytracerSettings.hpp"

namespace Raytracer
{
  const int max_image_size  = 4096;

  class Manager : public Refable
  {
    static Manager* _singleton;

    ObsLink<Settings> _settings;

    Manager();
  public:
    static Manager* get_singletonA(){g_assert(_singleton);return _singleton;}

    static Settings& get_settings(){g_assert(get_singletonA()->_settings);*get_singletonA()->_settings;}

    static void open_settings();

    static Glib::RefPtr<Manager> create(){return Glib::RefPtr<Manager>(new Manager);}

    ~Manager()throw();
  };
}