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

#ifndef _SPACE_DEMINER_BASE_MACROS_H_
#define _SPACE_DEMINER_BASE_MACROS_H_

#define NOTHING_MACRO

/** \brief Catches all Exception
 *
 * Equivalent to:
 * \code
 *catch(const Glib::Exception& e)
 *{
 *  std::cout << WHERE << "an Glib::Exception has been detected: '"<<e.what()<<"'\n";
 *  WHAT_DO
 *}catch(const std::exception& e)
 *{
 *  std::cout << WHERE << "an std::exception has been detected: '"<<e.what()<<"'\n";
 *  WHAT_DO
 *}catch(...)
 *{
 *  std::cout << WHERE << "an unkown exception has been detected!\n";
 *  WHAT_DO
 *}
 * \endcode
 * */
#define CATCH_ALL(WHERE, WHAT_DO)\
  catch(const Glib::Exception& e)\
  {\
    std::cout << WHERE << "an Glib::Exception has been detected: '"<<e.what()<<"'\n";\
    WHAT_DO\
  }catch(const std::exception& e)\
  {\
    std::cout << WHERE << "an std::exception has been detected: '"<<e.what()<<"'\n";\
    WHAT_DO\
  }catch(...)\
  {\
    std::cout << WHERE << "an unkown exception has been detected!\n";\
    WHAT_DO\
  }

#endif