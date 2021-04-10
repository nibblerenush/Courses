#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>

class Matrix
{
public:
  Matrix()
  {}
  
  Matrix(int num_rows, int num_cols)
  {
    Reset(num_rows, num_cols);
  }
  
  void Reset(int num_rows, int num_cols)
  {
    if (num_rows < 0 || num_cols < 0)
    {
      throw std::out_of_range("num_rows < 0 || num_cols < 0");
    }

    if (num_rows > 0 && num_cols > 0)
    {
      m_matrix.resize(num_rows);
      for (auto& row : m_matrix)
      {
        row.assign(num_cols, 0);
      }
    }
    else
    {
      m_matrix.clear();
    }
  }

  int At(int row, int col) const
  {
    return m_matrix.at(row).at(col);
  }

  int& At(int row, int col)
  {
    return m_matrix.at(row).at(col);
  }

  int GetNumRows() const
  {
    return m_matrix.size();
  }

  int GetNumColumns() const
  {
    if (!m_matrix.empty())
    {
      return m_matrix.at(0).size();
    }
    else
    {
      return 0;
    }
  }
private:
  std::vector<std::vector<int>> m_matrix;
};

std::istream& operator>>(std::istream& stream, Matrix& matrix)
{
  bool ok{ true };
  
  auto stream_check = [&stream, &ok](auto& value)
  {
    return ok = ok && (stream >> value);
  };
  
  int num_rows;
  if (!stream_check(num_rows)) {return stream; }
  
  int num_cols;
  if (!stream_check(num_cols)) { return stream; }
  
  Matrix temp_matrix{ num_rows, num_cols };
  for (int i = 0; i < num_rows; ++i)
  {
    for (int j = 0; j < num_cols; ++j)
    {
      int value;
      if (!stream_check(value)) { return stream; }
      temp_matrix.At(i, j) = value;
    }
  }
  
  matrix = temp_matrix;
  return stream;
}

std::ostream& operator<<(std::ostream& stream, const Matrix& matrix)
{
  stream << matrix.GetNumRows() << ' ' << matrix.GetNumColumns() << std::endl;
  for (int i = 0; i < matrix.GetNumRows(); ++i)
  {
    for (int j = 0; j < matrix.GetNumColumns(); ++j)
    {
      stream << matrix.At(i, j);
      if (j != matrix.GetNumColumns() - 1)
      {
        stream << ' ';
      }
    }
    stream << std::endl;
  }
  return stream;
}

bool operator==(const Matrix& lhs, const Matrix& rhs)
{
  if (lhs.GetNumRows() != rhs.GetNumRows())
  {
    return false;
  }
  else if (lhs.GetNumColumns() != rhs.GetNumColumns())
  {
    return false;
  }
  else
  {
    for (int i = 0; i < lhs.GetNumRows(); ++i)
    {
      for (int j = 0; j < lhs.GetNumColumns(); ++j)
      {
        if (lhs.At(i, j) != rhs.At(i, j))
        {
          return false;
        }
      }
    }
  }
  return true;
}

Matrix operator+(const Matrix& lhs, const Matrix& rhs)
{
  if (lhs.GetNumRows() != rhs.GetNumRows())
  {
    throw std::invalid_argument("lhs.GetNumRows() != rhs.GetNumRows()");
  }
  else if (lhs.GetNumColumns() != rhs.GetNumColumns())
  {
    throw std::invalid_argument("lhs.GetNumColumns() != rhs.GetNumColumns()");
  }
  
  Matrix result{ lhs.GetNumRows(), lhs.GetNumColumns() };
  for (int i = 0; i < lhs.GetNumRows(); ++i)
  {
    for (int j = 0; j < lhs.GetNumColumns(); ++j)
    {
      result.At(i, j) = lhs.At(i, j) + rhs.At(i, j);
    }
  }
  return result;
}

int main()
{
  try
  {
    Matrix one;
    Matrix two;

    std::cin >> one >> two;
    std::cout << one + two << std::endl;
  }
  catch (const std::out_of_range& ex)
  {
    std::cout << ex.what() << std::endl;
  }
  catch (const std::invalid_argument& ex)
  {
    std::cout << ex.what() << std::endl;
  }

  return 0;
}
