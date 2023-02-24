#include <Core/MarketUtils/MarketUtils.h>

namespace Market
{
  char *get_timestamp()
  {
    static char buffer[50];

    // timestamp
    std::time_t nowtime = std::time(0);
    std::tm *timeinfo = std::gmtime(&nowtime);
    std::strftime(buffer, 50, "%Y-%m-%dT%H:%M:%S", timeinfo);

    return buffer;
  }

  //! escape non-url characters to %NN
  std::string url_encode(const std::string &value)
  {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i)
    {
      std::string::value_type c = (*i);

      // Keep alphanumeric and other accepted characters intact
      if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
      {
        escaped << c;
        continue;
      }

      // Any other characters are percent-encoded
      escaped << std::uppercase;
      escaped << '%' << std::setw(2) << int((unsigned char)c);
      escaped << std::nouppercase;
    }

    return escaped.str();
  }
}
