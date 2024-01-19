#include "gui.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

namespace gui {
    void init(gui::vars& vars) {
        vars.mainCube->position[0] = 2.f - ((rand() % 401) / 100.f);
        vars.mainCube->position[1] = 2.f - ((rand() % 401) / 100.f);
        vars.mainCube->position[2] = -3.f - ((rand() % 401) / 100.f);
        vars.mainCube->rotation[0] = 180.f - ((rand() % 3601) / 10.f);
        vars.mainCube->rotation[1] = 180.f - ((rand() % 3601) / 10.f);
        vars.mainCube->rotation[2] = 180.f - ((rand() % 3601) / 10.f);
        vars.mainCube->scale[0] = 4.f - ((rand() % 201) / 100.f);
        vars.mainCube->scale[1] = 4.f - ((rand() % 201) / 100.f);
        vars.mainCube->scale[2] = 4.f - ((rand() % 201) / 100.f);
    }

    void render(gui::vars& vars) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("rov-sim-env GUI", NULL, ImGuiWindowFlags_AlwaysAutoResize);

            if(ImGui::TreeNode("CAMERA")) {
                ImGui::ColorEdit3("Clear",*vars.framebufferClearColor);
                ImGui::SliderFloat("FOV", vars.fov, 30.f, 90.f);
                ImGui::SliderFloat("Camera X", vars.cameraX, -180.f, 180.f);
                ImGui::SliderFloat("Camera Y", vars.cameraY, -180.f, 180.f);
                ImGui::TreePop();
            }

            if(ImGui::TreeNode("MAIN")) {
                ImGui::SeparatorText("Transform");
                ImGui::SliderFloat3("Translate", vars.mainCube->position, -10.f, 10.f);
                ImGui::SliderFloat3("Scale", vars.mainCube->scale, 0.01f, 10.f);
                ImGui::SliderFloat3("Rotate", vars.mainCube->rotation, 180.f, -180.f);

                ImGui::SeparatorText("Color");
                for (size_t i = 0; i < 4; i++) {
                    char colorSliderText[50];
                    sprintf(colorSliderText, "Vertex %d", (int)i);
                    ImGui::ColorEdit3(colorSliderText, vars.mainCube->vertexColors[i]);
                }
                ImGui::SliderFloat("Mix Weight", &vars.mainCube->mixWeight, 0.f, 1.f);
                ImGui::TreePop();
            }

            if(ImGui::TreeNode("BATCH")) {
                ImGui::SeparatorText("Operations");
                if(ImGui::Button("Push Random Object")) {
                    vars.batchCubes->push_back(object::generateRandomBatchCube(-3.f , 1.5f, .5f, 0.1f));
                }; ImGui::SameLine(); if(ImGui::Button("Pop Object")) {
                    if(vars.batchCubes->size() > 0)
                        vars.batchCubes->pop_back();
                } ImGui::SameLine(); ImGui::Text("%ld Object(s)", vars.batchCubes->size());
                if(ImGui::TreeNode("Objects")) {
                    {
                        ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 1), ImVec2(FLT_MAX, ImGui::GetTextLineHeightWithSpacing() * 4));
                        ImGui::BeginChild("ConstrainedChild", ImVec2(0.f, .0f), 1, ImGuiWindowFlags_AlwaysVerticalScrollbar);
                        for (int i = 0; i < vars.batchCubes->size(); i++) {
                            ImGui::Text("Object %d: ", i); ImGui::SameLine(); ImGui::ColorButton("Object Color", *((ImVec4*)&vars.batchCubes->at(i).mixColor), 0, { ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight() }); 
                        }
                        ImGui::EndChild();
                    }
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
            
            if(ImGui::TreeNode("DEBUG")) {
                ImGui::Checkbox("Wireframe Toggle", &vars.isWireframe);
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
