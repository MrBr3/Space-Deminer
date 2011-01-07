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

#include "./../ui/main-window.hpp"
#include <gtkmm.h>

const Gdk::Rectangle* dummy_r=nullptr;

class GUITest : public Framework::Window {
public:
  enum AbortTests {ABORT_TESTS, USER_ABORTS_TESTS};

  void on_expose(EventExpose& paint_tool) {
    paint_tool.draw_color_rect(0.f, 0.f, 800.f, 600.f, MenuFrame::get_back_color_r(), MenuFrame::get_back_color_g(), MenuFrame::get_back_color_b());

    Framework::Window::on_expose(paint_tool);
  }

  GUITest() {
  }

  static void update_arrowtype(Gtk::Arrow* arrow, const Gtk::ToggleButton* left_tbutton) {
    g_assert(left_tbutton);
    g_assert(arrow);

    if(left_tbutton->get_active())
      arrow->set(Gtk::ARROW_LEFT, Gtk::SHADOW_IN);
    else
      arrow->set(Gtk::ARROW_RIGHT, Gtk::SHADOW_IN);
  }
  static void update_dependent_togglebutton(const Gtk::ToggleButton* self_tbutton, Gtk::ToggleButton* other_tbutton) {
    g_assert(self_tbutton);
    g_assert(other_tbutton);

    other_tbutton->set_active(!self_tbutton->get_active());
  }
  static void set_image(Gtk::Image* img, Glib::RefPtr<Gdk::Pixbuf>& pb) {
    g_assert(img);
    g_assert(pb);

    img->set(pb);
  }

  void compare(const Glib::ustring& test_name);
  void exchange_bytes(guint8* a, guint8* b, gsize n);
};

template<typename T>
void check_expect(const T& result, const T& reference)
{
  if(result!=reference) {
    std::cout<<"**GUITest::check_expect** test failed\n";
    throw GUITest::USER_ABORTS_TESTS;
  }
}

template<typename T=std::string>
void check_expect(const std::string& result, const std::string& reference)
{
  if(result!=reference) {
    std::cout<<"**GUITest::check_expect** test failed\nexpected \n\""<<reference<<"\"\nthe real value is\n\""<<result<<"\"\n";
    throw GUITest::USER_ABORTS_TESTS;
  }
}

template<typename T>
void check_within(const T& result, const T& reference, const T& epsilon)
{
  if(abs(result-reference)>epsilon) {
    std::cout<<"**GUITest::check_within** test failed\n";
    throw GUITest::USER_ABORTS_TESTS;
  }
}

void check_within(const gfloat& result, const gfloat& reference, const gfloat& epsilon)
{
  if(abs(result-reference)>epsilon) {
    std::cout<<"**GUITest::check_within** test failed\nexpected\n"<<reference<<"\nthe real value is\n"<<result<<"\n";
    throw GUITest::USER_ABORTS_TESTS;
  }
}

template<typename T>
void check_within(const T& result, const T& reference)
{
  check_within(result, reference, T(1.e-4f));
}

template<typename T=Vector2>
void check_within(const Vector2& result, const Vector2& reference, gfloat epsilon=1.e-4f)
{
  try
  {
    check_within(result.x, reference.x, epsilon);
    check_within(result.y, reference.y, epsilon);
  }catch(...)
  {
    std::cout<<"expected \n"<<reference.str()<<"\n got \n"<<result.str()<<"\n";
    throw;
  }
}

template<typename T=Vector3>
void check_within(const Vector3& result, const Vector3& reference, gfloat epsilon=1.e-4f)
{
  try
  {
    check_within(result.x, reference.x, epsilon);
    check_within(result.y, reference.y, epsilon);
    check_within(result.z, reference.z, epsilon);
  }catch(...)
  {
    std::cout<<"expected \n"<<reference.str()<<"\n got \n"<<result.str()<<"\n";
    throw;
  }
}

template<typename T=Vector4>
void check_within(const Vector4& result, const Vector4& reference, gfloat epsilon=1.e-4f)
{
  try
  {
    check_within(result.x, reference.x, epsilon);
    check_within(result.y, reference.y, epsilon);
    check_within(result.z, reference.z, epsilon);
    check_within(result.w, reference.w, epsilon);
  }catch(...)
  {
    std::cout<<"expected \n"<<reference.str()<<"\n got \n"<<result.str()<<"\n";
    throw;
  }
}

