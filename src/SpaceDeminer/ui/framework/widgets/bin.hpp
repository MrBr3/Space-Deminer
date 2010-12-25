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

#ifndef _SPACEDEMINER_UI_FRAMEWORK_BIN_H_
#define _SPACEDEMINER_UI_FRAMEWORK_BIN_H_

#include "./container.hpp"

namespace Framework
{
  class Bin : public Container
  {
  public:
    class BinHasAlreadyChild : public Glib::Exception
    {
    public:
      Glib::ustring what()const{return _("Trying to add a second widget to bin containing already one child.");}

      ~BinHasAlreadyChild()throw(){}
    };

    /** \brief Removes a Widget
     *
     * \note throws an exception if there's no child.
     * */
    void remove();

    /** \brief Removes a Widget if possible
     * */
    void remove_if_child()
    {
      if(get_child())
        remove();
    }

    Widget* get_child(){return _child;}
    const Widget* get_child()const{return _child;}

    /** Creates a Label and adds it.
     *
     * \note The created child will be
     *
     * */
    void set_label(const Glib::ustring& label, bool use_memnotic=false, Real xalign=.5, Real yalign=.5);
    void set_image(const ResPtr<Image>& image, Real xalign=.5, Real yalign=.5);

    void remove(Widget& w);

    void add(Widget& widget);

    /**
     * The returned size is equivalent to re size request of the child (or zero if there's no child) plus the border_with multiplied with two:
     *  \code
     *  void Bin::on_size_request(int& w, int& h)
     *  {
     *    Container::on_size_request(w, h);
     *
     *    if(_child)
     *    {
     *      w += _child->get_size_request_width();
     *      h += _child->get_size_request_height();
     *    }
     *  }
     * \endcode
     * */
    void on_size_request(int& w, int& h);

  protected:
    Bin();
    ~Bin()throw();

  private:
    Widget* _child;
    Widget* _private_child;
  };
}

#endif