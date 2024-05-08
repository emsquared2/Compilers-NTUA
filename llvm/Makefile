.PHONY: clean distclean default

LLVMCONFIG=llvm-config

CPP_FILES := $(wildcard *.cpp)

HPP_FILES := $(wildcard *.hpp)

OBJECT_FILES := $(patsubst %.cpp,%.o,$(CPP_FILES))

CXX=clang++
CXXFLAGS=-Wall -g `$(LLVMCONFIG) --cxxflags`
LDFLAGS=`$(LLVMCONFIG) --ldflags --system-libs --libs all`

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
	$(CXX) $(CXXFLAGS) -o grace $^ $(LDFLAGS)

clean: 
	$(RM) lexer.cpp parser.cpp parser.hpp parser.output *.output *.o

distclean: clean
	$(RM) grace
