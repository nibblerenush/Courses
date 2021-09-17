#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>

bool check_p(std::uint64_t record) {
  int p = record & 0b1;
  return p;
}

int main() {
  std::uint64_t m, q, r;
  std::cin >> m >> q >> r;

  std::unordered_map<std::uint64_t, std::uint64_t> paddr_value;
  for (std::uint64_t i = 0; i < m; ++i) {
    std::uint64_t paddr;
    std::uint64_t value;
    std::cin >> paddr >> value;
    paddr_value[paddr] = value;
  }

  std::vector<std::uint64_t> logical_addresses;
  logical_addresses.reserve(q);
  for (std::uint64_t i = 0; i < q; ++i) {
    std::uint64_t logical_address;
    std::cin >> logical_address;
    logical_addresses.push_back(logical_address);
  }

  for (std::uint64_t i = 0; i < q; ++i) {
    std::uint64_t logical_address = logical_addresses[i];

    int offset = logical_address & 0b111111111111;
    int table = (logical_address >> 12) & 0b111111111;
    int directory = (logical_address >> 21) & 0b111111111;
    int directory_ptr = (logical_address >> 30) & 0b111111111;
    int pml4 = (logical_address >> 39) & 0b111111111;

    // 1 step
    std::uint64_t record = paddr_value[r + 8 * pml4];
    if (!check_p(record)) {
      std::cout << "fault\n";
      continue;
    }
    std::uint64_t physical_address = (record >> 12) & 0b1111111111111111111111111111111111111111;
    physical_address <<= 12;

    // 2 step
    record = paddr_value[physical_address + 8 * directory_ptr];
    if (!check_p(record)) {
      std::cout << "fault\n";
      continue;
    }
    physical_address = (record >> 12) & 0b1111111111111111111111111111111111111111;
    physical_address <<= 12;

    // 3 step
    record = paddr_value[physical_address + 8 * directory];
    if (!check_p(record)) {
      std::cout << "fault\n";
      continue;
    }
    physical_address = (record >> 12) & 0b1111111111111111111111111111111111111111;
    physical_address <<= 12;

    // 4 step
    record = paddr_value[physical_address + 8 * table];
    if (!check_p(record)) {
      std::cout << "fault\n";
      continue;
    }
    physical_address = (record >> 12) & 0b1111111111111111111111111111111111111111;
    physical_address <<= 12;

    // 5 step
    physical_address += offset;
    std::cout << physical_address << '\n';
  }

  return 0;
}
