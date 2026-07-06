#include "shared_state.h"
#include <opencv2/videoio.hpp> // cv::VideoCapture

void capture_thread(SharedState &state, int camera_id)
{
    cv::VideoCapture cap(camera_id, cv::CAP_V4L2);
    if (!cap.isOpened())
    {
        cap.open(camera_id);
    }
    if (!cap.isOpened())
    {
        throw std::runtime_error("Unable to open camera: " + std::to_string(camera_id));
    }

    while (state.running)
    {
        // grab camera frame
        cv::Mat mat;
        cap >> mat;

        // skip empty frames
        if (mat.empty())
        {
            continue;
        }

        size_t total_bytes = mat.total() * mat.channels();
        Frame frame;
        frame.data.assign(mat.data, mat.data + total_bytes);
        frame.channels = mat.channels();
        frame.height = mat.rows;
        frame.width = mat.cols;
        // only lock while working with buffer
        {
            std::lock_guard<std::mutex> lock(state.mutex);
            if (state.buffer.full())
            {
                state.dropped_frames++;
            }
            state.buffer.push(frame);
        }

        state.cv.notify_one();
    }
}