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

class ImageFile : public Refable
{
  ImageFile();
  ImageFile(const ImageFile&);

  Glib::ustring _filename;
  bool _needs_to_be_warped;
  // resize_to_preview_size

  sigc::signal<void> _signal_imagefile_changed;
  sigc::signal<void> _signal_needs_to_be_warped_chenged;

  sigc::signal<void> _signal_something_changed;

  COLORCURVE_SETTING(color_curve);
  CURVE_SETTING(contrast_curve);
public:
  enum CorrectionCurveType
  {
    CONTRAST_CURVE,
    COLOR_CURVE,
    NO_CURVE,
  };

  const CorrectionCurveType correction_curve_type;

  sigc::signal<void>& signal_needs_to_be_warped_changed(){return _signal_needs_to_be_warped_chenged;}
  sigc::signal<void>& signal_imagefile_changed(){return _signal_imagefile_changed;}
  sigc::signal<void>& signal_something_changed(){return _signal_something_changed;}

  ~ImageFile()throw();

  const Glib::ustring get_filename()const{return _filename;}
  void set_filename(const Glib::ustring& filename);

  bool get_needs_to_be_warped()const{return _needs_to_be_warped;}
  void set_needs_to_be_warped(bool b);

  /** \brief Creates a RefPtr ift there's a valif Imagefile other wise a null pointer.
   *
   * \return A RefPtr to a Pixbuf (can be nullptr)
   * */
  Glib::RefPtr<Gdk::Pixbuf> create_pixbuf();

  static Glib::RefPtr<ImageFile> create(CorrectionCurveType correction_curve_type)
  {
    return Glib::RefPtr<ImageFile>(new ImageFile(correction_curve_type));
  }

private:
  ImageFile(CorrectionCurveType correction_curve_type);
};
