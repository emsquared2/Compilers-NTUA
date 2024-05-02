.PHONY: clean distclean default

CPP_FILES := $(wildcard *.cpp)

OBJECT_FILES := $(patsubst %.cpp,%.o,$(CPP_FILES))

CXX=c++
CXXFLAGS=-Wall -std=c++11 -g

default: grace

lexer.cpp: lexer.l
	flex -s -o lexer.cpp lexer.l

parser.hpp parser.cpp: parser.y
	bison -dv -o parser.cpp parser.y

lexer.o: lexer.cpp lexer.hpp parser.hpp ast.hpp

parser.o: parser.cpp lexer.hpp ast.hpp

general.o  : general.c general.h error.h

error.o    : error.c general.h error.h

symbol.o   : symbol.c symbol.h general.h error.h

grace: $(OBJECT_FILES) symbol.o parser.o lexer.o general.o error.o
	$(CXX) $(CXXFLAGS) -o grace $^

clean: 
	$(RM) lexer.cpp parser.cpp parser.hpp parser.output *.output *.o

distclean: clean
	$(RM) grace