template<typename T=Matrix44>
void check_within(const Matrix44& result, const Matrix44& reference, gfloat epsilon=1.e-4f)
{
  try
  {
    for(gsize i=0; i<16; ++i)
      check_within(result.m[i], reference.m[i], epsilon);
  }catch(...)
  {
    std::cout<<"expected \n"<<reference.str()<<"\n got \n"<<result.str()<<"\n";
    throw;
  }
}

#define SHOULD_FAIL(x) try{x; std::cout<<"the test \"" #x "\" should fail\n";throw GUITest::USER_ABORTS_TESTS;}catch(...){}

#include "./test-matrix.hpp"

void start_gui_test() {
  try {
    {
      int w=0, h=0;
      bool need_to_resize = false;
      MainWindow::get_singleton()->get_size(w, h);
      if(MenuAlignment::get_singleton()->get_width()<800) {
        need_to_resize  = true;
        w  += 800-MenuAlignment::get_singleton()->get_width();
      }
      if(MenuAlignment::get_singleton()->get_height()<600) {
        h  += 600-MenuAlignment::get_singleton()->get_height();
        need_to_resize  = true;
      }

      if(need_to_resize) {
        MainWindow::get_singleton()->resize(w, h);

        Glib::Timer timer;
        timer.start();

        while(Gtk::Main::events_pending() || timer.elapsed()<0.5)
          Gtk::Main::iteration();
      }
    }

    g_assert(main_window);
    GUITest gui_test;
    gui_test.set_theme(main_window->gl_drawing_area.get_dummy_theme());
    gui_test.set_size(800, 600);
    MainWindow::get_window_manager()->register_window(0xffff, gui_test);
    gui_test.show();

    std::cout<<"==== initialising Tests ====\n";

    Framework::ResPtr<Framework::Image> img_16x16_0x00ff00 = Framework::Image::create_from_file(Glib::filename_from_utf8(apply_filename_macros("$(exe-share)/ui/tests/img_16x16_0x00ff00.png")));
    Framework::ResPtr<Framework::Image> img_skull48 = Framework::Image::create_from_file(Glib::filename_from_utf8(apply_filename_macros("$(exe-share)/ui/icons/48/skull.png")));

    std::cout<<"==== Testing String ====\n";

    {
      check_expect<Glib::ustring>(clip_last_slash("/abc"), "/abc");
      check_expect<Glib::ustring>(clip_last_slash("/abc/"), "/abc");
      check_expect<Glib::ustring>(clip_last_slash("/abc//\\//\\/\\"), "/abc");
      check_expect<Glib::ustring>(clip_last_slash("//\\//\\/\\"), "/");
      check_expect<Glib::ustring>(clip_last_slash("\\//\\//\\/\\"), "\\");
      check_expect<Glib::ustring>(clip_last_slash("/"), "/");
      check_expect<Glib::ustring>(clip_last_slash(""), "");
    }
    {
      check_expect<Glib::ustring>(str_copy_replace_all_with("", "a", "b"), "");
      check_expect<Glib::ustring>(str_copy_replace_all_with("~/Desktop/Foo.svg", "a", "b"), "~/Desktop/Foo.svg");
      check_expect<Glib::ustring>(str_copy_replace_all_with("~/Desktop/Foo.svg", "e", "x"), "~/Dxsktop/Foo.svg");
      check_expect<Glib::ustring>(str_copy_replace_all_with("~/Desktop/Foo.svg", "o", "i"), "~/Desktip/Fii.svg");
      check_expect<Glib::ustring>(str_copy_replace_all_with("~/Desktop/Foo.svg", "esk", "isk"), "~/Disktop/Foo.svg");
      check_expect<Glib::ustring>(str_copy_replace_all_with("~/Desktop/Foo.svg", "", "isk"), "~/Desktop/Foo.svgisk");
      check_expect<Glib::ustring>(str_copy_replace_all_with("", "", "isk"), "isk");
      check_expect<Glib::ustring>(str_copy_replace_all_with("abc", "b", ""), "ac");
      check_expect<Glib::ustring>(str_copy_replace_last_with("~/Desktop/Foo.svg", '.', "-large."), "~/Desktop/Foo-large.svg");
      check_expect<Glib::ustring>(str_copy_replace_last_with("~/Desktop/earth.day.svg", '.', ".large."), "~/Desktop/earth.day.large.svg");
      check_expect<Glib::ustring>(str_copy_replace_last_with("~/Desktop/earth.day.svg", 0, ".png"), "~/Desktop/earth.day.svg.png");
      check_expect<Glib::ustring>(str_copy_replace_last_with("~/Desktop/Foo.svg", 'x', "-large."), "~/Desktop/Foo.svg");
    }
    std::cout<<"==== Testing Vector ====\n";
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
    test_matrix();
    std::cout<<"==== Testing GUI ====\n";
    {
      Framework::Layout layout, layout2;
      Framework::DummyWidget dummy[2];

      gui_test.add(layout);

      dummy[0].set_color(0xff0000ff);
      dummy[1].set_color(0x00ff00ff);

      layout.add(dummy[0], 16, 16, 32, 16);

      dummy_r = &dummy[0].get_allocation();

      layout.show();

      g_assert(layout.get_size_request_width()==0);

      dummy[0].show();
      dummy[1].show();

      g_assert(layout.get_size_request_width()==48);

      layout.add(dummy[1], 224, 240, 32, 16);

      g_assert(layout.get_size_request_width()==256);

      gui_test.compare("layout-001");

      layout.set_border_width(4);

      g_assert(layout.get_size_request_width()==264);

      gui_test.compare("layout-002");

      gui_test.remove(layout);
      gui_test.add(layout2);

      layout2.add(layout, -6, -6, 256, 256);
      layout2.show();

      gui_test.compare("layout-003");
    }
    {
      const gsize N = 16;
      gsize i=0;

      Framework::Layout layout;
      Framework::Alignment alignment[N];
      Framework::DummyWidget dummy[N];

      gui_test.add(layout);

      g_assert(i<N);
      layout.add(alignment[i], 500, 0, 300, 200);
      alignment[i].add(dummy[i]);
      dummy[i].set_color(0x0000ffff);
      dummy[i].set_size_request(-1, 300);
      ++i;

      g_assert(i<N);
      layout.add(alignment[i], 250, 0, 250, 200);
      alignment[i].add(dummy[i]);
      alignment[i].set_border_width(2);
      alignment[i].set_padding(8, 16, 24, 32);
      dummy[i].set_color(0x808080ff);
      dummy[i].set_size_request(75, 25);
      ++i;

      g_assert(i<N);
      layout.add(alignment[i], 250, 200, 250, 200);
      alignment[i].add(dummy[i]);
      alignment[i].set_padding(8, 16, 24, 32);
      dummy[i].set_color(0x008080ff);
      dummy[i].set_size_request(75, 25);
      ++i;

      g_assert(i<N);
      layout.add(alignment[i], 250, 400, 250, 200);
      alignment[i].add(dummy[i]);
      alignment[i].set(0.0f, 0.0f, 0.f, 0.f);
      dummy[i].set_color(0x000080ff);
      dummy[i].set_size_request(64, 4);
      ++i;
      g_assert(i<N);
      layout.add(alignment[i], 250, 400, 250, 200);
      alignment[i].add(dummy[i]);
      alignment[i].set(0.0f, 1.0f, 0.f, 0.f);
      dummy[i].set_color(0x000080ff);
      dummy[i].set_size_request(32, 4);
      ++i;
      g_assert(i<N);
      layout.add(alignment[i], 250, 400, 250, 200);
      alignment[i].add(dummy[i]);
      alignment[i].set(1.0f, 0.0f, 0.f, 0.f);
      dummy[i].set_color(0x000080ff);
      dummy[i].set_size_request(64, 8);
      ++i;
      g_assert(i<N);
      layout.add(alignment[i], 250, 400, 250, 200);
      alignment[i].add(dummy[i]);
      alignment[i].set(1.0f, 1.0f, 0.f, 0.f);
      dummy[i].set_color(0x000080ff);
      dummy[i].set_size_request(32, 8);
      ++i;

      g_assert(i<N);
      layout.add(alignment[i], 250, 400, 250, 200);
      alignment[i].add(dummy[i]);
      alignment[i].set(0.5f, 0.5f, 0.5f, 0.5f);
      dummy[i].set_color(0x008000ff);
      dummy[i].set_size_request(75, 25);
      ++i;

      g_assert(i<N);
      layout.add(alignment[i], 250, 400, 250, 200);
      alignment[i].add(dummy[i]);
      alignment[i].set(0.5f, 0.5f, 0.f, 0.f);
      dummy[i].set_color(0x800000ff);
      dummy[i].set_size_request(75, 25);
      ++i;

      g_assert(i<N);
      layout.add(alignment[i], 500, 200, 300, 200);
      alignment[i].add(dummy[i]);
      alignment[i].set_border_width(1);
      dummy[i].set_color(0x00ff00ff);
      ++i;

      g_assert(i<N);
      layout.add(alignment[i], 501, 401, 298, 198);
      alignment[i].add(dummy[i]);
      dummy[i].set_color(0xff0000ff);
      dummy[i].set_size_request(75, 25);
      ++i;

      gui_test.show_all_children();
      gui_test.compare("alignment-001");
    }
    {
      const gsize N = 24;
      gsize i=0;

      Framework::Layout layout;
      Framework::VBox vbox[4];
      Framework::DummyWidget dummy[N];

      for(int j=0; j<4; ++j) {
        dummy[0+j*6].set_color(0xff0000ff);
        dummy[1+j*6].set_color(0x00ff00ff);
        dummy[2+j*6].set_color(0x0000ffff);
        dummy[3+j*6].set_color(0x800000ff);
        dummy[4+j*6].set_color(0x008000ff);
        dummy[5+j*6].set_color(0x000080ff);
        if(j!=0)
          for(int k=0; k<6; ++k)
            dummy[j*6+k].set_size_request(75, 25);
      }

      layout.add(vbox[0],   0, 0, 199, 600);
      layout.add(vbox[1], 201, 0, 10,  600);
      layout.add(vbox[2], 401, 0, 198, 600);
      layout.add(vbox[3], 601, 0, 199, 600);

      vbox[0].set_name("GuiTest::Box-01::vbox[0]");
      vbox[0].pack_start(dummy[ 0]);
      vbox[0].pack_start(dummy[ 1]);
      vbox[0].pack_start(dummy[ 2]);
      vbox[0].pack_start(dummy[ 3]);
      vbox[0].pack_start(dummy[ 4]);
      vbox[0].pack_start(dummy[ 5]);

      dummy[9].set_size_request(198, -1);
      vbox[1].set_name("GuiTest::Box-01::vbox[1]");
      vbox[1].pack_start(dummy[ 6], false, false);
      vbox[1].pack_start(dummy[ 7], false, false);
      vbox[1].pack_start(dummy[ 8], false, false);
      vbox[1].pack_start(dummy[ 9], false, false);
      vbox[1].pack_start(dummy[10], false, false);
      vbox[1].pack_start(dummy[11], false, false);

      vbox[2].set_name("GuiTest::Box-01::vbox[2]");
      vbox[2].set_spacing(3);
      vbox[2].pack_start(dummy[12]);
      vbox[2].pack_start(dummy[13]);
      vbox[2].pack_start(dummy[14]);
      vbox[2].pack_end(dummy[17]);
      vbox[2].pack_end(dummy[16]);
      vbox[2].pack_end(dummy[15]);

      vbox[3].set_name("GuiTest::Box-01::vbox[3]");
      vbox[3].set_spacing(3);
      vbox[3].set_border_width(1);
      vbox[3].pack_start(dummy[18]);
      vbox[3].pack_start(dummy[19]);
      vbox[3].pack_start(dummy[20]);
      vbox[3].pack_end(dummy[23]);
      vbox[3].pack_end(dummy[22]);
      vbox[3].pack_end(dummy[21]);

      gui_test.add(layout);
      gui_test.show_all_children();

      g_assert(vbox[2].get_size_request_height() == 6*25+5*3);
      g_assert(vbox[3].get_size_request_height() == 6*25+5*3+2);

      gui_test.compare("vbox-001");

      vbox[0].remove_all();
      vbox[1].remove_all();
      vbox[2].remove_all();
      vbox[3].remove_all();

      vbox[1].pack_start(dummy[ 6], true, false);
      vbox[1].pack_start(dummy[ 7], true, false);
      vbox[1].pack_start(dummy[ 8], true, false);
      vbox[1].pack_start(dummy[ 9], true, false);
      vbox[1].pack_start(dummy[10], true, false);
      vbox[1].pack_start(dummy[11], true, false);

      vbox[2].set_spacing(0);
      vbox[2].pack_start(dummy[12]);
      vbox[2].pack_start(dummy[13]);
      vbox[2].pack_start(dummy[14], true, true, 3);
      vbox[2].pack_end(dummy[17]);
      vbox[2].pack_end(dummy[16]);
      vbox[2].pack_end(dummy[15], true, true, 1);

      dummy[18].set_size_request(-1, 100);
      dummy[19].set_size_request(-1, 100);
      dummy[20].set_size_request(-1, 100);
      dummy[21].set_size_request(-1, 100);
      dummy[22].set_size_request(-1, 100);
      dummy[23].set_size_request(-1, 100);

      vbox[3].pack_start(dummy[18]);
      vbox[3].pack_start(dummy[19]);
      vbox[3].pack_start(dummy[20]);
      vbox[3].pack_end(dummy[23]);
      vbox[3].pack_end(dummy[22]);
      vbox[3].pack_end(dummy[21]);

      gui_test.compare("vbox-002");
    }
    {
      const gsize N = 24;
      gsize i=0;

      Framework::Layout layout;
      Framework::HBox hbox[4];
      Framework::DummyWidget dummy[N];

      for(int j=0; j<4; ++j) {
        dummy[0+j*6].set_color(0xff0000ff);
        dummy[1+j*6].set_color(0x00ff00ff);
        dummy[2+j*6].set_color(0x0000ffff);
        dummy[3+j*6].set_color(0x800000ff);
        dummy[4+j*6].set_color(0x008000ff);
        dummy[5+j*6].set_color(0x000080ff);
        if(j!=0)
          for(int k=0; k<6; ++k)
            dummy[j*6+k].set_size_request(25, 75);
      }

      layout.add(hbox[0], 0, 0, 800, 149);
      layout.add(hbox[1], 0, 151, 800, 10);
      layout.add(hbox[2], 0, 301, 800, 148);
      layout.add(hbox[3], 0, 451, 800, 149);

      hbox[0].set_name("GuiTest::Box-01::hbox[0]");
      hbox[0].pack_start(dummy[ 0]);
      hbox[0].pack_start(dummy[ 1]);
      hbox[0].pack_start(dummy[ 2]);
      hbox[0].pack_start(dummy[ 3]);
      hbox[0].pack_start(dummy[ 4]);
      hbox[0].pack_start(dummy[ 5]);

      dummy[9].set_size_request(-1, 148);
      hbox[1].set_name("GuiTest::Box-01::hbox[1]");
      hbox[1].pack_start(dummy[ 6], false, false);
      hbox[1].pack_start(dummy[ 7], false, false);
      hbox[1].pack_start(dummy[ 8], false, false);
      hbox[1].pack_start(dummy[ 9], false, false);
      hbox[1].pack_start(dummy[10], false, false);
      hbox[1].pack_start(dummy[11], false, false);

      hbox[2].set_name("GuiTest::Box-01::hbox[2]");
      hbox[2].set_spacing(3);
      hbox[2].pack_start(dummy[12]);
      hbox[2].pack_start(dummy[13]);
      hbox[2].pack_start(dummy[14]);
      hbox[2].pack_end(dummy[17]);
      hbox[2].pack_end(dummy[16]);
      hbox[2].pack_end(dummy[15]);

      hbox[3].set_name("GuiTest::Box-01::hbox[3]");
      hbox[3].set_spacing(3);
      hbox[3].set_border_width(1);
      hbox[3].pack_start(dummy[18]);
      hbox[3].pack_start(dummy[19]);
      hbox[3].pack_start(dummy[20]);
      hbox[3].pack_end(dummy[23]);
      hbox[3].pack_end(dummy[22]);
      hbox[3].pack_end(dummy[21]);

      gui_test.add(layout);
      gui_test.show_all_children();

      g_assert(hbox[2].get_size_request_width() == 6*25+5*3);
      g_assert(hbox[3].get_size_request_width() == 6*25+5*3+2);

      gui_test.compare("hbox-001");

      hbox[0].remove_all();
      hbox[1].remove_all();
      hbox[2].remove_all();
      hbox[3].remove_all();

      hbox[1].pack_start(dummy[ 6], true, false);
      hbox[1].pack_start(dummy[ 7], true, false);
      hbox[1].pack_start(dummy[ 8], true, false);
      hbox[1].pack_start(dummy[ 9], true, false);
      hbox[1].pack_start(dummy[10], true, false);
      hbox[1].pack_start(dummy[11], true, false);

      hbox[2].set_spacing(0);
      hbox[2].pack_start(dummy[12]);
      hbox[2].pack_start(dummy[13]);
      hbox[2].pack_start(dummy[14], true, true, 3);
      hbox[2].pack_end(dummy[17]);
      hbox[2].pack_end(dummy[16]);
      hbox[2].pack_end(dummy[15], true, true, 1);

      dummy[18].set_size_request(-1, 100);
      dummy[19].set_size_request(-1, 100);
      dummy[20].set_size_request(-1, 100);
      dummy[21].set_size_request(-1, 100);
      dummy[22].set_size_request(-1, 100);
      dummy[23].set_size_request(-1, 100);

      hbox[3].pack_start(dummy[18]);
      hbox[3].pack_start(dummy[19]);
      hbox[3].pack_start(dummy[20]);
      hbox[3].pack_end(dummy[23]);
      hbox[3].pack_end(dummy[22]);
      hbox[3].pack_end(dummy[21]);

      gui_test.compare("hbox-002");
    }
    {
      const gsize N = 30;
      gsize i=0;

      Framework::Layout layout;
      Framework::HButtonBox hbtnbox[5];
      Framework::DummyWidget dummy[N];

      for(int j=0; j<5; ++j) {
        dummy[0+j*6].set_color(0xff0000ff);
        dummy[1+j*6].set_color(0x00ff00ff);
        dummy[2+j*6].set_color(0x0000ffff);
        dummy[3+j*6].set_color(0x800000ff);
        dummy[4+j*6].set_color(0x008000ff);
        dummy[5+j*6].set_color(0x000080ff);
        //for(int k=0; k<6; ++k)
        dummy[j*6+2].set_size_request(75, 25);

        hbtnbox[j].pack_start(dummy[0+j*6]);
        hbtnbox[j].pack_start(dummy[1+j*6], false, true);
        hbtnbox[j].pack_start(dummy[2+j*6], true, true, 64);
        hbtnbox[j].pack_end(dummy[3+j*6], true, false, 1);
        hbtnbox[j].pack_end(dummy[4+j*6], false, false);
        hbtnbox[j].pack_end(dummy[5+j*6], true, true, 1);
      }

      hbtnbox[0].set_layout(Framework::BUTTONBOX_SPREAD);
      hbtnbox[1].set_layout(Framework::BUTTONBOX_EDGE);
      hbtnbox[2].set_layout(Framework::BUTTONBOX_START);
      hbtnbox[3].set_layout(Framework::BUTTONBOX_END);
      hbtnbox[4].set_layout(Framework::BUTTONBOX_CENTER);

      layout.add(hbtnbox[0], 0, 0, 800, 120);
      layout.add(hbtnbox[1], 0, 120, 800, 120);
      layout.add(hbtnbox[2], 0, 240, 800, 120);
      layout.add(hbtnbox[3], 0, 360, 800, 120);
      layout.add(hbtnbox[4], 0, 480, 800, 120);

      gui_test.add(layout);
      gui_test.show_all_children();
      gui_test.compare("hbuttonbox-001");

      for(int j=0; j<5; ++j) {
        hbtnbox[j].set_spacing(16);
      }

      gui_test.compare("hbuttonbox-002");
    }
    {
      Framework::Layout layout;
      Framework::Button btn[8];
      Framework::Alignment align[3];

      layout.add(btn[0], 8, 8, 32, 32);

      layout.add(align[0],  40, 0, 64, 64);
      layout.add(align[1], 104, 0, 48, 48);
      layout.add(btn[3], 200, 0, 128, 128);

      align[0].set(0.5f, 0.5f, 0.f, 0.f);
      align[1].set(0.5f, 0.5f, 0.f, 0.f);

      align[0].add(btn[1]);
      align[1].add(btn[2]);
      btn[1].set_image(img_16x16_0x00ff00);
      btn[2].set_image(img_skull48);
      btn[3].set_image(img_skull48);
      g_assert(btn[1].get_size_request_width()==6+16);

      gui_test.add(layout);
      gui_test.show_all_children();
      gui_test.compare("button");
    }
    {
      Framework::Layout layout;
      Framework::ImageWidget img[4];

      img[0].set(img_skull48);
      img[0].show();
      img[0].set_back_color(0.f, 0.f, 0.f, 1.f);

      layout.add(img[0], 0, 0, 64, 64);

      gui_test.add(layout);
      gui_test.show_all_children();
      gui_test.compare("imagewidget");
    }
    {
      Framework::Layout layout;
      Framework::Label label[8];

      label[0].set_label("Abc");
      label[0].set_name("guitest-label[0]");
      label[0].set_back_color(0.f, 0.f, 0.f, 1.f);
      layout.add(label[0], 0, 0, 64, 64);

      label[1].set_label("Abcdefghijklmnopqrstuvwxyz");
      label[1].set_name("guitest-label[1]");
      label[1].set_back_color(0.f, 0.f, 0.f, 1.f);
      layout.add(label[1], 72, 8, 32, 32);

      label[2].set_label("H");
      label[2].set_alignment(0.f, 0.f);
      label[2].set_name("guitest-label[2]");
      label[2].set_back_color(0.f, 0.f, 0.f, 1.f);
      layout.add(label[2], 120, 8, 32, 32);

      label[3].set_label("H");
      label[3].set_alignment(1.f, 1.f);
      label[3].set_name("guitest-label[2]");
      label[3].set_back_color(0.f, 0.f, 0.f, 1.f);
      layout.add(label[3], 160, 8, 32, 32);

      gui_test.add(layout);
      gui_test.show_all_children();
      gui_test.compare("label-001");
    }
    std::cout<<"==== All Tests Passed :) ====\n";
  } catch(GUITest::AbortTests at) {
    switch(at) {
    case GUITest::ABORT_TESTS:
      std::cout<<"User aborts the following tests.\n";
      break;
    case GUITest::USER_ABORTS_TESTS:
      std::cout<<"The following tests were ignored.\n";
      break;
    default:
      g_assert_not_reached();
    }
  }
  CATCH_ALL("**GUITest::start_gui_test** ", throw;)
}

