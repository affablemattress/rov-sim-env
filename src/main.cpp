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
#include "renderer/model.hpp"

#include "renderer/uniforms/viewModel.hpp"
#include "renderer/uniforms/light.hpp"

#include "util/containers/tree.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/DefaultLogger.hpp"

#include <stdlib.h>
#include <time.h>
#include <vector>
#include <list>


int main(int argc, char **args) {
    // ! ||--------------------------------------------------------------------------------||
    // ! ||                                 INITIALIZE APP                                 ||
    // ! ||--------------------------------------------------------------------------------||
    app::lifetime::startApp();

    app::lifetime::initSPDLOG(spdlog::level::level_enum::debug, "Started rov-sim-env...");
    app::lifetime::initSTBI();
    app::lifetime::initGLFW(callbackGLFW::error, "GLFW init.");

    GLFWwindow *window = window::createWindow(app::settings.defaultWindowWidth, app::settings.defaultWindowHeight, "rov-sim-env");
    window::configureWindowAndSetContext(window, app::settings.defaultWindowWidth, app::settings.defaultWindowHeight);
    callbackGLFW::setWindowCallbacks(window);

    app::lifetime::initGLAD();
    app::lifetime::initIMGUI(window);

    Assimp::Importer aiImporter;
    Assimp::DefaultLogger::create("assimp-log.txt", Assimp::Logger::VERBOSE);

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
// ! ||                        INITIALIZE SCENE DATA STRUCTURES                        ||
// ! ||--------------------------------------------------------------------------------||
    std::vector<renderer::Shader> shaders;
    shaders.reserve(100);
    std::vector<renderer::Texture2D> textures;
    textures.reserve(100);
    std::vector<renderer::Material> materials;
    materials.reserve(100);
    std::vector<renderer::Mesh> meshes;
    meshes.reserve(100);
    std::vector<math::TransformMatrix> localTransforms;
    localTransforms.reserve(100);
    std::vector<renderer::Part> parts;
    parts.reserve(100);
    std::vector<renderer::Model> models;
    models.reserve(100);

// ! ||--------------------------------------------------------------------------------||
// ! ||                                INITIALIZE SHADER                               ||
// ! ||--------------------------------------------------------------------------------||
    renderer::Shader* mainShader = &shaders.emplace_back();

    const GLchar *mainShaderUniformNames[] = { "map_diffuse", "map_specular", "map_normal", "map_emissive", "map_occlusion", "specularShininess" };
    const GLchar *mainShaderUniformBlockNames[] = { "CameraData", "ModelData", "ActiveLights" };
    const GLint mainShaderUniformBlockBindingPoints[] = { 0, 1, 2 };
    renderer::initShader(*mainShader, PROJECT_PATH "res/shader/vertexMain.glsl", PROJECT_PATH "res/shader/fragmentMain.glsl", 
                         sizeof(mainShaderUniformNames) / sizeof(GLchar*), mainShaderUniformNames,
                         sizeof(mainShaderUniformBlockNames) / sizeof(GLchar*), mainShaderUniformBlockNames, mainShaderUniformBlockBindingPoints);
    
    renderer::useShader(*mainShader);
    renderer::setUniform(*mainShader, glUniform1i, "map_diffuse", 0);
    renderer::setUniform(*mainShader, glUniform1i, "map_specular", 1);
    renderer::setUniform(*mainShader, glUniform1i, "map_normal", 2);
    renderer::setUniform(*mainShader, glUniform1i, "map_emissive", 3);
    renderer::setUniform(*mainShader, glUniform1i, "map_occlusion", 4);

// ! ||--------------------------------------------------------------------------------||
// ! ||                               INITIALIZE MATERIAL                              ||
// ! ||--------------------------------------------------------------------------------||
    renderer::Texture2D* cubeDiffuseMap = &textures.emplace_back();
    renderer::Texture2D* cubeSpecularMap = &textures.emplace_back();

    renderer::d_Texture2D* diffuseMapData = renderer::loadTexture2DFromFile(PROJECT_PATH "res/texture/learnopengl/container/diffuse.png", renderer::TextureType::k_diffuse);
    renderer::pushTexture2D(*cubeDiffuseMap, diffuseMapData);
    renderer::freeTexture2D(diffuseMapData);

    renderer::d_Texture2D* specularMapData = renderer::loadTexture2DFromFile(PROJECT_PATH "res/texture/learnopengl/container/specular.png", renderer::TextureType::k_specular);
    renderer::pushTexture2D(*cubeSpecularMap, specularMapData);
    renderer::freeTexture2D(specularMapData);

    renderer::Material* cubeMaterial = &materials.emplace_back();
    renderer::initMaterial(*cubeMaterial, cubeDiffuseMap, cubeSpecularMap, nullptr, nullptr, nullptr, mainShader);

// ! ||--------------------------------------------------------------------------------||
// ! ||                                INITIALIZE MODEL                                ||
// ! ||--------------------------------------------------------------------------------||
    renderer::Mesh* cubeMesh = &meshes.emplace_back();

    renderer::d_Mesh* cubeMeshData = renderer::loadMeshFromStaticBuffer("Cube", sizeof(cubeVertices), cubeVertices, sizeof(cubeIndices), cubeIndices);
    renderer::pushMesh(*cubeMesh, cubeMeshData);
    renderer::freeMesh(cubeMeshData);

    math::TransformMatrix* cubeTransform = &localTransforms.emplace_back();
    *cubeTransform = math::identityTransformMatrix;

    renderer::Part* cubePart = &parts.emplace_back();
    renderer::initPart(*cubePart, cubeTransform, cubeMesh, cubeMaterial);

    renderer::Model* cubeModel = &models.emplace_back(cubePart);

// ! ||--------------------------------------------------------------------------------||
// ! ||                                 LOAD MODEL TRY                                 ||
// ! ||--------------------------------------------------------------------------------||
    std::string modelPath = PROJECT_PATH "res/model/survival-backpack/scene.gltf";
    std::string modelDirectory = modelPath.substr(0, modelPath.find_last_of('/') + 1);

    spdlog::info("Loading model at path: {0}", modelPath.c_str());

    const aiScene* scene = aiImporter.ReadFile(modelPath.c_str(), 
                                                 aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace |aiProcess_RemoveRedundantMaterials | aiProcess_FixInfacingNormals | aiProcess_ImproveCacheLocality | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        spdlog::error("Couldn't load model: {0}", modelPath.c_str());
        spdlog::error(aiImporter.GetErrorString());
        app::lifetime::killAll(1);
    }

    aiString texturePath;
    spdlog::debug("diffuse {0}", (int)scene->mMaterials[0]->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath));
    spdlog::debug("path {0}", texturePath.C_Str());
    spdlog::debug("specular {0}", (int)scene->mMaterials[0]->GetTexture(aiTextureType_METALNESS, 0, &texturePath));
    spdlog::debug("path {0}", texturePath.C_Str());
    spdlog::debug("normal {0}", (int)scene->mMaterials[0]->GetTexture(aiTextureType_NORMALS, 0, &texturePath));
    spdlog::debug("path {0}", texturePath.C_Str());
    aiNode* currentNode = scene->mRootNode;
    
    math::TransformMatrix& nodeLocalTransform = localTransforms.emplace_back(glm::mat4());
    memcpy(&nodeLocalTransform, &currentNode->mTransformation, sizeof(glm::mat4));
    nodeLocalTransform = glm::transpose(nodeLocalTransform);
    
    /*for(size_t i = 0; i < currentNode->mNumMeshes; i++) {
        aiMesh* currentAssimpMesh = scene->mMeshes[currentNode->mMeshes[i]];

        renderer::Mesh newMesh;
        renderer::d_Mesh* newMeshData renderer::loadMeshFromAssimpMesh(currentAssimpMesh); 
        renderer::pushMesh(newMesh, newMeshData);

        // ! ||--------------------------------------------------------------------------------||
        // ! ||                                 LOAD MATERIALS                                 ||
        // ! ||--------------------------------------------------------------------------------||
        aiMaterial* currentMaterial = scene->mMaterials[newMesh.mMaterialIndex];

        spdlog::info(currentMaterial->GetName().C_Str());
        spdlog::info(currentMaterial->mProperties[0]->);
        for(size_t j = 0; j < currentMaterial->GetTextureCount(aiTextureType_DIFFUSE); j++) {

        }

        for(size_t j = 0; j < currentMaterial->GetTextureCount(aiTextureType_SPECULAR) && j < 2; j++) {

        }

        for(size_t j = 0; j < currentMaterial->GetTextureCount(aiTextureType_NORMALS) && j < 2; j++) {
        }

        newPart.shader = modelShader;

        renderer::freeMesh(newMeshData);

    }*/

    // ! ||--------------------------------------------------------------------------------||
    // ! ||                                INITIALIZE SCENE                                ||
    // ! ||--------------------------------------------------------------------------------||
    gameobject::light::Point pointLight({0.f, 0.5f, 1.f, 1.f}, {1.f, 0.9f, 0.9f, 1.f}, {-0.6f, 3.2f, -0.191f},
                                        0.2f, 0.5f, 1.f, 1.f, 1.f);
    gameobject::light::Point pointLight2({0.8f, 0.2f, 0.f, 1.f}, {1.f, 0.0f, 0.f, 1.f}, {-3.f, 4.f, -4.f},
                                         0.2f, 0.5f, 1.f, 1.f, 1.f);

    renderer::uniformBuffer::ActiveLights activeLights;
    memcpy(&activeLights.pointLights[0], &pointLight, sizeof(renderer::uniform::block::PointLight));
    memcpy(&activeLights.pointLights[1], &pointLight2, sizeof(renderer::uniform::block::PointLight));
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
            glBufferData(GL_UNIFORM_BUFFER, sizeof(renderer::uniformBuffer::ActiveLights), &activeLights, GL_DYNAMIC_DRAW);

            mainCamera.framebufferWidth = app::window_vars.framebufferWidth;
            mainCamera.framebufferHeight = app::window_vars.framebufferHeight;

            renderer::uniformBuffer::CameraData cameraMatrices(mainCamera);
            glBindBuffer(GL_UNIFORM_BUFFER, u_cameraData);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(renderer::uniformBuffer::CameraData), &cameraMatrices, GL_DYNAMIC_DRAW);

            {
                renderer::usePart(*cubePart);

                renderer::setUniform(*cubeMaterial->shader, glUniform1f, "specularShininess", specularShininess);

                *cubeTransform = math::buildTransformMatrix(mainCubeData.position, mainCubeData.rotation, mainCubeData.scale);
                renderer::drawPart(*cubePart, cubeTransform, u_modelData);
            }

            {
                renderer::usePart(*cubePart);

                for (size_t i = 0; i < batchCubes.size(); i++)
                {
                    math::TransformMatrix cubeTransformMatrix = math::buildTransformMatrix(batchCubes.at(i).position, batchCubes.at(i).rotation, batchCubes.at(i).scale);
                    renderer::drawPart(*cubePart, cubeTransform, u_modelData);
                }
            }

            /*{
                for(const renderer::Part& part: parts) {
                    renderer::usePart(part);

                    renderer::uniform::buffer::ModelData modelData(math::zeroes3, math::zeroes3, math::ones3);
                    glBindBuffer(GL_UNIFORM_BUFFER, u_modelData);
                    glBufferData(GL_UNIFORM_BUFFER, sizeof(renderer::uniform::buffer::ModelData), &modelData, GL_DYNAMIC_DRAW);

                    renderer::drawPart(part); 
                }
            }*/
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
