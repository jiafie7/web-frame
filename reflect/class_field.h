#pragma once

#include <string>

namespace melon
{
  namespace reflect
  {
    class ClassField
    {
      public:
        ClassField()
          : m_offset(0)
        {}

        ClassField(const std::string& name, const std::string& type, size_t offset)
          : m_name(name)
          , m_type(type)
          , m_offset(offset)
        {}

        ~ClassField() = default;
   
        const std::string& get_name() const { return m_name; }
        const std::string& get_type() const { return m_type; }
        size_t get_offset() const { return m_offset; }
      
      private:
        std::string m_name;
        std::string m_type;
        size_t m_offset;
    };
  }
}
