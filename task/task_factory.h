#pragma once

#include "task/http_task.h"
using namespace melon::task;

namespace melon
{
  namespace task
  {
    class TaskFactory
    {
    public:
      static Task* create(int socket_fd)
      {
        return new HttpTask(socket_fd);
      }
    };
  }
}
