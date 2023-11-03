#!/usr/bin/env bash

# Generates N Grace programs in the 'progs' dir.
#   (N is a command-line argument.)
#
# To use it, you need to first declare the dirs
# where Erlang/OTP and PropEr are in your system.

readonly ERL=/usr/bin/erl
export ERL_LIBS=/mnt/c/Users/manos/Documents/GitHub/Compilers-NTUA/syntax-gen/proper

${ERL}c -pa ${ERL_LIBS}/ebin +debug_info ./*.erl

mkdir -p progs
for i in $(seq 1 "$1")
do
  $ERL -noshell -noinput -s grace_pp p -s erlang halt > progs/p"$i".grc
done