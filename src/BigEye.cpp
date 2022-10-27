#include "engine/engine.hpp"
#include "input/stream.hpp"
#include "ui/window.hpp"
#include "ui/feedback.hpp"
#include "excepts.hpp"

int main(int argc, char *argv[])
{
    // OpenCV Test
    cv::VideoCapture streamVideo = input::openVideo("./video.webm");
    cv::VideoCapture streamCam = input::openCamera(0);

    streamVideo.release();
    streamCam.release();

    // ui/feedback test (check taskpool pls)
    ui::error("Error_test");
    ui::warn("Warn_test");
    ui::info("Info_test");
    ui::msg("Message_test");

    // excepts test
    throw excepts::error("msg","file","function","info");

    // QT Test
    QCoreApplication a(argc, argv);
    return a.exec();
}
