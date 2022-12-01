#include "input/stream.hpp"
#include "excepts.hpp"

namespace input {

cv::VideoCapture openCamera(const int deviceID) {
    cv::VideoCapture cap;
    int apiID = cv::CAP_ANY;
    cap.open(deviceID, apiID);
    if (!cap.isOpened())
        throw excepts::error("Unable to open camera! Descriptor: " + std::to_string(deviceID));
    return cap;
}
}  // namespace input
