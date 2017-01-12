#!/bin/bash

LIB_LOC="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

if [ ! -e $LIB_LOC/BUILT_LIBS/libz_s.so ]; then
	echo "libz not found"
	
	#libz
	cd zlib
	mkdir bin
	cd bin
	cmake clean
	cmake ..
	cmake --build .
	#make
	#TEMPORARY UNTIL HEADER WRITTEN
	#make install
	cp ./libz.so ../../BUILT_LIBS/libz_s.so
	cmake clean
	cd ..
	rm -r bin
	cd ..
fi

if [ ! -e $LIB_LOC/BUILT_LIBS/libssl_s.so ] || [ ! -e $LIB_LOC/BUILT_LIBS/libcrypto_s.so  ]; then
	echo "libssl not found"
	
	#openssl
	cd openssl
	make clean
	mkdir _post
	./config --prefix=$LIB_LOC/openssl/openssl_build --openssldir=$LIB_LOC/openssl/openssl_build/_post
	make
	#TEMPORARY UNTIL HEADER WRITTEN
	#make install
	cp ./libcrypto.so ../BUILT_LIBS/libcrypto_s.so
	cp ./libssl.so ../BUILT_LIBS/libssl_s.so
	make clean
	cd ..
fi
