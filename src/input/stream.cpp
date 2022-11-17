#include "input/stream.hpp"

namespace input
{

    cv :: VideoCapture openCamera(const int deviceID)
    {
        cv::VideoCapture cap;
        int apiID = cv::CAP_ANY;
        cap.open(deviceID, apiID);
    //    if (!cap.isOpened()) {
    //        cerr << "ERROR! Unable to open camera\n";
    //        return -1;
    //    }
        return cap;
    }
}
