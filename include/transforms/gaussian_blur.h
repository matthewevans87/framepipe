#pragma once
#include "transforms/image_transform.h"

class GaussianBlurTransform : public ImageTransform
{
private:
    int radius_;

public:
    GaussianBlurTransform(int radius);
    void apply(Frame &frame) override;
    std::string name() const override { return "gaussianblur"; }
};