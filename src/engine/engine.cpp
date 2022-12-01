#include "engine/engine.hpp"

using namespace std;

namespace engine
{
    cv :: Mat adjustment(cv :: Mat& img)
    {
       cv :: Mat imgGray, imgBlur, imgCanny, imgDil;
       cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);
       GaussianBlur(imgGray, imgBlur, cv :: Size(3,3), 3, 0);
       Canny(imgBlur, imgCanny, 25, 75);
       cv :: Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
       dilate(imgCanny, imgDil, kernel);

       return imgDil;
    }

    void getContours(cv::Mat img)
    {
        vector<vector<cv::Point>> contours;
        vector<vector<cv::Vec4i>> hierarchy;

        findContours(img, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        for(int i = 0; i < contours.size(); i++)
        {
            double area = cv :: contourArea(contours[i]);
            std::cout << area << std::endl;
        }
    }

    cv::Mat& face_detection (cv::Mat& frame, cv::CascadeClassifier& faceCascade)
    {
        std::vector<cv::Rect> faces;
        cv :: Rect roi;
        cv::Mat crop;

        faceCascade.detectMultiScale(frame,faces, 1.1, 3);
    //    if (frame.empty()) {
    //        cerr << "ERROR! blank frame grabbed\n";
    //        break;
    //    }

       for(size_t i = 0; i < faces.size(); ++i)
       {
           int x1 = faces[i].x;
           int y1 = faces[i].y;
           int x2 = faces[i].x + faces[i].width;
           int y2 = faces[i].y + faces[i].height;
           rectangle(frame, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(255, 0, 255), 3);

           roi.x = faces[i].x;
           roi.y = faces[i].y;
           roi.width = faces[i].width;
           roi.height = faces[i].height;
           crop = frame(roi);
           cv::imwrite("face.jpg", crop);
       }

       cv :: Mat img = cv :: imread("face.jpg");

       img = adjustment(img);

       getContours(img);
       return frame;
    }
}
