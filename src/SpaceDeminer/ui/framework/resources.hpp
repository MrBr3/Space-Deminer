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

#ifndef _SPACEDEMINER_UI_FRAMEWORK_RESOURCES_H_
#define _SPACEDEMINER_UI_FRAMEWORK_RESOURCES_H_

#include <gdkmm/pixbuf.h>
#include <base.hpp>
#include "./enums.hpp"

namespace Framework
{
  class Resource;

  class ResourceManager : public StaticManager<ResourceManager>
  {
    ResourceManager(const ResourceManager&);
  public:
    class UnkownResourceType : public Glib::Exception
    {
    public:
      Glib::ustring what()const{return _("unkown resource type");}

      ~UnkownResourceType()throw(){}
    };

    static void purging_resource(Resource* res)
    {
      g_assert(get_singleton()->_n_resources>0);

      get_singleton()->_n_resources--;
    }
    static void loading_ressource(Resource* res)
    {
      get_singleton()->_n_resources++;
    }

    static gsize get_n_resources()throw(NoInstance, NotInherited){return get_singleton()->_n_resources;}

  private:
    gsize _n_resources;

  public:
    ResourceManager()
    {
      _n_resources=0;
    }
    ~ResourceManager()throw()
    {
      if(_n_resources!=0)
        std::cout<<"Engine::_n_resources: "<<_n_resources<<"\n";
      g_assert(_n_resources==0);
    }
  };

  class Resource : public Refable
  {
    Resource(const Resource&);

  public:
    class CantReloadRes : public Glib::Exception
    {
    public:
      Glib::ustring what()const{return _("Trying to reload a ersource, which can't be reloaded.");}

      ~CantReloadRes()throw(){}
    };
    class PurgedResource : public Glib::Exception
    {
    public:
      Glib::ustring what()const{return _("The resource is purged");}

      ~PurgedResource()throw(){}
    };
    class LoadedResource : public Glib::Exception
    {
    public:
      Glib::ustring what()const{return _("The resource is loaded");}

      ~LoadedResource()throw(){}
    };
    class ResourceInUsage : public Glib::Exception
    {
    public:
      Glib::ustring what()const{return _("The Resource is in usage");}

      ~ResourceInUsage()throw(){}
    };

  public:
    void inc_using()
    {
      g_assert(this);
      _using++;

      if(_using==1 && !get_is_loaded())
      {
        ResourceManager::loading_ressource(this);
        reload_resource();
      }
    }
    void decr_using()
    {
      g_assert(this);
      if(_using==0)
        throw std::out_of_range("Resource::decr_using has been called too often");
      _using--;

      if(_using==0 && get_is_loaded())
      {
        ResourceManager::purging_resource(this);
        purge_resource();
      }
    }

    bool get_is_loaded()const throw(){g_assert(this);return _is_loaded;}
    bool get_in_usage()const throw(){g_assert(this);return _using>0;}

    ~Resource()throw()
    {
      g_assert(!_is_loaded);
    }

  protected:
    /** Overloadable getting called, when the Resource should be  Purged.
     *
     * \note Don't call this directly, use purge_resource instead.
     * */
    virtual void v_purge_resource()=0;
    virtual void v_reload_resource()=0;

    Resource()throw()
    {
      _using  = 1;
      _is_loaded  = true;

      ResourceManager::loading_ressource(this);
    }

    /** \brief Wraps around v_purge_resource.
     *
     * Makes also sure, the get_is_loaded() inspector will return false.
     *
     * Will throw an exception (PurgedResource), when the resourcfe is alredy purged.
     * Will throw an Exception (ResourceInUsage), when the resource is still used.
     * */
    void purge_resource()
    {
      if(get_in_usage()>0)
        throw ResourceInUsage();
      if(!get_is_loaded())
        throw PurgedResource();
      v_purge_resource();
      if(get_is_loaded())
        throw std::runtime_error("the resource should not be loaded now!");
    }
    /** \brief Wraps around v_purge_resource.
     *
     * Makes also sure, the is_loaded inspector will return true.
     *
     * Will throw an exception (LoadedResource), when the resourcfe is alredy loade and couldn'T be purged
     * */
    void reload_resource()
    {
      if(get_is_loaded())
        purge_resource();
      if(get_is_loaded())
        throw LoadedResource();
      v_reload_resource();
      if(!get_is_loaded())
        throw std::runtime_error("the resource should be loaded now!");
    }

  private:
    guint _using;
    bool _is_loaded;

  protected:
    void set_is_loaded(bool is_loaded)throw(){_is_loaded=is_loaded;}
  };

  template<class T_res>
  class ResPtr : public Glib::RefPtr<T_res>
  {
  public:
    ResPtr(const ResPtr<T_res>& rc) : Glib::RefPtr<T_res>(rc)
    {
      if(rc)
        rc->inc_using();
    }

    ResPtr<T_res>& operator=(const ResPtr<T_res>& rc)
    {
      if(*this)
        (*this)->decr_using();
      Glib::RefPtr<T_res>::operator=(rc);
      if(rc)
        rc->inc_using();
      return *this;
    }

    template<class T_other>
    ResPtr(const Glib::RefPtr<T_other>& rc) : Glib::RefPtr<T_res>(rc)
    {
      if(rc)
        rc->inc_using();
    }

    template<class T_other>
    ResPtr<T_res>& operator=(const Glib::RefPtr<T_other>& rc)
    {
      if(*this)
        (*this)->decr_using();
      Glib::RefPtr<T_res>::operator=(rc);
      if(rc)
        rc->inc_using();
      return *this;
    }

    ResPtr()
    {
    }

    void reset()
    {
      if(*this)
        (*this)->decr_using();
      Glib::RefPtr<T_res>::reset();
    }

    explicit ResPtr(T_res* rc) : Glib::RefPtr<T_res>(rc)
    {
    }

    ~ResPtr()
    {
      if(*this)
        (*this)->decr_using();
    }
  };
}

#endif