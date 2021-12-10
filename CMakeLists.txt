cmake_minimum_required(VERSION 3.13) # For target_link_options
project(RaylibSnake CXX)

set(${PROJECT_NAME}_SOURCES main.cpp
							src/game.cpp
							src/mainmenu.cpp
							src/engine.cpp)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src/include)

target_link_options(${PROJECT_NAME} PRIVATE -static) # Link staticly
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wno-missing-braces) # Set warning flags

set(OPENGL_VERSION "2.1" CACHE INTERNAL "" FORCE) # At least 2.1 is neaded for RenderTexture support

if(CMAKE_BUILD_TYPE MATCHES Debug)
	target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
else()
	target_compile_options(${PROJECT_NAME} PRIVATE -Ofast)
	target_link_options(${PROJECT_NAME} PRIVATE -mwindows) # Disable console output if flag is supported
endif()

include(cmake/raylib.cmake) # Include raylib and featch if required
target_link_libraries(${PROJECT_NAME} raylib)

# Linking required by raylib
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

