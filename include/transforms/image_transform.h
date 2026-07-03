#pragma once
#include "frame.h"
#include <string>

class ImageTransform
{
public:
    virtual void apply(Frame &frame) = 0;
    virtual std::string name() const = 0;
    virtual ~ImageTransform() = default;
};