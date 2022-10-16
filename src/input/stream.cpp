#include "input/stream.hpp"

namespace input {

cv::VideoCapture openVideo(const std::string path) {
    cv::VideoCapture cap;
    cap.open("./");
    return cap;
}

cv::VideoCapture openCamera(const int descriptor) {
    cv::VideoCapture cap;
    cap.open(descriptor);
    return cap;
}

} // namespace input