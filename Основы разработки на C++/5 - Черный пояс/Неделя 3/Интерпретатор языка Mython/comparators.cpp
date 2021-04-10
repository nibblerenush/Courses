#include "comparators.h"
#include "object.h"
#include "object_holder.h"

#include <functional>
#include <optional>
#include <sstream>

using namespace std;

namespace Runtime {

bool Equal(ObjectHolder lhs, ObjectHolder rhs) {
  if (lhs.TryAs<Bool>() && rhs.TryAs<Bool>()) {
    return lhs.TryAs<Bool>()->GetValue() == rhs.TryAs<Bool>()->GetValue();
  }

  if (lhs.TryAs<Number>() && rhs.TryAs<Number>()) {
    return lhs.TryAs<Number>()->GetValue() == rhs.TryAs<Number>()->GetValue();
  }

  if (lhs.TryAs<String>() && rhs.TryAs<String>()) {
    return lhs.TryAs<String>()->GetValue() == rhs.TryAs<String>()->GetValue();
  }

  if (lhs.TryAs<Class>() && rhs.TryAs<Class>()) {
    return lhs.TryAs<Class>()->GetName() == rhs.TryAs<Class>()->GetName();
  }

  if (lhs.TryAs<ClassInstance>() && rhs.TryAs<ClassInstance>()) {
    std::ostringstream s_lhs;
    std::ostringstream s_rhs;
    lhs->Print(s_lhs);
    rhs->Print(s_rhs);
    return s_lhs.str() == s_rhs.str();
  }
  
  throw runtime_error("Equal");
}

bool Less(ObjectHolder lhs, ObjectHolder rhs) {
  if (lhs.TryAs<Number>() && rhs.TryAs<Number>()) {
    return lhs.TryAs<Number>()->GetValue() < rhs.TryAs<Number>()->GetValue();
  }
  
  if (lhs.TryAs<String>() && rhs.TryAs<String>()) {
    return lhs.TryAs<String>()->GetValue() < rhs.TryAs<String>()->GetValue();
  }
  
  throw runtime_error("Less");
}

} /* namespace Runtime */
