#pragma once

#include "web/controller.h"
using namespace melon::web;

namespace melon
{
  namespace app
  {
    class Index : public Controller
    {
    public:
      void index(const Request& req, Response& resp);
      void hello(const Request& req, Response& resp);
    };
  }
}
