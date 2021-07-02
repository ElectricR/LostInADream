all:
	cmake -Bbuild -DCMAKE_EXPORT_COMPILE_COMMANDS=1
	make shaders

build:
	cd build/; make

run:
	./LostInADream

debug:
	gdb ./LostInADream

shaders:
	glslc src/shaders/vertex.vert -o res/shaders/vert.spv
	glslc src/shaders/fragment.frag -o res/shaders/frag.spv

install:
	hg clone https://hg.libsdl.org/SDL SDL; \
	cd SDL; \
	mkdir build; \
	cd build; \
	../configure --prefix $(shell pwd)/lib; \
	make -j6; \
	make install; \
	cd ../..; \
	rm SDL -r; \
	hg clone http://hg.libsdl.org/SDL_image/ SDL_image; \
	cd SDL_image; \
	mkdir build; \
	cd build; \
	../configure --prefix $(shell pwd)/lib --with-sdl-prefix=$(shell pwd)/lib; \
	make -j6; \
	make install; \
	cd ../..; \
	rm SDL_image -r
	

.PHONY: all build run debug
