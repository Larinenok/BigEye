#include "engine/engine.hpp"

namespace engine
{
    cv::Mat adjustment(cv::Mat frame)
    {
        cv::Mat frameGray, frameBlur, frameCanny, frameDil;
        cvtColor(frame, frameGray, cv::COLOR_BGR2GRAY);
        GaussianBlur(frameGray, frameBlur, cv::Size(3, 3), 3, 0);
        Canny(frameBlur, frameCanny, 25, 75);
        cv::Mat kernel = getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
        dilate(frameCanny, frameDil, kernel);
        return frameDil;
    }

    cv::Mat face_detection (cv::Mat frame)
    {
        cv::CascadeClassifier faceCascade;
        faceCascade.load("C:/opencv/sources/data/haarcascades/haarcascade_frontalface_default.xml");
        std::vector<cv::Rect> faces;

        faceCascade.detectMultiScale(frame,faces, 1.1, 3);
    //    if (frame.empty()) {
    //        cerr << "ERROR! blank frame grabbed\n";
    //        break;
    //    }
        for(int i = 0; i < faces.size(); i++)
        {
            rectangle(frame, faces[i].tl(), faces[i].br(), cv::Scalar(255, 0, 255), 3);
        }

        return frame;
    }
}
