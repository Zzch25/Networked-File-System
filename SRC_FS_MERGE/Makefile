NAME = test
SRC = \
		mgr.cpp

RM = rm -r

LIBRARY_DIRS:=-L./EXT_LIBS/BUILT_LIBS
LIBRARIES:=-lssl -lcrypto -lz

all:
	g++ -ggdb -std=c++11 $(LIBRARY_DIRS) $(SRC) -o $(NAME) $(LIBRARIES)

clean:
	-$(RM) *.o
	-$(RM) *.c~
	-$(RM) *.h~
	-$(RM) *~
	-$(RM) .*~
	-$(RM) $(NAME)*
	-$(RM) .swp
	-$(RM) *.swp
	-$(RM) .*.swp
	-$(RM) .*.*.swp
	-$(RM) .*.un~
	-$(RM) .*.un~~
	-$(RM) .*.*.un~
	-$(RM) .*.*.un~~