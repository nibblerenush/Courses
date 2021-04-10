#include "object.h"
#include "statement.h"

#include <sstream>
#include <string_view>

using namespace std;

namespace Runtime {
  
  void ClassInstance::Print(std::ostream& os) {
    const Method* str_method = m_class.GetMethod("__str__");
    if (str_method) {
      Closure str_method_args = {
        {"self", ObjectHolder::Share(*this)}
      };
      str_method->body->Execute(str_method_args)->Print(os);
    }
    else {
      os << this;
    }
  }

  bool ClassInstance::HasMethod(const std::string& method, size_t argument_count) const {
    const Method* find_method = m_class.GetMethod(method);
    if (find_method) {
      return find_method->formal_params.size() == argument_count;
    }
    else {
      return false;
    }
  }

  const Closure& ClassInstance::Fields() const {
    return m_fields;
  }

  Closure& ClassInstance::Fields() {
    return m_fields;
  }

  ClassInstance::ClassInstance(const Class& cls) : m_class(cls)
  {}

  ObjectHolder ClassInstance::Call(const std::string& method, const std::vector<ObjectHolder>& actual_args) {
    if (HasMethod(method, actual_args.size())) {
      Closure method_args = {
        {"self", ObjectHolder::Share(*this)}
      };
      
      std::size_t arg_index = 0;
      const Method* need_method = m_class.GetMethod(method);
      
      for (const auto& arg_name : need_method->formal_params) {
        method_args[arg_name] = actual_args[arg_index];
        arg_index++;
      }

      return need_method->body->Execute(method_args);
    }
    else {
      throw runtime_error("ClassInstance::Call");
    }
  }

  Class::Class(std::string name, std::vector<Method> methods, const Class* parent) : m_name(move(name)), m_methods(move(methods)) {
    if (parent) {
      m_vtable = parent->m_vtable;
    }

    for (const auto& method : m_methods) {
      m_vtable[method.name] = &method;
    }
  }

  const Method* Class::GetMethod(const std::string& name) const {
    if (m_vtable.count(name)) {
      return m_vtable.at(name);
    }
    else {
      return nullptr;
    }
  }

  void Class::Print(ostream& os) {
    os << m_name;
  }

  const std::string& Class::GetName() const {
    return m_name;
  }

  void Bool::Print(std::ostream& os) {
    if (GetValue()) {
      os << "True";
    }
    else {
      os << "False";
    }
  }

} /* namespace Runtime */
