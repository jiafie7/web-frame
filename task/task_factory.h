#pragma once

#include <map>
#include <mutex>

#include "thread/task.h"
using namespace melon::thread;

#include "utility/singleton.h"
using namespace melon::utility;

namespace melon
{
  namespace task
  {
    class TaskFactory
    {
      SINGLETON(TaskFactory);
    public:
      Task* create(int socket_fd);
      void remove(int socket_fd);

    private:
      std::mutex m_mutex;
      // each connection corresponds to a Task.
      std::map<int, Task*> m_socket_task;
    };
  }
}
