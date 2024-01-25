#!/bin/bash

curl -L https://sourceforge.net/projects/boost/files/boost/1.84.0/boost_1_84_0.tar.gz/download > ./lib/boost_1_84.tar.gz

tar --bzip2 -xf ./lib/boost_1_84.tar.gz -C ./lib

cd lib/boost_1_84_0/
./bootstrap.sh --prefix=/usr/local/


# ./lib/boost_1_84_0/bootstrap.sh --prefix=./lib

# rm ./lib/boost_1_84.tar.gz ./lib/boost_1_84