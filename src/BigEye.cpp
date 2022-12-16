#include "ui/sqlwindow.h"
#include "ui/mainwindow.h"

#include <QApplication>

#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/dnn/dnn.hpp>

#include "engine/engine.hpp"
#include "input/camera.hpp"
#include "input/stream.hpp"
#include "utils/args.hpp"
#include "utils/host.hpp"
#include "ui/feedback.hpp"
#include "db/db.hpp"
#include "runtime.hpp"
#include "excepts.hpp"

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

QImage Mat2QImage(cv::Mat const& src);
void pullUpdates(db::db& database, MainWindow& w, uint32_t& journalLastID, size_t& journalCount, size_t& counter);

int main(int argc, char** argv) {
    //* It may change runtime::FLAG_ *//
    utils::parseArgs(argc, argv);

    //------ Cringe GUI ------//
    QApplication a(argc, argv);

    SqlWindow sw;
    sw.show();
    while (true) {
        a.processEvents();
        if (sw.isClosed) break;
    }

    MainWindow w;
    w.show();
    w.easterSecret(utils::getHostname());

    //------ Database connect ------//
    /// Arg string to backend1
    backend = db::backends::none;
    if (runtime::KEY_db_backend == "postgres") backend = db::backends::postgres;
    if (runtime::KEY_db_backend == "sqlite") backend = db::backends::sqlite;

    /// Arg string to addr
    db::addr remoteHost{};
    remoteHost.fromRemoteHost(runtime::KEY_db_address);

    /// Connect
    db::db database { backend, runtime::KEY_db_user, runtime::KEY_db_passwd, runtime::KEY_db_name, remoteHost };
    try {
        database.connect();
    } catch (std::exception& e) {
        ui::error(e.what());
        ui::warn("Something goes whong while BigEye trying to access database! Using DryRun mode!");
        runtime::FLAG_dryRun = true;
        database = {};
    }
    database.setup();

    //------ Looking for cameras ------//
    std::vector<input::cameraDevice> cameraList = {input::cameraDevice()};
    if (!runtime::FLAG_noScan)
        cameraList = input::getCameraList();

    //------ Database lookup ------//
    uint32_t serviceLastID = 0;
    uint32_t journalLastID = 0;

    size_t serviceCount = database.getRowsCount("service");
    auto serviceDump = database.serviceRead(serviceCount);
    for (auto& i : serviceDump){
        w.addNewServiceItem(
            std::to_string(i.id),
            "Later...",
            i.data);
        
        if (serviceLastID < i.id) serviceLastID = i.id;
    }

    size_t journalCount = database.getRowsCount("journal");
    auto journalDump = database.journalRead(journalCount);
    for (auto& i : journalDump) {
        w.addNewJournalItem(i.datetime, i.metadata, i.metadata, std::to_string(i.id), i.image);
        if (journalLastID < i.id) journalLastID = i.id;
    }

    database.serviceWrite({0, db::dataRows::service::types::connectEvent,
                           utils::getDatetime() + ";" + utils::getHostname() + ";" +
                           std::to_string(cameraList.size())});

    //------ DNN Init ------//
    auto dnn = engine::dnnLayer(
        "./deploy.prototxt", "./res10_300x300_ssd_iter_140000_fp16.caffemodel",
        {0.5, runtime::FLAG_useCuda ?
            engine::dnnLayer::dnnBackends::cuda :
            engine::dnnLayer::dnnBackends::cpu
        });
    engine::dnnReturns ret;
    cv::Mat frame;

    //------ Camera init ------//
    input::cameraDevice camera = cameraList.at(0);
    cv::VideoCapture cap{input::openCamera(camera.descriptor)};
    if ((camera.mode.y == 0) || (camera.mode.x == 0)) {
        cap.read(frame);
        if (frame.empty())
            ui::error("Unable to capture frame from default camera! Is it connected?");
        camera.mode.y = frame.size().width;
        camera.mode.x = frame.size().height;
    }
    int scaller = camera.mode.y / 500;
    int newWidth = camera.mode.y / scaller;
    int newHeight = camera.mode.x / scaller;

    // Local runtime variables
    std::vector <double> Points;
    size_t counter = 1;

    // Temporary loop
    while (true) {
        cap.read(frame);
        // DNN
        dnn.processFrame(frame, true, Points, database);

        // Draw the image on GUI
        resize(frame, frame, cv::Size(newWidth, newHeight), cv::INTER_LINEAR);
        w.updateFrame(Mat2QImage(frame));
        pullUpdates(database, w, journalLastID, journalCount, counter);

        // Update events
        a.processEvents();
        if (w.isClosed) break;
    }

    // Disconnecting
    database.serviceWrite({0, db::dataRows::service::types::disconnectEvent,
                           utils::getHostname() + ";" + utils::getDatetime()});
    database.disconnect();

    return 0;
}

QImage Mat2QImage(cv::Mat const& src) {
    cv::Mat temp; 
    cvtColor(src, temp, cv::COLOR_BGR2RGB); 
    QImage dest ((const uchar *)temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
    dest.bits(); 
    return dest;
}

void pullUpdates(db::db& database, MainWindow& w, uint32_t& journalLastID, size_t& journalCount, size_t& counter) {
    if (counter <= 30) {
        counter++;
        return;
    }
    counter = 1;

    size_t newCount = database.getRowsCount("journal");
    if (newCount == journalCount) return;
    // If new exists:
    journalCount = newCount;
    auto journalDump = database.journalRead(newCount);
    for (auto& i : journalDump) {
        if (i.id > journalLastID) {
            std::cout << "[ " << std::to_string(i.id) << " | " << i.datetime << " | " << i.metadata
                    << i.image.size() << " ]\n";
            w.addNewJournalItem(i.datetime, i.metadata, i.metadata, std::to_string(i.id), {});
            journalLastID = i.id;
        }
    }
}