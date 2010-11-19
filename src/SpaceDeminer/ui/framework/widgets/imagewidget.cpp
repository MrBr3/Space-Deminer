/* Space Deminer is a small 2D Arcade Game. Your task ist to eliminate
 * intelligent Mines created in the last galactic war.
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

#include "./imagewidget.hpp"

namespace Framework
{
  ImageWidget::ImageWidget(const Framework::ResPtr<Framework::Image>& image)
  {
    set(image);
    _resize = false;
    set_name("image-widget");
  }

  ImageWidget::ImageWidget()
  {
    _resize = false;
  }

  ImageWidget::~ImageWidget()throw()
  {
  }

  void ImageWidget::on_size_request(int& w, int& h)
  {
    if(!_image)
    {
      w=h=0;
      return;
    }

    w = MAX(0, _image->get_width());
    h = MAX(0, _image->get_height());
  }

  void ImageWidget::set_resize(bool resize)
  {
    _resize = resize;

    invalidate();
  }

  void ImageWidget::set(const Framework::ResPtr<Framework::Image>& image)
  {
    _image  = image;

    recalc_size_request();
    invalidate();
  }

  void ImageWidget::clear()
  {
    _image.reset();

    recalc_size_request();
    invalidate();
  }

  void ImageWidget::on_expose(EventExpose& paint_tool)
  {
    if(!_image || 0>=_image->get_width() || 0>=_image->get_height())
      return;

    Misc::on_expose(paint_tool);

    if(!_resize)
    {
      paint_tool.draw_image(
        _image,
        (get_width() -_image->get_width() )*get_xalign(),
        (get_height()-_image->get_height())*get_yalign()
      );
    }else
    {
      paint_tool.draw_resized_image(_image, 0, 0, get_width(), get_height(), false, get_xalign(), get_yalign());
    }
  }
}