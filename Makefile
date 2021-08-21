all:
	cmake -Bbuild -DCMAKE_EXPORT_COMPILE_COMMANDS=1
	# make shaders

build:
	cd build/; make

run:
	make build
	./LostInADream

debug:
	gdb ./LostInADream

shaders:
	glslc src/shaders/vertex.vert -o res/shaders/vert.spv
	glslc src/shaders/fragment.frag -o res/shaders/frag.spv

.PHONY: all build run debug
