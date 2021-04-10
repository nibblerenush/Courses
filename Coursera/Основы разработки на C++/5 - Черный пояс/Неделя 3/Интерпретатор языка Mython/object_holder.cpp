#include "object_holder.h"
#include "object.h"

namespace Runtime {
  ObjectHolder ObjectHolder::Share(Object& object) {
    return ObjectHolder{ std::shared_ptr<Object>(&object, [](auto*) { /* do nothing */ }) };
  }

  ObjectHolder ObjectHolder::None() {
    return ObjectHolder();
  }

  Object& ObjectHolder::operator *() {
    return *Get();
  }

  const Object& ObjectHolder::operator *() const {
    return *Get();
  }

  Object* ObjectHolder::operator ->() {
    return Get();
  }

  const Object* ObjectHolder::operator ->() const {
    return Get();
  }

  Object* ObjectHolder::Get() {
    return m_data.get();
  }

  const Object* ObjectHolder::Get() const {
    return m_data.get();
  }

  ObjectHolder::operator bool() const {
    return Get();
  }

  bool IsTrue(ObjectHolder object) {
    if (object.TryAs<Bool>()) {
      return object.TryAs<Bool>()->GetValue();
    }

    if (object.TryAs<Number>()) {
      return object.TryAs<Number>()->GetValue() != 0;
    }

    if (object.TryAs<String>()) {
      return object.TryAs<String>()->GetValue().length() != 0;
    }

    return object.TryAs<Class>() || object.TryAs<ClassInstance>();
  }
}
