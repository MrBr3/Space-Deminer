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

#ifndef _SPACEDEMINER_UI_FRAMEWORK_DIALOG_H_
#define _SPACEDEMINER_UI_FRAMEWORK_DIALOG_H_

#include "./../window.hpp"
#include "./alignment.hpp"
#include "./button.hpp"
#include "./separator.hpp"
#include "./buttonbox.hpp"
#include "./windowframe.hpp"

namespace Framework
{
  class Dialog : public FullscreenWindow
  {
    gint _response;
    bool _fade_other_out;

    sigc::signal<void, gint> _signal_response;
    std::map<gint, sigc::signal<void> > _signal_certain_response;

  public:
    typedef FullscreenWindow ParentClass;

    static const gint NO_RESPONSE = G_MAXINT;
    static const gint RESPONSE_NO = -2;
    static const gint RESPONSE_CANCEL = -1;
    static const gint RESPONSE_OK = 1;
    static const gint RESPONSE_YES = 2;
    static const gint RESPONSE_RETRY = 3;

    Alignment alignment;

    WindowFrame windowframe;
    VBox dlg_vbox;
    HSeparator separator;
    HButtonBox actionarea;

    void set_theme(const Glib::RefPtr<Theme>& theme);

    Button* add_button(const Glib::ustring& text, gint response_id, bool use_memnotics = true);

    void on_expose(EventExpose& paint_tool);

    void response(gint r);

    void run();

    void set_default_size(int w, int h);

    sigc::signal<void, gint>& signal_response(){return _signal_response;}
    sigc::signal<void>& signal_certain_response(gint response_id){return _signal_certain_response[response_id];}

    bool get_fade_other_out()const{return _fade_other_out;}
    void set_fade_other_out(bool b);

    Dialog();
    ~Dialog()throw();
  };
}

#endif
