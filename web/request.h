#pragma once

#include <cstring>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "json/json.h"
using namespace melon::json;

namespace melon
{
  namespace web
  {
    class Request
    {
    public:
      Request() = default;
      ~Request() = default;

      int parseHeader(const char* buf, int len);
      void parseBody(const char* buf, int len);

       // return request method type
      bool isGet() const;
      bool isPost() const;
      bool isDelete() const;

      // return request parameters
      std::string get(const std::string& name) const;
      Json post(const std::string& name = "") const;

      std::string header(const std::string& name) const;
      std::string cookie(const std::string& name) const;
      std::string path() const;
      std::string userAgent() const;
      std::string userHost() const;
      int contentLength() const;

      void show() const;

    private:
      std::string m_method; // request method
      std::string m_uri;    // uri
      std::string m_protocol;  // http protocol

      std::string m_path;    // request resource path
      std::string m_query_string;  //  request parameters
                                   
      std::map<std::string, std::string> m_get;  // key-value parameters of get request
      Json m_post;  // parameters of post request

      std::map<std::string, std::string> m_headers; // http request headers
      std::map<std::string, std::string> m_cookies; // Cookie

      std::string m_body;   // request body
    };
  }
}
