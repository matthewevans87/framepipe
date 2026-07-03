#include <gtest/gtest.h>
#include "ring_buffer.h"

TEST(RingBuffer, PushPopBasic) // push 3, pop 3, verify order (FIFO)
{
    RingBuffer<int, 10> rb;
    rb.push(1);
    rb.push(2);
    rb.push(3);
    int output;
    bool ok;
    ok = rb.pop(output);
    ASSERT_EQ(output, 1);
    ASSERT_TRUE(ok);
    ok = rb.pop(output);
    ASSERT_EQ(output, 2);
    ASSERT_TRUE(ok);
    ok = rb.pop(output);
    ASSERT_EQ(output, 3);
    ASSERT_TRUE(ok);
}
TEST(RingBuffer, EmptyPop) // pop on empty returns false
{
    RingBuffer<int, 10> rb;
    int output;
    bool ok = rb.pop(output);
    ASSERT_FALSE(ok);
}
TEST(RingBuffer, FullFlag) // push N items, verify full() is true
{
    RingBuffer<int, 3> rb;
    rb.push(1);
    rb.push(2);
    rb.push(3);
    ASSERT_TRUE(rb.full());
}
TEST(RingBuffer, OverwriteOnFull) // push N+1, verify oldest is gone
{
    RingBuffer<int, 3> rb;
    rb.push(1);
    rb.push(2);
    rb.push(3);
    rb.push(4);
    int output;
    bool ok = rb.pop(output);
    ASSERT_TRUE(ok);
    ASSERT_EQ(output, 2);
}
TEST(RingBuffer, Wraparound) // push/pop past the array boundary; verify indices wrap
{
    RingBuffer<int, 3> rb;
    rb.push(1);
    rb.push(2);
    rb.push(3);
    rb.push(4);

    int output;
    bool ok;
    ok = rb.pop(output);
    ASSERT_TRUE(ok);
    ASSERT_EQ(output, 2);

    ok = rb.pop(output);
    ASSERT_TRUE(ok);
    ASSERT_EQ(output, 3);

    ok = rb.pop(output);
    ASSERT_TRUE(ok);
    ASSERT_EQ(output, 4);

    ok = rb.pop(output);
    ASSERT_FALSE(ok);
}
TEST(RingBuffer, SizeTracking) // interleaved push/pop; verify size() stays correct
{
    RingBuffer<int, 3> rb;
    int output;
    bool ok;

    rb.push(1);
    ASSERT_EQ(rb.size(), 1);
    ok = rb.pop(output);
    ASSERT_TRUE(ok);
    ASSERT_EQ(output, 1);
    ASSERT_EQ(rb.size(), 0);

    rb.push(2);
    ASSERT_EQ(rb.size(), 1);
    ok = rb.pop(output);
    ASSERT_TRUE(ok);
    ASSERT_EQ(output, 2);
    ASSERT_EQ(rb.size(), 0);

    rb.push(3);
    ASSERT_EQ(rb.size(), 1);
    ok = rb.pop(output);
    ASSERT_TRUE(ok);
    ASSERT_EQ(output, 3);
    ASSERT_EQ(rb.size(), 0);
}