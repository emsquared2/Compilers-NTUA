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
- [Compiling Grace Programs](#compiling-grace-programs)
  - [Hello World example](#hello-world-example)
  - [More functionalities / Error Examples](#more-functionalities--error-examples)
    - [Lexical Analysis](#lexical-analysis)
    - [Syntax Analysis](#syntax-analysis)
    - [Semantic Analysis](#semantic-analysis)
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
fun main() {
    print("Hello, World!")
}
```

- ### Prime Numbers

A program to compute and display prime numbers.

```grace
fun isPrime(n: int): bool {
    if (n <= 1) return false
    for (i in 2 to n/2) {
        if (n % i == 0) return false
    }
    return true
}

fun main() {
    for (i in 1 to 100) {
        if (isPrime(i)) print(i)
    }
}
```

- ### Bubble Sort

An implementation of the bubble sort algorithm.

```grace
fun bubbleSort(arr: ref int[]) {
    var n: int = arr.length
    var swapped: bool
    do {
        swapped = false
        for (i in 1 to n-1) {
            if (arr[i-1] > arr[i]) {
                var temp: int = arr[i-1]
                arr[i-1] = arr[i]
                arr[i] = temp
                swapped = true
            }
        }
    } while (swapped)
}

fun main() {
    var arr: ref int[] = [64, 34, 25, 12, 22, 11, 90]
    bubbleSort(arr)
    print(arr)
}
```

## Project Structure

We have kept the original project development path to keep the internal compiler routines (e.g. parsing, AST gen etc.) accessible to the user by keeping each project folder (e.g `lexer`, `parser` etc.) with the discrete compiler functionality. Moreover for the folders below: 

- `lexer`: only contains the `lexer.l` file used by the `flex` tool to generate the tokens used by the parser.
- `parser`: grace compiler can check if a `grace` program has syntactical correctness.
- `semantic`: semantic actions are added on our compiler. Type checking, variable declaration are some of the semantic actions that are being implemented. Compiler checks if a programs is semantically correct along side the above functions. The AST can also be printed in this stage.
- `llvm`: the full formed grace compiler that provides an end-to-end compilation of a grace program.
- `syntax-gen`: a syntactically correct grace program generator from the following repo [kostis/ntua_compilers/grace](https://github.com/kostis/ntua_compilers/tree/master/grace/syntax_gen). Was used to check the correctness of our parser on an arbitrarely large amount of grace programs. Though that does not conclude to a necessarely correct parser...

## Prerequisites

- `flex 2.6.4` tool for lexical analysis
- `bison 3.8.2` tool for parser generation
- `llvm 18 or later` c++ library for Intermediate Code generation
- `clang/clang++`

### Installation

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

Install llvm by using the automatic installation script that LLVM provides.

```
# bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"
```
> Note! It will download the latest stable version of the llvm libary

- `clang` installation

```console
$ sudo apt-get update
$ sudo apt-get install clang
```

## Compiling Grace Programs

First of all, for the main compiler functionalities you have to go to the `llvm` directory and run `make`. 

```console
$ cd llvm/
$ make
```


To compile your grace program to an executable program you can use the `gracexec.sh` script as follows:

```console
$ ./gracexec.sh -o ../path/to/program.grc
```

> The `-o` is an optional flag to enable compiler optimizations.

> The output files will be to the path where the `.grc` program is located with a `.out` suffix.

The shell script is using the main `grace` compiler that creates the final code (in assembly) from a `grace` program.

You can also use the `grace` backbone and see more information about the compilation and code generation from the initial grace program. 

```console
$ ./grace -ifo /path/to/file.grc
Usage: ./grace [options] <input_file>
Options:
  -i             Print the intermediate code
  -f             Print the final code
  -o             Optimize code
  -h             Show this help message
```

### Hello World example

In this section we will show an example on how to use our grace compiler on a simple hello world program. 

```console
$ ls 
hello.grc

$ ./gracexec.sh -o hello.grc
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

If you do not wish to compile a grace program but only to check the syntax or semantic correctness of a grace program you can use the early functionalities that our compiler had (we chose to keep in them in seperated folders) and are used to the end result for each discrete function.

#### Lexical Analysis
There is no way to only do lexical analysis in our project so we only show an example of a program with a lexical error and how it shown in our compiler

```console 
$ cat helloworld_with_lexerror.grc
fun hello () : nothing
{
   x <- 3%
   writeString("Hello world!\n");
}

$ ./grace < helloworld_with_lexerror.grc
Illegal character % at line 3
```

#### Syntax Analysis

Go to `parser` folder and run `make`. Input a grace program to the `grace` executable as follows:

```console 
$ ./grace < path/to/file.grc
Success!
```

If `Success!` is shown then the program is syntactically correct. Otherwise a syntax error is shown with the corresponding description.

```console
$ ./grace < path/to/syntax/error/file.grc
syntax error, unexpected '=', expecting <-
```

### Semantic Analysis

Go to `semantic` folder and run `make`. Input a grace program to the `grace` executable as follows:

```console 
$ ./grace -a hello.grc
AST: FuncDef(Header(fun Id(hello)() : RetType(void)) LocalDefList() Block(CallStmt(Id(writeString)(ExprList(ConstStr("Hello world!\n"))))))
Success.
```

> With the `-a` you can also see the AST generating by the parser from the previous step.

If `Success` is shown then the program is syntactically correct. Otherwise a semantic error is shown with the corresponding description.

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

Ofcourse the above errors are just some examples of the full error detection capabilities of our compiler. Also, the finished compiler has the same capabilities regarding error detecting as each discrete building block of the project.



## Contributors 
- [@emsquared2](https://github.com/emsquared2)
- [@alex1on](https://github.com/alex1on)