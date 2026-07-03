#include "shared_state.h"
#include "pipeline.h"
#include "frame.h"
#include <opencv2/highgui.hpp> // cv::imshow, cv::waitKey

void process_thread(SharedState &state, TransformPipeline &pipeline)
{
    while (state.running || !state.buffer.empty())
    {
        Frame frame;
        {
            std::unique_lock<std::mutex> lock(state.mutex);
            state.cv.wait(lock, [&]
                          { return !state.buffer.empty() || !state.running; });

            // Guard against spurious wakeups
            if (state.buffer.empty())
            {
                break;
            }
            state.buffer.pop(frame);
        }
        pipeline.run(frame);
        cv::Mat mat(frame.height, frame.width, CV_8UC3, frame.data.data());
        cv::imshow("framepipe", mat);
        if (cv::waitKey(1) == 'q')
        {
            state.running = false;
        }
    }
}