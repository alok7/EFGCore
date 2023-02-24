#pragma once

#include <Core/Message/Message.h>

namespace EFG
{
namespace Core
{
namespace Message
{

template<typename EventType, size_t Capacity>
class EventCenter
{
public:
  template<typename F>
  void visit(const InstrumentId& id, size_t N, F&& f) const
  {
    auto it = mBuffer.find(id);
    if(it!=mBuffer.end())
    {
      (it->second)->visit(N, f);        
    } 
  }
  template<typename F>
  void rvisit(const InstrumentId& id, size_t N, F&& f) const
  {
    auto it = mBuffer.find(id);
    if(it!=mBuffer.end())
    {
      (it->second)->rvisit(N, f);        
    } 
  }
  template<typename F, typename PRED>
  void rvisit(const InstrumentId& id, PRED& pred, F&& f) const
  {
    auto it = mBuffer.find(id);
    if(it!=mBuffer.end())
    {
      (it->second)->rvisit(pred, f);        
    } 
  }
  template<typename PRED>
  size_t visitWindowSize(const InstrumentId& id, PRED& pred) const
  {
    auto it = mBuffer.find(id);
    if(it!=mBuffer.end())
    {
      return (it->second)->visitWindowSize(pred);        
    }
    return 0; 
  }
  const size_t size(const InstrumentId& id) const
  {
    auto it = mBuffer.find(id);
    return it!=mBuffer.end() ? (it->second)->size() : 0;
  }
  auto& peek(const InstrumentId& id) const
  {
    auto it = mBuffer.find(id);
    return (it->second)->peek();
  }	  
  auto& peek(const InstrumentId& id, size_t window) const
  {
    auto it = mBuffer.find(id);
    return (it->second)->peek(window);
  }	  
protected:
  std::unordered_map<InstrumentId, std::unique_ptr<RingBuffer<EventType, Capacity>>, InstrumentIdHash> mBuffer;
}; 


}
}
}
