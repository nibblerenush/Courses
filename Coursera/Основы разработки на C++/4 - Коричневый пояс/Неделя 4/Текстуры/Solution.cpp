#include "Common.h"
#include <stdexcept>
#include <utility>

using namespace std;

// Этот файл сдаётся на проверку
// Здесь напишите реализацию необходимых классов-потомков `IShape`

class Shape : public IShape
{
public:
  template <typename T>
  unique_ptr<T> CloneShape() const
  {
    auto clone_shape = make_unique<T>();
    clone_shape->SetPosition(m_position);
    clone_shape->SetSize(m_size);
    clone_shape->SetTexture(m_texture);
    return move(clone_shape);
  }

  void SetPosition(Point position) final {
    m_position = move(position);
  }

  Point GetPosition() const final {
    return m_position;
  }

  void SetSize(Size size) final {
    m_size = move(size);
  }

  Size GetSize() const final {
    return m_size;
  }

  void SetTexture(shared_ptr<ITexture> texture) final {
    m_texture = move(texture);
  }

  ITexture* GetTexture() const final {
    return m_texture.get();
  }
  
  void Draw(Image& image) const final
  {
    Size image_size =
    {
      static_cast<int>(image.empty() ? 0 : image[0].size()),
      static_cast<int>(image.size())
    };

    int i = 0;
    int j = 0;
    for (i = m_position.y; i < m_position.y + m_size.height && i < image_size.height; ++i)
    {
      for (j = m_position.x; j < m_position.x + m_size.width && j < image_size.width; ++j)
      {
        if (CheckPoint({ j - m_position.x, i - m_position.y }))
        {
          image[i][j] = '.';
        }
      }
    }
    int new_height = i;
    int new_width = j;
    
    if (m_texture)
    {
      const Image& texture_image = m_texture->GetImage();
      Size texture_size = m_texture->GetSize();
      
      for (i = m_position.y; i < m_position.y + texture_size.height && i < new_height; ++i)
      {
        for (j = m_position.x; j < m_position.x + texture_size.width && j < new_width; ++j)
        {
          if (CheckPoint({ j - m_position.x, i - m_position.y }))
          {
            image[i][j] = texture_image[i - m_position.y][j - m_position.x];
          }
        }
      }
    }
  }

private:
  virtual bool CheckPoint(Point point) const = 0;

protected:
  Size m_size = {};
  Point m_position = {};
  shared_ptr<ITexture> m_texture;
};

class Rectangle : public Shape
{
public:
  unique_ptr<IShape> Clone() const final
  {
    return CloneShape<Rectangle>();
  }
  
private:
  bool CheckPoint(Point) const final
  {
    return true;
  }
};

class Ellipse : public Shape
{
public:
  unique_ptr<IShape> Clone() const final
  {
    return CloneShape<Ellipse>();
  }

private:
  bool CheckPoint(Point point) const final
  {
    return IsPointInEllipse(point, m_size);
  }
};

// Напишите реализацию функции
unique_ptr<IShape> MakeShape(ShapeType shape_type)
{
  switch (shape_type)
  {
  case ShapeType::Rectangle:
    return make_unique<Rectangle>();
  case ShapeType::Ellipse:
    return make_unique<Ellipse>();
  default:
    throw runtime_error("Unknown ShapeType");
  }
}
