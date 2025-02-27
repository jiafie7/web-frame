#pragma once

#include "reflect/class_register.h"
using namespace melon::reflect;

#include "web/request.h"
#include "web/response.h"
using namespace melon::web;

namespace melon
{
  namespace web
  {
    class Controller : public Object
    {
    public:
      Controller() {}
      virtual ~Controller() {}
    };

#define CONTROLLER(classname) \
  REGISTER_CLASS(classname)

#define ACTION(classname, method) \
  REGISTER_CLASS_METHOD(classname, method, void, const Request&, Response&)
  }
}
