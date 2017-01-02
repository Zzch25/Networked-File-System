NAME = test
SRC = \
		mgr.cpp

WORKING_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
LIBRARY_DIRS:=-L./EXT_LIBS/BUILT_LIBS
LIBRARIES:=-loaes_s -lssh2_s -lcrypto_s -lz_s

all:
	g++ -ggdb -std=c++11 $(LIBRARY_DIRS) $(SRC) -o $(NAME) $(LIBRARIES)
