#include "object.hpp"

#include <random>
#include <time.h>

namespace object {
    object::batchCube generateRandomBatchCube(float positionZOffset, float positionRange, float scaleOffset, float scaleRange) {
        object::batchCube returnCube = { 0 };

        returnCube.mixColor[0] = 1.f - (rand() % (100 + 1) / 100.f);
        returnCube.mixColor[1] = 1.f - (rand() % (100 + 1) / 100.f);
        returnCube.mixColor[2] = 1.f - (rand() % (100 + 1) / 100.f);
        returnCube.mixColor[3] = 1.f;


        returnCube.position[0] = positionRange - (rand() % (int)(positionRange * 2 * 100 + 1) / 100.f);
        returnCube.position[1] = positionRange - (rand() % (int)(positionRange * 2 * 100 + 1) / 100.f);
        returnCube.position[2] = positionZOffset + positionRange - (rand() % (int)(positionRange * 2 * 100 + 1) / 100.f);

        returnCube.rotation[0] = 180.f - ((rand() % 3601) / 10.f);
        returnCube.rotation[1] = 180.f - ((rand() % 3601) / 10.f);
        returnCube.rotation[2] = 180.f - ((rand() % 3601) / 10.f);

        returnCube.scale[0] = scaleOffset + scaleRange - (rand() % (int)(scaleRange * 2 * 100 + 1) / 100.f);
        returnCube.scale[1] = scaleOffset + scaleRange - (rand() % (int)(scaleRange * 2 * 100 + 1) / 100.f);
        returnCube.scale[2] = scaleOffset + scaleRange - (rand() % (int)(scaleRange * 2 * 100 + 1) / 100.f);
        return returnCube;
    }
}