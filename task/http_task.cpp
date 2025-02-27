#include "task/http_task.h"
using namespace melon::task;

#include "socket/socket_handler.h"
using namespace melon::socket;

HttpTask::HttpTask(int socket_fd)
  : m_socket_fd(socket_fd)
{
}

HttpTask::~HttpTask()
{
}

void HttpTask::run()
{
  log_debug("http task run.");
  
  Socket socket(m_socket_fd);

  char buf[1024] = {0};

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

  Request req;
  int ret = req.parseHeader(buf, len);
  req.parseBody(buf + ret, len - ret);
  req.show(); //  log request parse results

  Server* server = Singleton<Server>::getInstance();
  std::string resp = server->handle(req);

  socket.send(resp.c_str(), resp.size());
}

void HttpTask::destroy()
{
  log_debug("http task destroy.");
  if (m_closed)
    ::close(m_socket_fd);
  else
    Singleton<SocketHandler>::getInstance()->attach(m_socket_fd);
  delete this;
}
