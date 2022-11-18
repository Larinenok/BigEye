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

    cv::Mat frame;
    int deviceID = 0;

    cv::CascadeClassifier faceCascade;
    faceCascade.load("./haarcascade_frontalface_default.xml");
    cv::VideoCapture cap = input::openCamera(deviceID);
    
    while (true)
    {
        cap.read(frame);
        resize(frame, frame, cv::Size(320, 180), 0, 0, cv::INTER_CUBIC);

        imshow("Live", engine::face_detection(frame, faceCascade));
        if (cv::waitKey(5) == 27) break; // ESC
    }
}
