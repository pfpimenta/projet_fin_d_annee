cmake_minimum_required(VERSION 2.6)

project(main)

set(CMAKE_BUILD_TYPE debug)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -O2 -g -std=c++11 -Wno-unused-parameter")

set(CMAKE_CPP_FLAGS "-I/usr/include/irrlicht")

set(CMAKE_LD_FLAGS "-lIrrlicht -lGL -lGLU -lXxf86vm -lXext -lX11")


FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
find_package( OpenCV REQUIRED )

file(
	GLOB
	source_files
        code_projet/src/*.[ch]pp
        code_projet/lib/*.[ch]pp
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})





add_executable(
	projetFA
	${source_files}
)


TARGET_LINK_LIBRARIES(projetFA -lm -ldl -lGLEW ${CMAKE_LD_FLAGS} ${OPENGL_LIBRARIES} ${QT_LIBRARIES} ${QT_GL_LIBRARIES} ${QT_QTOPENGL_LIBRARY} ${OpenCV_LIBS})
