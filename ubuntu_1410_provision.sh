#!/usr/bin/env bash
sudo apt-get update
sudo apt-get install flex phantomjs cmake g++-4.8 libsqlite3-dev libgflags-dev libboost-log1.55-dev libboost-iostreams1.55-dev libboost-filesystem1.55-dev libboost-system1.55-dev libboost-chrono1.55-dev libboost1.55-dev libgtest-dev libhtmlcxx-dev libcurl4-openssl-dev libjsoncpp-dev libleveldb-dev libsnappy-dev libprotobuf-dev protobuf-compiler
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 50
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 50
./gtest_provision.sh
