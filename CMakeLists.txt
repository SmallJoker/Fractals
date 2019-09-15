# Set up project
cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 11)
project(Fractals)

find_path(CAIRO_INCLUDE_DIR cairo/cairo.h)
find_library(CAIRO_LIBRARY NAMES cairo)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED) # freeglut3-dev

if (NOT MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ffast-math -fomit-frame-pointer")
endif()

set(IMGUI_SOURCE_DIR "${PROJECT_SOURCE_DIR}/imgui" CACHE STRING
	"Path to the Dear ImGui source (and include) files"
)
message(WARNING "${IMGUI_SOURCE_DIR}")

# Source files
set(SRC_FILES
	fractal.cpp
	gui.cpp
	main.cpp
	${IMGUI_SOURCE_DIR}/imgui.cpp
	${IMGUI_SOURCE_DIR}/imgui_draw.cpp
	${IMGUI_SOURCE_DIR}/examples/imgui_impl_opengl2.cpp
	${IMGUI_SOURCE_DIR}/examples/imgui_impl_freeglut.cpp
)

# Include Directories
include_directories(
	${PROJECT_SOURCE_DIR}
	"${CAIRO_INCLUDE_DIR}/cairo"
	${GLUT_INCLUDE_DIR}
	${IMGUI_SOURCE_DIR}
	${OPENGL_INCLUDE_DIR}
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(
	${PROJECT_NAME}
	${CAIRO_LIBRARY}
	${GLUT_LIBRARIES}
	${OPENGL_gl_LIBRARY}
)
