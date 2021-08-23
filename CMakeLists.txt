cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)

project(LostInADream)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ferror-limit=100 -Wall -Wextra -Wshadow -Wconversion -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic  -Wconversion -Wsign-conversion -Wnull-dereference -Wdouble-promotion -Wextra-semi -Wformat=2 -g")

set(SOURCE_PATH src)

set(SOURCES
	${SOURCE_PATH}/main.cpp
    ${SOURCE_PATH}/system/RenderSystem.cpp
    ${SOURCE_PATH}/system/rendering/Instance.cpp
    ${SOURCE_PATH}/system/rendering/QueueFamilies.cpp
    ${SOURCE_PATH}/system/rendering/Device.cpp
    ${SOURCE_PATH}/system/rendering/SwapChain.cpp
    ${SOURCE_PATH}/system/rendering/utils.cpp
    ${SOURCE_PATH}/system/rendering/wrappers/InstanceWrapper.cpp
    ${SOURCE_PATH}/system/rendering/wrappers/LogicalDeviceWrapper.cpp
    ${SOURCE_PATH}/system/rendering/wrappers/DebugMessangerWrapper.cpp
    ${SOURCE_PATH}/system/rendering/wrappers/SurfaceWrapper.cpp
    ${SOURCE_PATH}/system/rendering/wrappers/WindowWrapper.cpp
	)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} glfw vulkan dl pthread X11 Xxf86vm Xrandr Xi)
