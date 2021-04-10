#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Figure
{
public:
  virtual std::string Name() const = 0;
  virtual double Perimeter() const = 0;
  virtual double Area() const = 0;
};

class Triangle : public Figure
{
public:
  Triangle(int a, int b, int c):
    a_(a), b_(b), c_(c)
  {}

  std::string Name() const override
  {
    return "TRIANGLE";
  }

  double Perimeter() const override
  {
    int result = a_ + b_ + c_;
    return result;
  }

  double Area() const override
  {
    double p = Perimeter() / 2;
    double result = std::sqrt(p * (p - a_) * (p - b_) * (p - c_));
    return result;
  }

private:
  int a_, b_, c_;
};

class Rect : public Figure
{
public:
  Rect(int width, int height) :
    width_(width), height_(height)
  {}

  std::string Name() const override
  {
    return "RECT";
  }

  double Perimeter() const override
  {
    int result = 2 * (width_ + height_);
    return result;
  }

  double Area() const override
  {
    double result = static_cast<double>(width_) * height_;
    return result;
  }

private:
  int width_, height_;
};

class Circle : public Figure
{
public:
  Circle(int r) : r_(r)
  {}

  std::string Name() const override
  {
    return "CIRCLE";
  }

  double Perimeter() const override
  {
    double result = 2 * 3.14 * r_;
    return result;
  }

  double Area() const override
  {
    double result = 3.14 * r_ * r_;
    return result;
  }

private:
  int r_;
};

std::shared_ptr<Figure> CreateFigure(std::istringstream& is)
{
  std::string name;
  is >> name;

  if (name == "RECT")
  {
    int width, height;
    is >> width >> height;
    return std::make_shared<Rect>(width, height);
  }
  else if (name == "TRIANGLE")
  {
    int a, b, c;
    is >> a >> b >> c;
    return std::make_shared<Triangle>(a, b, c);
  }
  else // name == "CIRCLE"
  {
    int r;
    is >> r;
    return std::make_shared<Circle>(r);
  }
}

int main() {
  vector<shared_ptr<Figure>> figures;
  for (string line; getline(cin, line); ) {
    istringstream is(line);

    string command;
    is >> command;
    if (command == "ADD") {
      figures.push_back(CreateFigure(is));
    }
    else if (command == "PRINT") {
      for (const auto& current_figure : figures) {
        cout << fixed << setprecision(3)
          << current_figure->Name() << " "
          << current_figure->Perimeter() << " "
          << current_figure->Area() << endl;
      }
    }
  }
  return 0;
}
