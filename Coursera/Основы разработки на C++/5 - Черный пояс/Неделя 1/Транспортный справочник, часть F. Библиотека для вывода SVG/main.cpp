#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace Svg
{
  template <typename T>
  std::string quote(const T& value)
  {
    std::ostringstream oss;
    oss << " \"" << value << "\" ";
    return oss.str();
  }

  struct Point
  {
    Point() = default;
    Point(double x_value, double y_value);
    
    double x = 0.0;
    double y = 0.0;
  };
  
  struct Rgb
  {
    Rgb() = default;
    Rgb(std::uint8_t red_value, std::uint8_t green_value, std::uint8_t blue_value);

    std::uint8_t red = 0;
    std::uint8_t green = 0;
    std::uint8_t blue = 0;
  };
  
  std::ostream& operator << (std::ostream& out, const Rgb& rgb);

  class Color
  {
  public:
    Color() = default;
    Color(const std::string& str);
    Color(const char str[]);
    Color(Rgb rgb);
  
  private:
    std::variant<std::string, Rgb> m_color = "none";
    friend std::ostream& operator << (std::ostream& out, const Color& color);
  };
  
  std::ostream& operator << (std::ostream& out, const Color& color);

  static const Color NoneColor;
  
  template <typename T>
  class Object
  {
  public:
    Object() = default;
    
    T& SetFillColor(const Color& fill)
    {
      m_fill = fill;
      return static_cast<T&>(*this);
    }

    T& SetStrokeColor(const Color& stroke)
    {
      m_stroke = stroke;
      return static_cast<T&>(*this);
    }
    
    T& SetStrokeWidth(double stroke_width)
    {
      m_stroke_width = stroke_width;
      return static_cast<T&>(*this);
    }

    T& SetStrokeLineCap(const std::string& stroke_linecap)
    {
      m_stroke_linecap = stroke_linecap;
      return static_cast<T&>(*this);
    }

    T& SetStrokeLineJoin(const std::string& stroke_linejoin)
    {
      m_stroke_linejoin = stroke_linejoin;
      return static_cast<T&>(*this);
    }

    virtual ~Object() = default;

  private:
    Color m_fill = NoneColor;
    Color m_stroke = NoneColor;
    double m_stroke_width = 1.0;
    std::string m_stroke_linecap;
    std::string m_stroke_linejoin;

    template <typename U>
    friend std::ostream& operator << (std::ostream& out, const Object<U>& color);
  };

  template <typename U>
  std::ostream& operator << (std::ostream& out, const Object<U>& color)
  {
    out
      << "fill=" << quote(color.m_fill)
      << "stroke=" << quote(color.m_stroke)
      << "stroke-width=" << quote(color.m_stroke_width);

    if (!color.m_stroke_linecap.empty()) {
      out << "stroke-linecap=" << quote(color.m_stroke_linecap);
    }

    if (!color.m_stroke_linejoin.empty()) {
      out << "stroke-linejoin=" << quote(color.m_stroke_linejoin);
    }

    return out;
  }

  class Circle : public Object<Circle>
  {
  public:
    Circle& SetCenter(Point center);
    Circle& SetRadius(double radius);

  private:
    Point m_center;
    double m_radius = 1.0;
    friend std::ostream& operator << (std::ostream& out, const Circle& circle);
  };

  std::ostream& operator << (std::ostream& out, const Circle& circle);

  class Polyline : public Object<Polyline>
  {
  public:
    Polyline& AddPoint(Point point);

  private:
    std::vector<Point> m_points;
    friend std::ostream& operator << (std::ostream& out, const Polyline& polyline);
  };

  std::ostream& operator << (std::ostream& out, const Polyline& polyline);

  class Text : public Object<Text>
  {
  public:
    Text& SetPoint(Point point);
    Text& SetOffset(Point offset);
    Text& SetFontSize(std::uint32_t font_size);
    Text& SetFontFamily(const std::string& font_family);
    Text& SetData(const std::string& data);

  private:
    Point m_point;
    Point m_offset;
    std::uint32_t m_font_size = 1;
    std::string m_font_family;
    std::string m_data;
    friend std::ostream& operator << (std::ostream& out, const Text& text);
  };

  std::ostream& operator << (std::ostream& out, const Text& text);

  class Document
  {
  public:
    Document() = default;
    void Add(const Circle& circle);
    void Add(Circle&& circle);
    void Add(const Polyline& polyline);
    void Add(Polyline&& polyline);
    void Add(const Text& text);
    void Add(Text&& text);
    void Render(std::ostream& out);

  private:
    std::vector<std::variant<Circle, Polyline, Text>> m_objects;
  };
}

namespace Svg
{
  // ===== Point =====
  Point::Point(double x_value, double y_value) : x(x_value), y(y_value)
  {}

  // ===== Rgb =====
  Rgb::Rgb(std::uint8_t red_value, std::uint8_t green_value, std::uint8_t blue_value) :
    red(red_value), green(green_value), blue(blue_value)
  {}

