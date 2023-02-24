#include <Core/RateLimit/RateController.h>

namespace EFG
{
namespace Core
{

  RateController::RateController(int64_t _rate_limit_count, int64_t _rate_limit_interval_ms):
    rate_limit_count(_rate_limit_count), rate_limit_interval_ms(_rate_limit_interval_ms)
  {

  }
  bool RateController::allow()
  {
    using namespace std::chrono;
    auto ts = time_point_cast<milliseconds>(system_clock::now()).time_since_epoch();
    //std::chrono::nanoseconds ts = std::chrono::duration_cast< std::chrono::nanoseconds >(std::chrono::system_clock::now().time_since_epoch());
    int64_t now = ts.count();
    int64_t window_start_ts = now - rate_limit_interval_ms;
    while(!timestamps_log.empty() && timestamps_log.front() <= window_start_ts) // remove the older timestamp which fall outside the window 
    {
      timestamps_log.pop();
    }
    timestamps_log.push(now);
    return timestamps_log.size()<=rate_limit_count; 
  }


}
}
