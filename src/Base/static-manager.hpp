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

#ifndef _SPACE_DEMINER_BASE_STATICMANAGER_H_
#define _SPACE_DEMINER_BASE_STATICMANAGER_H_

#include "./refable.hpp"

/** \brief
 *
 * \note T_class is only allowed to be a derived class.
 * */
template<class T_class, class T_baseclass = Refable>
class StaticManager : public T_baseclass
{
public:
  class ExistingInstance : public Glib::Exception
  {
  public:
    Glib::ustring what()const{return _("Trying to create a second instance of a singleton object.");}

    ~ExistingInstance()throw(){}
  };
  class NoInstance : public Glib::Exception
  {
  public:
    Glib::ustring what()const{return _("There's no instance of a desired singleton object.");}

    ~NoInstance()throw(){}
  };
  class NotInherited : public Glib::Exception
  {
  public:
    Glib::ustring what()const{return _("T_class is not a inherited class.");}

    ~NotInherited()throw(){}
  };

  /** Gets a pointer to the current singleton object.
   *
   * \return the pointer to the instance - or nullptr if there's no instance initialized.
   *
   * \see get_singletonE
   * */
  static T_class* get_singleton()throw(NoInstance, NotInherited)
  {
    if(!_singleton)
      throw NoInstance();

    if(!dynamic_cast<StaticManager<T_class, T_baseclass>* >(_singleton))
      throw NotInherited();

    return _singleton;
  }

  StaticManager()throw(ExistingInstance)
  {
    g_assert(this);

    if(_singleton)
      throw ExistingInstance();

    _singleton  = static_cast<T_class*>(this);
  }
  ~StaticManager()throw()
  {
    g_assert(_singleton);

    _singleton  = nullptr;
  }

private:
  static T_class* _singleton;
};

template<class T_class, class T_baseclass>
T_class* StaticManager<T_class, T_baseclass>::_singleton  = nullptr;

#endif