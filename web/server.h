#pragma once

#include <functional>
#include <iostream>

#include "utility/singleton.h"
#include "utility/string.h"
using namespace melon::utility;

#include "web/request.h"
#include "web/response.h"
using namespace melon::web;

namespace melon
{
  namespace web
  {
    typedef std::function<void(const Request&, Response&)> server_handler;
    class Server
    {
      SINGLETON(Server);
    public:
      void start();

      // route callback for uri
      void bind(const std::string& path, server_handler handler);
      std::string handle(const Request& req);

      // static resources
      void setTemplateFolder(const std::string& template_folder);
      std::string getTemplateFolder() const;

      void setStaticFolder(const std::string& static_folder);
      std::string getStaticFolder() const;

    private:
      // m_path to handler
      std::map<std::string, server_handler> m_handlers;
      
      // default path of static resources
      std::string m_template_folder = "template";
      std::string m_static_folder = "static";
    };
  }
}
