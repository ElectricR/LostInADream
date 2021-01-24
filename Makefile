all:
	cmake -H. -Bbuild

build:
	cd build/; make

run:
	./build/build/Ornament

debug:
	gdb ./build/build/Ornament

.PHONY: all build run debug
