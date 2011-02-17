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

#include "./messagedialog.hpp"

namespace Framework
{
  MessageDialog::MessageDialog(const Glib::ustring& text, bool use_memnotics) : _text(text, use_memnotics)
  {
    dlg_vbox.pack_start(_hbox);

    _hbox.show();
    _hbox.pack_start(_img, false, false);
    _hbox.pack_start(_text);
    _text.set_alignment(0.f, 0.f);
  }

  MessageDialog::MessageDialog()
  {
    dlg_vbox.pack_start(_hbox);

    _hbox.show();
    _hbox.pack_start(_img, false, false);
    _hbox.pack_start(_text);
    _text.set_alignment(0.f, 0.f);
  }

  MessageDialog::MessageDialog(const Framework::ResPtr<Framework::Image>& image, const Glib::ustring& text, bool use_memnotics) : _img(image), _text(text, use_memnotics)
  {
    _hbox.show();

    set_image(image);

    _hbox.show();
    _hbox.pack_start(_img, false, false);
    _hbox.pack_start(_text);
    _text.set_alignment(0.f, 0.f);
  }

  MessageDialog::~MessageDialog()throw()
  {
  }

  void MessageDialog::set_text(const Glib::ustring& text, bool use_memnotics)
  {
    _text.set_label(text, use_memnotics);
  }

  void MessageDialog::set_image(const Framework::ResPtr<Framework::Image>& image)
  {
    _img.set(image);

    if(!image)
      _img.hide();
    else
      _img.show();
  }

  void MessageDialog::set_theme(const Glib::RefPtr<Theme>& theme)
  {
    ParentClass::set_theme(theme);

    if(theme)
      _hbox.set_spacing(theme->get_spacing(Theme::SPACING_NORMAL));
  }

  void MessageDialog::setup_ok(const Framework::ResPtr<Framework::Image>& image, const Glib::ustring& text, bool use_memnotics)
  {
    set_image(image);
    set_text(text, use_memnotics);

    add_button(_("_Ok"), RESPONSE_OK);
  }

  bool MessageDialog::setup_yes_no(const Framework::ResPtr<Framework::Image>& image, const Glib::ustring& text, bool use_memnotics)
  {
    set_image(image);
    set_text(text, use_memnotics);

    add_button(_("_Yes"), RESPONSE_YES);
    add_button(_("_No"), RESPONSE_NO);
  }
}
