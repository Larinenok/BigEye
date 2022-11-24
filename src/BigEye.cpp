#include <string>

#include "db/db.hpp"
#include "engine/engine.hpp"
#include "excepts.hpp"
#include "input/stream.hpp"
#include "runtime.hpp"
#include "ui/feedback.hpp"
#include "ui/window.hpp"
#include "utils/args.hpp"

// Runtime defaults
bool runtime::FLAG_headless = false;
bool runtime::FLAG_dryRun   = false;

int main(int argc, char *argv[]) {
    utils::parseArgs(argc, argv);  // this function can change runtime:: flags!

    // Database test
    try {
        db::db database{db::backends::postgres,
                        "postgres",
                        "youmu",
                        "bigeye",
                        {"127.0.0.1", db::getDefaults(db::backends::postgres).address.port}};
        
        database.setup();

        database.journal->write({"001", "Wed Nov 23 06:51:57 PM +07 2022", "data:"});
        database.journal->write({"002", "Wed Nov 23 06:52:33 PM +07 2022", "data:"});

        auto buff = database.journal->read();
        for (auto& i : buff) {
            std::cout << "[ " << i.id << " | " << i.datetime << " | " << i.metadata << " ]\n";
        }

/*
- database.journal->read ---\
- database.registered->read -+  --> backend::read("")
- database.service->read ---/
*/

/*
- database.read("journal", {1}) -> backend::read("")
*/

    } catch (std::exception &e) {
        ui::error(e.what());
        ui::error("Something goes whong while BigEye trying to access database! Using DryRun mode!");
        runtime::FLAG_dryRun = true;
    }

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
