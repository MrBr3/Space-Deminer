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

#ifndef _SPACE_DEMINER_BASE_STATE_MACHINE_H_
#define _SPACE_DEMINER_BASE_STATE_MACHINE_H_

#include "./refable.hpp"

/** \brief Manages several states.
 *
 * \typedef T_id
 * */
template<class T_this, class T_state, class T_id>
class StateMachine : public Refable
{
  StateMachine(const T_this&);

  typedef typename Glib::RefPtr<T_state> StatePtr;
  typedef typename Glib::RefPtr<const T_state> ConstStatePtr;
  typedef typename std::map<T_id, StatePtr> StateMap;
  typedef typename std::map<T_id, StatePtr>::iterator StateMapIter;
  typedef typename StateMap::const_iterator StateMapConstIter;
public:
  StateMachine()
  {
  }
  ~StateMachine()throw()
  {
  }

  /** \brief A Class realizing the different states.
   * */
  class State : public Refable
  {
    friend class StateMachine<T_this, T_state, T_id>;

    State(const State&);

    ObsLink<T_this> _state_machine;
  public:
    /** \brief Gets the state machine, the State has been added to.
     *
     * \return The state machine, the State has been added to or nullptr, if there is no one.
     * */
    T_this* get_state_machine(){return _state_machine;}

    /** \brief Overridable getting called when the state gets activated.
     * */
    virtual void on_activate(){}

    /** \brief Overridable getting called when the state gets deactivated.
     * */
    virtual void on_deactivate(){}

    State()
    {
    }

    ~State()throw()
    {
    }
  };

  /** \brief Gets the current state - or a nullpointer, if there's no current state.
   * */
  ConstStatePtr get_active_state()const
  {
    StateMapConstIter iter  = _state_map.find(_curr_state);

    if(iter!=_state_map.end())
      return iter->second;
    return Glib::RefPtr<const T_state>();
  }

  /** \brief Gets the current state - or a nullpointer, if there's no current state.
   * */
  StatePtr get_active_state()
  {
    StateMapIter iter  = _state_map.find(_curr_state);

    if(iter!=_state_map.end())
      return iter->second;
    return Glib::RefPtr<T_state>();
  }

  /** \brief Adds a new state to the state machine.
   *
   * \param id the id, the state will be activated with and represented with - if there's already a state using that id, it will be simply replaced
   * \param state the object representing the state - is not allowed to be a nullpointer. The state must not be already assigned to another state machine.
   * */
  void add_state(T_id id, const StatePtr& state) throw(std::exception)
  {
    if(!state)
      throw std::invalid_argument("**StateMachine::add_state** state is not allowed to be a nullptr!");

    if(state->get_state_machine())
     throw std::logic_error("**StateMachine::add_state** state is not allowed to be already assigned to a state machine");

    state->_state_machine = dynamic_cast<T_this*>(this);
    g_assert(state->get_state_machine());
    _state_map[id]  = state;
  }

  /** \brief Activates the state represented by \c id.
   *
   * \param id the id of the newly activated state - doesn't have to be valid.
   * */
  void activate_state(T_id id)
  {
    _curr_state = id;
  }

private:
  StateMap _state_map;
  T_id _curr_state;

  StateMap map;
};

#endif