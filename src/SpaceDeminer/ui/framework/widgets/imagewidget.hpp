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

#ifndef _SPACEDEMINER_UI_FRAMEWORK_IMAGEWIDGET_H_
#define _SPACEDEMINER_UI_FRAMEWORK_IMAGEWIDGET_H_

#include "./misc.hpp"

namespace Framework
{
  class ImageWidget : public Misc
  {
  public:
    ImageWidget();
    ~ImageWidget()throw();

    ImageWidget(const Framework::ResPtr<Framework::Image>& image);

  public:

    const Framework::ResPtr<Framework::Image>& get()const{return _image;}

    void set(const Framework::ResPtr<Framework::Image>& image);
    void clear();

    void on_expose(EventExpose& paint_tool);

    bool get_resize()const throw(){return _resize;}
    void set_resize(bool resize=true);

    void on_size_request(int& w, int& h);

  private:
    Framework::ResPtr<Framework::Image> _image;
    bool _resize;
  };
}

#endif
