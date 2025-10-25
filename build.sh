#!/bin/bash
clang -c planet.c -o planet.o
clang -c destinations.c -o destinations.o
clang -c navigation.c -o navigation.o
clang -c main.c -o main.o
clang planet.o destinations.o navigation.o main.o -o space_navigator
./space_navigator