inline void GUITest::exchange_bytes(guint8* a, guint8* b, gsize n) {
  typedef guint32 guint_x_;
  guint_x_* ax  = (guint_x_*)a;
  guint_x_* bx  = (guint_x_*)b;

  gsize i = n/sizeof(guint_x_);

  a += i*sizeof(guint_x_);
  b += i*sizeof(guint_x_);
  n %= sizeof(guint_x_);

  while(i>0) {
    guint_x_ tmp  = *ax;
    *ax = *bx;
    *bx = tmp;

    ax = &ax[1];
    bx = &bx[1];

    --i;
  }

  while(n!=0) {
    guint8 tmp  = *a;
    *a = *b;
    *b = tmp;

    ++a;
    ++b;
    --n;
  }
}

void GUITest::compare(const Glib::ustring& test_name) {
  std::cout<<"**GUITest::compare** beginning test \""<<test_name.c_str()<<"\"\n";

  while(Gtk::Main::events_pending())
    Gtk::Main::iteration();

  Glib::ustring filename  = apply_filename_macros("$(test-references)/"+test_name);
  if(!make_all_parent_directories(filename))
    g_assert_not_reached();

  guint8 snapshot_data[800*600*3];
  {
    glReadPixels(0, 0, 800, 600, GL_RGB, GL_UNSIGNED_BYTE, snapshot_data);

    for(int y=0; y<300; ++y)
      exchange_bytes(&snapshot_data[800*3*y], &snapshot_data[800*3*(599-y)], 800*3);
  }

  Glib::RefPtr<Gdk::Pixbuf> snapshot = Gdk::Pixbuf::create_from_data(snapshot_data, Gdk::COLORSPACE_RGB, false, 8, 800, 600, 800*3);

  if(!snapshot) {
    std::cout<<"**GUITest::compare** Couldn't create pixbuf from snapshot!\n";
    return;
  }

  try {
    Glib::RefPtr<Gdk::Pixbuf> old_version;

    if(exist_regular_file(filename+".png"))
      old_version = Gdk::Pixbuf::create_from_file(Glib::filename_from_utf8(filename+".png"));

    if(old_version) {
      bool different_pixels=true;
      const guint8* old_line  = old_version->get_pixels();
      const guint8* new_line  = snapshot->get_pixels();

      g_assert(snapshot->get_width()==800);
      g_assert(snapshot->get_height()==600);
      g_assert(old_version->get_width()==800);
      g_assert(old_version->get_height()==600);

      if(snapshot->get_n_channels() == old_version->get_n_channels()) {
        different_pixels  = false;

        g_assert(old_version->get_rowstride() >= 800*snapshot->get_n_channels());

        for(int y=0; y<600; ++y) {
          if(memcmp(old_line, new_line, 800*snapshot->get_n_channels())!=0) {
            different_pixels  = true;
            break;
          }

          old_line  += old_version->get_rowstride();
          new_line  += snapshot->get_rowstride();
        }
      }

      if(different_pixels) {
        const int RESPONSE_KEEP_BOTH = 1;
        const int RESPONSE_KEEP_NEW  = 2;
        const int RESPONSE_KEEP_OLD  = 3;

        Gtk::Dialog compare_dlg(Glib::ustring::compose(_("Different Snapshots (%1)"),test_name), *MainWindow::get_singleton());
        Gtk::Label label(_("Compare the new with the old result and choose an action"));
        Gtk::Button btn_keep_both(_("Keep _both"), true), btn_keep_new(_("Keep n_ew"), true), btn_keep_old(_("Keep o_ld"), true);
        Gtk::CheckButton cb_continue_tests(_("_Continue tests"), true);
        Gtk::VBox vbox;
        Gtk::HButtonBox bb_choose_old_new_display;
        Gtk::ToggleButton btn_show_old(_("Show _old"), true), btn_show_new(_("Show _new"), true);
        Gtk::Arrow arrow(Gtk::ARROW_RIGHT, Gtk::SHADOW_IN);
        Gtk::ScrolledWindow sw_img;
        Gtk::Image image;

        g_assert(compare_dlg.get_vbox());

        compare_dlg.set_position(Gtk::WIN_POS_CENTER);
        compare_dlg.set_deletable(false);
        compare_dlg.get_vbox()->pack_start(vbox, true, true);
        vbox.show();
        vbox.set_border_width(SPACING_NORMAL);
        vbox.set_spacing(SPACING_NORMAL);
        vbox.pack_start(bb_choose_old_new_display, false, false);
        bb_choose_old_new_display.show();
        bb_choose_old_new_display.set_layout(Gtk::BUTTONBOX_CENTER);
        bb_choose_old_new_display.set_spacing(0);
        bb_choose_old_new_display.pack_start(btn_show_old);
        btn_show_old.show();
        btn_show_old.signal_toggled().connect(sigc::bind(sigc::ptr_fun(update_dependent_togglebutton), &btn_show_old, &btn_show_new));
        btn_show_old.signal_toggled().connect(sigc::bind(sigc::ptr_fun(update_arrowtype), &arrow, &btn_show_old));
        btn_show_old.signal_toggled().connect(sigc::bind(sigc::ptr_fun(set_image), &image, old_version));
        bb_choose_old_new_display.pack_start(arrow);
        arrow.show();
        bb_choose_old_new_display.pack_start(btn_show_new);
        btn_show_new.show();
        btn_show_new.set_active(true);
        btn_show_new.signal_toggled().connect(sigc::bind(sigc::ptr_fun(update_dependent_togglebutton), &btn_show_new, &btn_show_old));
        btn_show_new.signal_toggled().connect(sigc::bind(sigc::ptr_fun(set_image), &image, snapshot));
        vbox.pack_start(sw_img);
        sw_img.show();
        sw_img.set_shadow_type(Gtk::SHADOW_IN);
        sw_img.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_NEVER);
        compare_dlg.signal_show().connect(sigc::bind(sigc::mem_fun(sw_img, &Gtk::ScrolledWindow::set_policy), Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC));
        sw_img.add(image);
        ((Gtk::Viewport*)image.get_parent())->set_shadow_type(Gtk::SHADOW_NONE);
        image.set(snapshot);
        image.show();
        vbox.pack_end(cb_continue_tests, false, false);
        cb_continue_tests.show();
        cb_continue_tests.set_tooltip_text(_("If active, the tests will continue, otherwise they will be aborted"));
        cb_continue_tests.set_active(true);
        compare_dlg.add_action_widget(btn_keep_both, RESPONSE_KEEP_BOTH);
        compare_dlg.add_action_widget(btn_keep_new, RESPONSE_KEEP_NEW);
        compare_dlg.add_action_widget(btn_keep_old, RESPONSE_KEEP_OLD);
        btn_keep_both.show();
        btn_keep_new.show();
        btn_keep_old.show();
        btn_keep_both.set_tooltip_text(_("Saves the the result to the same folder as the old result, including a \"-new.png\" suffix."));
        btn_keep_new .set_tooltip_text(_("Discards the old result and saves the new result"));
        btn_keep_old .set_tooltip_text(_("Discards the new result and changes nothing"));

        switch(compare_dlg.run()) {
        case RESPONSE_KEEP_BOTH:
          snapshot->save(Glib::filename_from_utf8(filename+"-new.png"), "png");
          break;
        case RESPONSE_KEEP_NEW:
          snapshot->save(Glib::filename_from_utf8(filename+".png"), "png");
          break;
        default:
        case RESPONSE_KEEP_OLD:
          break;
        }

        if(!cb_continue_tests.get_active()) {
          std::cout<<"**GUITest::compare** ending test \""<<test_name.c_str()<<"\"\n";
          throw USER_ABORTS_TESTS;
        }
      }
    } else {
      snapshot->save(Glib::filename_from_utf8(filename+".png"), "png");
    }
  } catch(AbortTests) {
    throw;
  } catch(...) {
  }
  std::cout<<"**GUITest::compare** ending test \""<<test_name.c_str()<<"\"\n";
}
