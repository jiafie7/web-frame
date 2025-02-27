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

  // if the request path is bound to a handler
  auto it = m_handlers.find(path);
  if (it != m_handlers.end())
  {
    Response resp;
    it->second(req, resp);
    return resp.data();
  }

  // if it's a static request
  if (String::endsWith(path, ".html"))  // "/xxx.html"
  {
    const std::string& filename = getTemplateFolder() + path;
    std::ifstream ifs(filename);
    if (ifs.good())
    {
      std::ostringstream oss;
      oss << ifs.rdbuf();
      Response resp;
      resp.data(Response::HTML, oss.str());
      return resp.data();
    }
  }
  else if (String::endsWith(path, ".js"))  // "/xxx.js" 
  {
    const std::string& filename = getStaticFolder() + path;
    std::ifstream ifs(filename);
    if (ifs.good())
    {
      std::ostringstream oss;
      oss << ifs.rdbuf();
      Response resp;
      resp.data(Response::JS, oss.str());
      return resp.data();
    }
  }
  else if (String::endsWith(path, ".css"))  // "/xxx.css"
  {
    const std::string& filename = getStaticFolder() + path;
    std::ifstream ifs(filename);
    if (ifs.good())
    {
      std::ostringstream oss;
      oss << ifs.rdbuf();
      Response resp;
      resp.data(Response::CSS, oss.str());
      return resp.data();
    }
  }
  else if (String::endsWith(path, ".jpg"))  // "/xxx.jpg"
  {
    const std::string& filename = getStaticFolder() + path;
    std::ifstream ifs(filename);
    if (ifs.good())
    {
      std::ostringstream oss;
      oss << ifs.rdbuf();
      Response resp;
      resp.data(Response::JPG, oss.str());
      return resp.data();
    }
  }
  else if (String::endsWith(path, ".png"))  // "/xxx.png"
  {
    const std::string& filename = getStaticFolder() + path;
    std::ifstream ifs(filename);
    if (ifs.good())
    {
      std::ostringstream oss;
      oss << ifs.rdbuf();
      Response resp;
      resp.data(Response::PNG, oss.str());
      return resp.data();
    }
  }
  else if (String::endsWith(path, ".gif"))  // "/xxx.gif"
  {
    const std::string& filename = getStaticFolder() + path;
    std::ifstream ifs(filename);
    if (ifs.good())
    {
      std::ostringstream oss;
      oss << ifs.rdbuf();
      Response resp;
      resp.data(Response::GIF, oss.str());
      return resp.data();
    }
  }
  else if (String::endsWith(path, ".ico"))  // "/xxx.ico"
  {
    const std::string& filename = getStaticFolder() + path;
    std::ifstream ifs(filename);
    if (ifs.good())
    {
      std::ostringstream oss;
      oss << ifs.rdbuf();
      Response resp;
      resp.data(Response::ICO, oss.str());
      return resp.data();
    }
  } 

  return Response::pageNotFound();
}

void Server::setTemplateFolder(const std::string& template_folder)
{
  m_template_folder = template_folder;
}

std::string Server::getTemplateFolder() const
{
  const std::string& root_path = Singleton<System>::getInstance()->getRootPath();
  return root_path + "/" + m_template_folder;
}

void Server::setStaticFolder(const std::string& static_folder)
{
  m_static_folder = static_folder;
}

std::string Server::getStaticFolder() const
{
  const std::string& root_path = Singleton<System>::getInstance()->getRootPath();
  return root_path + "/" + m_static_folder;
}
