#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace input {

const uint8_t searchRange = 32;

struct cameraMode {
    uint x = 0;
    uint y = 0;
    uint fps = 0;
};

struct cameraDevice {
    std::string name = "default";
    int descriptor = -1;
    cameraMode mode = {0, 0, 0};
};

std::vector<cameraDevice> getCameraList();

}  // namespace input