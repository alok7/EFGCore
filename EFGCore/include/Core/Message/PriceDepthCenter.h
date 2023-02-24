#pragma once

#include <Core/Message/Message.h>
#include <Core/Message/EventCenter.h>

namespace EFG
{
namespace Core
{
namespace Message
{

template<typename SamplingTimePeriod, typename TimeHorizon = TimeUnits::Hours<12>, bool Sampled=false>
class PriceDepthCenter : public EventCenter<DepthBook,
                                            Utils::getNextPowerOftwo(TimeHorizon::Units/SamplingTimePeriod::Units)>
{
public:
  using SamplingTime = SamplingTimePeriod;
  using EventType    = DepthBook;
  static const constexpr size_t mSamples = Utils::getNextPowerOftwo(TimeHorizon::Units/SamplingTime::Units);
  using Base = EventCenter<EventType,mSamples>;
  void onEvent(const DepthBook& depth)
  {
    uint64_t ts = depth.apiIncomingTime;
    auto it = Base::mBuffer.find(depth.mInstrumentId);
    if(it!=Base::mBuffer.end())
    {
      auto& depths = *(it->second);
      auto& last_depth = depths.peek();
      if(((ts - depths.mLastUpdateTime) >= SamplingTime::Units) || !Sampled)
      {
        depths.insert(depth);
        depths.mLastUpdateTime  = ts;
      }
    }
    else
    {
      auto new_depths = std::make_unique<RingBuffer<DepthBook, mSamples>>();
      new_depths->mLastUpdateTime  = ts;
      new_depths->insert(depth);
      Base::mBuffer.insert({depth.mInstrumentId, std::move(new_depths)});
    } 
  } 
}; 


}
}
}
