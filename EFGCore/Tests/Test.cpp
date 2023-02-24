#include <iostream>
#include <Core/Transport/SHM/Publisher.h>
#include <Core/Transport/SHM/Subscriber.h>

class Application
{
 public:
   void runTests()
   {

   }

};

struct EventA
{
  const static size_t mId = 1;
};
struct EventB
{
  const static size_t mId = 2;
};
struct EventC
{
  const static size_t mId = 3;
};

struct EventD
{
  const static size_t mId = 4;
};

int main()
{
  using PUBLISHER =  EFG::Core::Publisher<EventA, EventC>;	
  using SUBSCRIBER = EFG::Core::Subscriber<EventA, EventC>;	
  PUBLISHER publisher("ftx_feed", 65536); 
  SUBSCRIBER subscriber("ftx_feed", 65536); 
  auto eventAReader = [](const EventA& a){ std::cout << "eventA received from update channel, mId " <<  a.mId << std::endl;};
  auto eventCReader = [](const EventC& c){ std::cout << "eventC received from update channel, mId " <<  c.mId << std::endl;};
  subscriber.start(eventAReader, eventCReader);
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(200ms);
  EventA event1;
  EventC event2;
  for(int i =0; i < 1000000; ++i)
  {
    publisher.publish(event1);
    publisher.publish(event2);
  }  
  return 0;
}
