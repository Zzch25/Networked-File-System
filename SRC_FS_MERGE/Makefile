NAME = test
SRC = \
		mgr.cpp

LIBRARY_DIRS:=-L./EXT_LIBS/BUILT_LIBS
LIBRARIES:=-lssl -lcrypto -lz

all:
	g++ -ggdb -std=c++11 $(LIBRARY_DIRS) $(SRC) -o $(NAME) $(LIBRARIES)
