#!/usr/bin/env bash
sudo apt-get update
sudo apt-get install phantomjs cmake g++-4.8 libgflags-dev libboost-log-dev libboost-iostreams-dev libboost-filesystem-dev libboost-system-dev libboost-chrono-dev libboost-dev libgtest-dev libhtmlcxx-dev libcurl4-openssl-dev libjsoncpp-dev libleveldb-dev libsnappy-dev libprotobuf-dev protobuf-compiler
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 50
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 50
./gtest_provision.sh
