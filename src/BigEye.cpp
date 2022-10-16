#include "engine/engine.hpp"
#include "input/stream.hpp"
#include "ui/window.hpp"

int main(int argc, char *argv[])
{
    // OpenCV Build Test
    cv::VideoCapture streamVideo = input::openVideo("./video.webm");
    cv::VideoCapture streamCam = input::openCamera(0);

    streamVideo.release();
    streamCam.release();

    // QT Build Test
    QCoreApplication a(argc, argv);

    return a.exec();
}
