#include <Core/Lock/SpinLock.h>
#include <mutex>

using namespace EFG::Core;

template<typename EventType, int size>
class DummyQueue
{
  public:
    void push(EventType&& event)
    {
      std::lock_guard<SpinLock> lock(_syncRequest);
      //push implementation
    }
    void pop()
    {
      std::lock_guard<SpinLock> lock(_syncRequest);
      // pop implementation
    }
  private:
    SpinLock _syncRequest;
      
};

int main(int argc, char *argv[], char *envp[])
{

  DummyQueue<int, 1024> dummyQueue;
  // concurrent push and pop from different thread 
  dummyQueue.push(10);
  dummyQueue.pop(); 
  return 0;
}

