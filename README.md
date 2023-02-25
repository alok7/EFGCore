# A Core library for quant and Algo trading 

This repository has implementations for the core components and modules required to develop an Algo & Quant system for trading, research and backtesting 

## Features 
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
cmake .. \ 
-DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

If you want to install EFGCore for external use
- Install in directory

```shell
cmake --install . --prefix <install-folder>

Example:
mkir lib
cd build
cmake --install . --prefix ../lib 
```

- Install system wide
```shell
cmake --install . 
```

## Use

Example:
 
