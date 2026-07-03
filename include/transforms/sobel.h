#pragma once
#include "transforms/image_transform.h"

class SobelTransform : public ImageTransform
{
public:
    std::string name() const override { return "sobel"; }
    void apply(Frame &frame) override;
};