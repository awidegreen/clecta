#!/bin/sh
set -ex

# install gtest 
sudo apt-get install -qq libgtest-dev 

# compile gtest
GTEST_DIR="gtest"
cores=`nproc`
mkdir -p $GTEST_DIR

# prepare 
cd $GTEST_DIR
cmake -DCMAKE_BUILD_TYPE=RELEASE /usr/src/gtest/
make -j$cores && sudo mv libg* /usr/lib

