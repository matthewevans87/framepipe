# Framepipe

A lightweight C++ frame processing pipeline that demonstrates a thread-safe ring buffer between capture and processing stages, with configurable image transforms.

## Features

- Real-time frame capture and processing
- Demonstrates a producer/consumer ring buffer design
- Pluggable transform pipeline
- Built-in transforms: grayscale, Gaussian blur, Sobel, threshold
- Unit test for ring buffer behavior

## Requirements

- CMake 3.16+
- C++20 compiler
- OpenCV (if required by your local setup)

## Build
```sh
cmake -S . -B build
cmake --build build
```
## Run
```
./build/framepipe <camera_id> [transforms...]
```
Example:
```sh
./build/framepipe 0 grayscale blur:5 sobel threshold:10
```
## Available Transforms

- grayscale
- blur:<radius>
- sobel
- threshold:<value>

## Test
```sh
ctest --test-dir build --output-on-failure
```