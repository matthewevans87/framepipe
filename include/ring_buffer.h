#pragma once
#include <array>
#include <cstddef>

// Fixed-capacity circular buffer. No heap allocation. No dynamic sizing.
// N is a compile-time constant. Storage is a std::array which lives inline.
// Overwrite-on-full policy: oldest frame is discarded when capacity is reached.
// NOT thread-safe on its own; callers must hold a mutex across push/pop.

template <typename T, std::size_t N>
class RingBuffer
{
public:
    void push(const T &value);
    bool pop(T &out); // returns false if empty
    bool empty() const { return count_ == 0; }
    bool full() const { return count_ == N; }
    std::size_t size() const { return count_; }
    static constexpr std::size_t capacity() { return N; }

private:
    std::array<T, N> buf_{};
    std::size_t head_ = 0; // next write index
    std::size_t tail_ = 0; // next read index
    std::size_t count_ = 0;
};

template <typename T, std::size_t N>
void RingBuffer<T, N>::push(const T &value)
{
    buf_[head_] = value;
    head_ = (head_ + 1) % N;

    if (count_ < N)
    {
        count_++;
    }
    else
    {
        // if full, advance tail
        tail_ = (tail_ + 1) % N;
    }
}

template <typename T, std::size_t N>
bool RingBuffer<T, N>::pop(T &out)
{
    if (count_ == 0)
    {
        return false;
    }
    else
    {
        out = std::move(buf_[tail_]);
        tail_ = (tail_ + 1) % N;
        count_--;
        return true;
    }
}