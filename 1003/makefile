CXX = g++
CXXFLAGS = -std=c++11 -Wall

all: my_program run

my_program: main.cpp
	$(CXX) $(CXXFLAGS) -o my_program main.cpp

run: my_program
	./my_program example_1.txt
	./my_program example_2.txt
	./my_program example_3.txt
	./my_program example_4.txt
	./my_program example_5.txt
	./my_program example_6.txt


clean:
	rm -f my_program
