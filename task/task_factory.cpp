#include "task/task_factory.h"
#include "task/http_task.h"
using namespace melon::task;

Task* TaskFactory::create(int socket_fd)
{
  // lock before create Task
  std::unique_lock<std::mutex> lock(m_mutex);

  auto it = m_socket_task.find(socket_fd);
  if (it != m_socket_task.end())
    return it->second;
  
  auto task = new HttpTask(socket_fd);
  m_socket_task[socket_fd] = task;
  return task;
}

void TaskFactory::remove(int socket_fd)
{
  // lock before remove Task
  std::unique_lock<std::mutex> lock(m_mutex);

  auto it = m_socket_task.find(socket_fd);
  if (it != m_socket_task.end())
  {
    delete it->second;
    m_socket_task.erase(it);
  }
  ::close(socket_fd);
}
