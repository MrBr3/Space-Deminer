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

#ifndef _SPACEDEMINER_UI_FRAMEWORK_ENGINE_H_
#define _SPACEDEMINER_UI_FRAMEWORK_ENGINE_H_

#include "./theme.hpp"

namespace Framework
{
  class Widget;
  class WindowManager;
  class PaintTool : public Refable
  {
  public:
    class Offset
    {
    public:
      int dx, dy;
      Offset(){}
      Offset(int dx_, int dy_) : dx(dx_), dy(dy_){}
    };
    class ScissorRect
    {
    public:
      int left, top, right, bottom;
      ScissorRect(){}
      ScissorRect(int left_, int top_, int right_, int bottom_) : left(left_), top(top_), right(right_), bottom(bottom_){}
    };
  public:
    /** \brief Draws a piece of an image.
     *
     * The drawn image will exactly fill the rectangular area defined with x, y, width and height.
     *
     * Only the area of the image defined by sub_img_x, sub_img_y, sub_img_width and sub_img_height will be visibly (0nly exception: titing)
     *
     * If <tt>sub_img_x/y<0 || sub_img_x/y+sub_img_width/height> width/height</tt> the (whole!) image will be simpget_bits_per_samplely tiled.
     *
     * \param img the image to draw
     * \param x the horizontal position, where to draw
     * \param y the vertical   position, where to draw
     * \param width  the horizontal size, of the drawn image
     * \param height the vertical   size, of the drawn image
     * \param sub_img_x the horizontal position  of the rectangluar region within the image to draw in pixels
     * \param sub_img_y the vertical   position  of the rectangluar region within the image to draw in pixels
     * \param sub_img_width  the horizontal size of the rectangluar region within the image to draw in pixels
     * \param sub_img_height the verticalsize    of the rectangluar region within the image to draw in pixels
     **/
    virtual void draw_image(const ResPtr<Image>& img, Real x, Real y, Real width, Real height, Real sub_img_x, Real sub_img_y, Real sub_img_width, Real sub_img_height)=0;

    /** \brief Draws an image.
     *
     * The whole drawn image will exactly fill the rectangular area defined with x, y, width and height.
     *
     * Logically identical to
     * \code
     *   draw_image(img, x, y, width, height, 0.f, 0.f, width, height);
     * \endcode
     *
     * \param img the image to draw
     * \param x the horizontal position, where to draw
     * \param y the vertical   position, where to draw
     * \param width  the horizontal size, of the drawn image
     * \param height the vertical   size, of the drawn image
     **/
    virtual void draw_image(const ResPtr<Image>& img, Real x, Real y, Real width, Real height)=0;

    /** \brief Draws an image.
     *
     * The whole drawn image will exactly fill the rectangular area defined with x, y, width and height.
     *
     * Logically identical to
     * \code
     *   draw_image(img, x, y, img->get_width(), img->get_height(), 0.f, 0.f, width, height);
     * \endcode
     *
     * \param img the image to draw
     * \param x the horizontal position, where to draw
     * \param y the vertical   position, where to draw
     **/
    virtual void draw_image(const ResPtr<Image>& img, Real x, Real y)
    {
      draw_image(img, x, y, img->get_width(), img->get_height());
    }

    /** \brief A helper function to draw an image tiled.
     *
     * Calls internal
     * \code
     * draw_image(img, x, y, width, height, sub_img_x, sub_img_y, width, height);
     * \endcode
     * */
    void draw_image_tiled(const ResPtr<Image>& img, Real x, Real y, Real width, Real height, Real sub_img_x=0.f, Real sub_img_y=0.f)
    {
      draw_image(img, x, y, width, height, sub_img_x, sub_img_y, width, height);
    }

    virtual void draw_color_rect(Real x, Real y, Real width, Real height, Real r, Real g, Real b, Real a=1.f)=0;
    virtual void draw_rect_border(Real x, Real y, Real width, Real height, Real r, Real g, Real b, Real a=1.f, int line_width=1);
    void draw_rect_border(const Gdk::Rectangle& rect, Real r, Real g, Real b, Real a=1.f, int line_width=1){draw_rect_border(rect.get_x(), rect.get_y(), rect.get_width(), rect.get_height(), r, g, b, a, line_width);}

    /** \brief Draws an image resized, in order to draw the image as enlarged as possible with clipping nothing/es less as possible.
     *
     * The drawn image won't exceed the rectangular area defined with x, y, width and height. If \c clip is true, the drawn image will exactly
     * fill the area otherwise it will be smaller.
     *
     * This function wraps around <tt>PaintTool::draw_image</tt>.
     *
     * \param img the image to draw
     * \param x the horizontal position, where to draw
     * \param y the vertical   position, where to draw
     * \param width  the horizontal size, where the image will be visible
     * \param height the vertical   size, where the image will be visible
     * \param clip if true, the priority is to fill the whole area. Otherwise the priority is to show the whole image.
     * */
    void draw_resized_image(const ResPtr<Image>& img, Real x, Real y, Real width, Real height, bool clip, Real xalign=.5f, Real yalign=.5f);


  public:
    const Glib::RefPtr<const Theme>& get_theme()throw(){return _theme;}

    const Gdk::Region& get_invalid_region()const throw(){return _invalid_region;}

    const Gdk::Rectangle& get_invalid_area()const throw(){return _invalid_area;}

