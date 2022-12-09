#include "ui/mainwindow.h"

#include <QApplication>

#include <cstdlib>
#include <opencv2/core.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <pqxx/util.hxx>
#include <string>

#include "db/db.hpp"
#include "engine/engine.hpp"
#include "excepts.hpp"
#include "input/camera.hpp"
#include "input/stream.hpp"
#include "runtime.hpp"
#include "ui/feedback.hpp"
#include "ui/window.hpp"
#include "utils/args.hpp"
#include "utils/host.hpp"

// Runtime defaults
bool runtime::FLAG_headless = false;
bool runtime::FLAG_dryRun = false;

std::string runtime::KEY_db_backend;
std::string runtime::KEY_db_address;
std::string runtime::KEY_db_user;
std::string runtime::KEY_db_passwd;
std::string runtime::KEY_db_name;

db::backends::available backend;

int main(int argc, char* argv[]) {
    //* Args parsing. It may change runtime::FLAG_ *//
    utils::parseArgs(argc, argv);

    //* QMessageBox test... *//
    ui::error("Msg1");
    ui::info("Msg2");
    ui::warn("Msg3");
    ui::msg("Msg4");

    /*
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
    */

    //* Database connect *//
    /// Arg string to backend
    backend = db::backends::none;
    if (runtime::KEY_db_backend == "postgres") backend = db::backends::postgres;
    if (runtime::KEY_db_backend == "sqlite") backend = db::backends::sqlite;

    /// Arg string to addr
    db::addr remoteHost{};
    remoteHost.fromRemoteHost(runtime::KEY_db_address);

    /// Connect
    db::db database{backend, runtime::KEY_db_user, runtime::KEY_db_passwd, runtime::KEY_db_name,
                    remoteHost};
    try {
        database.connect();
    } catch (std::exception& e) {
        ui::error(e.what());
        ui::error(
            "Something goes whong while BigEye trying to access database! Using DryRun mode!");
        runtime::FLAG_dryRun = true;
        database = {};
    }
    database.setup();

    //* Looking for cameras *//
    auto cameraList = input::getCameraList();
    database.serviceWrite({0, db::dataRows::service::types::connectEvent,
                           utils::getDatetime() + ";" + utils::getHostname() + ";" +
                               std::to_string(cameraList.size())});

    //* Database lookup *//
    std::cout << "\t[Service table]\n";
    auto serviceDump = database.serviceRead(database.getRowsCount("service"));
    for (auto& i : serviceDump)
        std::cout << "[ " << std::to_string(i.id) << " | " << i.type << " | " << i.data << " ]\n";

    std::cout << "\n\t[Journal table]\n";
    auto journalDump = database.journalRead(database.getRowsCount("journal"));
    for (auto& i : journalDump)
        std::cout << "[ " << std::to_string(i.id) << " | " << i.datetime << " | " << i.metadata
                  << i.image.size() << " ]\n";

    //* Engine test... *//
    auto dnn = engine::dnnLayer("./deploy.prototxt", "./res10_300x300_ssd_iter_140000_fp16.caffemodel", {0.5, engine::dnnLayer::dnnBackends::cuda});
    engine::dnnReturns ret;
    cv::Mat frame;

    input::cameraDevice camera = cameraList.at(0);
    cv::VideoCapture cap{input::openCamera(camera.descriptor)};

    // For jpeg upload
    std::vector<unsigned char> buff;
    std::vector<int> param {cv::IMWRITE_JPEG_QUALITY, 60};

    // Temporary loop
    while (true) {
        cap.read(frame);
        dnn.processFrame(frame, true);

        cv::imencode(".jpg", frame, buff, param);
        
        database.journalWrite({0, utils::getDatetime(), "TESTDATA", buff}); // TEMPORARY

        cv::imshow(camera.name, frame);
        if (cv::waitKey(5) == 27) break;
        //break; // TEMPORARY
    }

    // Disconnecting
    database.serviceWrite({0, db::dataRows::service::types::disconnectEvent,
                           utils::getHostname() + ";" + utils::getDatetime()});
    database.disconnect();

    return 0;
}
