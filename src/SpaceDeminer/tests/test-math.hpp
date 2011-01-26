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

    check_expect<guint>(abs(-0.5f), 0.5f);
    check_expect<guint>(abs(0.5f), 0.5f);
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
  {
    check_within(get_angle_from_dir(cos(120.f*degree), sin(120.f*degree))*360.f, 120.f);
    check_within(get_angle_from_dir(cos(5.f*degree), sin(5.f*degree))*360.f, 5.f);
    check_within(get_angle_from_dir(cos(0.f*degree), sin(0.f*degree))*360.f, 0.f);
    check_within(get_angle_from_dir(cos(180.f*degree), sin(180.f*degree))*360.f, 180.f);
    check_within(get_angle_from_dir(cos(190.f*degree), sin(190.f*degree))*360.f, 190.f);
    check_within(get_angle_from_dir(cos(270.f*degree), sin(270.f*degree))*360.f, 270.f);
  }
  std::cout<<"---- Testing Vector ----\n";
  {
    check_within(Vector2( 1.0f, 0.0f).rotate_90_cw() , Vector2( 0.f, -1.f));
    check_within(Vector2( 1.0f, 0.0f).rotate_90_ccw(), Vector2( 0.f,  1.f));
    check_within(Vector2( 0.5f,-1.0f)*Vector2( 0.5f,-1.0f).rotate_90_ccw(), 0.f);
    check_within(Vector2( 0.5f,-1.0f)*Vector2( 0.5f,-1.0f).rotate_90_cw(),  0.f);
    check_within(Vector2( 0.5f,-1.0f).rotate_90_cw(),  Vector2(-1.f, -0.5));
    check_within<Vector3>(cross(Vector2(0.5f,-1.0f), Vector3(0.f, 0.f, 1.f)), Vector3(-1.f, -0.5f, 0.f));
    check_within<Vector3>(cross(Vector2(0.5f,-1.0f), Vector3(0.f, 0.f,-1.f)), Vector3( 1.f,  0.5f, 0.f));
    check_within(Vector3(0.5f,-1.0f, 0.f) * Vector3(0.f, 0.f,-1.f), 0.f);
    check_within(Vector2(INV_SQRT_2, INV_SQRT_2).get_length(), 1.f);
  }
  std::cout<<"---- Testing Planes ----\n";
  {
    gfloat t;
    Plane plane1(Vector3(-1.f, 0.f, 1.f), Vector3(0.f, -1.f, 0.f), Vector3(5.f, 0.f, 0.f));

    check_within(plane1, Plane(Vector3(INV_SQRT_2, 0.f, INV_SQRT_2), Vector3(5.f, 0.f, 0.f)));
    check_expect(plane1.check_point(Vector3(5.0f, 0.f, 0.f)), Math::INSIDE);
    check_expect(plane1.check_point(Vector3(5.0f, 256.f, 0.f)), Math::INSIDE);
    check_expect(plane1.check_point(Vector3(5.0f,-256.f, 0.f)), Math::INSIDE);
    check_expect(plane1.check_point(Vector3(5.1f,-256.f, 0.f)), Math::FORESIDE);
    check_expect(plane1.check_point(Vector3(4.9f,-256.f, 0.f)), Math::BACKSIDE);
    check_expect<gfloat>(plane1.get_distance(Vector3(0.f, 0.f, 0.f)), fabs(plane1.d));
    check_within(plane1.get_distance(Vector3(5.f, 0.f, 0.f)), 0.f);
    check_expect(plane1.check_ray(Math::Ray(Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)), t), false);
    check_expect(plane1.check_ray(Math::Ray(Vector3(0.f, 0.f, 0.f), Vector3(INV_SQRT_2, 0.f,-INV_SQRT_2)), t), false);
    check_expect(plane1.check_ray(Math::Ray(Vector3(0.f, 0.f, 0.f), Vector3(-INV_SQRT_2, 0.f, INV_SQRT_2)), t), false);
    check_expect(plane1.check_ray(Math::Ray(Vector3(0.f, 0.f, 0.f), Vector3(INV_SQRT_2, 0.f, INV_SQRT_2)), t), true);
    check_within<gfloat>(t, fabs(plane1.d));
    check_expect(plane1.check_ray(Math::Ray(Vector3(-INV_SQRT_2, 20.f, -INV_SQRT_2), Vector3(INV_SQRT_2, 0.f, INV_SQRT_2)), t), true);
    check_within<gfloat>(t, 1.f+fabs(plane1.d));
    check_expect(plane1.check_ray(Math::Ray(Vector3(5.f, 0.f, 0.f), Vector3(INV_SQRT_2, 0.f, INV_SQRT_2)), t), true);
    check_within(t, 0.f);
    check_expect(plane1.check_ray(Math::Ray(Vector3(5.f, 0.f, 0.f), Vector3(-INV_SQRT_2, 0.f, INV_SQRT_2)), t), true);
    check_within(t, 0.f);

    Math::Sphere sphere(Matrix44::identity, fabs(plane1.d));
    check_expect(plane1.check_sphere(sphere), Math::OVERLAPPING);
    sphere.transformation.set_translate(-1.f, 5.f, 0.f);
    check_expect(plane1.check_sphere(sphere), Math::BACKSIDE);
    sphere.transformation.set_translate(10.f, 5.f, 10.f);
    check_expect(plane1.check_sphere(sphere), Math::FORESIDE);
    plane1.normal*=-1.f;
    plane1.d*=-1.f;
    check_expect(plane1.check_sphere(sphere), Math::BACKSIDE);
    sphere.transformation.set_translate(-1.f, 5.f, 0.f);
    check_expect(plane1.check_sphere(sphere), Math::FORESIDE);
  }
  std::cout<<"---- Testing Line cuts ----\n";
  {
    check_expect(between(-1.0f, 0.f, 1.f), false);
    check_expect(between( 0.0f, 0.f, 1.f), true);
    check_expect(between( 1.0f, 0.f, 1.f), true);
    check_expect(between( 0.5f, 0.f, 1.f), true);
    check_expect(between( 1.5f, 0.f, 1.f), false);
    check_expect(between(-1.0f, 1.f, 0.f), false);
    check_expect(between( 0.0f, 1.f, 0.f), true);
    check_expect(between( 1.0f, 1.f, 0.f), true);
    check_expect(between( 0.5f, 1.f, 0.f), true);
    check_expect(between( 1.5f, 1.f, 0.f), false);

    check_expect(intersection_1d( 0.0f, 1.0f, 2.0f, 3.0f), false);
    check_expect(intersection_1d( 0.0f, 1.0f, 1.0f, 2.0f), true);
    check_expect(intersection_1d( 0.0f, 1.0f, 0.5f, 2.0f), true);
    check_expect(intersection_1d( 0.0f, 1.0f, 0.0f, 1.0f), true);
    check_expect(intersection_1d( 0.0f, 1.0f, 0.0f, 1.0f), true);
    check_expect(intersection_1d( 0.0f, 1.0f,-1.0f, 1.0f), true);
    check_expect(intersection_1d( 0.0f, 1.0f,-1.0f, 0.5f), true);
    check_expect(intersection_1d( 0.0f, 1.0f,-1.0f,-1.0f), false);
    check_expect(intersection_1d( 0.0f, 1.0f,-2.0f,-1.0f), false);
    check_expect(intersection_1d( 0.0f, 1.0f,-2.0f,-1.0f), false);
    check_expect(intersection_1d( 0.0f, 1.0f,-2.0f,-0.0000001f), false);
    check_expect(intersection_1d( 0.0f, 1.0f,-2.0f, 0.0000001f), true);
    check_expect(intersection_1d( 0.0f, 1.0f,-2.0f, 0.f), true);
    check_expect(intersection_1d(-2.0f, 2.0f, 1.0f,-1.0f), true);
    check_expect(intersection_1d(-1.0f, 1.0f, 2.0f,-2.0f), true);

#define testblock\
    check_expect(line_cuts_hline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(3.273f), y(2.211f)), xm(1.703f), false), false);\
    check_expect(line_cuts_hline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(1.570f), y(2.211f)), xp(1.703f), false), false);\
    check_expect(line_cuts_hline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(3.390f), y(2.211f)), xm(1.820f), false), true);\
    check_expect(line_cuts_hline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(1.570f), y(2.211f)), xp(1.820f), false), true);\
    check_expect(line_cuts_hline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(4.420f), y(1.387f)), xp(0.995f), false), false);\
    check_expect(line_cuts_hline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(5.415f), y(1.387f)), xm(0.995f), false), false);\
    check_expect(line_cuts_hline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(4.420f), y(1.435f)), xp(0.995f), false), true);\
    check_expect(line_cuts_hline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(5.415f), y(1.435f)), xm(0.995f), false), true);\
    check_expect(line_cuts_hline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(0.000f), y(3.716f)), xp(0.995f), false), false);\
    check_expect(line_cuts_hline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(0.995f), y(3.716f)), xm(0.995f), false), false);\
    check_expect(line_cuts_hline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(0.000f), y(3.660f)), xp(0.995f), false), true);\
    check_expect(line_cuts_hline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(0.995f), y(3.660f)), xm(0.995f), false), true);\
    check_expect(line_cuts_hline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(3.273f), y(2.211f)), xm(1.703f), true), false);\
    check_expect(line_cuts_hline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(1.570f), y(2.211f)), xp(1.703f), true), true);\
    check_expect(line_cuts_hline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(3.390f), y(2.211f)), xm(1.820f), true), true);\
    check_expect(line_cuts_hline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(1.570f), y(2.211f)), xp(1.820f), true), true);\
    check_expect(line_cuts_hline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(0.000f), y(3.716f)), xp(0.995f), true), false);\
    check_expect(line_cuts_hline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(0.000f), y(3.716f)), xm(0.995f), true), false);\
    check_expect(line_cuts_hline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(0.995f), y(3.716f)), xp(0.995f), true), false);\
    check_expect(line_cuts_hline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(0.995f), y(3.716f)), xm(0.995f), true), false);\
    check_expect(line_cuts_hline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(4.420f), y(1.387f)), xp(0.995f), true), false);\
    check_expect(line_cuts_hline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(4.420f), y(1.387f)), xm(0.995f), true), false);\
    check_expect(line_cuts_hline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(5.415f), y(1.387f)), xp(0.995f), true), false);\
    check_expect(line_cuts_hline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(5.415f), y(1.387f)), xm(0.995f), true), false);
