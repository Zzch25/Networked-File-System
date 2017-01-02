#!/bin/bash
#Zachary Job

LIB_LOC=./EXT_LIBS/BUILT_LIBS
libs_exist=true

if [ ! -f $LIB_LOC/libssh2_s.so ]; then
	libs_exist=false
fi
if [ ! -f $LIB_LOC/libz_s.so ]; then
	libs_exist=false
fi
if [ ! -f $LIB_LOC/libaes_s.a ]; then
	libs_exist=false
fi
if [ ! -f $LIB_LOC/libcrypto_s.a ]; then
	libs_exist=false
fi

if [ $libs_exist == false ]; then
	echo "A library was not found, building..."
	cd ./EXT_LIBS
	./build.sh
	cd ..
fi

make
