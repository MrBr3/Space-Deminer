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

class UVMeshExporter : public Refable
{
  static UVMeshExporter* _singleton;

  bool _draw_warped;
  Cairo::RefPtr<Cairo::Context> _cc;
public:
  static void add_triangle(const Vector2& a, const Vector2& b, const Vector2& c, bool is_warped);
  static void add_quad(const Vector2& a, const Vector2& b, const Vector2& c, const Vector2& d, bool is_warped);

  static void export_uv_mesh(bool warped);

  static bool initialized(){return _singleton;}

public:
  UVMeshExporter(const Glib::ustring& filename, bool draw_warped, int n_segments);
  ~UVMeshExporter()throw();
};