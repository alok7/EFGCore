#pragma once
#include <atomic>
#include <memory>
#include <thread>
#include <sched.h>

namespace EFG
{
namespace Core
{
#define LIKELY(x) __builtin_expect((x), 1)
#define UNLIKELY(x) __builtin_expect((x), 0)

class SpinLock
{
  public:
    inline void lock() noexcept 
    {
      for(uint8_t count= 0; !try_lock(); ++count)
      {
        count < 16 ? __builtin_ia32_pause() : std::this_thread::yield();
        count = (count&(count&15));
      } 	
    }
    inline void unlock() noexcept
    {
      mLock.store(false, std::memory_order_release);  
    }
  private:
    inline bool try_lock() noexcept
    {
      return !mLock.load(std::memory_order_relaxed) && !mLock.exchange(true, std::memory_order_acquire);
    }
    std::atomic<bool> mLock {false};    
};

}
}
