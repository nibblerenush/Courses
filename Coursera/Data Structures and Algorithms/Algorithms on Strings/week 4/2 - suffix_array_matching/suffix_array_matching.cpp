#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::string;
using std::vector;

int LetterToIndex(char letter) {
  switch (letter) {
    case '$': return 0;
    case 'A': return 1;
    case 'C': return 2;
    case 'G': return 3;
    case 'T': return 4;
  }
  return -1;
}

std::vector<int> SortCharacters(const std::string& text) {
  const int text_len = text.size();
  const int count_len = 5;

  std::vector<int> order(text_len);
  std::vector<int> count(count_len, 0);

  for (char letter : text) {
    count[LetterToIndex(letter)]++;
  }

  for (int i = 1; i < count_len; ++i) {
    count[i] += count[i - 1];
  }

  for (int i = text_len - 1; i >= 0; --i) {
    char letter = text[i];
    count[LetterToIndex(letter)]--;
    order[count[LetterToIndex(letter)]] = i;
  }

  return order;
}

std::vector<int> ComputeCharClasses(const std::string& text, const std::vector<int>& order) {
  const int text_len = text.size();

  std::vector<int> classes(text_len);
  classes[order[0]] = 0;

  for (int i = 1; i < text_len; ++i) {
    if (text[order[i]] != text[order[i - 1]]) {
      classes[order[i]] = classes[order[i - 1]] + 1;
    }
    else {
      classes[order[i]] = classes[order[i - 1]];
    }
  }

  return classes;
}

vector<int> SortDoubled(const string& text, int cyclic_size, const vector<int>& order, const vector<int>& classes) {
  const int text_len = text.size();

  std::vector<int> count(text_len, 0);
  std::vector<int> new_order(text_len);

  for (int i = 0; i < text_len; ++i) {
    count[classes[i]]++;
  }

  for (int i = 1; i < text_len; ++i) {
    count[i] += count[i - 1];
  }

  for (int i = text_len - 1; i >= 0; --i) {
    int start = (order[i] - cyclic_size + text_len) % text_len;
    int class_of_start = classes[start];
    count[class_of_start]--;
    new_order[count[class_of_start]] = start;
  }

  return new_order;
}

vector<int> UpdateClasses(const vector<int>& new_order, const vector<int>& classes, int cyclic_size) {
  const int order_len = new_order.size();
  std::vector<int> new_classes(order_len);
  new_classes[new_order[0]] = 0;

  for (int i = 1; i < order_len; ++i) {
    int cur_idx = new_order[i];
    int prev_idx = new_order[i - 1];

    int mid_cur_idx = (cur_idx + cyclic_size) % order_len;
    int mid_prev_idx = (prev_idx + cyclic_size) % order_len;

    if (classes[cur_idx] != classes[prev_idx] || classes[mid_cur_idx] != classes[mid_prev_idx]) {
      new_classes[cur_idx] = new_classes[prev_idx] + 1;
    }
    else {
      new_classes[cur_idx] = new_classes[prev_idx];
    }
  }

  return new_classes;
}

// Build suffix array of the string text and
// return a vector result of the same length as the text
// such that the value result[i] is the index (0-based)
// in text where the i-th lexicographically smallest
// suffix of text starts.
vector<int> BuildSuffixArray(const string& text) {
  vector<int> order = SortCharacters(text);
  vector<int> classes = ComputeCharClasses(text, order);

  int cyclic_size = 1;
  int text_len = text.size();

  while (cyclic_size < text_len) {
    order = SortDoubled(text, cyclic_size, order, classes);
    classes = UpdateClasses(order, classes, cyclic_size);
    cyclic_size *= 2;
  }
  
  return order;
}

vector<int> FindOccurrences(const string& pattern, const string& text, const vector<int>& suffix_array) {
  vector<int> result;
  
  // write your code here
  int min_idx = 0;
  int max_idx = text.size();
  
  while (min_idx < max_idx) {
    int middle = (min_idx + max_idx) / 2;
    
    if (pattern > text.substr(suffix_array[middle])) {
      min_idx = middle + 1;
    }
    else {
      max_idx = middle;
    }
  }
  
  int start = min_idx;
  max_idx = text.size();
  
  while (min_idx < max_idx) {
    int middle = (min_idx + max_idx) / 2;
    std::string suffix = text.substr(suffix_array[middle]);
    
    bool is_prefix = true;
    if (suffix.size() <= pattern.size()) {
      is_prefix = false;
    }
    else {
      for (std::size_t i = 0; i < pattern.size(); ++i) {
        if (pattern[i] != suffix[i]) {
          is_prefix = false;
          break;
        }
      }
    }
    
    if (is_prefix) {
      min_idx = middle + 1;
    }
    else {
      max_idx = middle;
    }
  }
  
  int end = max_idx;
  
  if (start < end) {
    for (int idx = start; idx < end; ++idx) {
      result.push_back(suffix_array[idx]);
    }
  }

  return result;
}

int main() {
  char buffer[100001];
  scanf("%s", buffer);
  string text = buffer;
  text += '$';
  vector<int> suffix_array = BuildSuffixArray(text);
  int pattern_count;
  scanf("%d", &pattern_count);
  vector<bool> occurs(text.length(), false);
  for (int pattern_index = 0; pattern_index < pattern_count; ++pattern_index) {
    scanf("%s", buffer);
    string pattern = buffer;
    vector<int> occurrences = FindOccurrences(pattern, text, suffix_array);
    for (int j = 0; j < static_cast<int>(occurrences.size()); ++j) {
      occurs[occurrences[j]] = true;
    }
  }
  for (int i = 0; i < static_cast<int>(occurs.size()); ++i) {
    if (occurs[i]) {
      printf("%d ", i);
    }
  }
  printf("\n");
  return 0;
}
