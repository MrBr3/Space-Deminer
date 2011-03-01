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

const gsize ENSURED_N_TEXTURE_STAGES = 16; // must be less than 32

class Texture : public Refable
{
private:
  bool _initialized;
  GLuint _texture;

  Texture();
  Texture(const Texture&);
  Texture(const Glib::RefPtr<ImageFile>& file);

  Glib::RefPtr<ImageFile> imagefile;

public:
  ~Texture()throw();

  enum WrapMode
  {
    REPEAT,
    CLAMP,
  };

  void set_wrapping(WrapMode wm_u, WrapMode wm_v, int texture_stage=-1);

  void bind(guint i, bool do_bind=true);
  static void unbind(guint i);

  void deinit();
  void init();

  static Glib::RefPtr<Texture> create(const Glib::RefPtr<ImageFile>& imagefile){return Glib::RefPtr<Texture>(new Texture(imagefile));}
};
