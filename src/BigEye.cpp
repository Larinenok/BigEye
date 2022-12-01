#include <cstdlib>
#include <opencv2/core.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
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

const size_t inWidth = 300;  /// output image size
const size_t inHeight = 300;
const double inScaleFactor = 1.0;
const cv::Scalar meanVal(104.0, 117.0, 123.0);

int main(int argc, char* argv[]) {
    //* Args parsing. It may change runtime::FLAG_ *//
    utils::parseArgs(argc, argv);

    //* QMessageBox test... *//
    ui::error("Msg1");
    ui::info("Msg2");
    ui::warn("Msg3");
    ui::msg("Msg4");

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
                  << " ]\n";

    //* Engine test... *//
    // database.journalWrite({0, utils::getDatetime(), "TESTDATA"});

    input::cameraDevice camera = cameraList.at(0);
    float confidenceThreshold = 0.5;

    /// Reads a network model stored in Caffe model in memory
    cv::dnn::Net net = cv::dnn::readNetFromCaffe("./deploy.prototxt",
                                                 "./res10_300x300_ssd_iter_140000_fp16.caffemodel");
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
    if (net.empty()) throw excepts::error("Unable to open DNN files.");

    cv::Mat frame;
    cv::VideoCapture cap{input::openCamera(camera.descriptor)};

    while (true) {
        cap.read(frame);

        // if (frame.channels() == 4) cvtColor(frame, frame, cv::COLOR_BGRA2BGR);

        /// Pass the image to network
        net.setInput(cv::dnn::blobFromImage(frame, inScaleFactor, cv::Size(inWidth, inHeight), meanVal, false, false), "data");

        // Вычисляем вывод, это 4-мерное число, строки и столбцы могут содержать только 2 измерения,
        // поэтому они здесь не используются и устанавливаются на -1
        cv::Mat detection = net.forward("detection_out");
        std::cout << cv::getTickFrequency() << std::endl;
        std::vector<double> layersTimings;
        
        /// Используется для возврата частоты процессора. получить Tick Frequency. Единица
        /// измерения здесь - секунды, то есть количество повторений за одну секунду.
        double freq = cv::getTickFrequency() / 1000;
        double time = net.getPerfProfile(layersTimings) / freq;
        
        cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F,
                             detection.ptr<float>());  // Матрица 101 * 7

        std::ostringstream ss;
        ss << "FPS: " << 1000 / time << " ; time: " << time << " ms";
        putText(frame, ss.str(), cv::Point(20, 20), 0, 0.5, cv::Scalar(0, 0, 255));

        for (int i = 0; i < detectionMat.rows; i++) {
            float trust =
                detectionMat.at<float>(i, 2);  // Во втором столбце хранится уровень достоверности

            if (trust > confidenceThreshold) {  // Удовлетворение пороговому условию
                // Сохраняем информацию о местоположении на изображении, где находится лицо
                int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
                int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
                int xRightTop = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
                int yRightTop = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);

                rectangle(frame, cv::Point(xLeftBottom, yLeftBottom),
                          cv::Point(xRightTop, yRightTop), cv::Scalar(0, 255, 0));  // Рисуем рамку

                ss.str("");
                ss << trust;
                cv::String conf(ss.str());
                cv::String label = "Face (trust: " + conf + ")";
                int baseLine = 0;
                cv::Size labelSize = getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);

                rectangle(frame,
                          cv::Rect(cv::Point(xLeftBottom, yLeftBottom - labelSize.height),
                                   cv::Size(labelSize.width, labelSize.height + baseLine)),
                          cv::Scalar(255, 255, 255), cv::FILLED);

                putText(frame, label, cv::Point(xLeftBottom, yLeftBottom), cv::FONT_HERSHEY_SIMPLEX,
                        0.5, cv::Scalar(0, 0, 0));
            }
        }

        cv::imshow(camera.name, frame);
        if (cv::waitKey(5) == 27) break;
    }

    // Disconnecting
    database.serviceWrite({0, db::dataRows::service::types::disconnectEvent,
                           utils::getHostname() + ";" + utils::getDatetime()});
    database.disconnect();

    return 0;
}
