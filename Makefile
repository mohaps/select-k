CC=g++
CXXFLAGS=-std=c++20 -Iinclude -O2

all: build
build: clean
	@mkdir -p bin
	$(CC) $(CXXFLAGS) -o bin/select_k_sample src/select_k_sample.cpp
clean:
	rm -rf bin/select_k_sample
run: build
	bin/select_k_sample
