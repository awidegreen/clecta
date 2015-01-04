#!/usr/bin/env bash

# install gtest 
sudo apt-get install -qq libboost-all-dev libgtest-dev 

# compile gtest
GTEST_DIR="gtest"
mkdir -p $GTEST_DIR

# prepare 
cd $GTEST_DIR
cmake -DCMAKE_BUILD_TYPE=RELEASE /usr/src/gtest/
make -j3 && sudo mv libg* /usr/lib

