#pragma once

#include "web/controller.h"
using namespace melon::web;

namespace melon
{
  namespace app
  {
    class File : public Controller
    {
    public:
      void index(const Request& req, Response& resp);
      void upload(const Request& req, Response& resp);
    };
  }
}
