#include <cmath>
#include <cstdint>
#include <new>

/**
 * Эти две функции вы должны использовать для аллокации
 * и освобождения памяти в этом задании. Считайте, что
 * внутри они используют buddy аллокатор с размером
 * страницы равным 4096 байтам.
 **/

 /**
  * Аллоцирует участок размером 4096 * 2^order байт,
  * выровненный на границу 4096 * 2^order байт. order
  * должен быть в интервале [0; 10] (обе границы
  * включительно), т. е. вы не можете аллоцировать больше
  * 4Mb за раз.
  **/
void* alloc_slab(int order) {
  std::size_t size = static_cast<std::size_t>(std::pow(2, 12 + order));
  return reinterpret_cast<void*>(new std::uint8_t[size]);
}

/**
 * Освобождает участок ранее аллоцированный с помощью
 * функции alloc_slab.
 **/
void free_slab(void* slab) {
  delete[] slab;
}

struct Object {
  Object* next = nullptr;
};

struct SlabHeader {
  SlabHeader* prev = nullptr;
  SlabHeader* next = nullptr;
  std::size_t free_object_count = 0;
  Object* object_head = nullptr;
};

/**
 * Эта структура представляет аллокатор, вы можете менять
 * ее как вам удобно. Приведенные в ней поля и комментарии
 * просто дают общую идею того, что вам может понадобится
 * сохранить в этой структуре.
 **/
struct cache {
  /* список пустых SLAB-ов для поддержки cache_shrink */
  SlabHeader* empty_slabs = nullptr;

  /* список частично занятых SLAB-ов */
  SlabHeader* partially_full_slabs = nullptr;

  /* список заполненых SLAB-ов */
  SlabHeader* full_slabs = nullptr;

  size_t object_size; /* размер аллоцируемого объекта */
  int slab_order; /* используемый размер SLAB-а */
  size_t slab_objects; /* количество объектов в одном SLAB-е */
};

SlabHeader* CreateNewSlab(const struct cache* cache) {
  std::uint8_t* slab = reinterpret_cast<std::uint8_t*>(alloc_slab(cache->slab_order));

  // 2^12 = 4096
  std::size_t slab_size = static_cast<std::size_t>(std::pow(2, 12 + cache->slab_order));

  SlabHeader* slab_header = new (slab) SlabHeader;
  slab_header->free_object_count =
    (slab_size - sizeof(SlabHeader)) / (sizeof(Object) + cache->object_size);

  Object* cur_object = new (slab + sizeof(SlabHeader)) Object;
  slab_header->object_head = cur_object;

  for (std::size_t i = 1; i < slab_header->free_object_count; ++i) {
    Object* next_object =
      new (reinterpret_cast<std::uint8_t*>(cur_object) + sizeof(Object) + cache->object_size) Object;

    cur_object->next = next_object;
    cur_object = next_object;
  }

  return slab_header;
}

/**
 * Функция инициализации будет вызвана перед тем, как
 * использовать это кеширующий аллокатор для аллокации.
 * Параметры:
 *  - cache - структура, которую вы должны инициализировать
 *  - object_size - размер объектов, которые должен
 *    аллоцировать этот кеширующий аллокатор
 **/
void cache_setup(struct cache* cache, size_t object_size) {
  cache->object_size = object_size;
  cache->slab_order = 10;

  cache->empty_slabs = CreateNewSlab(cache);
  cache->partially_full_slabs = nullptr;
  cache->full_slabs = nullptr;

  cache->slab_objects = cache->empty_slabs->free_object_count;
}

/**
 * Функция освобождения будет вызвана когда работа с
 * аллокатором будет закончена. Она должна освободить
 * всю память занятую аллокатором. Проверяющая система
 * будет считать ошибкой, если не вся память будет
 * освбождена.
 **/
void cache_release(struct cache* cache) {
  for (SlabHeader* slab_header :
    { cache->empty_slabs, cache->partially_full_slabs, cache->full_slabs })
  {
    while (slab_header) {
      SlabHeader* next_slab_header = slab_header->next;
      free_slab(reinterpret_cast<void*>(slab_header));
      slab_header = next_slab_header;
    }
  }
}

