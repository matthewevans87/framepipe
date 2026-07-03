#include "transforms/grayscale.h"
#include <algorithm> // std::clamp

void GrayscaleTransform::apply(Frame &frame)
{
    if (frame.channels < 3)
        return; // already grayscale or single channel

    // create new single channel image
    int w = frame.width;
    int h = frame.height;
    int c = frame.channels;
    Frame result(w, h, 1);

    for (int y = 0; y < h; ++y)
    {
        const uint8_t *src = frame.row(y); // pointer to start of row
        uint8_t *dst = result.row(y);

        for (int x = 0; x < w; ++x)
        {
            // ITU-R BT.601 luminance weights
            float lum = 0.299f * src[x * c + frame.R_INDEX]    // R
                        + 0.587f * src[x * c + frame.G_INDEX]  // G
                        + 0.114f * src[x * c + frame.B_INDEX]; // B
            dst[x] = static_cast<uint8_t>(std::clamp(lum, 0.0f, 255.0f));
        }
    }

    frame = std::move(result); // move assignment -- no copy
    frame.channels = 1;
}
