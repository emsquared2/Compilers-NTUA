.PHONY: clean distclean default

CXX=c++
CXXFLAGS=-Wall

default: grace

lexer.cpp: lexer.l
	flex -s -o lexer.cpp lexer.l

lexer.o: lexer.cpp lexer.hpp parser.hpp ast.hpp 

funcdef.o: funcdef.cpp funcdef.hpp ast.hpp localdef.hpp local.hpp
	$(CXX) $(CXXFLAGS) -c funcdef.cpp

localdef.o: localdef.cpp localdef.hpp local.hpp ast.hpp
	$(CXX) $(CXXFLAGS) -c localdef.cpp

local.o: local.cpp local.hpp funcdef.hpp ast.hpp
	$(CXX) $(CXXFLAGS) -c local.cpp

parser.hpp parser.cpp: parser.y
	bison -dv -o parser.cpp parser.y

parser.o: parser.cpp lexer.hpp ast.hpp ASTabstracts.hpp types.hpp local.hpp funcdef.hpp localdef.hpp  
	$(CXX) $(CXXFLAGS) -c parser.cpp

grace: lexer.o parser.o funcdef.o localdef.o local.o 
	$(CXX) $(CXXFLAGS) -o grace lexer.o parser.o funcdef.o localdef.o local.o

clean: 
	$(RM) lexer.cpp parser.cpp parser.hpp parser.output *.output *.o

distclean: clean
	$(RM) grace