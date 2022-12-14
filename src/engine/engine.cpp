#include "engine/engine.hpp"
#include <opencv2/dnn/dnn.hpp>
#include "excepts.hpp"
#include "utils/host.hpp"
#include <iostream>
#include <cstdlib>

namespace engine {

double getOrientation(const std::vector<cv::Point> &pts, cv::Mat &img)
{
    //! [pca]
    //Construct a buffer used by the pca analysis
    int sz = static_cast<int>(pts.size());
    cv::Mat data_pts = cv::Mat(sz, 2, CV_64F);
    for (int i = 0; i < data_pts.rows; i++)
    {
        data_pts.at<double>(i, 0) = pts[i].x;
        data_pts.at<double>(i, 1) = pts[i].y;
    }

    //Perform PCA analysis
    cv::PCA pca_analysis(data_pts, cv::Mat(), cv::PCA::DATA_AS_ROW);

    //Store the center of the object
    cv::Point cntr = cv::Point(static_cast<int>(pca_analysis.mean.at<double>(0, 0)),
                      static_cast<int>(pca_analysis.mean.at<double>(0, 1)));

    //Store the eigenvalues and eigenvectors
    std::vector<cv::Point2d> eigen_vecs(2);
    std::vector<double> eigen_val(2);
    for (int i = 0; i < 2; i++)
    {
        eigen_vecs[i] = cv::Point2d(pca_analysis.eigenvectors.at<double>(i, 0),
                                pca_analysis.eigenvectors.at<double>(i, 1));

        eigen_val[i] = pca_analysis.eigenvalues.at<double>(i);
    }
    //! [pca]

    //! [visualization]

    double angle = atan2(eigen_vecs[0].y, eigen_vecs[0].x); // orientation in radians
    //! [visualization]
    angle = round(angle*100 )/100;
    return angle;
}

std::vector<double> finding_points(cv :: Mat& crop, std::vector <double>& Points, db::db& database)
{
    std::vector<unsigned char> buff;
    std::vector<int> param {cv::IMWRITE_JPEG_QUALITY, 60};
    std::vector<double> copy;
    cv::Mat gray;
    int count1 = 0;
    cvtColor(crop, gray, cv::COLOR_BGR2GRAY);

       // Convert image to binary
       cv::Mat bw;
       threshold(gray, bw, 50, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
       //! [pre-process]

       //! [contours]
       // Find all the contours in the thresholded image
       std::vector<std::vector<cv::Point> > contours;
       findContours(bw, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);

       for (size_t i = 0; i < contours.size(); i++)
           {
               // Calculate the area of each contour
               double area = contourArea(contours[i]);
               // Ignore contours that are too small or too large
               if (area < 1e2 || 1e5 < area) continue;
               // Find the orientation of each shape
               double angle = getOrientation(contours[i], crop);
               for(size_t j = 0; j < Points.size(); j++)
               {
                   if ((Points.at(j)-0.1 < angle) && ( angle < Points.at(j)+0.1))
                   {
                       count1 = 1;
                       break;
                   }
               }

               if(count1 == 1)
               {
                   break;
               }

               else
               {
                    copy.push_back(angle);
               }
           }
    if (count1 == 0)
    {
        for(size_t i = 0; i < copy.size(); i ++)
        {
            Points.push_back(copy.at(i));
        }

        // Encode the frame to JPE
        cv::imencode(".jpg", crop, buff, param);

        // General algo
        database.journalWrite({0, utils::getDatetime(), "TESTDATA", buff});
    }
    return Points;
}

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

dnnReturns dnnLayer::processFrame(cv::Mat& frame, const bool highlight, std::vector <double>& Points, db::db& database) {
    dnnReturns ret;

    net.setInput(cv::dnn::blobFromImage(frame, this->scaleFactor, this->blobSize), "data");

    cv::Mat detection = net.forward("detection_out");
    std::vector<double> layersTimings;
    int count = 0;

    const double freq = cv::getTickFrequency() / 1000;
    ret.frametime = net.getPerfProfile(layersTimings) / freq;
    
    cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F,
                            detection.ptr<float>());

    if (this->highlight) {
        std::ostringstream ss;
        ss << "FPS: " << 1000 / ret.frametime << " ; frameTime: " << ret.frametime << " ms";
        putText(frame, ss.str(), cv::Point(20, 20), 0, 0.5, cv::Scalar(0, 0, 255));

        for (int i = 0; i < detectionMat.rows; i++) {
            float trust = detectionMat.at<float>(i, 2);

            if (trust > confidenceThreshold) {  /// if above threshold

                int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
                int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
                int xRightTop = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
                int yRightTop = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);

                if (count == 5)
                {
                    cv::Mat crop = frame(cv::Range(yLeftBottom, yRightTop), cv::Range(xLeftBottom, xRightTop));
                    resize(crop, crop, cv::Size(250, 300), cv::INTER_LINEAR);
                    Points = engine ::finding_points(crop, Points, database);
                    count = 0;
                }
                else
                {
                    count++;
                }

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

            }
        }
    }

    return ret;
};

}  // namespace engine
