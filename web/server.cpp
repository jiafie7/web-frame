#include "web/server.h"
using namespace melon::web;

#include "frame/server.h"

void Server::start()
{
  try
  {
    Singleton<melon::frame::Server>::getInstance()->start();
  }
  catch (std::exception& e)
  {
    std::cout << e.what() << '\n';
  }
}
 
void Server::bind(const std::string& path, server_handler handler)
{
  m_handlers[path] = handler;
}

std::string Server::handle(const Request& req)
{
  const std::string& path = req.path();
  auto it = m_handlers.find(path);
  if (it != m_handlers.end())
  {
    Response resp;
    it->second(req, resp);
    return resp.data();
  }

  return Response::pageNotFound();
}
