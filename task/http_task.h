#pragma once

#include "socket/socket.h"
using namespace melon::socket;

#include "thread/task.h"
using namespace melon::thread;

#include "web/request.h"
using namespace melon::web;

namespace melon
{
  namespace task
  {
    class HttpTask : public Task
    {
    public:
      HttpTask() = delete;
      HttpTask(int socket_fd);
      ~HttpTask();
        
      void run();
      void destroy();

    private:
      int m_socket_fd = 0;
      bool m_closed = false;
    };
  }
}
