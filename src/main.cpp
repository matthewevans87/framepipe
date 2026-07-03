#include <iostream>
#include <thread>
#include "shared_state.h"
#include "capture.h"
#include "process.h"
#include "pipeline.h"
#include "transforms/grayscale.h"

int main(int argc, char *argv[])
{
    int camera_id = 0;

    SharedState state;
    TransformPipeline pipeline;
    pipeline.add(std::make_unique<GrayscaleTransform>());
    std::thread cap_t(capture_thread, std::ref(state), camera_id);
    std::thread proc_t(process_thread, std::ref(state), std::ref(pipeline));

    // wait for 'q' keypress in the display window
    // then:
    state.running = false;
    state.cv.notify_all(); // wake process thread so it can exit

    cap_t.join();
    proc_t.join();

    std::cout << "Dropped frames: " << state.dropped_frames << "\n";
}