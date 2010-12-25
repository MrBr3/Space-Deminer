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

#include "./engine.hpp"
#include "./window.hpp"

namespace Framework
{
  PaintTool::PaintTool(const Gdk::Region& invalid_region, const WindowManager& wm) : _invalid_region(invalid_region), _theme(wm.get_theme())
  {
    g_assert(_theme);
    g_assert(get_theme());

    _total_offset.dx  = 0.f;
    _total_offset.dy  = 0.f;
    _invalid_region.get_clipbox(_invalid_area);
  }

  PaintTool::~PaintTool()throw()
  {
  }

  void PaintTool::push_offset(int dx, int dy)
  {
    _offset_stack.push(Offset(dx, dy));

    _total_offset.dx  += dx;
    _total_offset.dy  += dy;

    on_offset_changed(dx, dy);
  }

  void PaintTool::pop_offset()
  {
    int dx  = _offset_stack.top().dx;
    int dy  = _offset_stack.top().dy;

    _total_offset.dx  -= dx;
    _total_offset.dy  -= dy;

    _offset_stack.pop();

#ifdef NDEBUG
    if(_offset_stack.size()==0)
    {
      g_assert(_total_offset.dy==_total_offset.dx && _total_offset.dx==0);
    }
#endif

    on_offset_changed(-dx, -dy);
  }

  void PaintTool::push_scissor(const Gdk::Rectangle& rect)
  {
    if(rect.get_width()<0)
      throw std::invalid_argument("rect.get_width()<0");
    if(rect.get_height()<0)
      throw std::invalid_argument("rect.get_height()<0");

    ScissorRect scissor_rect(rect.get_x()+_total_offset.dx, rect.get_y()+_total_offset.dy, rect.get_x()+_total_offset.dx+rect.get_width(), rect.get_y()+_total_offset.dy+rect.get_height());

    /*if(scissor_rect.left>scissor_rect.right)
      std::cout<<"";
    if(scissor_rect.top>scissor_rect.bottom)
      std::cout<<"";*/

    g_assert(scissor_rect.left<=scissor_rect.right);
    g_assert(scissor_rect.top <=scissor_rect.bottom);

    if(_scissor_stack.size())
    {
      scissor_rect.left   = CLAMP(scissor_rect.left,   _scissor_stack.top().left, _scissor_stack.top().right);
      scissor_rect.top    = CLAMP(scissor_rect.top,    _scissor_stack.top().top,  _scissor_stack.top().bottom);
      scissor_rect.right  = CLAMP(scissor_rect.right,  _scissor_stack.top().left, _scissor_stack.top().right);
      scissor_rect.bottom = CLAMP(scissor_rect.bottom, _scissor_stack.top().top,  _scissor_stack.top().bottom);
    }

    _scissor_stack.push(scissor_rect);

    on_scissor_changed();
  }

  void PaintTool::pop_scissor()
  {
    _scissor_stack.pop();

    on_scissor_changed();
  }

  void PaintTool::draw_resized_image(const ResPtr<Image>& img, Real x, Real y, Real width, Real height, bool clip, Real xalign, Real yalign)
  {
    if(width<0.f)
    {
      x += width;
      width = -width;
    }
    if(width==0.f)
      return;
    if(height<0.f)
    {
      y += height;
      height = -height;
    }
    if(height==0.f)
      return;

    Real img_height = img->get_height();
    Real img_width = img->get_width();

    if(img_width==0.f || img_height==0.f)
      return;

    g_assert(img_height>0.f);
    g_assert(img_width>0.f);

    Real dest_aspect_ration = height/width;
    Real img_aspect_ration = img_height/img_width;

    g_assert(dest_aspect_ration>0.f);
    g_assert(img_aspect_ration>0.f);

    struct Rect
    {
      Real x, y, w, h;
    }img_part, img_dest;

    if(clip)
    {
      img_dest.x  = x;
      img_dest.y  = y;
      img_dest.w  = width;
      img_dest.h  = height;

      if(dest_aspect_ration > img_aspect_ration)  // clip left and right part of the image
      {
        img_part.h  = img_height;
        img_part.y  = 0.f;
        img_part.w  = img_height/dest_aspect_ration;
        img_part.x  = (img_width-img_part.w)*xalign;

        g_assert(img_part.w<=img_width);
      }else // clip top and bottom part of the image
      {
        img_part.w  = img_width;
        img_part.x  = 0.f;
        img_part.h  = img_width*dest_aspect_ration;
        img_part.y  = (img_height-img_part.h)*yalign;

        g_assert(img_part.h<=img_height);
      }
    }else
    {
      img_part.x  = 0.f;
      img_part.y  = 0.f;
      img_part.w  = img_width;
      img_part.h  = img_height;

      if(dest_aspect_ration > img_aspect_ration)  // add space above and below the image
      {
        img_dest.w  = width;
        img_dest.x  = 0.f;
        img_dest.h  = width*img_aspect_ration;
        img_dest.y  = (height-img_dest.h)*yalign;
      }else // add space left and right the image
      {
        img_dest.h  = height;
        img_dest.y  = 0.f;
        img_dest.w  = height/img_aspect_ration;
        img_dest.x  = (width-img_dest.w)*xalign;
      }
    }

    draw_image(img, img_dest.x, img_dest.y, img_dest.w, img_dest.h, img_part.x, img_part.y, img_part.w, img_part.h);
  }

  void PaintTool::draw_rect_border(Real x, Real y, Real width, Real height, Real r, Real g, Real b, Real a, int line_width)
  {
    draw_color_rect(x, y, width, line_width, r, g, b, a);
    draw_color_rect(x, y+height-line_width, width, line_width, r, g, b, a);
    draw_color_rect(x, y+line_width, line_width, height-line_width*2, r, g, b, a);
    draw_color_rect(x+width-line_width, y+line_width, line_width, height-line_width*2, r, g, b, a);
  }

  void PaintTool::draw_widget_back(const Glib::ustring& what, guint32 param, const Widget& w, int offset_x, int offset_y)
  {
    g_assert(get_theme());
    get_theme()->draw(*this, what, DRAWPASS_BACK, param, Gdk::Rectangle(offset_x, offset_y, w.get_width(), w.get_height()));
  }
}