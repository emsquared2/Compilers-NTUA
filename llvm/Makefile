.PHONY: clean distclean default

LLVMCONFIG=llvm-config-16

# Options for lib generation
LIB_CC := clang
LIB_CFLAGS := -c
AR := ar
ARFLAGS := -cvq
LIB_SRC_DIR := ./lib
LIB_OBJ_DIR := obj
LIB_SRC_FILES := $(wildcard $(LIB_SRC_DIR)/*.c)
LIB_OBJ_FILES := $(patsubst $(LIB_SRC_DIR)/%.c,$(LIB_OBJ_DIR)/%.o,$(LIB_SRC_FILES))

CPP_FILES := $(wildcard *.cpp)
HPP_FILES := $(wildcard *.hpp)
OBJECT_FILES := $(patsubst %.cpp,%.o,$(CPP_FILES))

CXX=clang++
CXXFLAGS=-Wall -g `$(LLVMCONFIG) --cxxflags`
LDFLAGS=`$(LLVMCONFIG) --ldflags --system-libs --libs all`

default: grace libgrc.a

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

libgrc.a: $(LIB_OBJ_FILES)
	$(AR) $(ARFLAGS) $@ $^

# Rule to compile .c files to .o files
$(LIB_OBJ_DIR)/%.o: $(LIB_SRC_DIR)/%.c | $(LIB_OBJ_DIR)
	$(LIB_CC) $(LIB_CFLAGS) -o $@ $<

# Create the object directory if it doesn't exist
$(LIB_OBJ_DIR):
	mkdir -p $(LIB_OBJ_DIR)

grace: $(OBJECT_FILES) symbol.o parser.o lexer.o general.o error.o
	$(CXX) $(CXXFLAGS) -o grace $^ $(LDFLAGS)

clean: 
	$(RM) lexer.cpp parser.cpp parser.hpp parser.output *.output *.o
	rm -rf $(OBJ_DIR) libgrc.a

distclean: clean
	$(RM) grace
