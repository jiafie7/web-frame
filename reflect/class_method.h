#pragma once

#include <string>

namespace melon
{
  namespace reflect
  {
    class ClassMethod
    {
      public:
        ClassMethod()
          : m_method(0)
        {}

        ClassMethod(const std::string& name, uintptr_t method)
          : m_name(name)
          , m_method(method)
        {}
        
        ~ClassMethod() = default;

        const std::string& get_name() const { return m_name; }
        uintptr_t get_method() const { return m_method; }
      
      private:
        std::string m_name;
        uintptr_t m_method;
    };
  }
}
