cmake_minimum_required(VERSION 3.10)

project(Ornament)

set(CMAKE_CXX_STANDARD 20)


#set(CMAKE_CXX_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/g++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build)

set(SOURCE_PATH src/)

set(SOURCES ${SOURCE_PATH}DefaultActionHandler.cpp 
	${SOURCE_PATH}DefaultConstructor.cpp 
	${SOURCE_PATH}DefaultInputController.cpp
	${SOURCE_PATH}Entity.cpp
	${SOURCE_PATH}DefaultRealmManager.cpp
	${SOURCE_PATH}main.cpp
	${SOURCE_PATH}Renderer.cpp
	${SOURCE_PATH}Player.cpp
	${SOURCE_PATH}SpiderSlime.cpp
	${SOURCE_PATH}Gun.cpp
	${SOURCE_PATH}Camera.cpp
	${SOURCE_PATH}2d_vector_math.cpp
	${SOURCE_PATH}Active.cpp
	${SOURCE_PATH}SpiderSlime.cpp
	${SOURCE_PATH}TextureController.cpp
	)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} SDL2_image)
target_link_libraries(${PROJECT_NAME} SDL2)
