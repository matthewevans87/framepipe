#pragma once
#include <vector>
#include <cstdint>
#include <chrono>

struct Frame
{
    Frame()
    {
    }
    Frame(int w, int h, int c) : width(w), height(h), channels(c)
    {
        data.resize(w * h * c);
    }

    static constexpr int B_INDEX = 0;
    static constexpr int G_INDEX = 1;
    static constexpr int R_INDEX = 2;

    std::vector<uint8_t> data; // raw BGR pixel bytes (OpenCV format)
    int width = 0;
    int height = 0;
    int channels = 0;
    uint64_t sequence_id = 0; // monotonically increasing frame counter
    std::chrono::steady_clock::time_point timestamp;

    uint8_t *row(int y)
    {
        return data.data() + y * width * channels;
    }
    const uint8_t *row(int y) const
    {
        return data.data() + y * width * channels;
    }
    uint8_t &at(int x, int y, int c)
    {
        return data[(y * width + x) * channels + c];
    }
};