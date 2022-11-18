#include "engine/engine.hpp"

namespace engine
{
   cv :: Mat& adjustment(cv :: Mat& frame)
    {
        //cv::Mat frameGray, frameBlur, frameCanny, frameDil;
        cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
        GaussianBlur(frame, frame, cv::Size(3, 3), 3, 0);
        Canny(frame, frame, 25, 75);
        cv::Mat kernel = getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
        dilate(frame, frame, kernel);
        return frame;
    }

    cv::Mat& face_detection (cv::Mat& frame, cv::CascadeClassifier& faceCascade, cv::Mat& img)
    {
        std::vector<cv::Rect> faces;
        std::vector<cv::Rect> faces1;

        faceCascade.detectMultiScale(frame,faces, 1.1, 2);
        faceCascade.detectMultiScale(img,faces1, 1.1, 2);
    //    if (frame.empty()) {
    //        cerr << "ERROR! blank frame grabbed\n";
    //        break;
    //    }

       for(size_t i = 0; i < faces.size(); ++i)
       {
           rectangle(frame, faces[i].tl(), faces[i].br(), cv::Scalar(255, 0, 255), 5);
       }

        return frame;
    }
}
