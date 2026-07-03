cmake_minimum_required(VERSION 3.20)
project(framepipe CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

add_executable(framepipe
    src/main.cpp
    src/capture.cpp
    src/pipeline.cpp
    src/process.cpp
    src/transforms/grayscale.cpp
    src/transforms/gaussian_blur.cpp
    src/transforms/sobel.cpp
    src/transforms/threshold.cpp
    )
target_include_directories(framepipe PRIVATE include ${OpenCV_INCLUDE_DIRS})
target_link_libraries(framepipe PRIVATE ${OpenCV_LIBS} Threads::Threads)

# --- tests ---
include(FetchContent)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(ring_buffer_test tests/ring_buffer_test.cpp)
target_include_directories(ring_buffer_test PRIVATE include)
target_link_libraries(ring_buffer_test PRIVATE GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(ring_buffer_test
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})