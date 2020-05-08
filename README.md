# What is this ?

As part of an effort to further my understanding of Compiler Design at a UG level, I have been working on an implementation of a compiler project for the EXPL langauge, developed by NIT-C. The homepage for this project is [here](https://silcnitc.github.io/index.html).

The project has its own well-defined [Roadmap](https://silcnitc.github.io/roadmap.html) consisting of various stages that are designed to incrementally build a compiler for the EXPL language. This repository contains my implementation for various stages of this roadmap. This is purely an academic effort, and has no practical value outside of the laboratory.

## Goals
1. To develop a working knowledge of the theoretical concepts used in compiler design
2. To develop experience in using the main tools for compiler design - Lex and Yacc
3. To develop an understanding of one of the main functions of a compiler - Intermediate Code Generation
4. To get exposed to optimizations at a high level (register allocation, machine independent optimizations, etc)

## Tools/Technology
1. Lex
2. Yacc
3. C/C++
4. the XSM simulator (information on setting up an environment [here](https://silcnitc.github.io/xsm-environment-tut.html))

## Specifications
1. [EXPL Language Specification](https://silcnitc.github.io/expl.html)
2. [Application Binary Interface (ABI)](https://silcnitc.github.io/abi.html)
3. [XSM Execution Environment](https://silcnitc.github.io/xsm-environment-tut.html)

## The Roadmap
* [x] Stage 1 : Code generation for Arithmetic Expressions
* [x] Stage 2 : Introduction to static storage allocation
* [x] Stage 3 : Adding Flow Control Statements
* [ ] Stage 4 : User Defined Variables and arrays **--in progress**
* [ ] Stage 5 : Adding Functions
* [ ] Stage 6 : User defined types and Dynamic Memory Allocation
* [ ] Stage 7 : Adding Objects – Data encapsulation

### Usage
Each of the folders in the repository contains an implementation of the requirements of that particular stage. Simply open the folder for a description, and instructions.
