#include <iostream>
#include "reflect/class_factory.h"

using namespace melon::reflect;
        
void Object::set_class_name(const std::string& class_name)
{
  m_class_name = class_name;
}

const std::string& Object::get_class_name() const
{
  return m_class_name;
}
 
int Object::get_field_count()
{
  return Singleton<ClassFactory>::getInstance()->get_class_field_count(get_class_name());
}

ClassField* Object::get_field(int index)
{
  return Singleton<ClassFactory>::getInstance()->get_class_field(get_class_name(), index);
}

ClassField* Object::get_field(const std::string& field_name)
{
  return Singleton<ClassFactory>::getInstance()->get_class_field(get_class_name(), field_name);
}

       
void ClassFactory::register_class(const std::string& class_name, create_object func)
{
  m_class[class_name] = func;
}

Object* ClassFactory::create_class(const std::string& class_name)
{
  auto it = m_class.find(class_name);
  if (it == m_class.end())
    return nullptr;
  std::cout << class_name << '\n';
  return it->second();
}

void ClassFactory::register_class_field(const std::string& class_name, const std::string& field_name, const std::string& field_type, size_t offset)
{
  m_class_fields[class_name].push_back(new ClassField(field_name, field_type, offset));
}

int ClassFactory::get_class_field_count(const std::string& class_name)
{
  return (int)m_class_fields[class_name].size();
}

ClassField* ClassFactory::get_class_field(const std::string& class_name, int index)
{
  int size = (int)m_class_fields[class_name].size();
  if (index < 0 || index >= size)
    return nullptr;
  return m_class_fields[class_name][index];
}

ClassField* ClassFactory::get_class_field(const std::string& class_name, const std::string& field_name)
{
  auto fields = m_class_fields[class_name];
  for (auto it = fields.begin(); it != fields.end(); ++ it)
    if ((*it)->get_name() == field_name)
      return *it;
  return nullptr;
}

void ClassFactory::register_class_method(const std::string& class_name, const std::string& method_name, uintptr_t method)
{
  m_class_methods[class_name].push_back(new ClassMethod(method_name, method)); 
}

int ClassFactory::get_class_method_count(const std::string& class_name)
{
  return (int)m_class_methods[class_name].size();
}

ClassMethod* ClassFactory::get_class_method(const std::string& class_name, int index)
{
  int size = (int)m_class_methods[class_name].size();
  if (index < 0 || index >= size)
    return nullptr;
  return m_class_methods[class_name][index];
}

ClassMethod* ClassFactory::get_class_method(const std::string& class_name, const std::string& method_name)
{
  for (auto it = m_class_methods[class_name].begin(); it != m_class_methods[class_name].end(); ++ it)
    if ((*it)->get_name() == method_name)
      return *it;
  return nullptr;
}


