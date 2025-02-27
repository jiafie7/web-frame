#include "web/request.h"
using namespace melon::web;

#include "utility/string.h"
#include "utility/log_system.h"
using namespace melon::utility;

int Request::parseHeader(const char* buf, int len)
{
  /*** parse http request line: <method> <uri> <protocol> ***/
  const char* s = buf;             // data start
  const char* e = buf + len - 1;   // data end
  const char* i = s;


  // skip ' ' before request method
  while ((i < e) && std::isspace(*i))
    ++ i;
  s = i;

  // parse request method
  while ((i < e) && (strchr(" ", *i) == nullptr))
    ++ i;
  m_method = std::string(s, 0, i - s);
        

  // skip ' ' before uri
  while ((i < e) && (strchr(" ", *i) != nullptr))
    ++ i;
  s = i;

  // parse uri
  while ((i < e) && (strchr(" ", *i) == nullptr))
    ++ i;
  m_uri = std::string(s, 0, i - s);


  // skip ' ' before protocol
  while ((i < e) && (strchr(" ", *i) != nullptr))
    ++ i;
  s = i;
  
  // parse protocol
  while ((i < e) && (strchr("\r\n", *i) == nullptr))
    ++ i;
  m_protocol = std::string(s, 0, i - s);
  

  // request line parsing ends, move to next line
  while ((i < e) && strchr("\r\n", *i) != nullptr)
    ++ i;
  s = i;

  
  // if request parameters exist
  std::size_t pos = m_uri.find_first_of('?');
  if (pos != std::string::npos)
  {
    // extract resource path and request parameters
    m_path = m_uri.substr(0, pos);
    m_query_string = m_uri.substr(pos + 1);

    // extract each key-value
    std::vector<std::string> kv_pairs = String::split(m_query_string, '&');
    for (auto it = kv_pairs.begin(); it != kv_pairs.end(); ++ it)
    {
      std::vector<std::string> kv_pair = String::split((*it), '=');
      m_get[kv_pair[0]] = kv_pair[1];
    }
  }
  else
  {
    m_path = m_uri;
  }


  /*** parse http request headers: <Header Field>: <Field Value> ***/
  while (i < e)
  {
    // find the position of ": "
    while ((i < e) && (strchr(": ", *i) == nullptr))
      ++ i;
    // parse header field
    std::string name = std::string(s, 0, i - s);


    // skip ": "
    while ((i < e) && (strchr(": ", *i) != nullptr))
      ++ i;
    s = i;

    // parse header field value
    while ((i < e) && (strchr("\r\n", *i) == nullptr))
      ++ i;
    std::string value = std::string(s, 0, i - s);

    m_headers[name] = value;


    // if two LINE BREAK，http request headers parsing ends
    if (strncmp(i, "\r\n\r\n", 4) == 0)
    {
      i += 4;
      break;
    }

    // current request header line parsing ends, move to next line
    while ((i < e) && (strchr("\r\n", *i) != nullptr))
      ++ i;
    s = i;
  }
  
  // return parse position (http request body start)
  return i - buf;
}
 
void Request::parseBody(const char* buf, int len)
{
  const std::string& content_type = header("Content-Type");
  if (content_type.find("application/json") != std::string::npos)
  {
    log_debug("body data=%s", buf);
    m_post.load(buf, len); 
  }
}

bool Request::isGet() const
{
  return m_method == "GET";
}

bool Request::isPost() const
{
  return m_method == "POST";
}

bool Request::isDelete() const
{
  return m_method == "DELETE";
}

std::string Request::get(const std::string& name) const
{
  auto it = m_get.find(name);
  if (it == m_get.end())
    return "";
  return it->second;
}

Json Request::post(const std::string& name) const
{
  if (name.empty())
    return m_post;
  return m_post.get(name);
}

std::string Request::header(const std::string& name) const
{
  auto it = m_headers.find(name);
  if (it == m_headers.end())
    return "";
  return it->second;
}

std::string Request::cookie(const std::string& name) const
{
  auto it = m_cookies.find(name);
  if (it == m_cookies.end())
    return "";
  return it->second;
}

std::string Request::path() const
{
  return m_path;
}

std::string Request::userAgent() const
{
  return header("User-Agent");
}

std::string Request::userHost() const
{
  return header("User-Host");
}
 
int Request::contentLength() const
{
  return std::atoi(header("Content-Length").c_str());
}
 
void Request::show() const
{
  log_debug("http request line --- start");
  log_debug("http method: %s", m_method.c_str());
  log_debug("http uri: %s", m_uri.c_str());
  log_debug("http protocol: %s", m_protocol.c_str());
  log_debug("http path: %s", m_path.c_str());
  log_debug("http query string: %s", m_query_string.c_str());
  log_debug("http request line --- end");

  log_debug("http request headers --- start");
  for (auto it = m_headers.begin(); it != m_headers.end(); ++ it)
    log_debug("http header: %s=%s", it->first.c_str(), it->second.c_str());
  log_debug("http request headers --- end");  

  log_debug("http get params --- start");
  for (auto it = m_get.begin(); it != m_get.end(); ++ it)
    log_debug("http get: %s=%s", it->first.c_str(), it->second.c_str());
  log_debug("http get params --- end");

  log_debug("http post params --- start");
  log_debug("http post: %s", m_post.toString().c_str());
  log_debug("http post params --- end");

  log_debug("http body: %s", m_body.c_str());
}
