.PHONY: clean distclean default

CXX=c++
CXXFLAGS=-Wall

default: grace

lexer.cpp: lexer.l
	flex -s -o lexer.cpp lexer.l

lexer.o: lexer.cpp lexer.hpp

grace: lexer.o
	$(CXX) $(CXXFLAGS) -o lexer lexer.o

clean: 
	$(RM) lexer.cpp output *.o

distclean: clean
	$(RM) lexer