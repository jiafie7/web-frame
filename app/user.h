#pragma once

#include <web/controller.h>
using namespace melon::web;

namespace melon
{
    namespace app
    {
        class User : public Controller
        {
        public:
            void profile(const Request& req, Response& resp);
        };
    }
}
