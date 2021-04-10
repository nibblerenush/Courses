#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_set>

char* init_buf = NULL;
size_t size_init_buf = 0;

char* global_buf = NULL;
int FREE_INFO_SIZE = 2 * sizeof(char) + 2 * sizeof(size_t) + 2 * sizeof(char*);
int ALLOC_INFO_SIZE = 2 * sizeof(char) + 2 * sizeof(size_t);

// Ёта функци€ будет вызвана перед тем как вызывать myalloc и myfree
    // используйте ее чтобы инициализировать ваш аллокатор перед началом
    // работы.
    //
    // buf - указатель на участок логической пам€ти, который ваш аллокатор
    //       должен распредел€ть, все возвращаемые указатели должны быть
    //       либо равны NULL, либо быть из этого участка пам€ти
    // size - размер участка пам€ти, на который указывает buf
void mysetup(void* buf, std::size_t size) {
  init_buf = (char*)buf;
  size_init_buf = size;

  char* my_buf = (char*)buf;

  // tags
  *(my_buf + 0) = 0;
  *(my_buf + size - 1) = 0;

  // sizes
  *(size_t*)(my_buf + 1) = size;
  *(size_t*)(my_buf + size - 1 - sizeof(size_t)) = size;

  // ptrs
  *(char**)(my_buf + 1 + sizeof(size_t)) = NULL;
  *(char**)(my_buf + 1 + 2 * sizeof(size_t)) = NULL;

  global_buf = my_buf;
}

// ‘ункци€ аллокации
void* myalloc(std::size_t size) {
  char* head = global_buf;

  size_t free_size = 0;
  bool is_alloc = false;

  while (head != NULL) {
    free_size = *(size_t*)(head + 1);
    if (free_size - FREE_INFO_SIZE >= size + ALLOC_INFO_SIZE) {
      is_alloc = true;
      break;
    }
    head = *(char**)(head + 1 + sizeof(size_t));
  }

  if (!is_alloc) {
    return NULL;
  }

  // === alloc part ===
  size_t alloc_size = size + ALLOC_INFO_SIZE;
  char* alloc_mem = head + free_size - alloc_size;
  // tags
  *(alloc_mem + 0) = 1;
  *(alloc_mem + alloc_size - 1) = 1;
  // sizes
  *(size_t*)(alloc_mem + 1) = alloc_size;
  *(size_t*)(alloc_mem + alloc_size - 1 - sizeof(size_t)) = alloc_size;


  // === free part ===
  free_size -= alloc_size;
  // tags
  *(head + 0) = 0;
  *(head + free_size - 1) = 0;
  // sizes
  *(size_t*)(head + 1) = free_size;
  *(size_t*)(head + free_size - 1 - sizeof(size_t)) = free_size;

  return alloc_mem + 1 + sizeof(size_t);
}

// ‘ункци€ освобождени€
void myfree(void* p) {
  char* alloc_mem = (char*)p;
  alloc_mem -= 1 + sizeof(size_t);

  int alloc_size = *(size_t*)(alloc_mem + 1);

  char* left_tag = (alloc_mem + 0);
  char* right_tag = (alloc_mem + alloc_size - 1);

  // ≈сли справа и слева зан€то
  if ((left_tag == init_buf || *(left_tag - 1) == 1) && (right_tag == init_buf + size_init_buf - 1 || *(right_tag + 1) == 1)) {
    // tags
    *left_tag = 0;
    *right_tag = 0;

    // ptrs
    *(char**)(alloc_mem + 1 + sizeof(size_t)) = global_buf;
    *(char**)(alloc_mem + 1 + 2 * sizeof(size_t)) = NULL;
    *(char**)(global_buf + 1 + 2 * sizeof(size_t)) = alloc_mem;
    global_buf = alloc_mem;
  }
  // ≈сли слева свободно, а справа зан€то
  else if ((left_tag != init_buf && *(left_tag - 1) == 0) /*&& (right_tag == init_buf + size_init_buf - 1 || *(right_tag + 1) == 1)*/) {
    size_t free_size = *(size_t*)(alloc_mem - 1 - sizeof(size_t));
    char* free_mem = alloc_mem - free_size;
    free_size += alloc_size;

    *right_tag = 0;
    *(size_t*)(free_mem + 1) = free_size;
    *(size_t*)(right_tag - sizeof(size_t)) = free_size;
  }
  // ≈сли слева зан€то, а справа свободно
  else if ((left_tag == init_buf || *(left_tag - 1) == 1) && (right_tag != init_buf + size_init_buf - 1 && *(right_tag + 1) == 0)) {
    char* free_mem = right_tag + 1;

    size_t free_size = *(size_t*)(free_mem + 1);
    size_t new_free_size = free_size + alloc_size;

    char* next_ptr = *(char**)(free_mem + 1 + sizeof(size_t));
    char* prev_ptr = *(char**)(free_mem + 1 + 2 * sizeof(size_t));

    if (prev_ptr != NULL) {
      *(char**)(prev_ptr + 1 + sizeof(size_t)) = alloc_mem;
    }

    // tags
    *left_tag = 0;

    // sizes
    *(size_t*)(alloc_mem + 1) = new_free_size;
    *(size_t*)(free_mem + free_size - 1 - sizeof(size_t)) = new_free_size;

    // ptrs
    *(char**)(alloc_mem + 1 + sizeof(size_t)) = next_ptr;
    *(char**)(alloc_mem + 1 + 2 * sizeof(size_t)) = prev_ptr;
  }
  // ≈сли свободно и справа и слева
  else {

  }
}

static const int BUF_SIZE = 500;
char* buf = nullptr;

char* myalloc_test(size_t size, char c) {
  char* space = (char*)myalloc(size);

  if (!space) {
    return space;
  }

  for (int i = 0; i < size; ++i) {
    space[i] = c;
  }
  return space;
}

int main() {
  buf = new char[BUF_SIZE];
  std::fill(buf, buf + BUF_SIZE, 0x55);

  mysetup(buf, BUF_SIZE);

  int vec_size = 25;
  std::vector<char*> vec;

  for (int i = 0; i < vec_size; ++i) {
    char* space = myalloc_test(16, i);
    if (space != nullptr) {
      vec.push_back(space);
    }
  }

  myfree(vec[0]);
  myfree(vec[4]);
  myfree(vec[5]);
  myfree(vec[0]);
  myfree(vec[7]);
  myfree(vec[12]);

  std::cout << "abc";

  /*char* space1 = myalloc_test(16, 0x41);
  char* space2 = myalloc_test(16, 0x42);
  char* space3 = myalloc_test(16, 0x43);*/
  
  /*myfree(space3);
  myfree(space2);
  myfree(space1);*/

  //std::cout << space1 << '\n' << space2;
  delete [] buf;
  return 0;
}
