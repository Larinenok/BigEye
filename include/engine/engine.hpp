#include <opencv2/core.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <vector>

namespace engine {

enum dnnBackends { keep, cuda };

class dnnLayer {
   private:
   public:
    dnnLayer(dnnBackends backend, std::string_view prototxtPath, std::string_view caffemodelPath);
    dnnLayer(dnnBackends backend, cv::dnn::Net network);

    cv::Mat processFrame(cv::Mat& frame, const bool highlight);
};

}  // namespace engine
