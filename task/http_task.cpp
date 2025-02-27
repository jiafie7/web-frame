#include "task/http_task.h"
#include "task/task_factory.h"
using namespace melon::task;

#include "socket/socket_handler.h"
using namespace melon::socket;

HttpTask::HttpTask(int socket_fd)
  : m_socket_fd(socket_fd)
  , m_total_len(0)
  , m_head_len(0)
  , m_body_len(0)
  , m_body_idx(0)
  , m_body(nullptr)
{
}

HttpTask::~HttpTask()
{
}

void HttpTask::reset()
{
  m_total_len = 0;
  m_head_len = 0;
  m_body_len = 0;
  m_body_idx = 0;
  if (m_body != nullptr)
  {
    delete[] m_body;
    m_body = nullptr;
  }
}

void HttpTask::run()
{
  log_debug("http task run.");
  
  Socket socket(m_socket_fd);

  char buf[recv_buff_size] = {0};

  int len = socket.recv(buf, sizeof(buf));

  if (len < 0) 
  {
    if (errno == EAGAIN || errno == EWOULDBLOCK) 
    {
      log_debug("socket recv/send would block: conn = %d", m_socket_fd);
      return;
    }
    else if (errno == EINTR)
    {
      log_debug("socket recv interrupted: conn = %d", m_socket_fd);
      return;
    }
    log_debug("socket connection abort: conn = %d", m_socket_fd);
    m_closed = true;
    return;
  }
  else if (len == 0)
  {
    log_debug("socket closed by peer: conn = %d", m_socket_fd);
    m_closed = true;
    return;
  }
  log_debug("recv: conn = %d, msg = %s", m_socket_fd, buf);

  // m_total_len accumulate the length of data received each request
  m_total_len += len;

  // if m_head_len is 0, it means that this is the first request coming from this connection
  if (m_head_len == 0)
  {
    // start parse http request headers
    m_head_len = m_req.parseHeader(buf, len);
    m_body_len = m_req.contentLength();

    // if m_body_len isn't 0, means that this request has a request body
    if (m_body_len > 0)
    {
      m_body = new char[m_body_len + 1];
      m_body[m_body_len] = 0;
      // extract request body data
      std::memcpy(m_body, buf + m_head_len, len - m_head_len);
      m_body_idx = len - m_head_len;
    }
  }
  else // not first request, all request body data
  {
    std::memcpy(m_body + m_body_idx, buf, len);
    m_body_idx += len;
  }

  // the request data of the current connection has been received end
  if (m_total_len >= m_head_len + m_body_len)
  {
    // start parse http request body
    m_req.parseBody(m_body, m_body_len);
  
    // handle request, generate http response
    Server* server = Singleton<Server>::getInstance();
    std::string resp = server->handle(m_req);
    // send http response to client
    socket.send(resp.c_str(), resp.size());

    // current request handle completed, reset all variables, waiting for next request
    reset();
  }
}

void HttpTask::destroy()
{
  log_debug("http task destroy.");
  if (m_closed)
  {
    // Each connection corresponds to a Task. Delete the Task before closing the connection.
    Singleton<TaskFactory>::getInstance()->remove(m_socket_fd);
  }
  else
  {
     Singleton<SocketHandler>::getInstance()->attach(m_socket_fd);
  }
}
