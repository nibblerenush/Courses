#include <iostream>

namespace RAII
{
  template <typename Provider>
  class Booking
  {
  public:
    Booking(Provider* provider, int counter) : m_provider(provider), m_counter(counter)
    {}
    
    Booking(const Booking&) = delete;
    Booking& operator = (const Booking&) = delete;

    Booking(Booking&& booking)
    {
      m_provider = booking.m_provider;
      booking.m_provider = nullptr;
    }

    Booking& operator = (Booking&& booking)
    {
      if (this != &booking)
      {
        m_provider = booking.m_provider;
        booking.m_provider = nullptr;
      }
      return *this;
    }
    
    ~Booking() {
      if (m_provider) {
        m_provider->CancelOrComplete(*this);
      }
    }
  
  private:
    Provider* m_provider;
    int m_counter;
  };
}
