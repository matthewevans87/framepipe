#include "transforms/threshold.h"
#include "frame.h"

ThresholdTransform::ThresholdTransform(int threshold)
    : threshold_(threshold)
{
}

void ThresholdTransform::apply(Frame &frame)
{
    int w = frame.width;
    int h = frame.height;
    Frame result(w, h, 1);

    for (int x = 0; x < w; ++x)
    {
        for (int y = 0; y < h; ++y)
        {
            uint8_t val = frame.at(x, y, 0);
            if (val < threshold_)
            {
                val = 0;
            }
            result.at(x, y, 0) = val;
        }
    }

    frame = std::move(result);
}