#define xp(foo)  foo
#define xm(foo) -foo
#define x(foo)   foo
#define y(foo)   foo
    testblock
#undef x
#undef y
#undef xp
#undef xm
#define xp(foo) -foo
#define xm(foo)  foo
#define x(foo)  -foo
#define y(foo)   foo
    testblock
#undef x
#undef y
#undef xp
#undef xm
#define xp(foo)  foo
#define xm(foo) -foo
#define x(foo)   foo
#define y(foo)  -foo
    testblock
#undef x
#undef y
#undef xp
#undef xm
#define xp(foo) -foo
#define xm(foo)  foo
#define x(foo)  -foo
#define y(foo)  -foo
    testblock
#undef x
#undef y
#undef xp
#undef xm
#undef testblock
#define testblock\
    check_expect(line_cuts_vline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(3.539), y(2.139f)), yp(0.995f)), false);\
    check_expect(line_cuts_vline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(3.539), y(2.139f)), ym(0.995f)), true);\
    check_expect(line_cuts_vline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(3.539), y(3.294f)), ym(0.995f)), false);\
    check_expect(line_cuts_vline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(3.539), y(3.294f)), yp(0.995f)), false);\
    check_expect(line_cuts_vline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(4.911), y(1.166)), ym(0.995f)), false);\
    check_expect(line_cuts_vline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(4.911), y(1.166)), yp(0.995f)), false);\
    check_expect(line_cuts_vline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(4.911), y(2.161)), ym(0.995f)), false);\
    check_expect(line_cuts_vline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(4.911), y(2.161)), yp(0.995f)), false);\
    check_expect(line_cuts_vline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(0.483), y(3.294)), ym(0.995f)), false);\
    check_expect(line_cuts_vline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(0.483), y(3.294)), yp(0.995f)), false);\
    check_expect(line_cuts_vline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(0.483), y(4.289)), ym(0.995f)), false);\
    check_expect(line_cuts_vline(Vector2(x(0.496f), y(3.695f)), Vector2(x(4.892f), y(1.412f)), Vector2(x(0.483), y(4.289)), yp(0.995f)), false);
