#pragma once

#include <Core/Message/Message.h>
#include <Core/Message/EventCenter.h>

namespace EFG
{
namespace Core
{
namespace Message
{

template<size_t N>
class TradeCenter : public EventCenter<Trade, Utils::getNextPowerOftwo(N)>
{
public:

  using EventType    = Trade;
  static const constexpr size_t mSamples = Utils::getNextPowerOftwo(N);
  using Base = EventCenter<EventType,mSamples>;
  void onEvent(const Trade& trade)
  {
    uint64_t ts = trade.apiIncomingTime;
    auto it = Base::mBuffer.find(trade.mInstrumentId);
    if(it!=Base::mBuffer.end())
    {
      auto& trades = *(it->second);
      trades.insert(trade);
      trades.mLastUpdateTime  = ts;
    }
    else
    {
      auto new_trades = std::make_unique<RingBuffer<Trade, N>>();
      new_trades->mLastUpdateTime  = ts;
      new_trades->insert(trade);
      Base::mBuffer.insert({trade.mInstrumentId, std::move(new_trades)});
    } 
  } 
}; 


}
}
}