    int get_scissor_stack_depth()const throw(){return _scissor_stack.size();}
    void push_scissor(int x, int y, int w, int h){push_scissor(Gdk::Rectangle(x, y, w, h));}
    void push_scissor(const Gdk::Rectangle& rect);
    void pop_scissor();
    const ScissorRect& get_top_scissor_rect()const throw()
    {
      if(!_scissor_stack.size())
        throw std::runtime_error("get_top_scissor_rect has been called, when there was no scissor on the stack!");
      return _scissor_stack.top();
    }
    virtual void on_scissor_changed(){}

    int get_offset_stack_depth()const throw(){return _offset_stack.size();}
    void push_offset(int dx, int dy);
    void pop_offset();
    const Offset& get_total_offset()const throw(){return _total_offset;}
    const ScissorRect& get_top_offset(int& top_offset_x,  int& top_offset_y)const throw()
    {
      if(!_offset_stack.size())
        throw std::runtime_error("get_top_offset has been called, when there was no offset on the stack!");
      top_offset_x  = _offset_stack.top().dx;
      top_offset_y  = _offset_stack.top().dy;
    }
    virtual void on_offset_changed(int dx, int dy){}

    bool is_in_region(Gdk::Rectangle rect)
    {
      rect.set_x(rect.get_x()+_total_offset.dx);
      rect.set_y(rect.get_y()+_total_offset.dy);

      return Gdk::OVERLAP_RECTANGLE_OUT!=_invalid_region.rect_in(rect);
    }

    void draw(const Glib::ustring& what, DrawPass pass, guint32 param, const Gdk::Rectangle& where)
    {
      get_theme()->draw(*this, what, pass, param, where);
    }
    void draw_widget_back(const Glib::ustring& what, guint32 param, const Widget& w, int offset_x=0, int offset_y=0);

    ~PaintTool()throw();

  protected:
    PaintTool(const Gdk::Region& invalid_region, const WindowManager& wm);

    Gdk::Region& get_invalid_region()throw(){return _invalid_region;}

  private:
    const Glib::RefPtr<const Theme> _theme;

    Gdk::Region _invalid_region;
    Gdk::Rectangle _invalid_area;

    Offset _total_offset;
    std::stack<Offset> _offset_stack;
    std::stack<ScissorRect> _scissor_stack;

    int _curr_depth;
  };

  class Engine : public StaticManager<Engine>
  {
  public:
    /** \brief Creates an image based
     * */
    ResPtr<Image> create_image(const Glib::RefPtr<Gdk::Pixbuf>& pixbuf, Image::Hint hint=Image::HINT_DEFAULT, const Image::SlotRecreateFromPixbuf& slot_recreate=sigc::ptr_fun(&Image::cant_reload_from_pixbuf))
    {
      ResPtr<Image> img = v_create_image(pixbuf, hint, slot_recreate);
      if(!img->get_is_loaded())
        throw std::runtime_error("the image should be loaded now");
      //++_n_resources;
      return  img;
    }

    ResPtr<Font> create_font(){return v_create_font();}

    /** \brief
     *
     * \param filename the full filepath to the image file to load.
     *
     * \note This function calls Glib::filename_from_utf8 and apply_filename_macros internal - so you don't have to call them by yourself.
     * */
    ResPtr<Image> create_image_from_file(const Glib::ustring& filename, Image::Hint hint=Image::HINT_DEFAULT)
    {
      ResPtr<Image> img = v_create_image_from_file(Glib::filename_from_utf8(apply_filename_macros(filename)), hint);
      if(!img->get_is_loaded())
        throw std::runtime_error("the image should be loaded now");
      //++_n_resources;
      return  img;
    }

  protected:
    virtual ResPtr<Font> v_create_font(){return ResPtr<Font>(new ImageFont());}

    static Glib::RefPtr<Gdk::Pixbuf> reload_pixbuf_from_file(const Glib::ustring& filename)
    {
      return Gdk::Pixbuf::create_from_file(filename);
    }

    virtual ResPtr<Image> v_create_image_from_file(const Glib::ustring& filename, Image::Hint hint)
    {
      //--_n_resources;
      return create_image(reload_pixbuf_from_file(filename), hint, sigc::bind(sigc::ptr_fun(reload_pixbuf_from_file), filename));
    }
    virtual ResPtr<Image> v_create_image(const Glib::RefPtr<Gdk::Pixbuf>& pixbuf, Image::Hint hint, const Image::SlotRecreateFromPixbuf& slot_recreate)=0;


  //----PaintTool----
  public:
    virtual Glib::RefPtr<PaintTool> create_paint_tool(const Gdk::Region& invalid_region, const WindowManager& wm)=0;

  //----EngineData----
  public:
    class EngineData : public Refable
    {
    public:

      ~EngineData()throw()
      {
      }

    private:
    };

  //----Ctors----
  public:
    static Glib::RefPtr<Engine> create_gl_engine(const Glib::RefPtr<Gdk::GL::Context>& context);

  protected:
    Engine();
    ~Engine()throw();

  private:
    Engine(const Engine&);
  };

  inline ResPtr<Image> Image::create(const Glib::RefPtr<Gdk::Pixbuf>& pixbuf, Image::Hint hint, const SlotRecreateFromPixbuf& slot_recreate){return Engine::get_singleton()->create_image(pixbuf, hint, slot_recreate);}
  inline ResPtr<Image> Image::create_from_file(const Glib::ustring& filename, Image::Hint hint){return Engine::get_singleton()->create_image_from_file(filename, hint);}
}

#endif