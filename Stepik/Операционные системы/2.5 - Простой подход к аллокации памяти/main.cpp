#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_set>

// init buffer begin
const char* init_buf = NULL;

// init buffer size
size_t size_init_buf = 0;

// head of linked list
char* global_buf = NULL;

const int FREE_INFO_SIZE = 2 * sizeof(char) + 2 * sizeof(size_t) + 2 * sizeof(char*);
const int ALLOC_INFO_SIZE = 2 * sizeof(char) + 2 * sizeof(size_t);

char* get_left_tag(char* buf) {
  return (buf + 0);
}

char* get_right_tag(char* buf, size_t size) {
  return (buf + size - 1);
}

size_t* get_left_size(char* buf) {
  return (size_t*)(buf + 1);
}

size_t* get_right_size(char* buf, size_t size) {
  return (size_t*)(buf + size - 1 - sizeof(size_t));
}

char** get_next_ptr(char* buf) {
  return (char**)(buf + 1 + sizeof(size_t));
}

char** get_prev_ptr(char* buf) {
  return (char**)(buf + 1 + 2 * sizeof(size_t));
}

// Ёта функци€ будет вызвана перед тем как вызывать myalloc и myfree
    // используйте ее чтобы инициализировать ваш аллокатор перед началом
    // работы.
    //
    // buf - указатель на участок логической пам€ти, который ваш аллокатор
    //       должен распредел€ть, все возвращаемые указатели должны быть
    //       либо равны NULL, либо быть из этого участка пам€ти
    // size - размер участка пам€ти, на который указывает buf
void mysetup(void* buf, size_t size) {
  init_buf = (char*)buf;
  size_init_buf = size;

  char* my_buf = (char*)buf;

  // tags
  *get_left_tag(my_buf) = 0;
  *get_right_tag(my_buf, size) = 0;

  // sizes
  *get_left_size(my_buf) = size;
  *get_right_size(my_buf, size) = size;

  // ptrs
  *get_next_ptr(my_buf) = NULL;
  *get_prev_ptr(my_buf) = NULL;

  global_buf = my_buf;
}

// ‘ункци€ аллокации
void* myalloc(size_t size) {
  // head of linked list
  char* head = global_buf;

  size_t free_size = 0;
  bool is_allocated = false;

  while (head != NULL) {
    free_size = *get_left_size(head);
    if (free_size - FREE_INFO_SIZE >= size + ALLOC_INFO_SIZE) {
      is_allocated = true;
      break;
    }
    head = *get_next_ptr(head);
    //head = *(char**)(head + 1 + sizeof(size_t));
  }
  
  if (!is_allocated) {
    return NULL;
  }

  // === alloc part ===
  size_t alloc_size = size + ALLOC_INFO_SIZE;
  char* alloc_mem = head + free_size - alloc_size;

  // tags
  //*(alloc_mem + 0) = 1;
  //*(alloc_mem + alloc_size - 1) = 1;
  *get_left_tag(alloc_mem) = 1;
  *get_right_tag(alloc_mem, alloc_size) = 1;

  // sizes
  //*(size_t*)(alloc_mem + 1) = alloc_size;
  //*(size_t*)(alloc_mem + alloc_size - 1 - sizeof(size_t)) = alloc_size;
  *get_left_size(alloc_mem) = alloc_size;
  *get_right_size(alloc_mem, alloc_size) = alloc_size;

  // === free part ===
  free_size -= alloc_size;

  // tags
  //*(head + 0) = 0;
  //*(head + free_size - 1) = 0;
  *get_left_tag(head) = 0;
  *get_right_tag(head, free_size) = 0;

  // sizes
  //*(size_t*)(head + 1) = free_size;
  //*(size_t*)(head + free_size - 1 - sizeof(size_t)) = free_size;
  *get_left_size(head) = free_size;
  *get_right_size(head, free_size) = free_size;

  return alloc_mem + 1 + sizeof(size_t);
}

