#include "db/db.hpp"
#include "engine/engine.hpp"
#include "excepts.hpp"
#include <string>
#include "input/stream.hpp"
#include "ui/feedback.hpp"
#include "ui/window.hpp"
#include "utils/args.hpp"
#include "runtime.hpp"

// Runtime defaults
bool runtime::FLAG_headless = false;

int main(int argc, char *argv[]) {
    utils::parseArgs(argc, argv);   // this function can change runtime:: flags!

    // OpenCV Test
    cv::Mat frame;
    int deviceID = 0;
    string path = "C:/Users/Danil/Desktop/OPD1/Q9Dp_wnYp0g.jpg";

    cv::Mat img = cv::imread(path);

    cv::CascadeClassifier faceCascade;
    faceCascade.load("C:/opencv/sources/data/haarcascades_cuda/haarcascade_frontalface_default.xml");
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

        imshow("Live", engine :: face_detection (frame, faceCascade, img));
        if (cv::waitKey(5) >= 0)
            break;
        }

    // excepts test
    throw excepts::error("msg", "file", "function", "info");

    // Database test
    db::db database{db::sqlite};
    // database.test();

    // QT Test
    QCoreApplication a(argc, argv);
    return a.exec();
}
