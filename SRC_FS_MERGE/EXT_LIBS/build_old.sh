#!/bin/bash

#libssh2
cd ./libssh2
mkdir bin
cd bin
cmake clean
cmake -DBUILD_SHARED_LIBS=ON -DDCRYPTO_BACKEND=OpenSSL -DENABLE_ZLIB_COMPRESSION=ON ..
cmake --build .
cp ./src/libssh2.so ../../BUILT_LIBS/libssh2_s.so
cd ../..

#libz
cd ./zlib
mkdir bin
cd bin
cmake clean
cmake ..
cmake --build .
cp ./libz.so ../../BUILT_LIBS/libz_s.so
cd ../..

#openssl
cd ./openssl
./config
make
cp ./libcrypto.a ../BUILT_LIBS/libcrypto_s.a
cd ..

#openaes
cd ./openaes
cmake clean
cmake .
cmake --build .
cp ./liboaes_lib.a ../BUILT_LIBS/liboaes_s.a
cd ..
