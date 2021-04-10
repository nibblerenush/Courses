#include <cstdint>
#include <iostream>
#include <map>
#include <queue>
#include <string>

struct Event
{
  std::int64_t time;
  std::string hotel;
  int client_id;
  int room_count;
};

struct HotelContent
{
  int all_room_count = 0;
  std::map<int, int> client_id_event_count;
};

class HotelManager
{
public:
  void Book(const std::int64_t& current_time, const std::string& current_hotel, const int& client_id, const int& room_count)
  {
    while (!m_event_queue.empty() && (m_event_queue.front().time <= current_time - 86400))
    {
      const std::string& old_hotel = m_event_queue.front().hotel;
      
      const int& old_room_count = m_event_queue.front().room_count;
      m_hotels[old_hotel].all_room_count -= old_room_count;
      
      const int& old_client_id = m_event_queue.front().client_id;
      auto& old_client_id_event_count = m_hotels[old_hotel].client_id_event_count;
      if (--old_client_id_event_count[old_client_id] == 0)
      {
        old_client_id_event_count.erase(old_client_id);
      }

      m_event_queue.pop();
    }

    m_hotels[current_hotel].all_room_count += room_count;
    m_hotels[current_hotel].client_id_event_count[client_id]++;
    m_event_queue.push({current_time, current_hotel, client_id, room_count});
  }

  std::size_t Clients(const std::string& current_hotel)
  {
    return m_hotels[current_hotel].client_id_event_count.size();
  }

  int Rooms(const std::string& current_hotel)
  {
    return m_hotels[current_hotel].all_room_count;
  }

private:
  std::queue<Event> m_event_queue;
  std::map<std::string, HotelContent> m_hotels;
};

int main()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  
  HotelManager hotel_manager;

  int Q;
  std::cin >> Q;

  for (int query_count = 0; query_count < Q; ++query_count)
  {
    std::string command;
    std::cin >> command;

    if (command == "BOOK")
    {
      std::string current_hotel;
      std::int64_t current_time;
      int client_id;
      int room_count;
      std::cin >> current_time>> current_hotel >> client_id >> room_count;
      hotel_manager.Book(current_time, current_hotel, client_id, room_count);
    }
    else if (command == "CLIENTS")
    {
      std::string current_hotel;
      std::cin >> current_hotel;
      std::cout << hotel_manager.Clients(current_hotel) << '\n';
    }
    else if (command == "ROOMS")
    {
      std::string current_hotel;
      std::cin >> current_hotel;
      std::cout << hotel_manager.Rooms(current_hotel) << '\n';
    }
  }
}