bool MoveSlab(SlabHeader*& from, SlabHeader*& to) {
  bool is_head_changed = false;

  if (from) {
    SlabHeader* temp_from = from;

    if (from->next) {
      from->next->prev = from->prev;
    }

    if (from->prev) {
      from->prev->next = from->next;
    }
    else {
      from = from->next;
      is_head_changed = true;
    }

    temp_from->next = to;
    temp_from->prev = nullptr;
    if (to) {
      to->prev = temp_from;
    }
    to = temp_from;
  }

  return is_head_changed;
}

/**
 * Функция аллокации памяти из кеширующего аллокатора.
 * Должна возвращать указатель на участок памяти размера
 * как минимум object_size байт (см cache_setup).
 * Гарантируется, что cache указывает на корректный
 * инициализированный аллокатор.
 **/
void* cache_alloc(struct cache* cache) {
  Object* result = nullptr;
  
  auto get_result = [](SlabHeader* slab_header) {
    Object* result = slab_header->object_head;
    slab_header->object_head = result->next;
    slab_header->free_object_count--;
    
    // Делаем грязный хак.
    // Сохраняем в указатель на Object* указатель на SlabHeader*.
    result->next = reinterpret_cast<Object*>(slab_header);
    return result;
  };
  
  if (cache->partially_full_slabs) {
    result = get_result(cache->partially_full_slabs);
    
    // В SLAB'е не осталось свободных блоков
    if (cache->partially_full_slabs->free_object_count == 0) {
      // Помещаем SLAB в список полностью занятых
      MoveSlab(cache->partially_full_slabs, cache->full_slabs);
    }
  }
  else if (cache->empty_slabs) {
    result = get_result(cache->empty_slabs);
    
    // Помещаем SLAB в список частично занятых
    MoveSlab(cache->empty_slabs, cache->partially_full_slabs);
  }
  else {
    cache->empty_slabs = CreateNewSlab(cache);
    return cache_alloc(cache);
  }
  
  return reinterpret_cast<std::uint8_t*>(result) + sizeof(Object);
}

/**
 * Функция освобождения памяти назад в кеширующий аллокатор.
 * Гарантируется, что ptr - указатель ранее возвращенный из
 * cache_alloc.
 **/
void cache_free(struct cache* cache, void* ptr) {
  std::uint8_t* buf = reinterpret_cast<std::uint8_t*>(ptr);

  Object* object = reinterpret_cast<Object*>(buf - sizeof(Object));
  // Делаем грязный хак.
  // Извлекаем из указателя на Object* указатель на SlabHeader*.
  SlabHeader* slab_header = reinterpret_cast<SlabHeader*>(object->next);

  slab_header->free_object_count++;
  object->next = slab_header->object_head;
  slab_header->object_head = object;

  if (slab_header->free_object_count == cache->slab_objects) {
    // Был в частично занятом.
    // Перемещаем в список свободных SLAB'об.
    if (MoveSlab(slab_header, cache->empty_slabs)) {
      cache->partially_full_slabs = slab_header;
    }
  }
  else if (slab_header->free_object_count == 1) {
    // Был в полностью занятом.
    // Перемещаем в список частично занятых SLAB'об.
    if (MoveSlab(slab_header, cache->partially_full_slabs)) {
      cache->full_slabs = slab_header;
    }
  }
}

/**
 * Функция должна освободить все SLAB, которые не содержат
 * занятых объектов. Если SLAB не использовался для аллокации
 * объектов (например, если вы выделяли с помощью alloc_slab
 * память для внутренних нужд вашего алгоритма), то освбождать
 * его не обязательно.
 **/
void cache_shrink(struct cache* cache) {
  SlabHeader* slab_header = cache->empty_slabs;
  while (slab_header) {
    SlabHeader* next_slab_header = slab_header->next;
    free_slab(reinterpret_cast<void*>(slab_header));
    slab_header = next_slab_header;
  }
  cache->empty_slabs = nullptr;
}

int main() {
  return 0;
}
