#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "reflect/class_field.h"
#include "reflect/class_method.h"
#include "utility/singleton.h"

using namespace melon::utility;

namespace melon
{
  namespace reflect
  {
    class Object
    {
      public:
        Object() = default;
        virtual ~Object() = default;

        void set_class_name(const std::string& class_name);
        const std::string& get_class_name() const;

        int get_field_count();
        ClassField* get_field(int index);
        ClassField* get_field(const std::string& field_name);

        template <typename T>
        void get_field_value(const std::string& field_name, T& value);

        template <typename T>
        void set_field_value(const std::string& field_name, const T& value);

        template <typename R = void, typename ...Args>
        R call(const std::string& method_name, Args&&... args);

      private:
        std::string m_class_name;
    };

    typedef Object* (*create_object)();

    class ClassFactory
    {
      SINGLETON(ClassFactory);
      public:
        void register_class(const std::string& class_name, create_object func);
        Object* create_class(const std::string& class_name);

        void register_class_field(const std::string& class_name, const std::string& field_name, const std::string& field_type, size_t offset);
        int get_class_field_count(const std::string& class_name);
        ClassField* get_class_field(const std::string& class_name, int index);
        ClassField* get_class_field(const std::string& class_name, const std::string& field_name);

        void register_class_method(const std::string& class_name, const std::string& method_name, uintptr_t method);
        int get_class_method_count(const std::string& class_name);
        ClassMethod* get_class_method(const std::string& class_name, int index);
        ClassMethod* get_class_method(const std::string& class_name, const std::string& method_name);

      private:
        std::map<std::string, create_object> m_class;
        std::map<std::string, std::vector<ClassField*>> m_class_fields;
        std::map<std::string, std::vector<ClassMethod*>> m_class_methods;
    };

    template <typename T>
    void Object::get_field_value(const std::string& field_name, T& value)
    {
      ClassField* field = Singleton<ClassFactory>::getInstance()->get_class_field(m_class_name, field_name);
      if (field == nullptr)
        return;

      size_t offset = field->get_offset();
      value = *((T*)((unsigned char*)(this) + offset));
    }

    template <typename T>
    void Object::set_field_value(const std::string& field_name, const T& value)
    {
      ClassField* field = Singleton<ClassFactory>::getInstance()->get_class_field(m_class_name, field_name);
      if (field == nullptr)
        return;

      size_t offset = field->get_offset();
      *((T*)((unsigned char*)(this) + offset)) = value;
    }

    template <typename R, typename ...Args>
    R Object::call(const std::string& method_name, Args&&... args)
    {
      ClassFactory* factory = Singleton<ClassFactory>::getInstance();
      ClassMethod* method = factory->get_class_method(m_class_name, method_name);
      if (method == nullptr)
        return R();
      uintptr_t func = method->get_method();
      typedef std::function<R(decltype(this), Args...)> class_method;
      return (*((class_method*)(func)))(this, args...);
    }   

  }
}
