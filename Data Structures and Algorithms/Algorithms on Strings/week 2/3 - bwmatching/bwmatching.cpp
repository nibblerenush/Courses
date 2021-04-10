#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using std::cin;
using std::istringstream;
using std::map;
using std::string;
using std::vector;

std::size_t letterToIndex(char letter) {
  switch (letter) {
  case '$': return 0; break;
  case 'A': return 1; break;
  case 'C': return 2; break;
  case 'G': return 3; break;
  case 'T': return 4; break;
  }
}

char indexToLetter(std::size_t index) {
  switch (index) {
  case 0: return '$'; break;
  case 1: return 'A'; break;
  case 2: return 'C'; break;
  case 3: return 'G'; break;
  case 4: return 'T'; break;
  }
}

// Preprocess the Burrows-Wheeler Transform bwt of some text
// and compute as a result:
//   * starts - for each character C in bwt, starts[C] is the first position 
//       of this character in the sorted array of 
//       all characters of the text.
//   * occ_count_before - for each character C in bwt and each position P in bwt,
//       occ_count_before[C][P] is the number of occurrences of character C in bwt
//       from position 0 to position P inclusive.
void PreprocessBWT(const string& bwt,
                   map<char, int>& starts,
                   map<char, vector<int> >& occ_count_before) {
  // Implement this function yourself
  
  std::vector<std::size_t> counter(5, 0);
  for (char c : bwt) {
    counter[letterToIndex(c)]++;
  }
  
  std::size_t last_index = 0;
  std::size_t last_counter = 0;
  
  for (std::size_t i = 0; i < counter.size(); ++i) {
    if (counter[i] != 0) {
      starts[indexToLetter(i)] = last_index + last_counter;
      last_index = starts[indexToLetter(i)];
      last_counter = counter[i];
    }
  }
  
  occ_count_before['$'].assign(bwt.size() + 1, 0);
  occ_count_before['A'].assign(bwt.size() + 1, 0);
  occ_count_before['C'].assign(bwt.size() + 1, 0);
  occ_count_before['T'].assign(bwt.size() + 1, 0);
  occ_count_before['G'].assign(bwt.size() + 1, 0);
  
  for (std::size_t i = 0; i < bwt.size(); ++i) {
    char c = bwt[i];
    std::size_t j = i + 1;
    
    for (auto& count_vector : occ_count_before) {
      count_vector.second[j] = count_vector.second[j - 1];
    }
    
    occ_count_before[c][j]++;
  }
}

// Compute the number of occurrences of string pattern in the text
// given only Burrows-Wheeler Transform bwt of the text and additional
// information we get from the preprocessing stage - starts and occ_counts_before.
int CountOccurrences(const string& pattern, 
                     const string& bwt, 
                     const map<char, int>& starts, 
                     const map<char, vector<int> >& occ_count_before) {
  // Implement this function yourself

  std::size_t top = 0;
  std::size_t bottom = bwt.size() - 1;
  int pattern_index = static_cast<int>(pattern.size()) - 1;
  
  while (top <= bottom) {
    if (pattern_index >= 0) {
      char symbol = pattern[pattern_index--];
      
      bool occurrence_of_symbol = false;
      for (std::size_t j = top; j <= bottom; ++j) {
        if (bwt[j] == symbol) {
          occurrence_of_symbol = true;
          break;
        }
      }
      
      if (occurrence_of_symbol) {
        top = starts.at(symbol) + occ_count_before.at(symbol).at(top);
        bottom = starts.at(symbol) + occ_count_before.at(symbol).at(bottom + 1) - 1;
      }
      else {
        return 0;
      }
    }
    else {
      return bottom - top + 1;
    }
  }
  return 0;
}

int main() {
  string bwt;
  cin >> bwt;
  int pattern_count;
  cin >> pattern_count;
  // Start of each character in the sorted list of characters of bwt,
  // see the description in the comment about function PreprocessBWT
  map<char, int> starts;
  // Occurrence counts for each character and each position in bwt,
  // see the description in the comment about function PreprocessBWT
  map<char, vector<int> > occ_count_before;
  // Preprocess the BWT once to get starts and occ_count_before.
  // For each pattern, we will then use these precomputed values and
  // spend only O(|pattern|) to find all occurrences of the pattern
  // in the text instead of O(|pattern| + |text|).
  PreprocessBWT(bwt, starts, occ_count_before);
  for (int pi = 0; pi < pattern_count; ++pi) {
    string pattern;
    cin >> pattern;
    int occ_count = CountOccurrences(pattern, bwt, starts, occ_count_before);
    printf("%d ", occ_count);
  }
  printf("\n");
  return 0;
}
