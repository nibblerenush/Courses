#include <cstdint>
#include <new>

struct Barrier {
  bool isFree = false;
  std::size_t size = 0;
};

struct Node {
  Node* prev = nullptr;
  Node* next = nullptr;
};

class Allocator {
public:
  void SetBuffer(void* buf, std::size_t size) {
    std::uint8_t* buffer = static_cast<std::uint8_t*>(buf);

    // left_barrier
    new (buffer) Barrier{ true, size };

    // right_barrier
    new (buffer + size - sizeof(Barrier)) Barrier{ true, size };

    // first node
    m_free_head = new (buffer + sizeof(Barrier)) Node{ nullptr, nullptr };

    // ”становка границ пам€ти
    m_begin = buffer;
    m_end = m_begin + size;
  }

  void* GetMemory(std::size_t size) {
    Node* cur_node = m_free_head;
    while (cur_node) {
      std::uint8_t* raw_free_lb = reinterpret_cast<std::uint8_t*>(cur_node) - sizeof(Barrier);
      Barrier* free_lb = reinterpret_cast<Barrier*>(raw_free_lb);
      
      // ќтрываем кусок пам€ти с конца свободного блока
      if (free_lb->size >= size + 2 * sizeof(Barrier) + sizeof(Node) + 2 * sizeof(Barrier)) {
        std::uint8_t* raw_free_rb = raw_free_lb + free_lb->size - sizeof(Barrier);
        Barrier* free_rb = reinterpret_cast<Barrier*>(raw_free_rb);
        
        // ѕравый барьер становитс€ зан€тым
        std::uint8_t* raw_alloc_rb = raw_free_rb;
        Barrier* alloc_rb = free_rb;

        alloc_rb->isFree = false;
        alloc_rb->size = size + 2 * sizeof(Barrier);
        
        std::uint8_t* alloc_memory = raw_alloc_rb - size - sizeof(Barrier);
        new (alloc_memory) Barrier{ *alloc_rb };

        // ќбновл€ем информацию по свободному куску пам€ти
        free_lb->size -= alloc_rb->size;
        std::uint8_t* new_free_raw_rb = alloc_memory - sizeof(Barrier);
        new (new_free_raw_rb) Barrier{ *free_lb };

        // ¬озвращаем кусок пам€ти без метаданных
        return static_cast<void*>(alloc_memory + sizeof(Barrier));
      }

      cur_node = cur_node->next;
    }

    return nullptr;
  }

  void FreeMemory(void* buf) {
    std::uint8_t* alloc_memory = reinterpret_cast<std::uint8_t*>(buf);

    std::uint8_t* raw_alloc_lb = alloc_memory - sizeof(Barrier);
    Barrier* alloc_lb = reinterpret_cast<Barrier*>(raw_alloc_lb);

    std::uint8_t* raw_alloc_rb = alloc_memory + alloc_lb->size - 2 * sizeof(Barrier);
    Barrier* alloc_rb = reinterpret_cast<Barrier*>(raw_alloc_rb);

    std::uint8_t* new_chunk = raw_alloc_lb;
    std::size_t new_size = alloc_lb->size;

    // ≈сли слева свободно
    if (raw_alloc_lb != m_begin && (alloc_lb - 1)->isFree) {
      Barrier* free_rb = alloc_lb - 1;

      std::uint8_t* raw_free_lb = raw_alloc_lb - free_rb->size;
      Barrier* free_lb = reinterpret_cast<Barrier*>(raw_free_lb);

      new_chunk = raw_free_lb;
      new_size += free_lb->size;

      Node* rm_node = reinterpret_cast<Node*>(raw_free_lb + sizeof(Barrier));
      RemoveNode(rm_node);
    }

    // ≈сли справа свободно
    if ((raw_alloc_rb + sizeof(Barrier)) < m_end && (alloc_rb + 1)->isFree) {
      Barrier* free_lb = alloc_rb + 1;
      std::uint8_t* raw_free_lb = reinterpret_cast<std::uint8_t*>(free_lb);

      new_size += free_lb->size;

      Node* rm_node = reinterpret_cast<Node*>(raw_free_lb + sizeof(Barrier));
      RemoveNode(rm_node);
    }

    // left_barrier
    new (new_chunk) Barrier{ true, new_size };

    // right_barrier
    new (new_chunk + new_size - sizeof(Barrier)) Barrier{ true, new_size };

    // new head
    Node* new_head = new (new_chunk + sizeof(Barrier)) Node{ nullptr, m_free_head };
    if (m_free_head) {
      m_free_head->prev = new_head;
    }

    m_free_head = new_head;
  }

private:
  void RemoveNode(Node* node) {
    if (node) {
      if (node->next) {
        node->next->prev = node->prev;
      }

      if (node->prev) {
        node->prev->next = node->next;
      }
      else {
        m_free_head = node->next;
      }
    }
  }

private:
  Node* m_free_head = nullptr;
  std::uint8_t* m_begin = nullptr;
  std::uint8_t* m_end = nullptr;
};

Allocator allocator;

// Ёта функци€ будет вызвана перед тем как вызывать myalloc и myfree
    // используйте ее чтобы инициализировать ваш аллокатор перед началом
    // работы.
    //
    // buf - указатель на участок логической пам€ти, который ваш аллокатор
    //       должен распредел€ть, все возвращаемые указатели должны быть
    //       либо равны NULL, либо быть из этого участка пам€ти
    // size - размер участка пам€ти, на который указывает buf
void mysetup(void* buf, std::size_t size) {
  allocator.SetBuffer(buf, size);
}

// ‘ункци€ аллокации
void* myalloc(std::size_t size) {
  return allocator.GetMemory(size);
}

// ‘ункци€ освобождени€
void myfree(void* p) {
  allocator.FreeMemory(p);
}

int main() {
  return 0;
}
