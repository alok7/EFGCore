#include <iomanip>
#include <iostream>
#include <cstdint>
#include <optional>
#include <Core/csvUtils/fastReader.h>

using namespace EFG::Core;

int main(int argc, char *argv[], char *envp[]){

  if(argc < 2){

  }
  else{
    std::string csvFilepath(argv[1]);
    CSV::Reader<10> eventBufferReader;
    bool init = eventBufferReader.init(csvFilepath,
                      "marketId",
                      "instrumetId",
                      "orderType",
                      "price",
                      "qty",
                      "clientOid",
                      "orderId",
                      "side",
                      "tif",
                      "orderAction"
                      );

    if(init)
    {
      std::string marketId, instrumentId, orderType;
      double price, qty;
      std::string clientOrderId, orderId, tif;
      std::string side, orderAction;
      while (eventBufferReader.readRow(marketId, instrumentId, orderType,
                                    price, qty, clientOrderId, orderId, side, tif, orderAction))
      {
        std::cout << " marketId " << marketId << " instrumentId " << instrumentId << " orderType " << orderType
                << " price " << price << " qty " << qty << " clientOrderId " << clientOrderId << " orderId " << orderId
                << " side " << side << '\n';
      }
    }
  }

  return 0;

}

