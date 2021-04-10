#include "Svg.h"

namespace Svg
{
  bool GlobalShielding = false;

  void SetShielding(bool shielding) {
    GlobalShielding = shielding;
  }

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

  // ===== Rgba =====
  Rgba::Rgba(std::uint8_t red_value, std::uint8_t green_value, std::uint8_t blue_value, double alpha_value):
    Rgb(red_value, green_value, blue_value), alpha(alpha_value)
  {}

  std::ostream& operator << (std::ostream& out, const Rgba& rgba)
  {
    out << "rgba("
      << static_cast<int>(rgba.red) << ','
      << static_cast<int>(rgba.green) << ','
      << static_cast<int>(rgba.blue) << ','
      << rgba.alpha << ')';

    return out;
  }

  // ===== Color =====
  Color::Color(const std::string& str) : m_color(str)
  {}

  Color::Color(Rgb rgb) : m_color(std::move(rgb))
  {}

  Color::Color(Rgba rgba) : m_color(std::move(rgba))
  {}

  Color::Color(const char str[]) : m_color(str)
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
      << "points=";

    if (GlobalShielding) {
      out << "\\\"";
    }
    else {
      out << "\"";
    }

    for (const auto& point : polyline.m_points) {
      out << point.x << ',' << point.y << ' ';
    }

    if (GlobalShielding) {
      out << "\\\"";
    }
    else {
      out << "\"";
    }
    out << "/>";

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
      << "<?xml version=" << quote("1.0") << " encoding=" << quote("UTF-8") << " ?>"
      << "<svg xmlns=" << quote("http://www.w3.org/2000/svg") << " version=" << quote("1.1") << '>';

    for (const auto& object : m_objects) {
      std::visit([&out](const auto& object) { out << object; }, object);
    }

    out << "</svg>";
  }
}
