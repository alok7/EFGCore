#!/bin/bash

sudo apt update
sudo apt upgrade

sudo apt -y install build-essential gdb autoconf \
                    cmake \
                    openssl \
                    libssl-dev \
                    libuv1-dev \
                    libc-ares2 libc-ares-dev \
                    zlib1g-dev \
                    txt2man \
                    libarmadillo-dev \
                    debhelper \
                    libmpd-dev

#                    doxygen \
#                    git \
#                    python3 python3-pip
#                    libboost-math-dev \
#                    libboost-program-options-dev \
#                    libboost-test-dev \
#                    libboost-serialization-dev \

pushd ~/

# uWebSockets
git clone -b v0.14 https://github.com/uNetworking/uWebSockets.git
pushd uWebSockets
make -j8
sudo make install
sudo ln -s /usr/lib64/libuWS.so /usr/lib/libuWS.so
popd

## mlpack
#wget https://www.mlpack.org/files/mlpack-3.4.2.tar.gz
#tar -xzf mlpack-3.4.2.tar.gz
#pushd mlpack-3.4.2
#mkdir build
#pushd build
#cmake -DBUILD_TESTS=OFF -DBUILD_PYTHON_BINDINGS=OFF -DBUILD_JULIA_BINDINGS=OFF -DBUILD_GO_BINDINGS=OFF -DBUILD_R_BINDINGS=OFF -DDEBUG=OFF -DPROFILE=OFF ..
#make -j8
#sudo make install
#popd
#popd

# curl with c-ares
git clone https://github.com/curl/curl.git
pushd curl
autoreconf -fi
CPPFLAGS="-I/usr/include/openssl/" LDFLAGS="-L/usr/lib/x86_64-linux-gnu/" ./configure --enable-ares --enable-versioned-symbols -with-openssl
make -j8
sudo make install
popd

## python3 packages
#pip3 install numpy pandas matplotlib argparse configparser

popd
