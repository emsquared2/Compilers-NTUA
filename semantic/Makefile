.PHONY: clean distclean default

CPP_FILES := $(wildcard *.cpp)

# Exclude ast.cpp from the list
# CPP_FILES := $(filter-out ast.cpp, $(CPP_FILES))

OBJECT_FILES := $(patsubst %.cpp,%.o,$(CPP_FILES))

CXX=c++
CXXFLAGS=-Wall -std=c++11 -g

default: grace

lexer.cpp: lexer.l
	flex -s -o lexer.cpp lexer.l

lexer.o: lexer.cpp lexer.hpp parser.hpp ast.hpp statements.hpp

ast.o: ast.cpp ast.hpp ASTabstracts.hpp types.hpp
	$(CXX) $(CXXFLAGS) -c ast.cpp

statements.o: statements.cpp statements.hpp ast.hpp
	$(CXX) $(CXXFLAGS) -c statements.cpp

expressions.o: expressions.cpp expressions.hpp ast.hpp
	$(CXX) $(CXXFLAGS) -c expressions.cpp

conditions.o: conditions.cpp conditions.hpp ASTabstracts.hpp
	$(CXX) $(CXXFLAGS) -c conditions.cpp

funcdef.o: funcdef.cpp funcdef.hpp ast.hpp localdeflist.hpp localdef.hpp
	$(CXX) $(CXXFLAGS) -c funcdef.cpp

localdeflist.o: localdeflist.cpp localdeflist.hpp localdef.hpp ast.hpp
	$(CXX) $(CXXFLAGS) -c localdeflist.cpp

localdef.o: localdef.cpp localdef.hpp funcdef.hpp ast.hpp
	$(CXX) $(CXXFLAGS) -c localdef.cpp

parser.hpp parser.cpp: parser.y
	bison -dv -o parser.cpp parser.y

parser.o: parser.cpp lexer.hpp ast.hpp localdef.hpp funcdef.hpp localdeflist.hpp statements.hpp expressions.hpp conditions.hpp
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