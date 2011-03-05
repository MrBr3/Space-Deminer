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

#define X_SETTING(x, xet_x, sn)\
private:\
x sn;\
sigc::signal<void> _signal_##sn##_changed;\
\
public:\
xet_x get_##sn()const{return sn;}\
void set_##sn(xet_x new_value);\
sigc::signal<void>& signal_##sn##_changed(){return _signal_##sn##_changed;}

#define REAL_SETTING(sn) X_SETTING(gfloat, gfloat, sn)
#define INTEGER_SETTING(sn) X_SETTING(int, int, sn)
#define BOOLEAN_SETTING(sn) X_SETTING(bool, bool, sn)
#define STRING_SETTING(sn) X_SETTING(Glib::ustring, const Glib::ustring&, sn)
#define COLOR_SETTING(sn) X_SETTING(Gdk::Color, const Gdk::Color&, sn)
#define GRADIENT_SETTING(sn) X_SETTING(GradientPtr, const GradientPtr&, sn)
#define CURVE_SETTING(sn) X_SETTING(CurvePtr, const CurvePtr&, sn)

#define X_GETTER_SETTER_SIGNAL(c, sn)\
sigc::mem_fun(*this, &c::get_##sn), sigc::mem_fun(*this, &c::set_##sn), signal_##sn##_changed()

class SettingsWidget;
