#pragma once

#include <ostream>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

namespace Svg
{
  extern bool GlobalShielding;

  void SetShielding(bool shielding);

  template <typename T>
  std::string quote(const T& value)
  {
    std::ostringstream oss;
    if (GlobalShielding) {
      oss << " \\\"" << value << "\\\" ";
    }
    else {
      oss << " \"" << value << "\" ";
    }
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

  struct Rgba : public Rgb
  {
    Rgba() = default;
    Rgba(std::uint8_t red_value, std::uint8_t green_value, std::uint8_t blue_value, double alpha_value);

    double alpha = 0.0;
  };

  std::ostream& operator << (std::ostream& out, const Rgba& rgba);

  class Color
  {
  public:
    Color() = default;
    Color(const std::string& str);
    Color(const char str[]);
    Color(Rgb rgb);
    Color(Rgba rgba);

  private:
    std::variant<std::string, Rgb, Rgba> m_color = "none";
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
