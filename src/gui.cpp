#include "gui.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

namespace gui {
    void init(gui::vars& vars) {
        srand(time(nullptr));
        (*vars.mainObjectPosition)[0] = 2.f - ((rand() % 401) / 100.f);
        (*vars.mainObjectPosition)[1] = 2.f - ((rand() % 401) / 100.f);
        (*vars.mainObjectPosition)[2] = -3.f - ((rand() % 401) / 100.f);
        (*vars.mainObjectRotation)[0] = 180.f - ((rand() % 3601) / 10.f);
        (*vars.mainObjectRotation)[1] = 180.f - ((rand() % 3601) / 10.f);
        (*vars.mainObjectRotation)[2] = 180.f - ((rand() % 3601) / 10.f);
        (*vars.mainObjectScale)[0] = 4.f - ((rand() % 201) / 100.f);
        (*vars.mainObjectScale)[1] = 4.f - ((rand() % 201) / 100.f);
        (*vars.mainObjectScale)[2] = 4.f - ((rand() % 201) / 100.f);
    }

    void render(gui::vars& vars) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("rov-sim-env GUI", NULL, ImGuiWindowFlags_AlwaysAutoResize);

            if(ImGui::TreeNode("CAMERA")) {
                ImGui::SliderFloat("FOV", vars.fov, 30.f, 90.f);
                ImGui::TreePop();
            }

            if(ImGui::TreeNode("MAIN")) {
                ImGui::SeparatorText("Transform");
                ImGui::SliderFloat3("Translate", *vars.mainObjectPosition, -10.f, 10.f);
                ImGui::SliderFloat3("Scale", *vars.mainObjectScale, 0.01f, 10.f);
                ImGui::SliderFloat3("Rotate", *vars.mainObjectRotation, 180.f, -180.f);

                ImGui::SeparatorText("Color");
                ImGui::ColorEdit3("Clear",*vars.framebufferClearColor);
                for (size_t i = 0; i < 4; i++) {
                    char colorSliderText[50];
                    sprintf(colorSliderText, "Vertex %d", (int)i);
                    ImGui::ColorEdit3(colorSliderText, (*vars.vertexColors)[i]);
                }
                ImGui::SliderFloat("Mix Weight", vars.mixWeight, 0.f, 1.f);
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
