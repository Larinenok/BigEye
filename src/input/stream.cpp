#include "input/stream.hpp"

cv :: VideoCapture openCamera(const int deviceID)
{
    cv::VideoCapture cap;
    int apiID = cv::CAP_ANY;
    cap.open(deviceID, apiID);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 320);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 180);
//    if (!cap.isOpened()) {
//        cerr << "ERROR! Unable to open camera\n";
//        return -1;
//    }
    return cap;
}
