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

void mouse_tests(GUITest& gui_test)
{
  std::cout<<"==== Testing Mouse events ====\n";
  std::cout<<"---- Testing Bad Mode ----\n";
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
    btn[1].set_label(":-)");
    btn[2].set_label("Hihi");
    btn[3].set_label("Hehe");

    gui_test.add(layout);
    gui_test.show_all_children();
    gui_test.compare("mouse-001");

    Framework::Widget::MouseEvent mouse_event;

    mouse_event.x  = 256;
    mouse_event.y  = 16;
    main_window->get_window_manager()->on_mouse_move(mouse_event);
  }
  std::cout<<"---- Testing Bad Push ----\n";
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
    btn[1].set_label(":-)");
    btn[2].set_label("Hihi");
    btn[3].set_label("Hehe");

    gui_test.add(layout);
    gui_test.show_all_children();

    {
      Framework::Widget::MouseEvent mouse_event;

      mouse_event.x  = 256;
      mouse_event.y  = 16;
      main_window->get_window_manager()->on_mouse_move(mouse_event);
    }
    {
      Framework::Widget::MouseButtonEvent mouse_event(1, 1);

      mouse_event.x  = 256;
      mouse_event.y  = 16;
      main_window->get_window_manager()->on_button_press(mouse_event);
    }
  }
  std::cout<<"---- Testing Move Bad Push ----\n";
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
    btn[1].set_label(":-)");
    btn[2].set_label("Hihi");
    btn[3].set_label("Hehe");

    gui_test.add(layout);
    gui_test.show_all_children();

    {
      Framework::Widget::MouseEvent mouse_event;

      mouse_event.x  = 256;
      mouse_event.y  = 16;
      main_window->get_window_manager()->on_mouse_move(mouse_event);
      mouse_event.x  = 256;
      mouse_event.y  = 15;
      main_window->get_window_manager()->on_mouse_move(mouse_event);
    }
    {
      Framework::Widget::MouseButtonEvent mouse_event(1, 1);

      mouse_event.x  = 256;
      mouse_event.y  = 15;
      main_window->get_window_manager()->on_button_press(mouse_event);
    }
  }
  std::cout<<"---- Testing Move Push Bad Up ----\n";
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
    btn[1].set_label(":-)");
    btn[2].set_label("Hihi");
    btn[3].set_label("Hehe");

    gui_test.add(layout);
    gui_test.show_all_children();

    {
      Framework::Widget::MouseEvent mouse_event;

      mouse_event.x  = 256;
      mouse_event.y  = 16;
      main_window->get_window_manager()->on_mouse_move(mouse_event);
      mouse_event.x  = 256;
      mouse_event.y  = 15;
      main_window->get_window_manager()->on_mouse_move(mouse_event);
    }
    {
      Framework::Widget::MouseButtonEvent mouse_event(1, 1);

      mouse_event.x  = 256;
      mouse_event.y  = 15;
      main_window->get_window_manager()->on_button_press(mouse_event);

      mouse_event.x  = 256;
      mouse_event.y  = 15;
      main_window->get_window_manager()->on_button_release(mouse_event);
    }
  }
}