#include "json/json.h"
#include "json/json_parser.h"
using namespace melon::json;

Json::Json()
  : m_type(json_null)
{
}

Json::Json(DataType type)
  : m_type(type)
{
  switch (type)
  {
    case json_null:
      break;
    case json_bool:
      m_value.m_bool = false;
      break;
    case json_int:
      m_value.m_int = 0;
      break;
    case json_double:
      m_value.m_double = 0.0;
      break;
    case json_string:
      m_value.m_string = new std::string("");
      break;
    case json_array:
      m_value.m_array = new std::vector<Json>();
      break;
    case json_object:
      m_value.m_object = new std::map<std::string, Json>();
      break;
    default:
      break;
  }
}

Json::Json(bool value)
  : m_type(json_bool)
{
  m_value.m_bool = value;
}

Json::Json(int value)
  : m_type(json_int)
{
  m_value.m_int = value;
}

Json::Json(double value)
  : m_type(json_double)
{
  m_value.m_double = value;
}

Json::Json(const char* value)
  : m_type(json_string)
{
  m_value.m_string = new std::string(value);
}

Json::Json(const std::string& value)
  : m_type(json_string)
{
  m_value.m_string = new std::string(value);
}

Json::Json(const Json& other)
  : m_type(json_null)
{
  copy(other); 
}

Json::Json(Json&& other)
{
  swap(other);
}

Json::~Json()
{
  clear();
}

Json& Json::operator=(bool value)
{
  clear();
  m_type = json_bool;
  m_value.m_bool = value;
  return *this;
}
     
Json& Json::operator=(int value)
{
  clear();
  m_type = json_int;
  m_value.m_int = value;
  return *this;
}

Json& Json::operator=(double value)
{
  clear();
  m_type = json_double;
  m_value.m_double = value;
  return *this;
}

Json& Json::operator=(const char* value)
{
  clear();
  m_type = json_string;
  m_value.m_string = new std::string(value);
  return *this;
}

Json& Json::operator=(const std::string& value)
{
  clear();
  m_type = json_string;
  m_value.m_string = new std::string(value);
  return *this;
}

Json& Json::operator=(const Json& other)
{
  clear();
  copy(other);
  return *this;
}

Json& Json::operator=(Json&& other) noexcept
{
  swap(other);
  return *this;
}

std::string Json::toString(int indent_level) const 
{
  std::stringstream ss;
  std::string indent(indent_level * 2, ' '); // 每层缩进两个空格

  switch (m_type) 
  {
    case json_null:
      ss << "null";
      break;
    case json_bool:
      ss << (m_value.m_bool ? "true" : "false");
      break;
    case json_int:
      ss << m_value.m_int;
      break;
    case json_double:
      ss << m_value.m_double;
      break;
    case json_string:
      ss << "\"" << *(m_value.m_string) << "\"";
      break;
    case json_array: 
    {
      ss << "[\n";
      for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); ++it) 
      {
        if (it != (m_value.m_array)->begin())
          ss << ",\n";
        ss << std::string((indent_level + 1) * 2, ' ') << (*it).toString(indent_level + 1);
      }
      ss << "\n" << indent << "]";
      break;
    }
    case json_object: 
    {
      ss << "{\n";
      for (auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); ++it) 
      {
        if (it != (m_value.m_object)->begin())
          ss << ",\n";
        ss << std::string((indent_level + 1) * 2, ' ') << "\"" << it->first << "\": " << it->second.toString(indent_level + 1);
      }
      ss << "\n" << indent << "}";
      break;
    }
    default:
      break;
    }

  return ss.str();
}

void Json::clear()
{
  switch (m_type)
  {
    case json_null:
    case json_bool:
    case json_int:
    case json_double:
      break;
    case json_string:
      if (m_value.m_string != nullptr)
      {
        delete m_value.m_string;
        m_value.m_string = nullptr;
      }
      break;
    case json_array:
      if (m_value.m_array != nullptr)
      {
        for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); ++ it)
          it->clear();
        delete m_value.m_array;
        m_value.m_array = nullptr;
      }
      break;
    case json_object:
      if (m_value.m_object != nullptr)
      {
        for (auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); ++ it)
          it->second.clear();
        delete m_value.m_object;
        m_value.m_object = nullptr;
      }
      break;
    default: break;
  }
  m_type = json_null;
}

void Json::copy(const Json& other)
{
  // clear();
  m_type = other.m_type;

  switch (m_type)
  {
    case json_null:
    case json_bool:
    case json_int:
    case json_double:
      m_value = other.m_value;
      break;
    case json_string:
      if (other.m_value.m_string != nullptr)
        m_value.m_string = new std::string(*(other.m_value.m_string));
      break;
    case json_array:
      if (other.m_value.m_array != nullptr)
      { 
        m_value.m_array = new std::vector<Json>();
        for (auto it = (other.m_value.m_array)->begin(); it != (other.m_value.m_array)->end(); ++ it)
          (m_value.m_array)->push_back(*it);
      }
      break;
    case json_object:
      if (other.m_value.m_object != nullptr)
      {
        m_value.m_object = new std::map<std::string, Json>();
        for (auto it = (other.m_value.m_object)->begin(); it != (other.m_value.m_object)->end(); ++ it)
          (*(m_value.m_object))[it->first] = it->second;
      }
      break;
    default: break;
  }
}

        
Json::operator bool()
{
  if (m_type != json_bool)
    throw std::logic_error("Type error, not bool.");
  return m_value.m_bool;
}

