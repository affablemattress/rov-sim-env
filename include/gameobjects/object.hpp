#pragma once

#include "config.hpp"

namespace object {
    struct MainCube {
        float position[3];
        float rotation[3];
        float scale[3];
    };

    struct BatchCube {
        float position[3];
        float rotation[3];
        float scale[3];
        float mixColor[4];
    };

    object::BatchCube generateRandomBatchCube(float positionZOffset, float positionRange, float scaleOffset, float scaleRange);
}