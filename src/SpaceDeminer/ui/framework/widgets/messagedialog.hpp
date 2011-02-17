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

#ifndef _SPACEDEMINER_UI_FRAMEWORK_MESSAGEDIALOG_H_
#define _SPACEDEMINER_UI_FRAMEWORK_MESSAGEDIALOG_H_

#include "./dialog.hpp"
#include "./label.hpp"
#include "./imagewidget.hpp"

namespace Framework
{
  class MessageDialog : public Dialog
  {
    ImageWidget _img;
    HBox _hbox;
    Label _text;
  public:
    typedef Dialog ParentClass;

    void set_text(const Glib::ustring& text, bool use_memnotics = false);
    void set_image(const Framework::ResPtr<Framework::Image>& image);

    const Glib::ustring& get_text()const{return _text.get_label();}
    const Framework::ResPtr<Framework::Image>& get_image()const{return _img.get();}

    void set_theme(const Glib::RefPtr<Theme>& theme);

    MessageDialog();
    MessageDialog(const Glib::ustring& text, bool use_memnotics=false);
    MessageDialog(const Framework::ResPtr<Framework::Image>& image, const Glib::ustring& text, bool use_memnotics=false);
    ~MessageDialog()throw();

    void setup_ok(const Glib::ustring& text, bool use_memnotics=true){setup_ok(Framework::ResPtr<Framework::Image>(), text, use_memnotics);}
    bool setup_yes_no(const Glib::ustring& text, bool use_memnotics=true){return setup_yes_no(Framework::ResPtr<Framework::Image>(), text, use_memnotics);}

    void setup_ok(const Framework::ResPtr<Framework::Image>& image, const Glib::ustring& text, bool use_memnotics=false);
    bool setup_yes_no(const Framework::ResPtr<Framework::Image>& image, const Glib::ustring& text, bool use_memnotics=false);
  };
}

#endif
