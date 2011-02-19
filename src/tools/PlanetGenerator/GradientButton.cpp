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

#include "./MainWindow.hpp"

GradientPreview::GradientPreview()
{
  _gradient = Gradient::create_black2white();
}

GradientPreview::~GradientPreview()throw()
{
}

void GradientPreview::set_gradient(const GradientPtr& g)
{
  _gradient = g;
  _gradient->require_puffer(256);
  invalidate(this);
}

bool GradientPreview::on_expose_event(GdkEventExpose* ee)
{
  return true;
}

void GradientPreview::on_size_request(Gtk::Requisition* r)
{
  if(r)
  {
    r->width = 35;
    r->height = 15;
  }
}

// ------------


// ------------

GradientButton::GradientButton()
{
  _gradient = Gradient::create_black2white();

  preview_.show();
  preview_.set_gradient(_gradient);
  frame_.add(preview_);

  frame_.show();
  frame_.unset_label();
  frame_.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);

  add(frame_);
}

GradientButton::~GradientButton()throw()
{
}

void GradientButton::set_gradient(const GradientPtr& g)
{
  _gradient = g;

  preview_.set_gradient(_gradient);
  signal_changed().emit();
}
