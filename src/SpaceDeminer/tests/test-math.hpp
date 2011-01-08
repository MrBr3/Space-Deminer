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

void test_math()
{
  std::cout<<"==== Testing Math stuff ====\n";
  std::cout<<"---- Testing simple Mathstuff ----\n";
  {
    check_expect(blog_int(0), G_MAXUINT);
    check_expect<guint>(blog_int(1), 0);
    check_expect<guint>(blog_int(2), 1);
    check_expect<guint>(blog_int(3), 1);
    check_expect<guint>(blog_int(4), 2);
    check_expect<guint>(blog_int(5), 2);
    check_expect<guint>(blog_int(6), 2);
    check_expect<guint>(blog_int(7), 2);
    check_expect<guint>(blog_int(8), 3);
    check_expect<guint>(blog_int(9), 3);
  }
  {
    gfloat x_1, x_2;

    check_expect<bool>(solve_quadric_formula(x_1, x_2, 1.f, 1.f, 1.f), false);
    check_expect<bool>(solve_quadric_formula(x_1, x_2, 5.f, 3.f, 2.f), false);
    check_expect<bool>(solve_quadric_formula(x_1, x_2, 1.f,-7.f, 3.f), true);
    check_within(x_1, 6.5413812651491f);
    check_within(x_2, 0.4586187348508f);
    check_expect<bool>(solve_quadric_formula(x_1, x_2, 1.f,-4.f, 4.f), true);
    check_within(x_1, 2.f);
    check_within(x_2, 2.f);
  }
  std::cout<<"---- Testing Vector ----\n";
  {
    check_within(Vector2( 1.0f, 0.0f).rotate_90_cw() , Vector2( 0.f, -1.f));
    check_within(Vector2( 1.0f, 0.0f).rotate_90_ccw(), Vector2( 0.f,  1.f));
    check_within(Vector2( 0.5f,-1.0f)*Vector2( 0.5f,-1.0f).rotate_90_ccw(), 0.f);
    check_within(Vector2( 0.5f,-1.0f)*Vector2( 0.5f,-1.0f).rotate_90_cw(),  0.f);
    check_within(Vector2( 0.5f,-1.0f).rotate_90_cw(),  Vector2(-1.f, -0.5));
    check_within<Vector3>(cross(Vector2(0.5f,-1.0f), Vector3(0.f, 0.f, 1.f)), Vector2(-1.f, -0.5f));
    check_within<Vector3>(cross(Vector2(0.5f,-1.0f), Vector3(0.f, 0.f,-1.f)), Vector2( 1.f,  0.5f));
    check_within(Vector3(0.5f,-1.0f, 0.f) * Vector3(0.f, 0.f,-1.f), 0.f);
  }
}