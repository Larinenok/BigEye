#include "engine/engine.hpp"
#include "excepts.hpp"
#include "input/stream.hpp"
#include "ui/feedback.hpp"
#include "ui/window.hpp"

int main(int argc, char *argv[]) {
    // OpenCV Test
    //cv::VideoCapture streamVideo = input::openVideo("./video.webm");
    cv::Mat frame;
    int deviceID = 0;
    cv::VideoCapture cap = input::openCamera(deviceID);

//    streamVideo.release();
//    streamCam.release();

    // ui/feedback test (check taskpool pls)
    ui::error("Error_test");
    ui::warn("Warn_test");
    ui::info("Info_test");
    ui::msg("Message_test");

    // Face detection

    while (true)
    {
        cap.read(frame);
        resize(frame, frame, cv::Size(320, 180), 0, 0, cv::INTER_CUBIC);

        frame = engine :: adjustment(frame);

        frame = engine :: face_detection (frame);

        imshow("Live", frame);
        if (cv::waitKey(5) >= 0)
            break;
        }

    // excepts test
    throw excepts::error("msg", "file", "function", "info");

    // QT Test
    QCoreApplication a(argc, argv);
    return a.exec();
}
