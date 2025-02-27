#pragma once

#include "socket/socket.h"
using namespace melon::socket;

#include "thread/task.h"
using namespace melon::thread;

#include "web/request.h"
#include "web/response.h"
#include "web/server.h"
using namespace melon::web;

namespace melon
{
  namespace task
  {
    const uint32_t recv_buff_size = 1024 * 8; // 8 KB
    class HttpTask : public Task
    {
    public:
      HttpTask() = delete;
      HttpTask(int socket_fd);
      ~HttpTask();

      void reset();
      void run();
      void destroy();

    private:
      int m_socket_fd = 0;
      bool m_closed = false;

      Request m_req;
      int m_total_len;
      int m_head_len;
      int m_body_len;
      int m_body_idx; // next continue location for uploading
      char* m_body;   // data recv buffer
    };
  }
}
