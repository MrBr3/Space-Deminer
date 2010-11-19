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

#ifndef _SPACEDEMINER_UI_FRAMEWORK_SEGMENTEDIMAGE_H_
#define _SPACEDEMINER_UI_FRAMEWORK_SEGMENTEDIMAGE_H_

#include <gdkmm/rectangle.h>
#include "./image.hpp"
#include "./segmentedimage.hpp"

namespace Framework
{
  class PaintTool;
  class SegmentedImage9 : public Resource
  {
  public:
    static ResPtr<SegmentedImage9> create_from_file(const Glib::ustring& filename, int cut_x, int cut_y, int cut_w, int cut_h, int frame_e, int frame_n, int frame_w, int frame_s);
    static ResPtr<SegmentedImage9> create_from_file(const Glib::ustring& filename, int frame_e, int frame_n, int frame_w, int frame_s)
    {
      return create_from_file(filename, 0, 0, -1, -1, frame_e, frame_n, frame_w, frame_e);
    }

    void draw(PaintTool& ee, int x, int y, int w, int h);
    void draw(PaintTool& ee, const Gdk::Rectangle& rect)
    {
      draw(ee, rect.get_x(), rect.get_y(), rect.get_width(), rect.get_height());
    }

  private:
    ResPtr<Image> image;
    int frame_e, frame_n, frame_w, frame_s;

    sigc::slot<void> slot_reload_resource;

    void reload_img_file(const Glib::ustring& filename, int cut_x, int cut_y, int cut_w, int cut_h);

    void v_purge_resource();
    void v_reload_resource();

    SegmentedImage9(const Glib::ustring& filename, int cut_x, int cut_y, int cut_w, int cut_h, int frame_e, int frame_n, int frame_w, int frame_s);
    ~SegmentedImage9()throw();
  };
}
#endif