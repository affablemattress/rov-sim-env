#include "renderer/model.hpp"

#include "app.hpp"

#include "spdlog/spdlog.h"

#include <queue>

namespace renderer {
    void loadAndPushAssimpModel(Assimp::Importer& aiImporter, Model& model, std::string_view modelPath,
                                Shader* shader, std::vector<Texture2D>& textures, std::vector<Material>& materials,
                                std::vector<Mesh>& meshes, std::vector<math::TransformMatrix>& localTransforms, std::vector<Part>& parts) {
        spdlog::info("Loading model at path: {0}", modelPath.data());

        std::string modelDirectory(modelPath.substr(0, modelPath.find_last_of('/') + 1));

        ///Load Scene
        const aiScene* scene = aiImporter.ReadFile(modelPath.data(), 
                                                 aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace |aiProcess_RemoveRedundantMaterials | aiProcess_FixInfacingNormals | aiProcess_ImproveCacheLocality | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            spdlog::error("Couldn't load model: {0}", modelPath.data());
            spdlog::error(aiImporter.GetErrorString());
            app::lifetime::killAll(1);
        }

        std::vector<Material*> loadedMaterials;
        std::vector<Mesh*> loadedMeshes;

        ///Load Materials
        for(size_t i = 0; i < scene->mNumMaterials; i++) {
            spdlog::info("Loading material [{0}] for model.", i);

            aiMaterial* srcAiMaterial = scene->mMaterials[i];

            Texture2D* diffuseTexture = nullptr;
            Texture2D* specularTexture = nullptr;
            Texture2D* normalTexture = nullptr;
            Texture2D* emissiveTexture = nullptr;
            Texture2D* occlusionTexture = nullptr;
            {
                if(srcAiMaterial->GetTextureCount(aiTextureType_DIFFUSE)) {
                    aiString relativeTexturePath;
                    srcAiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &relativeTexturePath);
                    std::string texturePath = modelDirectory + relativeTexturePath.C_Str();

                    for(size_t j = 0; j < textures.size(); j++) {
                        if(textures[j].filepath == texturePath) {
                            diffuseTexture = &textures[j];
                            spdlog::info("Texture already loaded: [{0}]", texturePath.c_str());
                            break;
                        }
                    }
                    if(!diffuseTexture) {
                        diffuseTexture = &textures.emplace_back();
                        d_Texture2D* textureData = loadTexture2DFromFile(texturePath, TextureType::k_diffuse);
                        pushTexture2D(*diffuseTexture, textureData);
                        freeTexture2D(textureData);
                    }
                }
                else {
                    spdlog::error("Material does not have diffuse component.");
                    app::lifetime::killAll(1);
                }
            }

            {
                if(srcAiMaterial->GetTextureCount(aiTextureType_METALNESS)) {
                    aiString relativeTexturePath;
                    srcAiMaterial->GetTexture(aiTextureType_METALNESS, 0, &relativeTexturePath);
                    std::string texturePath = modelDirectory + relativeTexturePath.C_Str();

                    for(size_t j = 0; j < textures.size(); j++) {
                        if(textures[j].filepath == texturePath) {
                            specularTexture = &textures[j];
                            spdlog::info("Texture already loaded: [{0}]", texturePath.c_str());
                            break;
                        }
                    }
                    if(!specularTexture) {
                        specularTexture = &textures.emplace_back();
                        d_Texture2D* textureData = loadTexture2DFromFile(texturePath, TextureType::k_specular);
                        pushTexture2D(*specularTexture, textureData);
                        freeTexture2D(textureData);
                    }
                }
                else {
                    spdlog::error("Material does not have specular component.");
                    app::lifetime::killAll(1);
                }
            }

            {
                if(srcAiMaterial->GetTextureCount(aiTextureType_NORMALS)) {
                    aiString relativeTexturePath;
                    srcAiMaterial->GetTexture(aiTextureType_NORMALS, 0, &relativeTexturePath);
                    std::string texturePath = modelDirectory + relativeTexturePath.C_Str();

                    for(size_t j = 0; j < textures.size(); j++) {
                        if(textures[j].filepath == texturePath) {
                            normalTexture = &textures[j];
                            spdlog::info("Texture already loaded: [{0}]", texturePath.c_str());
                            break;
                        }
                    }
                    if(!normalTexture) {
                        normalTexture = &textures.emplace_back();
                        d_Texture2D* textureData = loadTexture2DFromFile(texturePath, TextureType::k_normal);
                        pushTexture2D(*normalTexture, textureData);
                        freeTexture2D(textureData);
                    }
                }
                else {
                    spdlog::error("Material does not have normal component.");
                    app::lifetime::killAll(1);
                }
            }

            Material* currentMaterial = &materials.emplace_back();
            loadedMaterials.push_back(currentMaterial);
            
            initMaterial(*currentMaterial, diffuseTexture, specularTexture, normalTexture, nullptr, nullptr, shader);
        }

        //Load Meshes
        for(size_t i = 0; i < scene->mNumMeshes; i++) {
            Mesh* currentMesh = &meshes.emplace_back();
            loadedMeshes.push_back(currentMesh);

            d_Mesh* meshData = loadMeshFromAssimpMesh(scene->mMeshes[i]);
            pushMesh(*currentMesh, meshData);
            freeMesh(meshData);
        }

        //Construct Model (Tree)
        std::queue<ModelNode*> parentsQueue;
        std::queue<aiNode*> BFSQueue;

        parentsQueue.push(nullptr);
        BFSQueue.push(scene->mRootNode);

        while(!BFSQueue.empty()) {
            ModelNode* parentModelNode = parentsQueue.front();
            aiNode* currentAiNode = BFSQueue.front();

            ModelNode* currentModelNode = nullptr;
            if(parentModelNode == nullptr) {
                currentModelNode = &model;
            }
            else {
                currentModelNode = parentModelNode->emplaceChild();
            }

            currentModelNode->data = &parts.emplace_back();
            Part* currentPart = currentModelNode->data;

            ///LOAD PART HERE
            math::TransformMatrix* currentTransform = &localTransforms.emplace_back();
            memcpy(currentTransform, &currentAiNode->mTransformation, sizeof(float) * 16);
            *currentTransform = glm::transpose(*currentTransform);

            currentPart->localTransformMatrix = currentTransform;
            currentPart->mesh = loadedMeshes[currentAiNode->mMeshes[0]];
            currentPart->material = loadedMaterials[scene->mMeshes[currentAiNode->mMeshes[0]]->mMaterialIndex];

            for(size_t i = 0; i < currentAiNode->mNumChildren; i++) {
                parentsQueue.push(currentModelNode);
                BFSQueue.push(currentAiNode->mChildren[i]);
            }

            parentsQueue.pop();
            BFSQueue.pop();
        }

        spdlog::info("Loaded model with [{0}] mesh(es) and [{1}] material(s).", loadedMeshes.size(), loadedMaterials.size());
    }

    void drawModelNode(const ModelNode* node, const math::TransformMatrix* parentTransformMatrix, GLuint u_modelData) {
        usePart(*node->data);
        math::TransformMatrix currentTransform = drawPart(*node->data, parentTransformMatrix, u_modelData);

        for(int i = 0; i < node->children.size(); i++) {
            drawModelNode(node->children[i], &currentTransform, u_modelData);
        }
    }

    void drawModel(const Model* model, const math::TransformMatrix* modelTransformMatrix, GLuint u_modelData) {
        drawModelNode(model, modelTransformMatrix, u_modelData);
    }
}