#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include "shared_state.h"
#include "capture.h"
#include "process.h"
#include "pipeline.h"
#include "transforms/grayscale.h"
#include "transforms/gaussian_blur.h"
#include "transforms/sobel.h"
#include "transforms/threshold.h"

static void add_transform(TransformPipeline &pipeline, const std::string &arg)
{
    const auto starts_with = [&](const char *prefix)
    {
        const std::string p(prefix);
        return arg.size() >= p.size() && arg.compare(0, p.size(), p) == 0;
    };

    if (arg == "grayscale")
        pipeline.add(std::make_unique<GrayscaleTransform>());
    else if (starts_with("blur"))
    {
        int radius = 2;
        if (arg.size() > 5)
            radius = std::stoi(arg.substr(5));
        pipeline.add(std::make_unique<GaussianBlurTransform>(radius));
    }
    else if (arg == "sobel")
        pipeline.add(std::make_unique<SobelTransform>());
    else if (starts_with("threshold:"))
    {
        uint8_t value = static_cast<uint8_t>(std::stoi(arg.substr(10)));
        pipeline.add(std::make_unique<ThresholdTransform>(value));
    }
    else
    {
        std::cerr << "Unknown transform: " << arg;
    }
}

int main(int argc, char *argv[])
{

    int camera_id;
    try
    {
        camera_id = std::stoi(argv[1]);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error parsing Camera Id: " << argv[1];
    }

    SharedState state;
    TransformPipeline pipeline;

    for (int i = 2; i < argc; ++i)
    {
        std::string arg = argv[i];
        add_transform(pipeline, arg);
    }

    // pipeline.add(std::make_unique<GrayscaleTransform>());
    std::thread cap_t(capture_thread, std::ref(state), camera_id);
    std::thread proc_t(process_thread, std::ref(state), std::ref(pipeline));

    // Keep the main thread alive until processing thread requests shutdown.
    while (state.running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    state.cv.notify_all(); // wake process thread so it can exit

    cap_t.join();
    proc_t.join();

    std::cout << "Dropped frames: " << state.dropped_frames << "\n";
}