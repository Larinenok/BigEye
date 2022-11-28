#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace input {

const uint8_t searchRange = 32;

struct cameraMode {
   unsigned int x = 0;
   unsigned int y = 0;
   unsigned int fps = 0;
};

struct cameraDevice {
    std::string name = "default";
    int descriptor = -1;
    cameraMode mode = {0, 0, 0};
};

std::vector<cameraDevice> getCameraList();

}  // namespace input
