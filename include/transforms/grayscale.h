#pragma once
#include "transforms/image_transform.h"

class GrayscaleTransform : public ImageTransform
{
public:
    void apply(Frame &frame) override;
    std::string name() const
    {
        return "grayscale";
    }
};