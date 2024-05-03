.PHONY: clean distclean default

CPP_FILES := $(wildcard *.cpp)

# Exclude ast.cpp from the list
# CPP_FILES := $(filter-out ast.cpp, $(CPP_FILES))

HPP_FILES := $(wildcard *.hpp)

OBJECT_FILES := $(patsubst %.cpp,%.o,$(CPP_FILES))

CXX=c++
CXXFLAGS=-Wall -std=c++11 -g

default: grace

lexer.cpp: lexer.l
	flex -s -o lexer.cpp lexer.l

lexer.o: lexer.cpp lexer.hpp parser.hpp ast.hpp

parser.hpp parser.cpp: parser.y
	bison -dv -o parser.cpp parser.y

parser.o: parser.cpp $(HPP_FILES)
	$(CXX) $(CXXFLAGS) -c parser.cpp

general.o  : general.c general.h error.h

error.o    : error.c general.h error.h

symbol.o   : symbol.c symbol.h general.h error.h

grace: $(OBJECT_FILES) symbol.o parser.o lexer.o general.o error.o
	$(CXX) $(CXXFLAGS) -o grace $^

clean: 
	$(RM) lexer.cpp parser.cpp parser.hpp parser.output *.output *.o

distclean: clean
	$(RM) grace
