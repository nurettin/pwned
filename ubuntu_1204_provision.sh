#!/usr/bin/env bash
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt-get update -qq
sudo apt-get install -qq phantomjs cmake g++-4.8 libgflags-dev libboost-log1.48-dev libboost-iostreams1.48-dev libboost-filesystem1.48-dev libboost-system1.48-dev libboost-chrono1.48-dev libboost1.48-dev libgtest-dev libhtmlcxx-dev libcurl4-openssl-dev libjsoncpp-dev libleveldb-dev libsnappy-dev libprotobuf-dev protobuf-compiler
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 50
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 50
./gtest_provision.sh

