#include <Core/Transport/HTTPRequest/HTTPConfig.h>

namespace EFG
{
namespace Core
{
namespace Transport
{
  const std::string& HTTPConfig::getHttpEndPoint() const {
    return _httpBaseEndPoint;
  }
  void HTTPConfig::setHttpEndpoint(const std::string& s) {
    _httpBaseEndPoint = s;
  }
} // namespace Transport  
} // namespace Core
} // namespace EFG
