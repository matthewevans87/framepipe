#pragma once

#include "ring_buffer.h"
#include "frame.h"
#include <mutex>
#include <condition_variable>
#include <atomic>

struct SharedState
{
    RingBuffer<Frame, 8> buffer;
    std::mutex mutex;
    std::condition_variable cv;
    std::atomic<bool> running{true};
    uint64_t dropped_frames{0};
};