// ‘ункци€ освобождени€
void myfree(void* p) {
  char* alloc_mem = (char*)p;
  alloc_mem -= (1 + sizeof(size_t));

  //int alloc_size = *(size_t*)(alloc_mem + 1);
  int alloc_size = *get_left_size(alloc_mem);

  //char* left_tag = (alloc_mem + 0);
  char* left_tag = get_left_tag(alloc_mem);

  //char* right_tag = (alloc_mem + alloc_size - 1);
  char* right_tag = get_right_tag(alloc_mem, alloc_size);

  // ≈сли справа и слева зан€то
  if ((left_tag == init_buf || *(left_tag - 1) == 1) && (right_tag == init_buf + size_init_buf - 1 || *(right_tag + 1) == 1)) {
    // tags
    *left_tag = 0;
    *right_tag = 0;

    // ptrs
    *get_next_ptr(alloc_mem) = global_buf;
    *get_prev_ptr(alloc_mem) = NULL;

    *get_prev_ptr(global_buf) = alloc_mem;
    global_buf = alloc_mem;

    //*(char**)(alloc_mem + 1 + sizeof(size_t)) = global_buf;
    //*(char**)(alloc_mem + 1 + 2 * sizeof(size_t)) = NULL;
    //*(char**)(global_buf + 1 + 2 * sizeof(size_t)) = alloc_mem;
    //global_buf = alloc_mem;
  }
  // ≈сли слева свободно, а справа зан€то
  else if ((left_tag != init_buf && *(left_tag - 1) == 0) /*&& (right_tag == init_buf + size_init_buf - 1 || *(right_tag + 1) == 1)*/) {
    size_t free_size = *(size_t*)(alloc_mem - 1 - sizeof(size_t));
    char* free_mem = alloc_mem - free_size;
    free_size += alloc_size;

    *right_tag = 0;
    *get_left_size(free_mem) = free_size;
    *get_right_size(free_mem, free_size) = free_size;

    //*(size_t*)(free_mem + 1) = free_size;
    //*(size_t*)(right_tag - sizeof(size_t)) = free_size;
  }
  // ≈сли слева зан€то, а справа свободно
  /*else if ((left_tag == init_buf || *(left_tag - 1) == 1) && (right_tag != init_buf + size_init_buf - 1 && *(right_tag + 1) == 0)) {
    char* free_mem = right_tag + 1;

    //size_t free_size = *(size_t*)(free_mem + 1);
    size_t free_size = *get_left_size(free_mem);
    size_t new_free_size = free_size + alloc_size;

    //char* next_ptr = *(char**)(free_mem + 1 + sizeof(size_t));
    //char* prev_ptr = *(char**)(free_mem + 1 + 2 * sizeof(size_t));
    char* next_ptr = *get_next_ptr(free_mem);
    char* prev_ptr = *get_prev_ptr(free_mem);

    if (prev_ptr != NULL) {
      *(char**)(prev_ptr + 1 + sizeof(size_t)) = alloc_mem;
    }

    if (next_ptr != NULL) {
      *(char**)(next_ptr + 1 + 2 * sizeof(size_t)) = alloc_mem;
    }

    // tags
    *left_tag = 0;

    // sizes
    //*(size_t*)(alloc_mem + 1) = new_free_size;
    //*(size_t*)(free_mem + free_size - 1 - sizeof(size_t)) = new_free_size;
    *get_left_size(alloc_mem) = new_free_size;
    *get_right_size(alloc_mem, new_free_size) = new_free_size;

    // ptrs
    *(char**)(alloc_mem + 1 + sizeof(size_t)) = next_ptr;
    *(char**)(alloc_mem + 1 + 2 * sizeof(size_t)) = prev_ptr;
  }*/
  // ≈сли свободно и справа и слева
  /*else {

  }*/
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
  myfree(vec[12]);
  myfree(vec[1]);
  myfree(vec[10]);
  myfree(vec[2]);
  myfree(vec[11]);
  myfree(vec[3]);
  myfree(vec[9]);
  myfree(vec[4]);
  myfree(vec[8]);
  myfree(vec[5]);
  myfree(vec[7]);
  myfree(vec[6]);
  
  std::cout << "abc";

  /*char* space1 = myalloc_test(16, 0x41);
  char* space2 = myalloc_test(16, 0x42);
  char* space3 = myalloc_test(16, 0x43);*/
  
  /*myfree(space3);
  myfree(space2);
  myfree(space1);*/

  //std::cout << space1 << '\n' << space2;
  delete [] buf;

  int value = std::getchar();
  return 0;
}
