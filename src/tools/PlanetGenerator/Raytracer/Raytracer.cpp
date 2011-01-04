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

#include "./../MainWindow.hpp"

namespace Raytracer
{
  namespace Private
  {
    const int BIG_SIZE = 16;
    const int MAX_SIZE = 32;
    const int MEGA = (1<<20);

    class TextureFileChecker : public Refable
    {
      TextureFileChecker(Texture& tex, std::list<Glib::ustring>& warnings) : texture(tex)
      {
        size  = 0;

        if(!tex.visible)
        {
          return;
        }

        Glib::ustring full_filename = get_full_filename();

        Glib::RefPtr<Gio::File> file = Gio::File::create_for_path(full_filename);
        bool exist  = file->query_exists();

        g_assert(!texture.load_small_version);

        if(!exist)
        {
          texture.load_small_version = true;
          warnings.push_back(Glib::ustring::compose(_("File &lt;%1&gt;  <b>not found</b>"), full_filename));
        }else
        {
          Glib::RefPtr<Gio::FileInfo> info  = file->query_info(Glib::ustring::compose("%1,%2,%3", G_FILE_ATTRIBUTE_STANDARD_SIZE, G_FILE_ATTRIBUTE_ACCESS_CAN_READ, G_FILE_ATTRIBUTE_TIME_MODIFIED));

          bool can_read  = info->get_attribute_boolean(G_FILE_ATTRIBUTE_ACCESS_CAN_READ);

          if(!can_read)
          {
            texture.load_small_version = true;
          }else
          {
            Glib::TimeVal last_modification  = info->modification_time();

            texture.should_reload |= (last_modification != texture.last_modification_time);
            texture.last_modification_time  = last_modification;

            if(texture.should_reload)
            {
              size  = info->get_size();
              gfloat file_size_rounded  = 0.1f*round(int((100*size)/MEGA)*0.1f);
              if(size > MAX_SIZE*MEGA)
              {
                warnings.push_back(Glib::ustring::compose(_("<b>Can't load</b> File &lt;%1&gt;  with %2MB &gt; %3MB<b> too large</b>"), full_filename, file_size_rounded, MAX_SIZE));
                texture.load_small_version  = true;
              }else if(size > BIG_SIZE*MEGA)
                warnings.push_back(Glib::ustring::compose(_("<b>Warning:</b> File &lt;%1&gt;  %2MB <b>&gt; %3MB</b>"), full_filename, file_size_rounded, BIG_SIZE));
            }
          }
        }
      }

      goffset size;
    public:
      static bool cmp(const Glib::RefPtr<TextureFileChecker>& a, const Glib::RefPtr<TextureFileChecker>& b)
      {
        return a->size > b->size;
      }

      goffset get_size()const{return size;}

      Texture& texture;

      const Glib::ustring& get_filename()const{return texture.get_filename();}
      Glib::ustring get_full_filename()const{return apply_filename_macros(texture.get_filename());}

      static Glib::RefPtr<TextureFileChecker> create(Texture& tex, std::list<Glib::ustring>& warnings){return Glib::RefPtr<TextureFileChecker>(new TextureFileChecker(tex, warnings));}
    };
  }

  Manager* Manager::_singleton = nullptr;

  Manager::Manager()
  {
    g_assert(!_singleton);
    _singleton  = this;

    _settings = new Settings;

    _doing_preview  = true;

    base_texture.init();
    night_texture.init();
    weight_map.init();
    cloud_layer.init();
  }

  Manager::~Manager()throw()
  {
    g_assert(_singleton);
    _singleton  = nullptr;
  }

  void Manager::open_settings()
  {
    get_settings().bring_to_front();
  }

  bool Manager::prepare_textures()
  {

    using Private::TextureFileChecker;

    typedef std::vector<Glib::RefPtr<TextureFileChecker> > FileList;

    FileList files;
    std::list<Glib::ustring> warnings;

    // ---- resetting Filenames ----------------
    get_singletonA()->base_texture.reset_filename();
    get_singletonA()->night_texture.reset_filename();
    get_singletonA()->weight_map.reset_filename();
    get_singletonA()->cloud_layer.reset_filename();

    files.push_back(TextureFileChecker::create(get_singletonA()->base_texture, warnings));
    files.push_back(TextureFileChecker::create(get_singletonA()->night_texture, warnings));
    files.push_back(TextureFileChecker::create(get_singletonA()->weight_map, warnings));
    files.push_back(TextureFileChecker::create(get_singletonA()->cloud_layer, warnings));

    for(FileList::iterator i = files.begin(); i!=files.end();)
    {
      if((*i)->texture.visible)
        ++i;
      else
        i = files.erase(i);
    }

    Process::PushProcess pp("Preparing renderer", true, Process::PROCESS_RENDER, files.size(), true);

    switch(WarningListDialog::go(warnings, get_settings().get_use_large_texture() ? _("Use Preview Textures") : Glib::ustring()))
    {
    case WarningListDialog::OK:
    case WarningListDialog::USER:
      break;
    case WarningListDialog::CANCEL:
    default:
      return false;
    }

    // ---- sort by size ---------------- (so if a Image is too big, you don't have to load all images until you find out, the image is too big)
    std::sort(files.begin(), files.end(), TextureFileChecker::cmp);

    for(FileList::iterator i = files.begin(); i!=files.end() && !Process::is_curr_process_aborted(); ++i)
    {
      (*i)->texture.reload_file();
    }

    return !Process::is_curr_process_aborted();
  }

  void Manager::render(bool preview)
  {
    Process::PushProcess pp("Rendering", true, Process::PROCESS_RENDER, get_settings().get_n_render_tiles()*get_settings().get_n_render_tiles(), true);

    if(get_singletonA()->prepare_textures())
    {
      get_singletonA()->resulting_image.prepare(preview);

      // ==== Render ====

      get_singletonA()->resulting_image.render();

      if(Process::is_curr_process_aborted())
      {
        Gtk::MessageDialog("Rendering aborted by user. No image is saved").run();
      }else
      {
        Glib::RefPtr<Gdk::Pixbuf> result = get_resulting_image().get_pixbuf();

        if(result)
        {
          Process::PushProcess pp("Saving Result", false, Process::PROCESS_RENDER, 0, true);
          //TODO make parent dir if not existing
          result->save(apply_filename_macros(get_settings().get_dest_file()), "png");
        }
      }

      // ==== Let the user decide, how long the results are aviable to see
      Process::PushProcess pp("View Render Result", false, Process::PROCESS_RENDER, 0, true);
      set_doing_preview(true);

      while(get_doing_preview() && main_window->get_visible())
      {
        while(get_doing_preview() && Gtk::Main::events_pending())
        {
          Gtk::Main::iteration();
        }
      }
    }
  }
}