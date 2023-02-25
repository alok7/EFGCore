# A Core library for Quant and Algo trading 

This repository has implementations for the core components and modules required to develop an Algo & Quant system for trading, research and backtesting 

## Some of features and modules in the EFCore library
- Queues and Lock
- A generic Shared Memory Publisher & Subscriber 
- Market Events design and storage 
- Market Events DataStrutures like TradeCenter, PriceDepthCenter, TickerCenter etc...
- HTTP, WSS transport, TCP and UDP 
- Event Ringbuffer and a binary encoded shared memory Ringbuffer   
- Fast CSV reader 
- RateLimit, FixedString, Market Utils and Common Utils
- Instrument ids and InstrumentCenter 
- Bid & Ask OrderBook
- Matching Engine 
- And a tiny logger 


## Building and Installing
### Requirements
At minimum, compilation requires:
- A C++ compiler with good C++20 support (e.g. gcc/g++ >= 8)
- [CMake](https://cmake.org/) — version 3.20 or later, and ``make``
- A Linux-based operating system.

To build EFGCore from source with these dependencies, clone the repository:

```shell
git clone https://github.com/alok7/EFGCore.git && cd EFGCore
```
### Install Dependencies
```shell
./install.sh
```
```shell
cd EFGCore
mkdir -p build && cd build
```

Then build from source using CMake 
### cmake Build and Install
```shell
cmake .. 
make -j$(nproc)
```

If you want to install EFGCore for external use
- Install in directory

```shell
cmake --install . --prefix <install-folder>

Example:
mkdir lib
cd build
cmake --install . --prefix ../lib 
```

- Install system wide
```shell
cmake --install . 
```

## How to use EFGCore in your project 

Create CMakeLists.txt like below in your project

```cmake

cmake_minimum_required(VERSION 3.20)

project(Test VERSION 0.1)

set(EXECUTABLE ${PROJECT_NAME}-bin)
set(DEFAULT_BUILD_TYPE "Release")

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set compiler flags
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "${CMAKE_CXX_FLAGS} -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "${CMAKE_CXX_FLAGS} -O3")

add_executable(${EXECUTABLE} ${${PROJECT_NAME}_SRC} main.cpp)

find_package(EFGCore REQUIRED)
target_link_libraries(${EXECUTABLE} PRIVATE EFGCore)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${EXECUTABLE} PRIVATE Threads::Threads)

find_package(OpenSSL REQUIRED)
target_link_libraries(${EXECUTABLE} PRIVATE OpenSSL::SSL OpenSSL::Crypto)
```

Example: Shared Memory pub/sub


```cpp
//main.cpp

#include <iostream>
#include <Core/Transport/SHM/Publisher.h>
#include <Core/Transport/SHM/Subscriber.h>

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
  PUBLISHER publisher("event_feed", 65536); 
  SUBSCRIBER subscriber("event_feed", 65536); 
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

```



 
