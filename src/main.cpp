#include "config.hpp"

#include "app.hpp"
#include "math.hpp"

#include "window/window.hpp"
#include "window/callbackGLFW.hpp"

#include "systems/gui.hpp"
#include "systems/input.hpp"

#include "gameobjects/camera.hpp"
#include "gameobjects/object.hpp"
#include "gameobjects/light.hpp"

#include "scene/cubeData.hpp"

#include "renderer/texture.hpp"
#include "renderer/shader.hpp"
#include "renderer/mesh.hpp"
#include "renderer/part.hpp"
#include "renderer/uniforms/viewModel.hpp"
#include "renderer/uniforms/light.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <stdlib.h>
#include <time.h>

int main(int argc, char **args)
{
    // ! ||--------------------------------------------------------------------------------||
    // ! ||                                 INITIALIZE APP                                 ||
    // ! ||--------------------------------------------------------------------------------||
    app::lifetime::startApp();

    app::lifetime::initSPDLOG(spdlog::level::level_enum::info, "Started rov-sim-env...");
    app::lifetime::initSTBI();
    app::lifetime::initGLFW(callbackGLFW::error, "GLFW init.");

    GLFWwindow *window = window::createWindow(app::settings.defaultWindowWidth, app::settings.defaultWindowHeight, "rov-sim-env");
    window::configureWindowAndSetContext(window, app::settings.defaultWindowWidth, app::settings.defaultWindowHeight);
    callbackGLFW::setWindowCallbacks(window);

    app::lifetime::initGLAD();
    app::lifetime::initIMGUI(window);

    callbackGLFW::windowResize(window, 0, 0); // Call resize callback to set window vars

// ! ||--------------------------------------------------------------------------------||
// ! ||                           INITIALIZE UNIFORM BUFFERS                           ||
// ! ||--------------------------------------------------------------------------------||

    GLuint u_cameraData = 0, u_modelData = 0, u_lightData = 0;
    glGenBuffers(1, &u_cameraData);
    glGenBuffers(1, &u_modelData);
    glGenBuffers(1, &u_lightData);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, u_cameraData);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_modelData);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, u_lightData);

    // ! ||--------------------------------------------------------------------------------||
    // ! ||                              --INITIALIZE PARTS---                             ||
    // ! ||--------------------------------------------------------------------------------||
    size_t attributes[] = {3, 3, 2};
    renderer::Mesh mainMesh(sizeof(cubeVertices), cubeVertices, sizeof(cubeIndices), cubeIndices, sizeof(attributes) / sizeof(size_t), attributes);

    renderer::Texture2D mainDiffuseMap(PROJECT_PATH "res/texture/learnopengl/container/diffuse.png");
    renderer::Texture2D mainSpecularMap(PROJECT_PATH "res/texture/learnopengl/container/specular.png");

    renderer::shader::Program mainShader(PROJECT_PATH "res/shader/vertexMain.glsl", PROJECT_PATH "res/shader/fragmentMain.glsl");
    {
        const GLchar *programUniforms[] = {"diffuseMap", "specularMap", "specularShininess"};
        renderer::shader::pushUniforms(mainShader, sizeof(programUniforms) / sizeof(GLchar *), programUniforms);

        renderer::useProgram(mainShader);
        renderer::shader::setUniform(mainShader, glUniform1i, "diffuseMap", 0);
        renderer::shader::setUniform(mainShader, glUniform1i, "specularMap", 1);

        renderer::shader::bindUniformBlock(mainShader, "CameraData", 0);
        renderer::shader::bindUniformBlock(mainShader, "ModelData", 1);
        renderer::shader::bindUniformBlock(mainShader, "ActiveLights", 2);
    }

    renderer::Part cubePart(&math::identityTransformMatrix, &mainMesh,
                            &mainDiffuseMap, &mainSpecularMap, &mainSpecularMap,
                            &mainShader);

    // ! ||--------------------------------------------------------------------------------||
    // ! ||                                INITIALIZE SCENE                                ||
    // ! ||--------------------------------------------------------------------------------||
    gameobject::light::Point pointLight({0.f, 0.5f, 1.f, 1.f}, {1.f, 0.9f, 0.9f, 1.f}, {-0.6f, 3.2f, -0.191f},
                                        0.2f, 0.5f, 1.f, 1.f, 1.f);
    gameobject::light::Point pointLight2({0.8f, 0.2f, 0.f, 1.f}, {1.f, 0.0f, 0.f, 1.f}, {-3.f, 4.f, -4.f},
                                         0.2f, 0.5f, 1.f, 1.f, 1.f);

    uniform::buffer::ActiveLights activeLights;
    memcpy(&activeLights.pointLights[0], &pointLight, sizeof(uniform::PointLightBlock));
    memcpy(&activeLights.pointLights[1], &pointLight2, sizeof(uniform::PointLightBlock));
    activeLights.pointLightCount = 2;

    GLfloat specularShininess = 32.f;

    camera::Object mainCamera = {
        .framebufferClearColor = {0.92f, 0.92f, 0.92f, 1.0f},
        .position = {2.f, 2.0f, 2.0f},
        .rotation = {135.f, -45.f},
        .fov = 75,
        .framebufferWidth = app::window_vars.framebufferWidth,
        .framebufferHeight = app::window_vars.framebufferHeight};

    object::MainCube mainCubeData = {
        .position = {0.f, 0.f, 0.f},
        .rotation = {30.f, 60.f, 0.f},
        .scale = {1.f, 1.f, 1.f},
    };

    std::vector<object::BatchCube> batchCubes;

    // ! ||--------------------------------------------------------------------------------||
    // ! ||                              SET SYSTEM REFERENCES                             ||
    // ! ||--------------------------------------------------------------------------------||
    gui::References guiRefs = {
        .camera = &mainCamera,
        .mainCube = &mainCubeData,
        .batchCubes = &batchCubes,

        .pointLightPosition = glm::value_ptr(pointLight.pos),
        .pointLightColor = glm::value_ptr(pointLight.color),

        .specularShininess = &specularShininess,
        .specularStrength = &pointLight.specularIntensity,

        .ambientLightIntensity = &pointLight.ambientIntensity,
        .ambientLightColor = glm::value_ptr(pointLight.ambientColor)};
    gui::registerRefs(&guiRefs);

    input::References inputRefs = {
        .camera = &mainCamera,
    };
    input::registerRefs(&inputRefs);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    // ! ||--------------------------------------------------------------------------------||
    // ! ||                                    GAME LOOP                                   ||
    // ! ||--------------------------------------------------------------------------------||
    double prevFrameTime = 0.;
    double currentFrameTime = 0.;
    while (1)
    {
        currentFrameTime = glfwGetTime();
        app::window_vars.frametime = currentFrameTime - prevFrameTime;
        prevFrameTime = currentFrameTime;

        glfwPollEvents();
        input::processKeysAsync(window);

        /**
         * OPENGL RENDER
         */
        {
            glClearColor(mainCamera.framebufferClearColor[0], mainCamera.framebufferClearColor[1],
                         mainCamera.framebufferClearColor[2], mainCamera.framebufferClearColor[3]);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);

            if (app::state_vars.isWireframe)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            memcpy(&activeLights.pointLights[0], &pointLight, sizeof(gameobject::light::Point));
            glBindBuffer(GL_UNIFORM_BUFFER, u_lightData);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(uniform::buffer::ActiveLights), &activeLights, GL_DYNAMIC_DRAW);

            mainCamera.framebufferWidth = app::window_vars.framebufferWidth;
            mainCamera.framebufferHeight = app::window_vars.framebufferHeight;

            uniform::buffer::CameraData cameraMatrices(mainCamera);
            glBindBuffer(GL_UNIFORM_BUFFER, u_cameraData);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(uniform::buffer::CameraData), &cameraMatrices, GL_DYNAMIC_DRAW);

            {
                renderer::attachPart(cubePart);

                uniform::buffer::ModelData modelData(mainCubeData.position, mainCubeData.rotation, mainCubeData.scale);
                glBindBuffer(GL_UNIFORM_BUFFER, u_modelData);
                glBufferData(GL_UNIFORM_BUFFER, sizeof(uniform::buffer::ModelData), &modelData, GL_DYNAMIC_DRAW);

                renderer::shader::setUniform(mainShader, glUniform1f, "specularShininess", specularShininess);

                renderer::drawPart(cubePart);
            }

            {
                renderer::attachPart(cubePart);

                for (size_t i = 0; i < batchCubes.size(); i++)
                {
                    uniform::buffer::ModelData modelData(batchCubes.at(i).position, batchCubes.at(i).rotation, batchCubes.at(i).scale);
                    glBindBuffer(GL_UNIFORM_BUFFER, u_modelData);
                    glBufferData(GL_UNIFORM_BUFFER, sizeof(uniform::buffer::ModelData), &modelData, GL_DYNAMIC_DRAW);

                    renderer::drawPart(cubePart);
                }
            }
        }

        gui::render();

        glfwSwapBuffers(window);

        if (glfwWindowShouldClose(window))
        {
            break;
        }
    }

    app::lifetime::killAll(0);
    app::lifetime::endApp();
    exit(EXIT_SUCCESS);
}
