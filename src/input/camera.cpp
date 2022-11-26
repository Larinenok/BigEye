#include "input/camera.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

#include "utils/hostname.hpp"

namespace input {

std::vector<cameraDevice> getCameraList() {
    std::vector<cameraDevice> ret = {};
    ret.reserve(searchRange);

    // Just search /dev/video* and for windows return exception   :3 (joke)
    for (uint8_t i = 0; i < searchRange; i++) {
        cv::VideoCapture obj(i, cv::CAP_ANY);
        if (obj.isOpened())
            ret.push_back({utils::getHostname() + "-" + std::to_string(i),
                           i,
                           {static_cast<unsigned int>(obj.get(cv::CAP_PROP_FRAME_HEIGHT)),
                            static_cast<unsigned int>(obj.get(cv::CAP_PROP_FRAME_WIDTH)),
                            static_cast<unsigned int>(obj.get(cv::CAP_PROP_FPS))}});
        obj.release();
    }

    // if empty return default camera (0) : // TODO: no camera
    return ret.size() ? ret : std::vector{cameraDevice{}};
}

}  // namespace input