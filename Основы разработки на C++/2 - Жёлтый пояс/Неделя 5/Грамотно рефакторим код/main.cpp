#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Person
{
public:
  Person(const std::string& name, const std::string& type) :
    name_(name),
    type_(type)
  {}

  virtual void Walk(const std::string& destination) const
  {
    std::ostringstream oss;
    oss << "walks to: " << destination;
    PrintToStream(std::cout, oss.str());
  }

  std::string GetName() const
  {
    return name_;
  }

  std::string GetType() const
  {
    return type_;
  }

protected:
  void PrintToStream(std::ostream& out_stream, const std::string& out_string) const
  {
    out_stream << type_ << ": " << name_ << ' ' << out_string << std::endl;
  }

private:
  const std::string name_;
  const std::string type_;
};

class Student : public Person
{
public:
  Student(const std::string& name, const std::string& favourite_song):
    Person(name, "Student"),
    favourite_song_(favourite_song)
  {}
  
  void Walk(const std::string& destination) const override
  {
    Person::Walk(destination);
    SingSong();
  }

  void Learn() const
  {
    PrintToStream(std::cout, "learns");
  }
  
  void SingSong() const
  {
    std::ostringstream oss;
    oss << "sings a song: " << favourite_song_;
    PrintToStream(std::cout, oss.str());
  }

private:
  const std::string favourite_song_;
};


class Teacher : public Person
{
public:
  Teacher(const std::string& name, const std::string& subject):
    Person(name, "Teacher"),
    subject_(subject)
  {}

  void Teach() const
  {
    std::ostringstream oss;
    oss << "teaches: " << subject_;
    PrintToStream(std::cout, oss.str());
  }

private:
  const std::string subject_;
};


class Policeman : public Person
{
public:
  Policeman(const std::string& name):
    Person(name, "Policeman")
  {}

  void Check(const Person& person)
  {
    std::ostringstream oss;
    oss << "checks " << person.GetType() << ". " << person.GetType() << "\'s name is: " << person.GetName();
    PrintToStream(std::cout, oss.str());
  }
};

void VisitPlaces(const Person& person, const std::vector<std::string>& places)
{
  for (const auto& place : places)
  {
    person.Walk(place);
  }
}

int main() {
  Teacher t("Jim", "Math");
  Student s("Ann", "We will rock you");
  Policeman p("Bob");

  VisitPlaces(t, { "Moscow", "London" });
  p.Check(s);
  VisitPlaces(s, { "Moscow", "London" });
  return 0;
}
