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

#ifndef _SPACEDEMINER_UI_FRAMEWORK_IMAGE_H_
#define _SPACEDEMINER_UI_FRAMEWORK_IMAGE_H_

#include "./resources.hpp"

namespace Framework
{
  class Image : public Resource
  {
  public:
    typedef sigc::slot<Glib::RefPtr<Gdk::Pixbuf> > SlotRecreateFromPixbuf;

    typedef TextureHint Hint;

    /** \brief Does nothing than throwing an instance of CantReloadRes.
     * */
    static Glib::RefPtr<Gdk::Pixbuf> cant_reload_from_pixbuf(){throw CantReloadRes();}

    class ReLoader : public Refable
    {
    public:
      virtual void reload(Image& img)=0;

    };

    Glib::RefPtr<ReLoader> re_loader;

    int get_width()const throw(){g_assert(this);return _width;}
    int get_height()const throw(){g_assert(this);return _height;}

    /**
     * Identical to:
     * \code
     *   Engine::get_singleton()->create_image(pixbuf);
     * \endcode
     * */
    static ResPtr<Image> create(const Glib::RefPtr<Gdk::Pixbuf>& pixbuf, Hint hint=TEXTURE_HINT_DEFAULT, const SlotRecreateFromPixbuf& slot_recreate=sigc::ptr_fun(&cant_reload_from_pixbuf));

    /**
     * Identical to:
     * \code
     *   Engine::get_singleton()->create_image(filename);
     * \endcode
     * */
    static ResPtr<Image> create_from_file(const Glib::ustring& filename, Hint hint=TEXTURE_HINT_DEFAULT);

  protected:
    Image()
    {
      _width=0;
      _height=0;
    }
    ~Image()throw()
    {
    }

  protected:
    void set_width (Real w){_width =w;}
    void set_height(Real h){_height=h;}

  private:
    int _width, _height;
  };
}

#endif