Json::operator bool() const
{
  if (m_type != json_bool)
    throw std::logic_error("Type error, not bool.");
  return m_value.m_bool;
}

Json::operator int()
{
  if (m_type != json_int)
    throw std::logic_error("Type error, not int.");
  return m_value.m_int;
}

Json::operator int() const
{
  if (m_type != json_int)
    throw std::logic_error("Type error, not int.");
  return m_value.m_int;
}

Json::operator double()
{
  if (m_type != json_double)
    throw std::logic_error("Type error, not double.");
  return m_value.m_double;
}

Json::operator double() const
{
  if (m_type != json_double)
    throw std::logic_error("Type error, not double.");
  return m_value.m_double;
}

Json::operator std::string()
{
  if (m_type != json_string)
    throw std::logic_error("Type error, not string.");
  return *(m_value.m_string);
}

Json::operator std::string() const
{
  if (m_type != json_string)
    throw std::logic_error("Type error, not string.");
  return *(m_value.m_string);
}

Json::DataType Json::getType() const
{
  return m_type;
}

bool Json::isNull() const
{
  return m_type == json_null;
}

bool Json::isBool() const
{
  return m_type == json_bool;
}

bool Json::isInt() const
{
  return m_type == json_int;
}

bool Json::isDouble() const
{
  return m_type == json_double;
}

bool Json::isString() const
{
  return m_type == json_string;
}

bool Json::isArray() const
{
  return m_type == json_array;
}

bool Json::isObject() const
{
  return m_type == json_object;
}

void Json::append(const Json& value)
{
  if (m_type != json_array)
  {
    clear();
    m_type = json_array;
    m_value.m_array = new std::vector<Json>();
  }
  (m_value.m_array)->push_back(value);
}
    
bool Json::has(int index)
{
  if (m_type != json_array)
    return false;
  return index >= 0 && index < (int)(m_value.m_array)->size();
}

bool Json::has(int index) const
{
  if (m_type != json_array)
    return false;
  return index >= 0 && index < (int)(m_value.m_array)->size();
}

Json Json::get(int index)
{
  if (!has(index))
    return Json();
  return (m_value.m_array)->at(index);
}

Json Json::get(int index) const
{
  if (!has(index))
    return Json();
  return (m_value.m_array)->at(index);
}

void Json::remove(int index)
{
  if (m_type != json_array)
    throw std::logic_error("Type error: not array.");
  (m_value.m_array)->at(index).clear();
  (m_value.m_array)->erase((m_value.m_array)->begin() + index);
}

Json& Json::operator[](int index)
{
  if (m_type != json_array)
    throw std::logic_error("Type error: not array.");
  if (!has(index))
    throw std::logic_error("Array out of range.");
  return (m_value.m_array)->at(index);
}

bool Json::has(const char* key)
{
  if (m_type != json_object)
    return false;
  return (m_value.m_object)->find(key) != (m_value.m_object)->end();
}

bool Json::has(const char* key) const
{
  if (m_type != json_object)
    return false;
  return (m_value.m_object)->find(key) != (m_value.m_object)->end();
}
        
bool Json::has(const std::string& key)
{
  return has(key.c_str());
}

bool Json::has(const std::string& key) const
{
  return has(key.c_str());
}

Json Json::get(const char* key)
{
  if (!has(key))
    return Json();
  return (*(m_value.m_object))[key];
}

Json Json::get(const char* key) const
{
  if (!has(key))
    return Json();
  return (*(m_value.m_object))[key];
}

Json Json::get(const std::string& key)
{
  return get(key.c_str());
}

Json Json::get(const std::string& key) const
{
  return get(key.c_str());
}

void Json::remove(const char* key)
{
  if (m_type != json_object)
    return;

  auto it = (m_value.m_object)->find(key);
  if (it == (m_value.m_object)->end())
    return;
  it->second.clear();
  (m_value.m_object)->erase(it);
}

void Json::remove(const std::string& key)
{
  remove(key.c_str());
}

Json& Json::operator[](const char* key)
{
  if (m_type != json_object)
  {
    clear();
    m_type = json_object;
    m_value.m_object = new std::map<std::string, Json>();
  }
  return (*(m_value.m_object))[key];
}

Json& Json::operator[](const std::string& key)
{
  return (*this)[key.c_str()];
}

int Json::size() const
{
  switch (m_type)
  {
    case json_array:
      return (m_value.m_array)->size();
    case json_object:
      return (m_value.m_object)->size();
    default: break;
  }
  return -1;
}
        
bool Json::empty() const
{
  switch (m_type)
  {
    case json_null:
      return true;
    case json_array:
      return (m_value.m_array)->empty();
    case json_object:
      return (m_value.m_object)->empty();
    default: break;
  }
  return false;
}

void Json::load(const std::string& filename)
{
  JsonParser parser;
  parser.load(filename);
  *this = parser.parse();
}
        
void Json::load(const char* buf, int len)
{
  JsonParser parser;
  parser.load(buf, len);
  *this = parser.parse();
}

void Json::swap(Json& other)
{
  DataType type = other.m_type;
  Value value = other.m_value;

  other.m_type = m_type;
  other.m_value = m_value;

  m_type = type;
  m_value = value;
}

