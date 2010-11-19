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

#ifndef _SPACEDEMINER_UI_FRAMEWORK_ENUMS_H_
#define _SPACEDEMINER_UI_FRAMEWORK_ENUMS_H_

namespace Framework
{
  enum PolicyType
  {
    POLICY_ALWAYS,
    POLICY_NEVER,
    POLICY_AUTOMATIC,
  };

  enum ShadowType
  {
    SHADOW_NONE,
    SHADOW_IN,
    SHADOW_OUT
  };

  enum ReliefType
  {
    RELIEF_NEVER,
    RELIEF_OVER,
    RELIEF_ALWAYS,
  };

  enum ButtonBoxStyle
  {
    BUTTONBOX_SPREAD,
    BUTTONBOX_EDGE,
    BUTTONBOX_START,
    BUTTONBOX_END,
    BUTTONBOX_CENTER,
  };

  enum DrawPass
  {
    DRAWPASS_BACK,
    DRAWPASS_FRONT,
  };

  inline bool is_valid_enum_value(PolicyType policy_type)
  {
    return (policy_type==POLICY_ALWAYS || policy_type==POLICY_NEVER || policy_type==POLICY_AUTOMATIC);
  }
  inline bool is_valid_enum_value(ShadowType shadow_type)
  {
    return (shadow_type==SHADOW_NONE || shadow_type==SHADOW_IN || shadow_type==SHADOW_OUT);
  }
}

#endif