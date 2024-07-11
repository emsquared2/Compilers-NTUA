# Compilers-NTUA
Project assignment for the course "Compilers" of the School of Electrical and Computer Engineering at the National Technical University of Athens during Spring Semester 2023.

Compiler for the Grace programming language.

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




## Contributors 
- [@emsquared2](https://github.com/emsquared2)
- [@alex1on](https://github.com/alex1on)