#define yp(foo)  foo
#define ym(foo) -foo
#define x(foo)   foo
#define y(foo)   foo
    testblock
#undef x
#undef y
#undef yp
#undef ym
#define yp(foo)  foo
#define ym(foo) -foo
#define x(foo)  -foo
#define y(foo)   foo
    testblock
#undef x
#undef y
#undef yp
#undef ym
#define yp(foo) -foo
#define ym(foo)  foo
#define x(foo)   foo
#define y(foo)  -foo
    testblock
#undef x
#undef y
#undef yp
#undef ym
#define yp(foo) -foo
#define ym(foo)  foo
#define x(foo)  -foo
#define y(foo)  -foo
    testblock
#undef x
#undef y
#undef yp
#undef ym
#undef testblock

    {
      const gsize n_points = 6;
      const Vector2 points[n_points] = {Vector2(4.0f, 1.5f),
                                        Vector2(2.0f, 2.0f),
                                        Vector2(2.0f, 4.0f),
                                        Vector2(2.5f, 3.0f),
                                        Vector2(4.0f, 4.0f),
                                        Vector2(3.5f, 2.0f)};
      check_expect(within_ngon(Vector2(2.60f, 3.50f), points, n_points), true);
      check_expect(within_ngon(Vector2(3.00f, 1.60f), points, n_points), false);
      check_expect(within_ngon(Vector2(3.55f, 3.50f), points, n_points), true);
      check_expect(within_ngon(Vector2(3.55f, 2.00f), points, n_points), false);
      check_expect(within_ngon(Vector2(2.50f, 2.00f), points, n_points), true);
    }

    check_expect(line_cuts_vline(Vector2(0.f, 0.f), Vector2(0.f, 1.f), Vector2(1.f, 0.f), 1.f), false);
    check_expect(line_cuts_vline(Vector2(0.f, 0.f), Vector2(0.f, 1.f), Vector2(0.f, 0.f), 1.f), true);
    check_expect(line_cuts_vline(Vector2(0.f, 0.f), Vector2(0.f, 1.f), Vector2(0.f, 1.1f), 1.f), false);
    check_expect(line_cuts_vline(Vector2(0.f, 0.f), Vector2(2.f, 1.f), Vector2(1.f, 1.f), -1.f), true);
    check_expect(line_cuts_vline(Vector2(0.f, 0.f), Vector2(2.f, 1.f), Vector2(0.f, 1.f), -1.f), true);
    check_expect(line_cuts_vline(Vector2(0.f, 2.f), Vector2(2.f, 1.f), Vector2(0.f, 1.f), -1.f), false);
  }
}