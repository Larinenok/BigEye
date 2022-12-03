#include "engine/engine.hpp"
#include <opencv2/dnn/dnn.hpp>
#include "excepts.hpp"
#include <iostream>
#include <cstdlib>

namespace engine {

dnnLayer::dnnLayer(std::string prototxtPath, std::string caffemodelPath, config config)
    : dnnLayer(cv::dnn::readNetFromCaffe(prototxtPath, caffemodelPath), config) {};

dnnLayer::dnnLayer(cv::dnn::Net network, config config) :
        confidenceThreshold(config.confidenceThreshold),
        highlight(config.highlight),
        scaleFactor(config.scaleFactor),
        blobSize(config.blobSize) {
    this->net = std::move(network);
    this->setProperty(dnnBackend_p, config.backend);

    if (this->net.empty()) throw excepts::error("Unable to open DNN files.");
};

template <typename T>
void dnnLayer::setProperty(properties property, T value) {
    switch (property) {
        case confidenceThreshold_p : {
            this->confidenceThreshold = static_cast<float>(value);
            break;
        }
        case dnnBackend_p : {
            dnnBackends backend = value;
            switch (backend) {
                case cpu:
                    this->net.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
                    this->net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
                    break;

                case cuda:
                    this->net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
                    this->net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
                    break;
            }
            break;
        }
        case highlight_p : {
            this->highlight = value;
            break;
        }
    }
}

dnnReturns dnnLayer::processFrame(cv::Mat& frame, const bool highlight) {
    dnnReturns ret;
    
    net.setInput(cv::dnn::blobFromImage(frame, this->scaleFactor, this->blobSize), "data");

    cv::Mat detection = net.forward("detection_out");
    std::vector<double> layersTimings;

    const double freq = cv::getTickFrequency() / 1000;
    ret.frametime = net.getPerfProfile(layersTimings) / freq;
    
    cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F,
                            detection.ptr<float>());

    if (this->highlight) {
        std::cout << "Testpoint #1\n";
        std::ostringstream ss;
        ss << "FPS: " << 1000 / ret.frametime << " ; frameTime: " << ret.frametime << " ms";
        putText(frame, ss.str(), cv::Point(20, 20), 0, 0.5, cv::Scalar(0, 0, 255));
        std::cout << "Testpoint #2\n";

        for (int i = 0; i < detectionMat.rows; i++) {
            std::cout << i << "\n";
            float trust = detectionMat.at<float>(i, 2);

            if (trust > confidenceThreshold) {  /// if above threshold
                std::cout << "Testpoint #4\n";

                int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
                int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
                int xRightTop = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
                int yRightTop = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);

                /// Draw rectange
                rectangle(frame, cv::Point(xLeftBottom, yLeftBottom),
                          cv::Point(xRightTop, yRightTop), cv::Scalar(0, 255, 0));

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
                std::cout << "Testpoint #5\n";

            }
        }
    }

    return ret;
};

}  // namespace engine
