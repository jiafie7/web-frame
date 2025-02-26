#include "utility/string.h"

using namespace melon::utility;

std::string String::toLower(const std::string& input)
{
  std::string str = input;
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  return str;
}
        
std::string String::toUpper(const std::string& input)
{
  std::string str = input;
  std::transform(str.begin(), str.end(), str.begin(), ::toupper);
  return str;
}

std::string String::leftTrim(const std::string& input, const std::string& trims)
{
  std::string str = input;
  std::size_t pos = str.find_first_not_of(trims);
  if (pos != std::string::npos)
    str.erase(0, pos);
  else
    str.clear();
  return str;
}
        
std::string String::rightTrim(const std::string& input, const std::string& trims)
{
  std::string str = input;
  std::size_t pos = str.find_last_not_of(trims);
  if (pos != std::string::npos)
    str.erase(pos + 1);
  else 
    str.clear();
  return str;
}
        
std::string String::trim(const std::string& input, const std::string& trims)
{
  std::string str = leftTrim(input, trims);
  return rightTrim(str, trims);
}

std::vector<std::string> String::split(const std::string& input, const std::string& separator)
{
  std::vector<std::string> output;
  size_t last = 0;
  size_t index = input.find_first_of(separator, last);
  while (index != std::string::npos)
  {
    std::string str = input.substr(last, index - last);
    output.push_back(str);
    last = index + 1;
    index = input.find_first_of(separator, last);
  }
  if (index - last > 0)
    output.push_back(input.substr(last , index - last));
  return output;
}
        
std::vector<std::string> String::split(const std::string& input, char separator)
{
  return split(input, std::string(1, separator));
}

std::string String::join(const std::vector<std::string>& input, const std::string& separator)
{
  std::ostringstream oss;
  for (auto it = input.begin(); it != input.end(); ++ it)
  {
    if (it != input.begin())
      oss << separator;
    oss << *it;
  }
  return oss.str();
}

std::string String::join(const std::vector<std::string>& input, char separator)
{
  return join(input, std::string(1, separator));
}

bool String::startsWith(const std::string& input, const std::string& prefix)
{
  if (input.size() < prefix.size())
    return false;
  return input.substr(0, prefix.size()) == prefix;
}

bool String::endsWith(const std::string& input, const std::string& suffix)
{
  if (input.size() < suffix.size())
    return false;
  return input.substr(input.size() - suffix.size()) == suffix;
}
 
std::string String::capitalize(const std::string& input)
{
  std::string str = input;
  if (str.empty())
    return str;

  char ch = str[0];
  if (std::islower(ch))
  {
    ch = std::toupper(ch);
    std::replace(str.begin(), str.begin() + 1, str[0], ch);
  }
  return str;
}

std::string String::format(const char* fmt, ...)
{
  std::string str;
  va_list arg_ptr;
  va_start(arg_ptr, fmt);
  int len = vsnprintf(nullptr, 0, fmt, arg_ptr);
  va_end(arg_ptr);

  if (len > 0)
  {
    char* buffer = new char[len + 1];
    va_start(arg_ptr, fmt);
    vsnprintf(buffer, len + 1, fmt, arg_ptr);
    va_end(arg_ptr);
    buffer[len] = '\0';
    str = buffer;
    delete[] buffer;
  }

  return str;
}


