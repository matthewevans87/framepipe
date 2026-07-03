#include "pipeline.h"
#include <iostream>

void TransformPipeline::add(std::unique_ptr<ImageTransform> transform)
{
    transforms_.push_back(std::move(transform));
}

void TransformPipeline::run(Frame &frame) const
{
    for (const auto &transform : transforms_)
    {
        transform->apply(frame);
    }
}