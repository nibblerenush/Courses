#include <map>
#include <iostream>
#include <string>
#include <vector>

// Serialization

template <typename T>
void Serialize(T pod, std::ostream& out);

void Serialize(const std::string& str, std::ostream& out);

template <typename T>
void Serialize(const std::vector<T>& data, std::ostream& out);

template <typename T1, typename T2>
void Serialize(const std::map<T1, T2>& data, std::ostream& out);

template <typename T>
void Serialize(T pod, std::ostream& out) {
  out.write(reinterpret_cast<const char*>(&pod), sizeof(pod));
}

void Serialize(const std::string& str, std::ostream& out) {
  std::size_t size = str.size();
  out.write(reinterpret_cast<const char*>(&size), sizeof(size));
  out.write(str.c_str(), str.size());
}

template <typename T>
void Serialize(const std::vector<T>& data, std::ostream& out) {
  std::size_t size = data.size();
  out.write(reinterpret_cast<const char*>(&size), sizeof(size));
  for (const T& elem : data) {
    Serialize(elem, out);
  }
}

template <typename T1, typename T2>
void Serialize(const std::map<T1, T2>& data, std::ostream& out) {
  std::size_t size = data.size();
  out.write(reinterpret_cast<const char*>(&size), sizeof(size));
  for (const auto& [key, value] : data) {
    Serialize(key, out);
    Serialize(value, out);
  }
}

template <typename T>
void Deserialize(std::istream& in, T& pod);

void Deserialize(std::istream& in, std::string& str);

template <typename T>
void Deserialize(std::istream& in, std::vector<T>& data);

template <typename T1, typename T2>
void Deserialize(std::istream& in, std::map<T1, T2>& data);

// Deserialization

template <typename T>
void Deserialize(std::istream& in, T& pod) {
  in.read(reinterpret_cast<char*>(&pod), sizeof(pod));
}

void Deserialize(std::istream& in, std::string& str) {
  std::size_t size;
  in.read(reinterpret_cast<char*>(&size), sizeof(size));
  str.resize(size);
  in.read(str.data(), str.size());
}

template <typename T>
void Deserialize(std::istream& in, std::vector<T>& data) {
  std::size_t size;
  in.read(reinterpret_cast<char*>(&size), sizeof(size));
  data.resize(size);
  for (T& elem : data) {
    Deserialize(in, elem);
  }
}

template <typename T1, typename T2>
void Deserialize(std::istream& in, std::map<T1, T2>& data) {
  std::size_t size;
  in.read(reinterpret_cast<char*>(&size), sizeof(size));
  data.clear();
  for (std::size_t i = 0; i < size; ++i) {
    T1 key;
    Deserialize(in, key);
    T2 value;
    Deserialize(in, value);
    data[key] = value;
  }
}