  std::ostream& operator << (std::ostream& out, const Rgb& rgb)
  {
    out << "rgb("
      << static_cast<int>(rgb.red) << ','
      << static_cast<int>(rgb.green) << ','
      << static_cast<int>(rgb.blue) << ')';

    return out;
  }

  // ===== Color =====
  Color::Color(const std::string& str) : m_color(str)
  {}

  Color::Color(Rgb rgb) : m_color(std::move(rgb))
  {}

  Color::Color(const char str []) : m_color(str)
  {}

  std::ostream& operator << (std::ostream& out, const Color& color)
  {
    std::visit([&out](const auto& value) { out << value; }, color.m_color);
    return out;
  }

  // ===== Circle =====
  Circle& Circle::SetCenter(Point center)
  {
    m_center = center;
    return *this;
  }

  Circle& Circle::SetRadius(double radius)
  {
    m_radius = radius;
    return *this;
  }

  std::ostream& operator << (std::ostream& out, const Circle& circle)
  {
    out
      << "<circle " << dynamic_cast<const Object<Circle>&>(circle)
      << "cx=" << quote(circle.m_center.x)
      << "cy=" << quote(circle.m_center.y)
      << "r=" << quote(circle.m_radius) << "/>";

    return out;
  }

  // ===== Polyline =====
  Polyline& Polyline::AddPoint(Point point)
  {
    m_points.push_back(point);
    return *this;
  }

  std::ostream& operator << (std::ostream& out, const Polyline& polyline)
  {
    out
      << "<polyline " << dynamic_cast<const Object<Polyline>&>(polyline)
      << "points=\"";

    for (const auto& point : polyline.m_points) {
      out << point.x << ',' << point.y << ' ';
    }
    out << "\"/>";

    return out;
  }

  // ===== Text =====
  Text& Text::SetPoint(Point point)
  {
    m_point = point;
    return *this;
  }

  Text& Text::SetOffset(Point offset)
  {
    m_offset = offset;
    return *this;
  }

  Text& Text::SetFontSize(std::uint32_t font_size)
  {
    m_font_size = font_size;
    return *this;
  }

  Text& Text::SetFontFamily(const std::string& font_family)
  {
    m_font_family = font_family;
    return *this;
  }

  Text& Text::SetData(const std::string& data)
  {
    m_data = data;
    return *this;
  }

  std::ostream& operator << (std::ostream& out, const Text& text)
  {
    out
      << "<text " << dynamic_cast<const Object<Text>&>(text)
      << "x=" << quote(text.m_point.x)
      << "y=" << quote(text.m_point.y)
      << "dx=" << quote(text.m_offset.x)
      << "dy=" << quote(text.m_offset.y)
      << "font-size=" << quote(text.m_font_size);

    if (!text.m_font_family.empty()) {
      out << "font-family=" << quote(text.m_font_family);
    }

    out << ">" << text.m_data << "</text>";
    return out;
  }

  void Document::Add(const Circle& circle) {
    m_objects.push_back(circle);
  }

  void Document::Add(Circle&& circle) {
    m_objects.push_back(std::move(circle));
  }

  void Document::Add(const Polyline& polyline) {
    m_objects.push_back(polyline);
  }

  void Document::Add(Polyline&& polyline) {
    m_objects.push_back(std::move(polyline));
  }

  void Document::Add(const Text& text) {
    m_objects.push_back(text);
  }

  void Document::Add(Text&& text) {
    m_objects.push_back(std::move(text));
  }

  void Document::Render(std::ostream& out)
  {
    out
      << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
      << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">";

    for (const auto& object : m_objects) {
      std::visit([&out](const auto& object) { out << object; }, object);
    }

    out << "</svg>";
  }
}

int main()
{
  Svg::Document svg;

  svg.Add(
    Svg::Polyline{}
    .SetStrokeColor(Svg::Rgb{ 140, 198, 63 })  // soft green
    .SetStrokeWidth(16)
    .SetStrokeLineCap("round")
    .AddPoint({ 50, 50 })
    .AddPoint({ 250, 250 })
  );

  for (const auto point : { Svg::Point{50, 50}, Svg::Point{250, 250} }) {
    svg.Add(
      Svg::Circle{}
      .SetFillColor("white")
      .SetRadius(6)
      .SetCenter(point)
    );
  }

  svg.Add(
    Svg::Text{}
    .SetPoint({ 50, 50 })
    .SetOffset({ 10, -10 })
    .SetFontSize(20)
    .SetFontFamily("Verdana")
    .SetFillColor("black")
    .SetData("C")
  );
  svg.Add(
    Svg::Text{}
    .SetPoint({ 250, 250 })
    .SetOffset({ 10, -10 })
    .SetFontSize(20)
    .SetFontFamily("Verdana")
    .SetFillColor("black")
    .SetData("C++")
  );

  svg.Render(std::cout);

  return 0;
}
