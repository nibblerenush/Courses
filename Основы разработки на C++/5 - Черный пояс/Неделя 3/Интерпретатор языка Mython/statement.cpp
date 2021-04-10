#include "statement.h"
#include "object.h"

#include <iostream>
#include <sstream>

using namespace std;

namespace Ast {

using Runtime::Closure;

Assignment::Assignment(std::string var, std::unique_ptr<Statement> rv) :
  var_name(move(var)), right_value(move(rv))
{}

ObjectHolder Assignment::Execute(Closure& closure) {
  closure[var_name] = right_value->Execute(closure);
  return closure[var_name];
}

VariableValue::VariableValue(std::string var_name) {
  dotted_ids.push_back(move(var_name));
}

VariableValue::VariableValue(std::vector<std::string> dotted_ids_) : dotted_ids(move(dotted_ids_))
{}

ObjectHolder VariableValue::Execute(Closure& closure) {
  if (dotted_ids.size() == 1) {
    if (closure.find(dotted_ids[0]) == closure.end()) {
      throw runtime_error("VariableValue::Execute");
    }
    return closure[dotted_ids[0]];
  }
  
  std::size_t index = 0;
  Closure& temp = closure;
  while (index + 1 != dotted_ids.size()) {
    if (temp.find(dotted_ids[index]) == temp.end()) {
      throw runtime_error("VariableValue::Execute");
    }
    
    temp = temp[dotted_ids[index]].TryAs<Runtime::ClassInstance>()->Fields();
    index++;
  }
  
  return temp[dotted_ids[index]];
}

unique_ptr<Print> Print::Variable(std::string var) {
  return make_unique<Print>(make_unique<VariableValue>(move(var)));
}

Print::Print(unique_ptr<Statement> argument) {
  args.push_back(move(argument));
}

Print::Print(vector<unique_ptr<Statement>> args_) : args(move(args_))
{}

ObjectHolder Print::Execute(Closure& closure)  {
  bool first = true;
  for (auto& arg : args) {
    if (first) {
      first = false;
    }
    else {
      *output << ' ';
    }
    ObjectHolder arg_result = arg->Execute(closure);
    if (arg_result) {
      arg_result->Print(*output);
    }
    else {
      *output << "None";
    }
  }
  *output << '\n';
  return ObjectHolder::None();
}

ostream* Print::output = &cout;

void Print::SetOutputStream(ostream& output_stream) {
  output = &output_stream;
}

MethodCall::MethodCall(std::unique_ptr<Statement> object_, std::string method_, std::vector<std::unique_ptr<Statement>> args_) :
  object(std::move(object_)), method(std::move(method_)), args(move(args_))
{}

ObjectHolder MethodCall::Execute(Closure& closure) {
  ObjectHolder object_oh = object->Execute(closure);

  if (object_oh.TryAs<Runtime::ClassInstance>() && object_oh.TryAs<Runtime::ClassInstance>()->HasMethod(method, args.size())) {
    std::vector<ObjectHolder> method_args;
    for (auto& arg : args) {
      method_args.push_back(arg->Execute(closure));
    }
    return object_oh.TryAs<Runtime::ClassInstance>()->Call(method, method_args);
  }
  else {
    throw runtime_error("MethodCall::MethodCall");
  }
}

ObjectHolder Stringify::Execute(Closure& closure) {
  ostringstream oss;
  argument->Execute(closure)->Print(oss);
  return ObjectHolder::Own(Runtime::String(oss.str()));
}

ObjectHolder ExecBinaryOpOnClass(const string& op, ObjectHolder lhs, ObjectHolder rhs)
{
  if (lhs.TryAs<Runtime::ClassInstance>()->HasMethod(op, 1)) {
    return lhs.TryAs<Runtime::ClassInstance>()->Call(op, { rhs });
  }
  return ObjectHolder::None();
}

ObjectHolder Add::Execute(Closure& closure) {
  ObjectHolder lhs_oh = lhs->Execute(closure);
  ObjectHolder rhs_oh = rhs->Execute(closure);
  
  if (lhs_oh.TryAs<Runtime::Number>() && rhs_oh.TryAs<Runtime::Number>()) {
    Runtime::Number result = lhs_oh.TryAs<Runtime::Number>()->GetValue() + rhs_oh.TryAs<Runtime::Number>()->GetValue();
    return ObjectHolder::Own(move(result));
  }

  if (lhs_oh.TryAs<Runtime::String>() && rhs_oh.TryAs<Runtime::String>()) {
    Runtime::String result = lhs_oh.TryAs<Runtime::String>()->GetValue() + rhs_oh.TryAs<Runtime::String>()->GetValue();
    return ObjectHolder::Own(move(result));
  }
  
  if (lhs_oh.TryAs<Runtime::ClassInstance>()) {
    ObjectHolder result = ExecBinaryOpOnClass("__add__", lhs_oh, rhs_oh);
    if (!result) {
      throw runtime_error("Add::Execute");
    }
    return result;
  }
  
  throw runtime_error("Add::Execute");
}

ObjectHolder Sub::Execute(Closure& closure) {
  ObjectHolder lhs_oh = lhs->Execute(closure);
  ObjectHolder rhs_oh = rhs->Execute(closure);

  if (lhs_oh.TryAs<Runtime::Number>() && rhs_oh.TryAs<Runtime::Number>()) {
    Runtime::Number result = lhs_oh.TryAs<Runtime::Number>()->GetValue() - rhs_oh.TryAs<Runtime::Number>()->GetValue();
    return ObjectHolder::Own(move(result));
  }

  if (lhs_oh.TryAs<Runtime::ClassInstance>()) {
    ObjectHolder result = ExecBinaryOpOnClass("__sub__", lhs_oh, rhs_oh);
    if (!result) {
      throw runtime_error("Sub::Execute");
    }
    return result;
  }

  throw runtime_error("Sub::Execute");
}

ObjectHolder Mult::Execute(Runtime::Closure& closure) {
  ObjectHolder lhs_oh = lhs->Execute(closure);
  ObjectHolder rhs_oh = rhs->Execute(closure);

  if (lhs_oh.TryAs<Runtime::Number>() && rhs_oh.TryAs<Runtime::Number>()) {
    Runtime::Number result = lhs_oh.TryAs<Runtime::Number>()->GetValue() * rhs_oh.TryAs<Runtime::Number>()->GetValue();
    return ObjectHolder::Own(move(result));
  }

  if (lhs_oh.TryAs<Runtime::ClassInstance>()) {
    ObjectHolder result = ExecBinaryOpOnClass("__mult__", lhs_oh, rhs_oh);
    if (!result) {
      throw runtime_error("Mult::Execute");
    }
    return result;
  }

  throw runtime_error("Mult::Execute");
}

ObjectHolder Div::Execute(Runtime::Closure& closure) {
  ObjectHolder lhs_oh = lhs->Execute(closure);
  ObjectHolder rhs_oh = rhs->Execute(closure);

  if (lhs_oh.TryAs<Runtime::Number>() && rhs_oh.TryAs<Runtime::Number>()) {
    if (rhs_oh.TryAs<Runtime::Number>()->GetValue() == 0) {
      throw std::runtime_error("Division of zero");
    }
    Runtime::Number result = lhs_oh.TryAs<Runtime::Number>()->GetValue() / rhs_oh.TryAs<Runtime::Number>()->GetValue();
    return ObjectHolder::Own(move(result));
  }

  if (lhs_oh.TryAs<Runtime::ClassInstance>()) {
    ObjectHolder result = ExecBinaryOpOnClass("__div__", lhs_oh, rhs_oh);
    if (!result) {
      throw runtime_error("Div::Execute");
    }
    return result;
  }

  throw runtime_error("Div::Execute");
}

ObjectHolder Compound::Execute(Closure& closure) {
  for (auto& stmt : statements) {
    if (dynamic_cast<Return*>(stmt.get())) {
      return stmt->Execute(closure);
    }
    else if (dynamic_cast<MethodCall*>(stmt.get()) || dynamic_cast<IfElse*>(stmt.get())) {
      ObjectHolder result = stmt->Execute(closure);
      if (result) {
        return result;
      }
    }
    else {
      stmt->Execute(closure);
    }
  }
  return ObjectHolder::None();
}

ObjectHolder Return::Execute(Closure& closure) {
  return statement->Execute(closure);
}

ClassDefinition::ClassDefinition(ObjectHolder class_) :
  cls(move(class_)), class_name(cls.TryAs<Runtime::Class>()->GetName())
{}

ObjectHolder ClassDefinition::Execute(Runtime::Closure& closure) {
  closure[class_name] = cls;
  return closure[class_name];
}

FieldAssignment::FieldAssignment(VariableValue object_, std::string field_name_, std::unique_ptr<Statement> rv) :
  object(move(object_)), field_name(move(field_name_)), right_value(move(rv))
{}

ObjectHolder FieldAssignment::Execute(Runtime::Closure& closure) {
  Runtime::Closure& fields = object.Execute(closure).TryAs<Runtime::ClassInstance>()->Fields();
  fields[field_name] = right_value->Execute(closure);
  return fields[field_name];
}

IfElse::IfElse(std::unique_ptr<Statement> condition_, std::unique_ptr<Statement> if_body_, std::unique_ptr<Statement> else_body_) :
  condition(move(condition_)), if_body(move(if_body_)), else_body(move(else_body_))
{}

ObjectHolder IfElse::Execute(Runtime::Closure& closure) {
  ObjectHolder condition_oh = condition->Execute(closure);
  if (condition_oh && Runtime::IsTrue(condition_oh)) {
    if (if_body) {
      return if_body->Execute(closure);
    }
  }
  else if (else_body) {
    return else_body->Execute(closure);
  }
  return ObjectHolder::None();
}

ObjectHolder Or::Execute(Runtime::Closure& closure) {
  ObjectHolder lhs_oh = lhs->Execute(closure);
  ObjectHolder rhs_oh = rhs->Execute(closure);

  if (!lhs_oh) {
    lhs_oh = ObjectHolder::Own(Runtime::Bool(false));
  }

  if (!rhs_oh) {
    rhs_oh = ObjectHolder::Own(Runtime::Bool(false));
  }

  bool result = Runtime::IsTrue(lhs_oh) || Runtime::IsTrue(rhs_oh);
  return ObjectHolder::Own(Runtime::Bool(result));
}

ObjectHolder And::Execute(Runtime::Closure& closure) {
  ObjectHolder lhs_oh = lhs->Execute(closure);
  ObjectHolder rhs_oh = rhs->Execute(closure);

  if (!lhs_oh) {
    lhs_oh = ObjectHolder::Own(Runtime::Bool(false));
  }

  if (!rhs_oh) {
    rhs_oh = ObjectHolder::Own(Runtime::Bool(false));
  }

  bool result = Runtime::IsTrue(lhs_oh) && Runtime::IsTrue(rhs_oh);
  return ObjectHolder::Own(Runtime::Bool(result));
}

ObjectHolder Not::Execute(Runtime::Closure& closure) {
  ObjectHolder arg_oh = argument->Execute(closure);

  if (!arg_oh) {
    arg_oh = ObjectHolder::Own(Runtime::Bool(false));
  }

  bool result = !Runtime::IsTrue(arg_oh);
  return ObjectHolder::Own(Runtime::Bool(result));
}

Comparison::Comparison(Comparator cmp_, unique_ptr<Statement> lhs_, unique_ptr<Statement> rhs_) :
  comparator(cmp_), left(move(lhs_)), right(move(rhs_))
{}

ObjectHolder Comparison::Execute(Runtime::Closure& closure) {
  bool result = comparator(left->Execute(closure), right->Execute(closure));
  return ObjectHolder::Own(Runtime::Bool(result));
}

NewInstance::NewInstance(const Runtime::Class& class__, std::vector<std::unique_ptr<Statement>> args_) :
  class_(class__), args(move(args_))
{}

NewInstance::NewInstance(const Runtime::Class& class_) : NewInstance(class_, {})
{}

ObjectHolder NewInstance::Execute(Runtime::Closure& closure) {
  Runtime::ClassInstance result(class_);
  
  if (result.HasMethod("__init__", args.size())) {
    std::vector<ObjectHolder> ctor_args;
    for (auto& arg : args) {
      ctor_args.push_back(arg->Execute(closure));
    }
    result.Call("__init__", ctor_args);
  }

  return ObjectHolder::Own(move(result));
}

} /* namespace Ast */
