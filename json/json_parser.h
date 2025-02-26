#pragma once 

#include <fstream>
#include <sstream>
#include <string>

#include "json/json.h"

namespace melon
{
  namespace json
  {
    class JsonParser
    {
      public:
        JsonParser() = default;
        ~JsonParser() = default;

        void load(const std::string& filename);
        void load(const char* buf, int len);
        Json parse();

      private:
        void skipWhiteSpace();
        char getNextChar();
        bool inRange(int x, int lower, int upper) const;

        Json parseNull();
        Json parseBool();
        Json parseNumber();
        std::string parseString();
        Json parseArray();
        Json parseObject();


      private:
        std::string m_content;
        int m_index;
    };
  }
}
