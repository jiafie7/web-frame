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
  req.parseHeader(buf, len);
  req.show();

  std::string html = "  \
<!DOCTYPE html>         \
<html>                  \
  <head>                \
    <meta charset=\"utf-8\">    \
    <title>Web-Frame</title>    \
  </head>               \
  <body>                \
    <h1>Web Framework</h1>      \
    <p>A lightweight C++ web MVC frameword!</p> \
  </body>               \
</html>";

  std::ostringstream oss;
  oss << "HTTP/1.1 200 OK\r\n";
  oss << "Content-Type: text/html; charset: utf-8\r\n";
  oss << "Content-Length: " << html.size() << "\r\n\r\n";
  oss << html << "\r\n";

  std::string resp = oss.str();
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
