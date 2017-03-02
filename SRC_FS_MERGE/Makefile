NAME = test
SRC = \
		mgr.cpp

RM = rm -rf

LIBRARY_DIRS:=-L./EXT_LIBS/BUILT_LIBS
LIBRARIES:=-lssl -lcrypto -lz -lpthread

all:
	g++ -ggdb -std=c++14 $(LIBRARY_DIRS) $(SRC) -o $(NAME) $(LIBRARIES)

clean:
	-$(RM) $(NAME)*
	-find ./ -type f \( 		-iname \*.h~ 		-o			\
	  								-iname \*.o			-o		  	\
	  								-iname \*.o~		-o		  	\
	  								-iname \*.s			-o		  	\
	  								-iname \*.s~		-o		  	\
	  								-iname \*.cpp~		-o		  	\
	  								-iname \*~			-o		  	\
	  								-iname \.*~			-o		  	\
	  								-iname \.*.*.swp	-o		  	\
	  								-iname \.*.*.un~			  	\
	\) -delete
