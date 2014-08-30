CMAKE_MINIMUM_REQUIRED(VERSION 3.0.1)

SET(TEST_VERSION "")
SET(PROJECT_NAME "Squares")

PROJECT( ${PROJECT_NAME} )

SET(PROJECT_BINARY_DIR ".")
SET(EXECUTABLE_OUTPUT_PATH ../bin)

SET(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")

# Find these required libraries.
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})

# Setup the source files we are using
SET(CORE_SOURCE_FILES "square.cpp")

SET(CORE_SOURCE_HEADERS "")

add_executable(${PROJECT_NAME} ${CORE_SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
