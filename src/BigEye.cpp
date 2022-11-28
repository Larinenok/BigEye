#include <string>

#include "db/db.hpp"
#include "engine/engine.hpp"
#include "excepts.hpp"
#include "input/stream.hpp"
#include "input/camera.hpp"
#include "runtime.hpp"
#include "ui/feedback.hpp"
#include "ui/window.hpp"
#include "utils/args.hpp"
#include "utils/host.hpp"

// Runtime defaults
bool runtime::FLAG_headless = false;
bool runtime::FLAG_dryRun   = false;

int main(int argc, char *argv[]) {
    // Args parsing. It may change runtime::FLAG_*
    utils::parseArgs(argc, argv);

    // QMessageBox test...
    ui::error("Msg1");
    ui::info("Msg2");
    ui::warn("Msg3");
    ui::msg("Msg4");

    // Database connect
    db::db database{db::backends::postgres,
                    "postgres",
                    "youmu",
                    "bigeye",
                    {"127.0.0.1", db::getDefaults(db::backends::postgres).address.port}};
    try {
        database.connect();
    } catch (std::exception &e) {
        ui::error(e.what());
        ui::error("Something goes whong while BigEye trying to access database! Using DryRun mode!");
        runtime::FLAG_dryRun = true;
        database = {};
    }
    database.setup();

    // Database test...
    std::cout << "\t[Service table]\n";
    database.serviceWrite({0, db::dataRows::service::types::connectEvent, utils::getDatetime()});
    database.serviceWrite({0, db::dataRows::service::types::disconnectEvent, utils::getDatetime()});
    auto bufff = database.serviceRead(database.getRowsCount("service"));
    for (auto& i : bufff) {
        std::cout << "[ " << std::to_string(i.id) << " | "<< i.type << " | " << i.data << " ]\n";
    }

    std::cout << "\n\t[Journal table]\n";
    database.journalWrite({0, utils::getDatetime(), "data1:"});
    database.journalWrite({0, utils::getDatetime(), "data2:"});
    auto buff = database.journalRead(database.getRowsCount("journal"));
    for (auto& i : buff) {
        std::cout << "[ " << std::to_string(i.id) << " | "<< i.datetime << " | " << i.metadata << " ]\n";
    }
    // exit(-1);
    // Engine test...
    cv::Mat frame;
    auto cameraList = input::getCameraList();
    input::cameraDevice camera = cameraList.at(0);
    int deviceID = camera.descriptor;

    cv::CascadeClassifier faceCascade;
    faceCascade.load("./haarcascade_frontalface_default.xml");
    cv::VideoCapture cap = input::openCamera(deviceID);

    while (true)
    {
        cap.read(frame);
        resize(frame, frame, cv::Size(camera.mode.y, camera.mode.x), 0, 0, cv::INTER_CUBIC);

        imshow(camera.name, engine::face_detection(frame, faceCascade));
        if (cv::waitKey(5) == 27) break; // ESC
    }
}
