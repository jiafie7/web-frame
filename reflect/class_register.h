#pragma once

#include "reflect/class_factory.h"

using namespace melon::reflect;

namespace melon
{
  namespace reflect
  {
    class ClassRegister
    {
      public:
        ClassRegister(const std::string& class_name, create_object func)
        {
          Singleton<ClassFactory>::getInstance()->register_class(class_name, func);
        }

        ClassRegister(const std::string& class_name, const std::string& field_name, const std::string& field_type, size_t offset)
        {
          Singleton<ClassFactory>::getInstance()->register_class_field(class_name, field_name, field_type, offset);
        }

        ClassRegister(const std::string& class_name, const std::string& method_name, uintptr_t method)
        {
          Singleton<ClassFactory>::getInstance()->register_class_method(class_name, method_name, method);
        }
    };

#define REGISTER_CLASS(className)                                               \
    Object* createObject##className()                                           \
    {                                                                           \
      Object* obj = new className();                                            \
      obj->set_class_name(#className);                                          \
      return obj;                                                               \
    }                                                                           \
    ClassRegister ClassRegister##className(#className, createObject##className)


#define REGISTER_CLASS_FIELD(className, fieldName, fieldType) \
    className className##fieldName;                           \
    ClassRegister ClassRegister##className##fieldName(#className, #fieldName, #fieldType, (size_t)(&(className##fieldName.fieldName)) - (size_t)(&(className##fieldName)))


#define REGISTER_CLASS_METHOD(className, methodName, returnType, ...) \
    std::function<returnType(className*, ##__VA_ARGS__)> className##methodName##method = &className::methodName; \
    ClassRegister ClassRegister##className##methodName(#className, #methodName, (uintptr_t)(&className##methodName##method))
  }
}
