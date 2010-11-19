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

#ifndef _SPACE_DEMINER_BASE_OBSLINK_H_
#define _SPACE_DEMINER_BASE_OBSLINK_H_

#include "./refable.hpp"

/**
*
* \note This class does NOT have a virtual destructor!
*/
template<class t_obj>
class ObsLink
{
private:
  t_obj* m_object;

  /** \brief Gets called by the sigc::trackable Object, when the opbject is getting deleted
  */
  static void* notify_function(void* ptr)
  {
    ObsLink<t_obj>& ptr_this = *((ObsLink<t_obj>*)ptr);
    g_assert(&ptr_this);

    ptr_this.signal_observed_deleted.emit(ptr_this.m_object);
    ptr_this.m_object = nullptr;
    return ptr;
  }

  /** \brief Makes sure Obs Link wil cooperate with sigc::trackable objects
  *
  * Makes sure notify_function gets called when o will be deleted and notify_function won't be called, when the old object gets deleted.
  *
  */
  void set_obj(t_obj* o)
  {
    if(m_object)
    {
      signal_link_changed.emit(m_object, o);
      m_object->remove_destroy_notify_callback(this);
    }
    if(o)
    {
      o->add_destroy_notify_callback(this, &ObsLink<t_obj>::notify_function);
    }
    m_object  = o;
  }

public:
  /** \brief Called, when the Object pointed to has been destroyed.
  */
  sigc::signal<void, t_obj*> signal_observed_deleted;

  /** \brief Called, when the Pointer was changed via the = operator or this ObsLink is going to be destroyed.
  *
  * The first param is the old object, the second the new object.
  */
  sigc::signal<void, t_obj*, t_obj*> signal_link_changed;

public:
  t_obj* operator->()const{return m_object;}
  t_obj& operator*()const{return *m_object;}

  operator       t_obj*()     {return m_object;}
  operator const t_obj*()const{return m_object;}

  operator bool()const{return m_object;}
  bool operator!()const{return !m_object;}

  bool operator == (const ObsLink<t_obj>& ol)const{return ol.m_object == m_object;}
  bool operator != (const ObsLink<t_obj>& ol)const{return ol.m_object != m_object;}
  bool operator == (      t_obj* ol)const{return ol == m_object;}
  bool operator != (      t_obj* ol)const{return ol != m_object;}
  bool operator == (const t_obj* ol)const{return ol == m_object;}
  bool operator != (const t_obj* ol)const{return ol != m_object;}

/*template<class t_obj2>bool operator == (const t_obj2* ol)const{return ol == m_object;}
  template<class t_obj2>bool operator != (const t_obj2* ol)const{return ol != m_object;}*/
  template<class t_obj2>bool operator == (      t_obj2* ol)const{return ol == m_object;}
  template<class t_obj2>bool operator != (      t_obj2* ol)const{return ol != m_object;}

public:
  ObsLink(t_obj* o)
  {
    m_object  = nullptr;
    set_obj(o);
  }
  ObsLink& operator=(t_obj* o)
  {
    set_obj(o);
    return *this;
  }

public:
  ObsLink& operator=(const ObsLink<t_obj>& ol)
  {
    set_obj(ol.m_object);
    return *this;
  }
  ObsLink(const ObsLink<t_obj>& ol)
  {
    m_object  = nullptr;
    set_obj(ol.m_object);
  }
public:
  ObsLink()
  {
    m_object  = nullptr;
  }
  ~ObsLink()
  {
    set_obj(nullptr);
  }
};
#endif
