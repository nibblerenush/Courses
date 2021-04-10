#include <iostream>
#include <queue>
#include <vector>

struct Processor
{
  int index;
  long all_duration;
};

bool operator > (const Processor& lhs, const Processor& rhs)
{
  if (lhs.all_duration == rhs.all_duration) {
    return lhs.index > rhs.index;
  }
  else {
    return lhs.all_duration > rhs.all_duration;
  }
}

class ProcessorCompare
{
public:
  bool operator () (const Processor& lhs, const Processor& rhs) const {
    return lhs > rhs;
  }
};

int main()
{
  int processors_count;
  std::cin >> processors_count;
  std::priority_queue<Processor, std::vector<Processor>, ProcessorCompare> heap;
  for (int i = 0; i < processors_count; ++i) {
    heap.push(Processor{ i, 0 });
  }

  int tasks_count;
  std::cin >> tasks_count;
  for (int i = 0; i < tasks_count; ++i)
  {
    long time;
    std::cin >> time;
    
    auto processor = heap.top();
    heap.pop();
    
    std::cout << processor.index << ' ' << processor.all_duration << '\n';
    processor.all_duration += time;
    heap.push(processor);
  }

  return 0;
}
