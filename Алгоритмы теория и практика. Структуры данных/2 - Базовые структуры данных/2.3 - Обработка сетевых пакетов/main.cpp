#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

struct Event
{
  int index;
  int arrival;
  int duration;
};

class NetworkPacketHandler
{
public:
  NetworkPacketHandler(int buffer_size, int packet_count) :
    m_buffer_size(buffer_size), m_result(packet_count)
  {}

  void Handle(Event new_event)
  {
    QueueClear(new_event.arrival);
    
    if (m_queue.empty())
    {
      m_start = new_event.arrival;
      m_queue.push(new_event);
    }
    else if (m_queue.size() < static_cast<std::size_t>(m_buffer_size))
    {
      m_queue.push(new_event);
    }
    else
    {
      m_result[new_event.index] = -1;
    }
  }

  void GetResult()
  {
    QueueClear(std::numeric_limits<int>::max());
    
    for (const auto& result : m_result) {
      std::cout << result << '\n';
    }
  }

private:
  void QueueClear(int time)
  {
    while (!m_queue.empty())
    {
      Event top_event = m_queue.front();
      if (m_start + top_event.duration <= time)
      {
        m_result[top_event.index] = m_start;
        m_start += top_event.duration;
        m_queue.pop();
      }
      else {
        break;
      }
    }
  }

private:
  int m_start = 0;
  std::queue<Event> m_queue;

  int m_buffer_size;
  std::vector<int> m_result;
};

int main()
{
  int buffer_size;
  int packet_count;

  std::cin >> buffer_size >> packet_count;
  NetworkPacketHandler nph(buffer_size, packet_count);

  for (int i = 0; i < packet_count; ++i)
  {
    Event new_event;
    new_event.index = i;
    std::cin >> new_event.arrival >> new_event.duration;
    nph.Handle(std::move(new_event));
  }
  nph.GetResult();

  return 0;
}
