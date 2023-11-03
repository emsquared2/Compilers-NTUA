.PHONY: clean distclean default

CXX=c++
CXXFLAGS=-Wall

default: grace

lexer.cpp: lexer.l
	flex -s -o lexer.cpp lexer.l

lexer.o: lexer.cpp lexer.hpp parser.hpp

parser.hpp parser.cpp: parser.y
	bison -dv -o parser.cpp parser.y

parser.o: parser.cpp lexer.hpp

grace: lexer.o parser.o
	$(CXX) $(CXXFLAGS) -o grace lexer.o parser.o

clean: 
	$(RM) lexer.cpp parser.cpp parser.hpp parser.output *.output *.o

distclean: clean
	$(RM) grace