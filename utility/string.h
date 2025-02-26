#pragma once

#include <algorithm> 
#include <cctype>    
#include <cstdarg>  
#include <sstream>
#include <string>
#include <vector>

namespace melon
{
  namespace utility
  {
    class String
    {
      public:
        String() = default;
        ~String() = default;

        static std::string toLower(const std::string& input);
        static std::string toUpper(const std::string& input);

        static std::string leftTrim(const std::string& input, const std::string& trims);
        static std::string rightTrim(const std::string& input, const std::string& trims);
        static std::string trim(const std::string& input, const std::string& trims);

        static std::vector<std::string> split(const std::string& input, const std::string& separator);
        static std::vector<std::string> split(const std::string& input, char separator);

        static std::string join(const std::vector<std::string>& input, const std::string& separator);
        static std::string join(const std::vector<std::string>& input, char separator);

        static bool startsWith(const std::string& input, const std::string& prefix);
        static bool endsWith(const std::string& input, const std::string& suffix);

        static std::string capitalize(const std::string& input);
        static std::string format(const char* fmt, ...);

    };
  }
}
