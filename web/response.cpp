#include "web/response.h"
#include "web/server.h"
using namespace melon::web;

Response::Response()
  : m_code(200)
{
}

Response::~Response()
{
}

void Response::code(int code)
{
  m_code = code;
}

void Response::data(Type type, const std::string& data)
{
  m_type = type;
  m_data = data;
}

void Response::html(const std::string& data)
{
  m_type = HTML;
  m_data = data;
}

void Response::json(const std::string& data)
{
  m_type = JSON;
  m_data = data;
}

std::string Response::data() const
{
  std::ostringstream oss;
  // status line
  oss << "HTTP/1.1 " << m_code << " OK\r\n";

  // response headers
  switch (m_type)
  {
    case HTML:
      oss << "Content-Type: text/html; charset: utf-8\r\n";
      break;
    case JSON:
      oss << "Content-Type: application/json; charset: utf-8\r\n";
      break;
    case JS:
      oss << "Content-Type: text/javascript; charset: utf-8\r\n";
      break;
    case CSS:
      oss << "Content-Type: text/css; charset: utf-8\r\n";
      break;
    case JPG:
      oss << "Content-Type: image/jpeg; charset: utf-8\r\n";
      break;
    case PNG:
      oss << "Content-Type: image/png; charset: utf-8\r\n";
      break;
    case GIF:
      oss << "Content-Type: image/gif; charset: utf-8\r\n";
      break;
    case ICO:
      oss << "Content-Type: image/x-icon; charset: utf-8\r\n";
      break;
    default:
      break;
  }
  oss << "Content-Length: " << m_data.size() << "\r\n\r\n";

  // response body
  oss << m_data << "\r\n";

  return oss.str();
}
 
std::string Response::pageNotFound()
{
  std::string data = "  \
<!DOCTYPE html>         \
<html>                  \
    <head>              \
        <meta charset=\"utf-8\">    \
        <title>web-frame</title>     \
    </head>             \
    <body>              \
        <h1 style=\"text-align: center;\">404 Page Not Found</h1>   \
        <p style=\"text-align: center;\">Request Not Matched</p>    \
    </body>             \
</html>";

  std::ostringstream oss;

  // status line
  oss << "HTTP/1.1 404 Not Found\r\n";

  // response headers
  oss << "Content-Type: text/html; charset: utf-8\r\n";
  oss << "Content-Length: " << data.size() << "\r\n\r\n";

  // response body
  oss << data << "\r\n";

  return oss.str();
}

void Response::render(const std::string& filename)
{
  // get file path "root_path/template/filename"
  const std::string& template_folder = Singleton<Server>::getInstance()->getTemplateFolder();
  const std::string& file = template_folder + "/" + filename;

  // read file content
  std::ifstream ifs(file);
  if (ifs.fail())
    return;

  std::ostringstream oss;
  oss << ifs.rdbuf();

  m_type = HTML;
  m_data = oss.str();
}
