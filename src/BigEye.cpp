#include <QCoreApplication>
#include <opencv2/opencv.hpp>

int main(int argc, char *argv[])
{
    cv::VideoCapture cap;
    cap.open("./");
    QCoreApplication a(argc, argv);

    return a.exec();
}
