#pragma once

#include <functional>
#include <iostream>

#include "utility/singleton.h"
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

      void bind(const std::string& path, server_handler handler);
      std::string handle(const Request& req);

    private:
      // m_path to handler
      std::map<std::string, server_handler> m_handlers;
    };
  }
}
