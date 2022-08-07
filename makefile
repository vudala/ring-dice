#!/bin/bash

CC=g++
FLAGS=-Wall -std=c++17
SRC=$(wildcard *.cpp)
OBJS=$(subst .cpp,.o,$(SRC))
OUTPUT=program
RM=rm -f

all: $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(OUTPUT) $(LIBS)

%.o: %.cpp
	$(CC) $(FLAGS) -c $< $(LIBS)

run:
	./program

clean:
	@$(RM) $(OBJS)

purge: clean
	@$(RM) $(OUTPUT)