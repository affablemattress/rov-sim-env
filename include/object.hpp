#pragma once

#include "config.hpp"

namespace object {
    struct mainCube {
        float position[3];
        float rotation[3];
        float scale[3];
        float vertexColors[4][4];
        float mixWeight;
    };

    struct batchCube {
        float mixColor[4];
        float position[3];
        float rotation[3];
        float scale[3];
    };

    object::batchCube generateRandomBatchCube(float positionZOffset, float positionRange, float scaleOffset, float scaleRange);
}