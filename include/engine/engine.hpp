#include <opencv2/opencv.hpp>
#include <vector>

namespace engine
{
    cv :: Mat adjustment(cv :: Mat& img);
    cv::Mat& face_detection(cv::Mat& frame, cv::CascadeClassifier& faceCascade);
    void getContours(cv::Mat img);
}  // namespace engine
