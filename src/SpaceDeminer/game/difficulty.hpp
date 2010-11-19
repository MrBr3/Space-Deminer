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

#ifndef _SPACE_DEMINER_GAME_HIGHSCORE_H_
#define _SPACE_DEMINER_GAME_HIGHSCORE_H_

#include <base.hpp>
#include <gtkmm/liststore.h>

class Highscore
{
public:
  typedef Glib::RefPtr<Gtk::ListStore> ListStore;

  class NoInstance : public Glib::Exception
  {
  public:
    Glib::ustring what()const{return _("No Instance of the Object Columns");}

    ~NoInstance()throw(){}
  };

  class Columns : public Gtk::TreeModel::ColumnRecord
  {
  public:
    Gtk::TreeModelColumn<Glib::ustring> name;
    Gtk::TreeModelColumn<guint> points;

    Columns()
    {
      add(name);
      add(points);
    }
  };

  const Glib::RefPtr<Gtk::ListStore>& get_list_store()const;
  static const Columns& get_columns()throw(NoInstance)
  {
    if(!_columns)
      throw NoInstance();
    return *_columns.operator->();
  }

  Gtk::TreeModel::Children::iterator add_entry(const Glib::ustring& name, guint points);

  /** \brief Returns the place in the highscore with the difficulty \c diff, a new entry with \c points points would get
   * */
  guint get_insretion_position(guint points);

  static const gsize n_entries  = 10;

private:
  ListStore _liststore;
  static Glib::RefPtr<Columns> _columns;
};

enum DifficultyID
{
  DIFFICULTY_EASY,
  DIFFICULTY_MEDIUM,
  DIFFICULTY_HARD,
};

class Difficulty : public Refable
{
public:
  class InvalidID : public Glib::Exception
  {
  public:
    Glib::ustring what()const{return _("Invalid Diffilulty ID");}

    ~InvalidID()throw(){}
  };
  class IDAlreadyUsed : public Glib::Exception
  {
  public:
    Glib::ustring what()const{return _("This Diffilulty ID is already in use");}

    ~IDAlreadyUsed()throw(){}
  };

  typedef std::map<DifficultyID, Glib::RefPtr<Difficulty> > DifficultiesMap;

  static Difficulty& get_difficulty(DifficultyID diff)throw(InvalidID);
  static const DifficultiesMap& get_map();

  static void deinit_all();
  Difficulty(DifficultyID id, const Glib::ustring& label)throw(IDAlreadyUsed);

public:
  Highscore highscore;
};

#endif