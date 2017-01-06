#!/bin/bash
#Zachary Job

LIB_LOC="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )/EXT_LIBS/BUILT_LIBS"

libs_exist=true
if [ ! -e $LIB_LOC/libz_s.so ] || [ ! -e $LIB_LOC/libcrypto_s.so ] || [ ! -e $LIB_LOC/libssl_s.so ]; then
	echo "LIB NOT FOUND"
	libs_exist=false
fi

#lazzzzzy
if [ $libs_exist == false ]; then
	echo "A library was not found, building..."	
	cd ./EXT_LIBS
	#./build.sh
	cd ..
fi

if [ -e $LIB_LOC/libz_s.so ] || [ -e $LIB_LOC/libcrypto_s.so ] || [ -e $LIB_LOC/libssl_s.so ]; then	
	make
fi
