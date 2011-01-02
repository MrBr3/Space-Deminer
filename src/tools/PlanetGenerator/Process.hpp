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

class Process : public Refable
{
public:
  enum ID
  {
    PROCESS_UNKOWN,
    PROCESS_RENDER,
  };

private:
  Process(const Process& p);
  Process()
  {
    g_assert(!_singleton);
    _singleton  = this;
  }
  ~Process()throw()
  {
    g_assert(_singleton);
    _singleton  = nullptr;
  }

  static Process* get_singleton(){return _singleton;}
  static Process* get_singletonA(){g_assert(_singleton);return _singleton;}

  class ProcessStackEntry : public Refable
  {
    gsize _n_done_steps;

  public:
    const bool _abortable;
    const ID _id;
    const gsize _n_total_steps;
    const bool _block_changes;

    void inc_step(){_n_done_steps++;}
    gsize curr_step()const{return _n_done_steps;}

    const Glib::ustring _what_doing;

    ProcessStackEntry(const Glib::ustring& what_doing, bool abortable, ID id, gsize n_total_steps, bool block_changes) : _what_doing(what_doing), _abortable(abortable), _id(id), _n_total_steps(n_total_steps), _block_changes(block_changes)
    {
      _n_done_steps = 0;
    }
    ~ProcessStackEntry()throw()
    {
    }

    static Glib::RefPtr<ProcessStackEntry> create(const Glib::ustring& what_doing, bool abortable, ID id, gsize n_total_steps, bool block_changes){return Glib::RefPtr<ProcessStackEntry>(new ProcessStackEntry(what_doing, abortable, id, n_total_steps, block_changes));}
  };

  static Process* _singleton;

  typedef std::list<Glib::RefPtr<ProcessStackEntry> > ProcessStack;
  ProcessStack process_stack;

  Glib::ustring what_doing;

  sigc::signal<void> _signal_something_changed;

  void _block_changes(bool unblock);

public:
  class State : Refable
  {
  public:
    Glib::ustring state;
    bool abortable;
    bool has_progress;
    gdouble p;

    void reset()
    {
      state.clear();
      p = 0.;
      has_progress  = false;
      abortable = false;
    }

    State()
    {
      reset();
    }
    ~State()throw()
    {
    }
  };

  static sigc::signal<void>& signal_something_changed(){return get_singletonA()->_signal_something_changed;}

  static void push_process(const Glib::ustring& what_doing, bool abortable, ID id, gsize n_total_steps, bool block_changes)
  {
    if(what_doing.empty())
      throw std::invalid_argument("**Process::push_process** what_doing must not be empty");
    if(id==PROCESS_UNKOWN)
      throw std::invalid_argument("**Process::push_process** id must not be PROCESS_UNKOWN");
    get_singletonA()->what_doing.clear();
    get_singletonA()->process_stack.push_back(ProcessStackEntry::create(what_doing, abortable, id, n_total_steps, block_changes));

    if(block_changes)
    {
      get_singletonA()->_block_changes(false);
    }

    signal_something_changed().emit();
  }

  static void pop_process()
  {
    ProcessStack& process_stack = get_singletonA()->process_stack;

    g_assert(process_stack.size());

    Glib::RefPtr<ProcessStackEntry> top = *process_stack.rbegin();

    get_singletonA()->what_doing.clear();
    if(top->_block_changes)
    {
      get_singletonA()->_block_changes(true);
    }
    process_stack.pop_back();

    signal_something_changed().emit();
  }

  static void processing(const Glib::ustring& what_doing)
  {
    get_singletonA()->what_doing  = what_doing;

    signal_something_changed().emit();
  }

  static void processing_step_done(ID id)
  {
    for(ProcessStack::reverse_iterator i = get_singletonA()->process_stack.rbegin(); i != get_singletonA()->process_stack.rend(); ++i)
    {
      if((*i)->_id == id)
      {
        (*i)->inc_step();
        break;
      }
    }

    signal_something_changed().emit();
  }

  static void fill_state(State& state)
  {
    ProcessStack& process_stack = get_singletonA()->process_stack;

    state.reset();

    if(process_stack.size())
    {
      Glib::RefPtr<ProcessStackEntry> top = *process_stack.rbegin();
      g_assert(top);

      if(get_singletonA()->what_doing.empty())
      {
        state.state = top->_what_doing;
      }else
      {
        state.state = Glib::ustring::compose("%1  [%2]", top->_what_doing, get_singletonA()->what_doing);
      }

      if(top->_n_total_steps)
      {
        state.has_progress  = true;
        state.p = gdouble(top->curr_step()) / gdouble(top->_n_total_steps);
      }

      state.abortable = top->_abortable;
    }else if(get_singletonA()->what_doing.empty())
    {
      state.state = "Ready";
    }else
    {
      state.state = get_singletonA()->what_doing;
    }
  }

  static void abort_process()
  {
    std::cout<<"TODO: Abort\n";
  }

  static Glib::RefPtr<Process> create(){return Glib::RefPtr<Process>(new Process);}

  class PushProcess : public Refable
  {
  public:
    PushProcess(const Glib::ustring& what_doing, bool abortable, ID id, gsize n_total_steps, bool block_changes)
    {
      Process::push_process(what_doing, abortable, id, n_total_steps, block_changes);
    }
    ~PushProcess()throw()
    {
      Process::pop_process();
    }
  };

  class DoingProcessStep : public Refable
  {
    const ID _id;
  public:
    DoingProcessStep(const Glib::ustring& what_doing, ID id=PROCESS_UNKOWN) : _id(id)
    {
      Process::processing(what_doing);
    }
    ~DoingProcessStep()throw()
    {
      Process::processing_step_done(_id);
    }
  };
};