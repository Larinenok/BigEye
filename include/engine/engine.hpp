#include <opencv2/core.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "db/db.hpp"
#include <string>

namespace engine {

struct dnnReturns {
    cv::Mat positions;
    float frametime;
};

class dnnLayer {
   public:
    enum properties { confidenceThreshold_p, dnnBackend_p, highlight_p };
    enum dnnBackends { cpu, cuda/*, ...*/ };
    struct config {
        double confidenceThreshold  = 0.5;
        dnnBackends backend         = cpu;
        bool highlight              = true;
        double scaleFactor          = 1.0;
        cv::Size blobSize           = cv::Size(300, 300);
    };

   private:
    cv::dnn::Net net;
    // properties
    double confidenceThreshold;
    bool highlight;
    double scaleFactor;
    cv::Size blobSize;

   public:
    dnnLayer(std::string prototxtPath, std::string caffemodelPath, config config);
    dnnLayer(cv::dnn::Net network, config config);

    template <typename T>
    void setProperty(properties property, T value);

    dnnReturns processFrame(cv::Mat& frame, const bool highlight, std::vector <double>& Points, db::db& database);
};

// class logPhoto {};

}  // namespace engine
