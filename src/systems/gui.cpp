#include "gui.hpp"

#include "app.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

namespace gui {
    References* refs = nullptr;

    void registerRefs(gui::References* refs) {
        gui::refs = refs;
    }

    void render() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("rov-sim-env GUI", NULL, ImGuiWindowFlags_AlwaysAutoResize);

            if(ImGui::TreeNode("CAMERA")) {
                ImGui::ColorEdit3("Clear", refs->camera->framebufferClearColor);
                ImGui::SliderFloat("FOV", &refs->camera->fov, 30.f, 90.f);
                ImGui::SliderFloat("Camera X", &refs->camera->rotation.x, -180.f, 180.f);
                ImGui::SliderFloat("Camera Y", &refs->camera->rotation.y, app::settings.cameraMinY, app::settings.cameraMaxY);
                ImGui::TreePop();
            }

            if(ImGui::TreeNode("MAIN")) {
                ImGui::SeparatorText("Transform");
                ImGui::SliderFloat3("Translate", refs->mainCube->position, -10.f, 10.f);
                ImGui::SliderFloat3("Scale", refs->mainCube->scale, 0.01f, 10.f);
                ImGui::SliderFloat3("Rotate", refs->mainCube->rotation, 180.f, -180.f);
            }

            if(ImGui::TreeNode("LIGHT")) {
                ImGui::SeparatorText("Point Light");
                ImGui::SliderFloat3("Position", refs->pointLightPosition, -5.f, 5.f);
                ImGui::ColorEdit3("Pnt. Color", refs->pointLightColor);
                ImGui::SeparatorText("Ambient Light");
                ImGui::SliderFloat("Intensity", refs->ambientLightIntensity, 0.f, 1.f);
                ImGui::ColorEdit3("Amb. Color", refs->ambientLightColor);
                ImGui::TreePop();
            }

            if(ImGui::TreeNode("BATCH")) {
                ImGui::SeparatorText("Operations");
                if(ImGui::Button("Push Random Object")) {
                    refs->batchCubes->push_back(object::generateRandomBatchCube(-3.f , 1.5f, .5f, 0.1f));
                }; ImGui::SameLine(); if(ImGui::Button("Pop Object")) {
                    if(refs->batchCubes->size() > 0)
                        refs->batchCubes->pop_back();
                } ImGui::SameLine(); ImGui::Text("%ld Object(s)", refs->batchCubes->size());
                if(ImGui::TreeNode("Objects")) {
                    {
                        ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 1), ImVec2(FLT_MAX, ImGui::GetTextLineHeightWithSpacing() * 4));
                        ImGui::BeginChild("ConstrainedChild", ImVec2(0.f, .0f), 1, ImGuiWindowFlags_AlwaysVerticalScrollbar);
                        for (int i = 0; i < refs->batchCubes->size(); i++) {
                            ImGui::Text("Object %d: ", i); ImGui::SameLine(); ImGui::ColorButton("Object Color", *((ImVec4*)&refs->batchCubes->at(i).mixColor), 0, { ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight() }); 
                        }
                        ImGui::EndChild();
                    }
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
            
            if(ImGui::TreeNode("DEBUG")) {
                ImGui::Checkbox("Wireframe Toggle", &app::state_vars.isWireframe);
                ImGui::TreePop();
            }

            ImGui::SeparatorText("PERFORMANCE");
            ImGui::Text("Application average %.3f ms/f rame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}
