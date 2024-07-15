# Compilers-NTUA
Project assignment for the course "Compilers" of the School of Electrical and Computer Engineering at the National Technical University of Athens during Spring Semester 2023.

Compiler for the Grace programming language.

## Table of Contents

- [About the Grace Programming Language](#about-the-grace-programming-language)
  - [Language Specification](#language-specification)
  - [Example Programs](#example-programs)
    - [Hello, World!](#hello-world)
    - [Prime Numbers](#prime-numbers)
    - [Bubble Sort](#bubble-sort)
- [Project Structure](#project-structure)
- [Prerequisites](#prerequisites)
  - [Installation](#installation)
    - [Ubuntu](#ubuntu)
    - [macOS](#macos)
- [Compiling Grace Programs](#compiling-grace-programs)
  - [Hello World example](#hello-world-example)
  - [More functionalities / Error Examples](#more-functionalities--error-examples)
    - [Lexical Analysis](#lexical-analysis)
    - [Syntax Analysis](#syntax-analysis)
    - [Semantic Analysis](#semantic-analysis)
  - [Testing Suite](#testing-suite)
    - [Syntax Analysis Test](#syntax-analysis-test)
    - [Semantic Analysis Test](#semantic-analysis-test)
    - [Codegen Test](#codegen-test)
- [Contributors](#contributors)

## About the Grace Programming Language

Grace is a simple imperative programming language designed with a focus on ease of use and readability. Its design principles are influenced by the aesthetics and structure of languages such as Pascal and C, providing a familiar syntax and straightforward constructs. Here are the main features of Grace:

- **Simple Syntax and Structure**: Grace offers a clear and easy-to-read syntax, making it accessible for beginners and efficient for experienced programmers.
- **Basic Data Types**: The language supports fundamental data types including characters, integers, and arrays, allowing for a wide range of basic data manipulation and storage.
- **Functions**: Grace includes simple function definitions, with support for both pass-by-value and pass-by-reference parameter passing methods.
- **Variable Scope**: Variable scoping in Grace is similar to that found in Pascal, providing structured and predictable variable lifetimes and access rules.
- **Standard Library**: Grace comes with a standard library of functions that assist with common tasks, enhancing productivity and simplifying code development.

### Language Specification

The Grace language specification covers a range of essential topics including:

- **Lexical Structure**: Definition of keywords, identifiers, literals, operators, and delimiters.
- **Syntax**: The rules for writing correct and understandable code, including function definitions, variable declarations, and control structures.
- **Semantics**: Detailed description of how the language constructs behave during execution.
- **Standard Library Functions**: Documentation of the built-in functions available in Grace, providing utility for common programming tasks.

### Example Programs

Below are a few example programs written in Grace, demonstrating the language's capabilities.

- ### Hello, World!

This simple program prints "Hello, World!" to the console.

```grace
fun hello () : nothing
{
   writeString("Hello world!\n");
}
```

- ### Prime Numbers

A program to compute and display prime numbers.

```grace
fun main () : nothing
   
   fun prime (n : int) : int
      var i : int;
   {
      if n < 0            then return prime(-n);
      else if n < 2       then return 0;
      else if n = 2       then return 1;
      else if n mod 2 = 0 then return 0;
      else {
         i <- 3;
         while i <= n div 2 do {
            if n mod i = 0 then
               return 0;
            i <- i + 2;
         }
         return 1;
      }
   }
   
   var limit, number, counter : int;
   
{ $ main
   writeString("Please, give me the upper limit: ");
   limit <- readInteger();
   writeString("Prime numbers between 0 and ");
   writeInteger(limit);
   writeString(":\n\n");
   counter <- 0;
   if limit >= 2 then {
      counter <- counter + 1;
      writeString("2\n");
   }
   if limit >= 3 then {
      counter <- counter + 1;
      writeString("3\n");
   }
   number <- 6;
   while number <= limit do {
      if prime(number - 1) = 1 then {
         counter <- counter + 1;
         writeInteger(number - 1);
         writeString("\n");
      }
      if number # limit and prime(number + 1) = 1 then {
         counter <- counter + 1;
         writeInteger(number + 1);
         writeString("\n");
      }
      number <- number + 6;
   }

   writeString("\n");
   writeInteger(counter);
   writeString(" prime number(s) were found.\n");
} $ main
```

- ### Bubble Sort

An implementation of the bubble sort algorithm.

```grace
fun main () : nothing

   fun bsort (n : int; ref x : int[]) : nothing
   
      fun swap (ref x, y : int) : nothing
         var t : int;
      {
         t <- x;
         x <- y;
         y <- t;
      }
   
      var changed, i : int;
      
   { $ bsort
      changed <- 1;
      while changed > 0 do {
         changed <- 0;
         i <- 0;
         while i < n-1 do {
            if x[i] > x[i+1] then {
               swap(x[i], x[i+1]);
               changed <- 1;
            }
            i <- i+1;
         }
      }
   } $ bsort

   fun writeArray (ref msg : char[]; n : int; ref x : int[]) : nothing
      var i : int;
   {
      writeString(msg);
      i <- 0;
      while i < n do {
         if i > 0 then writeString(", ");
         writeInteger(x[i]);
         i <- i+1;
      }
      writeString("\n");
   }
   
   var seed, i : int;
   var x       : int[16];
   
{ $ main
   seed <- 65;
   i <- 0;
   while i < 16 do {
      seed <- (seed * 137 + 221 + i) mod 101;
      x[i] <- seed;
      i <- i+1;
   }
   writeArray("Initial array: ", 16, x);
   bsort(16, x);
   writeArray("Sorted  array: ", 16, x);
} $ main
```

## Project Structure

We have kept the original project development path to keep the internal compiler routines (e.g. parsing, AST gen etc.) accessible to the user by keeping each project folder (e.g `lexer`, `parser` etc.) with the discrete compiler functionality. Moreover, for the folders below: 

- `lexer`: Only contains the `lexer.l` file used by the `flex` tool to generate the tokens used by the parser.
- `parser`: The grace compiler can check if a `grace` program has syntactical correctness.
- `semantic`: Semantic actions are added on our compiler. Type checking, variable declaration are some of the semantic actions that are being implemented. Compiler checks if a programs is semantically correct along side the above functions. The AST can also be printed in this stage.
- `llvm`: The fully formed grace compiler that provides an end-to-end compilation of a grace program.
- `syntax-gen`: A syntactically correct grace program generator from the following repo [kostis/ntua_compilers/grace](https://github.com/kostis/ntua_compilers/tree/master/grace/syntax_gen). Was used to check the correctness of our parser on an arbitrarely large amount of grace programs. Though that does not conclude to a necessarely correct parser...

## Prerequisites

- `flex 2.6.4` tool for lexical analysis
- `bison 3.8.2` tool for parser generation
- `llvm 16 or later` c++ library for Intermediate Code generation
- `clang/clang++`

### Installation

### Ubuntu

- `flex` installation

```console
$ sudo apt-get update
$ sudo apt-get install flex
```


- `bison` installation

```console
$ sudo apt-get update
$ sudo apt-get install bison
```

- `llvm` installation

1. Import the GPG key for the LLVM repository

```console
$ wget https://apt.llvm.org/llvm-snapshot.gpg.key
$ sudo apt-key add llvm-snapshot.gpg.key

```

2. Add the LLVM repository to your list of sources

```console
$ sudo add-apt-repository "deb http://apt.llvm.org/$(lsb_release -sc)/ llvm-toolchain-$(lsb_release -sc)-16 main"

```

3. Update package lists

```console
$ sudo apt update
```

4. Install LLVM 16

```console
$ sudo apt install llvm-16
```

5. Set LLVM 16 as the default version

```console
$ sudo update-alternatives --install /usr/bin/llvm-config llvm-config /usr/bin/llvm-config-16 100
```

> Note! If you have previously installed llvm with the ```sudo apt install llvm``` command it will install package names `llvm-config` in your machine. Newer versions, as used in this project, are installed as `llvm-config-XX` where XX the llvm version installed. So if you have this type of llvm package then either check the step 5 on this section or manually change the default llvm package with the command  ```sudo update-alternatives --config llvm-config``` so that the default `llvm-config` package is pointing to your llvm version.

- `clang` installation

```console
$ sudo apt-get update
$ sudo apt-get install clang
```

### macOS

- `flex` installation
```console
$ brew install flex
```

- `bison` installation
```console
$ brew install bison
```

- `llvm` installation
```console
$ brew install llvm@16
```
Add the LLVM binaries to your PATH. Add the following lines to your shell configuration file (~/.bashrc, ~/.zshrc, etc.):
```console
export PATH="/opt/homebrew/opt/llvm@16/bin:$PATH"
export LDFLAGS="-L/opt/homebrew/opt/llvm@16/lib"
export CPPFLAGS="-I/opt/homebrew/opt/llvm@16/include"
```

Then, source the configuration file to apply the changes:
```console
$ source ~/.zshrc  # or source ~/.bashrc
```

> Note! If you encounter issues running on macOS, you may need to adjust the following line in your Makefile:
> ```console
> LLVMCONFIG=llvm-config-18
> ```
> Change it to:
> ```Makefile
> LLVMCONFIG=llvm-config
> ```


## Compiling Grace Programs

> Note: If you are using LLVM version 18 or later, follow these steps in ast.cpp:

> Uncomment line 307 by removing the leading //:
> ```c++
> auto FileType = llvm::CodeGenFileType::ObjectFile
> ```
> Comment out line 310 by adding // at the beginning:
> ```c++
> auto FileType = llvm::CGFT_AssemblyFile;
> ```
> This adjustment is necessary for compatibility with LLVM versions 18 and later.

To compile Grace programs, follow these steps:
1. Navigate to the LLVM Directory and Build:
```console
$ cd llvm/
$ make
```
This will build the main compiler functionalities.

2. Compile Your Grace Program:
```console
$ ./gracexec.sh -O ../path/to/program.grc
```
> The `-O` flag is optional and enables compiler optimizations.

The compiled output files will be placed in the same directory as the `.grc` program file, with a `.out` suffix. Additionally, a `.imm` and a `.asm` file is created with the intermediate and final assembly code of the input file.

The `gracexec.sh` script uses the main `grace` compiler to generate the final code (in assembly) from a `grace` program.

For more detailed information about the compilation process and code generation, you can refer to the Grace backbone documentation.


```console
$ ./grace -ifo /path/to/file.grc
Usage: ./grace [options] <input_file>
Options:
  -i             Print the intermediate code to stdout
  -f             Print the final code to stdout
  -O             Enable optimizations
  -h             Show this help message
```

The default behavior which is retained with any use of option is to create a `.imm` and a `.asm` file containing the intermediate and final assembly code respectively of the inputed file.

### Hello World example

In this section we will show an example on how to use our grace compiler on a simple hello world program. 

```console
$ ls 
hello.grc

$ ./gracexec.sh -O hello.grc
hello.asm hello.grc hello.imm hello.out

$ ./hello.out
Hello World!

$ ./grace -i hello.grc
 ; ModuleID = 'grace program'
source_filename = "grace program"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%stack_frame_struct_hello_13 = type {}

@0 = private unnamed_addr constant [14 x i8] c"Hello world!\0A\00", align 1

declare void @writeInteger(i64)

declare void @writeChar(i8)

declare void @writeString(ptr)

declare i64 @readInteger()

declare i8 @readChar()

declare void @readString(i64, ptr)

declare i64 @ascii(i8)

declare i8 @chr(i64)

declare i64 @strlen(ptr)

declare i64 @strcmp(ptr, ptr)

declare void @strcpy(ptr, ptr)

declare void @strcat(ptr, ptr)

define void @hello_13() {
entry:
  %stack_frame_hello_13 = alloca %stack_frame_struct_hello_13, align 8
  call void @writeString(ptr @0)
  ret void
}

define i64 @main() {
entry:
  call void @hello_13()
  ret i64 0
}

$ ./grace -f hello.grc
        .text
        .file   "grace program"
        .globl  hello_13
        .p2align        4, 0x90
        .type   hello_13,@function
hello_13:
        .cfi_startproc
        pushq   %rax
        .cfi_def_cfa_offset 16
        leaq    .L__unnamed_1(%rip), %rdi
        callq   writeString@PLT
        popq    %rax
        .cfi_def_cfa_offset 8
        retq
.Lfunc_end0:
        .size   hello_13, .Lfunc_end0-hello_13
        .cfi_endproc

        .globl  main
        .p2align        4, 0x90
        .type   main,@function
main:
        .cfi_startproc
        pushq   %rax
        .cfi_def_cfa_offset 16
        callq   hello_13@PLT
        xorl    %eax, %eax
        popq    %rcx
        .cfi_def_cfa_offset 8
        retq
.Lfunc_end1:
        .size   main, .Lfunc_end1-main
        .cfi_endproc

        .type   .L__unnamed_1,@object
        .section        .rodata.str1.1,"aMS",@progbits,1
.L__unnamed_1:
        .asciz  "Hello world!\n"
        .size   .L__unnamed_1, 14

        .section        ".note.GNU-stack","",@progbits
```

### More functionalities / Error Examples

If you do not wish to compile a Grace program but only want to check its lexical, syntax or semantic correctness, you can use the early functionalities of our compiler. We have retained these functionalities in separate folders, allowing you to use them for discrete functions without compiling the entire program.

#### Lexical Analysis
To perform lexical analysis to your grace program you have to go to the `lexer` folder and run `make`. Input a grace file to the `lexer` ass follows:

```console 
$ cat hello.grc
fun hello () : nothing
{
   writeString("Hello world!\n");
}

$ ./lexer -l ../programs/hello.grc
token = 1005, lexeme = "fun"
token = 1017, lexeme = "hello"
token = 40, lexeme = "("
token = 41, lexeme = ")"
token = 58, lexeme = ":"
token = 1010, lexeme = "nothing"
token = 123, lexeme = "{"
token = 1017, lexeme = "writeString"
token = 40, lexeme = "("
token = 1020, lexeme = ""Hello world!\n""
token = 41, lexeme = ")"
token = 59, lexeme = ";"
token = 125, lexeme = "}"
token = 0, lexeme = ""
Success.
```

> Use the -l flag to print the lexical tokens.

If `Success` is printed then the program is lexically correct. Otherwise a lexical error is shown with the corresponding description.

```console 
$ cat helloworld_with_lexerror.grc
fun hello () : nothing
{
   x <- 3%
   writeString("Hello world!\n");
}

$ ./lexer helloworld_with_lexerror.grc
Illegal character % at line 3
```

#### Syntax Analysis

Go to `parser` folder and run `make`. Input a grace program to the `grace` executable as follows:

```console 
$ ./grace < path/to/file.grc
Success.
```

If `Success.` is printed then the program is syntactically correct. Otherwise a syntax error is shown with the corresponding description.

```console
$ ./grace < path/to/syntax/error/file.grc
syntax error, unexpected '=', expecting <-
```

### Semantic Analysis

Go to `semantic` folder and run `make`. Input a grace program to the `grace` executable as follows:

```console 
$ ./grace -a ../programs/hello.grc
AST: FuncDef(Header(fun Id(hello)() : RetType(void)) LocalDefList() Block(CallStmt(Id(writeString)(ExprList(ConstStr("Hello world!\n"))))))
Success.
```

> Use the -a flag to print the AST generated by the parser to stdout.

If `Success` is printed then the program is syntactically correct. Otherwise a semantic error is shown with the corresponding description.

```console
$ cat semerror.grc
$$
  In this erroneous grace program an integer is added to a character with the
  simple plus operator.

  In the block of the while-do statement, the addition of c and 1 is not allowed.
$$

fun main() : nothing
  var i : int;
  var c : char;
  var result : int;

{ $ main
  $ Will compute the ascii code of 'f'
  result <- 0;
  c <- 'a';
  while c # 'f' do {
    c <- c + 1;
    result <- result + 1;
  }
  writeString("The ascii code of \'f\' is ");
  writeInteger(result);
  writeString(".\n");
} $ main

$ ./grace semerror.grc
Error: Type mismatch at line 18
```

These errors are just examples showcasing the comprehensive error detection capabilities of our compiler. Moreover, the final compiler retains these capabilities across each individual building block of the project.

## Testing Suite 

For each compiling stage (parsing, semantic analysis and codegen) besides lexical analysis we have created a testing script to verify each stage's integrity. We inputted both correct and erroneous programs to ensure that correct programs succeed while erroneous programs fail on the respective stage. 

### Syntax analysis test

1. Change to the appropriate paths in the `gen.sh` in `syntax-gen` folder

```shell
readonly ERL=/path/to/erl
export ERL_LIBS=/path/to/syntax-gen/proper
```

> You will need to have `erlang` installed in your machine.

2. Run the testing script

```console
$ ./syntax-gen/test_parser.sh
```

The script will ask you for the number of syntacticaly correct programs to test on the `parser`.

### Semantic analysis test

1. If you haven't previously built the semantic checked in `semantic` folder run:

```console
$ cd semantic/
$ make
```

2. Run the testing script

```console  
$ ./test_sem.sh
```

The tester inputs both the semanticaly correct programs (from the `programs/` directory) and the erroneous programs (from the `programs-erroneous/` directory) with semantic errors to the compiler.

### Codegen test

1. Uncomment the following line from the `parser.y` in `llvm` directory

```cpp
    // Comment out in order to run test_llvm.sh
    if (result == 0) printf("Success.\n");
```

2. Run the testing script

```console
$ ./test_llvm.sh
```

Note! This test will check only if the codegen succeeded and not if the logic of the generated executable is correct based on the original grace program.








## Contributors 
- [@emsquared2](https://github.com/emsquared2)
- [@alex1on](https://github.com/alex1on)