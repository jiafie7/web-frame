#pragma once

#include <fstream>
#include <sstream>
#include <string>

namespace melon
{
  namespace web
  {
    class Response
    {
    public:
      enum Type
      {
        HTML = 0,
        JSON,
        JS,
        CSS,
        JPG,
        PNG,
        GIF,
        ICO
      };

      Response();
      ~Response();

      void code(int code);
      void data(Type type, const std::string& data);
      void html(const std::string& data);
      void json(const std::string& data);
      
      std::string data() const;

      static std::string pageNotFound();

      void render(const std::string& filename);

    private:
      int m_code;
      Type m_type;
      std::string m_data;
    };
  }
}
