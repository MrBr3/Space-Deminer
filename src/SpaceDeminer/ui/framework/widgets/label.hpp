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

#ifndef _SPACEDEMINER_UI_FRAMEWORK_LABEL_H_
#define _SPACEDEMINER_UI_FRAMEWORK_LABEL_H_

#include "./misc.hpp"

namespace Framework
{
  class Label : public Misc
  {
  public:
    const Glib::ustring& get_label()const throw(){return _label;}
    bool get_use_memnotic()const throw(){return _memnotic;}

    void set_label(const Glib::ustring& label, bool use_memnotic=false);
    void set_theme(const Glib::RefPtr<Theme>& theme);

    void on_size_request(int& w, int& h);
    void on_expose(EventExpose& paint_tool);

    Label();
    Label(const Glib::ustring& label, bool use_memnotic=false);

    ~Label()throw();

  private:
    Glib::ustring _label;
    bool _memnotic;

    ResPtr<Font> _font;
  };
}

#endif