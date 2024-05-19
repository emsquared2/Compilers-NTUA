#!/bin/bash

./grace -io $1 > a.imm
llc -regalloc=pbqp a.imm -o a.asm
clang a.asm libgrc.a -no-pie -o a.out