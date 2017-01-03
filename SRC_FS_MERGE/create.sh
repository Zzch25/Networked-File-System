#!/bin/bash
#Zachary Job

LIB_LOC="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )/EXT_LIBS/BUILT_LIBS"
libs_exist=true

if [ ! -e "$LIB_LOC/libssh2_s.so" ]; then
	echo "LIBSSH2 NOT FOUND"
	libs_exist=false
fi
if [ ! -e $LIB_LOC/libz_s.so ]; then
	echo "LIBZ NOT FOUND"
	libs_exist=false
fi
if [ ! -e $LIB_LOC/liboaes_s.a ]; then
	echo "LIB OAES NOT FOUND"
	libs_exist=false
fi
if [ ! -e $LIB_LOC/libcrypto_s.a ]; then
	echo "LIBCRYPTO NOT FOUND"
	libs_exist=false
fi

#lazzzzzy
if [ $libs_exist == false ]; then
	echo "A library was not found, building..."	
	cd ./EXT_LIBS
	./build.sh
	cd ..
fi
	
make
