#include "ui/mainwindow.h"

#include <QApplication>
#include <QObject>
#include <QLabel>
#include <QObject>

#include <cstdlib>
#include <exception>
#include <opencv2/core.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <pqxx/util>
#include <string>
#include <thread>
#include <chrono>

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
bool runtime::FLAG_useCuda = false;
bool runtime::FLAG_noScan = false;

std::string runtime::KEY_db_backend;
std::string runtime::KEY_db_address;
std::string runtime::KEY_db_user;
std::string runtime::KEY_db_passwd;
std::string runtime::KEY_db_name;

db::backends::available backend;

QImage Mat2QImage(cv::Mat const& src) {
    cv::Mat temp; 
    cvtColor(src, temp, cv::COLOR_BGR2RGB); 
    QImage dest((const uchar *)temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
    dest.bits(); 
    // of QImage::QImage ( const uchar * data, int width, int height, Format format )
    return dest;
}

int main(int argc, char** argv) {
    //* Args parsing. It may change runtime::FLAG_ *//
    utils::parseArgs(argc, argv);

    //* QMessageBox test... *//
    ui::error("Msg1");
    ui::info("Msg2");
    ui::warn("Msg3");
    ui::msg("Msg4");

    //------ Cringe GUI ------//
    // if (!runtime::FLAG_headless) {}

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QImage myImage;

    //------ Database connect ------//
    /// Arg string to backend1
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

    //------ Looking for cameras ------//
    std::vector<input::cameraDevice> cameraList = {input::cameraDevice{}};
    if (!runtime::FLAG_noScan)
        cameraList = input::getCameraList();

    database.serviceWrite({0, db::dataRows::service::types::connectEvent,
                           utils::getDatetime() + ";" + utils::getHostname() + ";" +
                               std::to_string(cameraList.size())});

    //------ Database lookup ------//
    std::cout << "\t[Service table]\n";
    auto serviceDump = database.serviceRead(database.getRowsCount("service"));
    for (auto& i : serviceDump){
        std::cout << "[ " << std::to_string(i.id) << " | " << i.type << " | " << i.data << " ]\n";
        w.addNewServiceItem(
            std::to_string(i.id),
            "Later...",
            i.data);
    }

    std::cout << "\n\t[Journal table]\n";
    auto journalDump = database.journalRead(database.getRowsCount("journal"));
    for (auto& i : journalDump) {
        std::cout << "[ " << std::to_string(i.id) << " | " << i.datetime << " | " << i.metadata
                  << i.image.size() << " ]\n";
        w.addNewJournalItem(i.datetime, i.metadata, i.metadata, std::to_string(i.id), {});
    }

    //------ Engine test... ------//
    auto dnn = engine::dnnLayer(
        "./deploy.prototxt", "./res10_300x300_ssd_iter_140000_fp16.caffemodel",
        {0.5, runtime::FLAG_useCuda ?
            engine::dnnLayer::dnnBackends::cuda :
            engine::dnnLayer::dnnBackends::cpu
        });
    engine::dnnReturns ret;
    cv::Mat frame;

    input::cameraDevice camera = cameraList.at(0);
    cv::VideoCapture cap{input::openCamera(camera.descriptor)};

    // For jpeg upload
    std::vector <double> Points;

    // Temporary loop
    while (true) {
        cap.read(frame);
        // DNN
        dnn.processFrame(frame, true, Points, database);

        // Draw the image on GUI
        w.updateFrame(Mat2QImage(frame));

        // Update events
        a.processEvents();
    }

    // Disconnecting
    database.serviceWrite({0, db::dataRows::service::types::disconnectEvent,
                           utils::getHostname() + ";" + utils::getDatetime()});
    database.disconnect();

    return a.exec();
}
