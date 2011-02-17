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

#include "./segmentedimage.hpp"
#include "./engine.hpp"

namespace Framework
{
  SegmentedImage9::SegmentedImage9(const Glib::ustring& filename, int cut_x, int cut_y, int cut_w, int cut_h, int frame_e, int frame_n, int frame_w, int frame_s)
  {
    this->frame_e = frame_e;
    this->frame_n = frame_n;
    this->frame_w = frame_w;
    this->frame_s = frame_s;

    slot_reload_resource  = sigc::bind(sigc::mem_fun(*this, &SegmentedImage9::reload_img_file), filename, cut_x, cut_y, cut_w, cut_h);

    reload_img_file(filename, cut_x, cut_y, cut_w, cut_h);
  }

  SegmentedImage9::~SegmentedImage9()throw()
  {
    if(!get_is_loaded())
      return;

    image.reset();
    set_is_loaded(false);
  }

  void SegmentedImage9::reload_img_file(const Glib::ustring& filename, int cut_x, int cut_y, int cut_w, int cut_h)
  {
    Glib::RefPtr<Gdk::Pixbuf> pixbuf  = Gdk::Pixbuf::create_from_file(filename);

    g_assert(pixbuf);

    if(cut_w > pixbuf->get_width())
      throw std::invalid_argument("cut_w should be <= of the image's width");
    if(cut_h > pixbuf->get_height())
      throw std::invalid_argument("cut_h should be <= of the image's height");
    if(cut_x >= pixbuf->get_width() || cut_x<0)
      throw std::invalid_argument("cut_x should be within the image");
    if(cut_y >= pixbuf->get_height() || cut_y<0)
      throw std::invalid_argument("cut_y should be within the image");

    if(cut_w<0)
      cut_w = pixbuf->get_width()-cut_x;
    if(cut_h<0)
      cut_h = pixbuf->get_height()-cut_y;

    if((cut_w+cut_x > pixbuf->get_width())||(cut_h+cut_y > pixbuf->get_height()))
      throw std::invalid_argument("the clipped image is outside the source image");

    if(pixbuf->get_width()!=cut_w || cut_x!=0 || pixbuf->get_height()!=cut_h || cut_y!=0)
      pixbuf  = Gdk::Pixbuf::create_subpixbuf(pixbuf, cut_x, cut_y, cut_w, cut_h);

    g_assert(pixbuf);

    image = Image::create(pixbuf, TEXTURE_HINT_SIZEABLE);
    set_is_loaded(true);
  }

  ResPtr<SegmentedImage9> SegmentedImage9::create_from_file(const Glib::ustring& filename, int cut_x, int cut_y, int cut_w, int cut_h, int frame_e, int frame_n, int frame_w, int frame_s)
  {
    return ResPtr<SegmentedImage9>(new SegmentedImage9(filename, cut_x, cut_y, cut_w, cut_h, frame_e, frame_n, frame_w, frame_s));
  }

  void SegmentedImage9::draw(PaintTool& ee, int x, int y, int w, int h)
  {
    g_assert(image);

    int iw = image->get_width(), ih = image->get_height();

    ee.draw_image(image, x+frame_w, y+frame_n, w-frame_w-frame_e, h-frame_n-frame_s , frame_w, frame_n, iw-frame_w-frame_e, ih-frame_n-frame_s); // center
    ee.draw_image(image, x+frame_w, y, w-frame_w-frame_e, frame_n                   , frame_w, 0, iw-frame_w-frame_e, frame_n); // N
    ee.draw_image(image, x+frame_w, y+h-frame_s, w-frame_w-frame_e, frame_s         , frame_w, ih-frame_s, iw-frame_w-frame_s, frame_s); //S
    ee.draw_image(image, x, y, frame_w, frame_n                                     , 0, 0, frame_w, frame_n); // NW
    ee.draw_image(image, x+w-frame_e, y, frame_e, frame_n                           , iw-frame_e, 0, frame_e, frame_n); // NE
    ee.draw_image(image, x, y+h-frame_s, frame_w, frame_s                           , 0, ih-frame_s, frame_w, frame_s); // SW
    ee.draw_image(image, x+w-frame_e, y+h-frame_s, frame_e, frame_s                 , iw-frame_e, ih-frame_s, frame_e, frame_s); // SE
    ee.draw_image(image, x, y+frame_n, frame_w, h-frame_s-frame_n                   , 0, frame_n, frame_w, ih-frame_s-frame_n); // W
    ee.draw_image(image, x+w-frame_e, y+frame_n, frame_e, h-frame_s-frame_n         , iw-frame_e, frame_n, frame_e, ih-frame_s-frame_n); // E
  }

  void SegmentedImage9::v_purge_resource()
  {
    image.reset();
    set_is_loaded(false);
  }

  void SegmentedImage9::v_reload_resource()
  {
    slot_reload_resource();
    set_is_loaded(true);
  }
}
