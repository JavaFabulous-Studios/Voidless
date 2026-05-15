#include "Voidless/Window.hpp"
#include "Voidless/Audio.hpp"
#include <glad/glad.h>
#include <imgui.h>
#include <iostream>

enum class GameState {
    MainMenu,
    Playing
};

int main() {
    std::cout << "Starting Voidless Game..." << std::endl;

    Voidless::WindowProps props("Voidless", 1280, 720);
    Voidless::Window window(props);
    
    Voidless::Audio::Init();
    Voidless::Audio::PlayMusic("assets/menu_music.mp3", true);

    window.SetClearColor(0.05f, 0.02f, 0.1f, 1.0f);
    GameState currentState = GameState::MainMenu;

    while (!window.ShouldClose()) {
        window.Clear();
        window.BeginUI();

        if (currentState == GameState::MainMenu) {
            ImGui::SetNextWindowPos(ImVec2(540, 300), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(200, 150), ImGuiCond_Always);
            ImGui::Begin("Main Menu", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
            
            ImGui::SetCursorPosX(50);
            ImGui::Text("VOIDLESS");
            ImGui::Separator();
            
            ImGui::SetCursorPosY(60);
            if (ImGui::Button("START GAME", ImVec2(100, 40))) {
                currentState = GameState::Playing;
            }
            
            if (ImGui::Button("EXIT", ImVec2(100, 30))) {
                break;
            }
            
            ImGui::End();
        } else if (currentState == GameState::Playing) {
            ImGui::SetNextWindowPos(ImVec2(10, 10));
            ImGui::Begin("Game Info");
            ImGui::Text("Playing Voidless...");
            if (ImGui::Button("Back to Menu")) {
                currentState = GameState::MainMenu;
            }
            ImGui::End();
        }

        window.EndUI();
        window.OnUpdate();
    }

    Voidless::Audio::Shutdown();
    return 0;
}
