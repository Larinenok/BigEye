#include <opencv2/opencv.hpp>

namespace input {

cv::VideoCapture openVideo(const std::string path);

cv::VideoCapture openCamera(const int descriptor);

} // namespace input