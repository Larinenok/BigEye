#include <opencv2/opencv.hpp>
#include <vector>

namespace engine
{
    cv :: Mat& adjustment(cv :: Mat& frame);
    cv::Mat& face_detection(cv::Mat& frame, cv::CascadeClassifier& faceCascade);
}  // namespace engine
