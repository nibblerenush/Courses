#include <cstddef>
#include <unordered_map>

struct Nucleotide {
  char Symbol;
  size_t Position;
  int ChromosomeNum;
  int GeneNum;
  bool IsMarked;
  char ServiceInfo;
};

const std::unordered_map<char, int> symbol_number = {
  {'A', 0},
  {'T', 1},
  {'G', 2},
  {'C', 3}
};

const std::unordered_map<int, char> number_symbol = {
  {0, 'A'},
  {1, 'T'},
  {2, 'G'},
  {3, 'C'}
};

struct CompactNucleotide {
  std::uint64_t Position : 32;
  std::uint64_t ServiceInfo : 8;
  std::uint64_t GeneNum : 15;
  std::uint64_t IsMarked : 1;
  std::uint64_t ChromosomeNum : 6;
  std::uint64_t Symbol : 2;
};

bool operator == (const Nucleotide& lhs, const Nucleotide& rhs) {
  return (lhs.Symbol == rhs.Symbol)
      && (lhs.Position == rhs.Position)
      && (lhs.ChromosomeNum == rhs.ChromosomeNum)
      && (lhs.GeneNum == rhs.GeneNum)
      && (lhs.IsMarked == rhs.IsMarked)
      && (lhs.ServiceInfo == rhs.ServiceInfo);
}

CompactNucleotide Compress(const Nucleotide& n)
{
  CompactNucleotide result;
  result.Position = n.Position;
  result.ServiceInfo = n.ServiceInfo;
  result.GeneNum = n.GeneNum;
  result.IsMarked = n.IsMarked;
  result.ChromosomeNum = n.ChromosomeNum;
  result.Symbol = symbol_number.at(n.Symbol);
  return result;
};

Nucleotide Decompress(const CompactNucleotide& cn)
{
  Nucleotide result;
  result.Symbol = number_symbol.at(cn.Symbol);
  result.Position = cn.Position;
  result.ChromosomeNum = cn.ChromosomeNum;
  result.GeneNum = cn.GeneNum;
  result.IsMarked = cn.IsMarked;
  result.ServiceInfo = cn.ServiceInfo;
  return result;
}
