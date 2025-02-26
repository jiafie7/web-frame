#pragma once

#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace melon
{
  namespace json
  {
    class Json
    {
      public:
        enum DataType
        {
          json_null = 0,
          json_bool,
          json_int,
          json_double,
          json_string,
          json_array,
          json_object,
        };

        Json();
        Json(DataType type);
        Json(bool value);
        Json(int value);
        Json(double value);
        Json(const char* value);
        Json(const std::string& value);
        Json(const Json& other);
        Json(Json&& other);
        ~Json();

        Json& operator=(bool value);
        Json& operator=(int value);
        Json& operator=(double value);
        Json& operator=(const char* value);
        Json& operator=(const std::string& value);
        Json& operator=(const Json& other);
        Json& operator=(Json&& other) noexcept;

        friend std::ostream& operator<<(std::ostream& os, const Json& other)
        {
          os << other.toString();
          return os;
        }

        operator bool();
        operator bool() const;
        operator int();
        operator int() const;
        operator double();
        operator double() const;
        operator std::string();
        operator std::string() const;

        DataType getType() const;
        bool isNull() const;
        bool isBool() const;
        bool isInt() const;
        bool isDouble() const;
        bool isString() const;
        bool isArray() const;
        bool isObject() const;

        // array manipulation
        void append(const Json& value);
        bool has(int index);
        bool has(int index) const;
        Json get(int index);
        Json get(int index) const;
        void remove(int index);
        Json& operator[](int index);
        
        typedef std::vector<Json>::iterator array_iterator;
        array_iterator arrayBegin()
        {
          return (m_value.m_array)->begin();
        }
        array_iterator arrayEnd()
        {
          return (m_value.m_array)->end();
        }

        // object manipulation
        bool has(const char* key);
        bool has(const char* key) const;
        bool has(const std::string& key);
        bool has(const std::string& key) const;
        Json get(const char* key);
        Json get(const char* key) const;
        Json get(const std::string& key);
        Json get(const std::string& key) const;
        void remove(const char* key);
        void remove(const std::string& key);
        Json& operator[](const char* key);
        Json& operator[](const std::string& key);

        typedef std::map<std::string, Json>::iterator object_iterator;
        object_iterator objectBegin()
        {
          return (m_value.m_object)->begin();
        }
        object_iterator objectEnd()
        {
          return (m_value.m_object)->end();
        }

        int size() const;
        bool empty() const;
        std::string toString(int indent_level = 0) const; 
        
        void load(const std::string& filename);
        void load(const char* buf, int len);

      private:
        void clear();
        void copy(const Json& other);
        void swap(Json& other);

      private:
        union Value
        {
          bool m_bool;
          int m_int;
          double m_double;
          std::string* m_string;
          std::vector<Json>* m_array;
          std::map<std::string, Json>* m_object;
        };

        DataType m_type;
        Value m_value;
    };
  }
}
