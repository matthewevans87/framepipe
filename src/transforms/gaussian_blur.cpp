#include "transforms/gaussian_blur.h"
#include <vector>
#include <cmath>
#include <algorithm>

static std::vector<float> make_kernel(int radius)
{
    std::vector<float> k(2 * radius + 1);
    float sigma = radius / 2.0f;
    float sum = 0.0f;

    for (int i = -radius; i <= radius; i++)
    {
        k[i + radius] = std::exp(-(i * i) / (2.0f * sigma * sigma));
        sum += k[i + radius];
    }
    for (auto &v : k)
        v /= sum; // normalize
    return k;
}

GaussianBlurTransform::GaussianBlurTransform(int radius)
    : radius_(radius)
{
}

void GaussianBlurTransform::apply(Frame &frame)
{
    // Separable blur: horizontal pass then vertical pass

    // Only operates on single channel (grayscale) images for simplicity
    if (frame.channels > 1)
        return;

    int w = frame.width;
    int h = frame.height;
    auto kernel = make_kernel(radius_);
    int r = radius_;
    Frame h_pass_result(w, h, 1); // intermediate after horizontal pass
    Frame v_pass_result(w, h, 1); // final after vertical pass

    // Horizontal pass
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            float acc = 0.0f;
            for (int k = -r; k <= r; ++k)
            {
                int sx = std::clamp(x + k, 0, w - 1); // clamp to border
                acc += kernel[k + r] * frame.at(sx, y, 0);
            }
            h_pass_result.at(x, y, 0) = static_cast<uint8_t>(std::clamp(acc, 0.0f, 255.0f));
        }
    }

    // Vertical pass
    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            float acc = 0.0f;
            for (int k = -r; k <= r; ++k)
            {
                int sy = std::clamp(y + k, 0, h - 1);
                acc += kernel[k + r] * h_pass_result.at(x, sy, 0);
            }
            v_pass_result.at(x, y, 0) = static_cast<uint8_t>(std::clamp(acc, 0.0f, 255.0f));
        }
    }

    frame = std::move(v_pass_result);
}