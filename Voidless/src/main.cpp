#include "Voidless/Audio.hpp"
#include "Voidless/Window.hpp"
#include "Voidless/Network.hpp"
#include "Voidless/Shader.hpp"
#include "Voidless/Camera.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <iostream>
#include <vector>

enum class GameState { MainMenu, Playing };

// Global Camera setup
Voidless::Camera camera(glm::vec3(0.0f, 1.0f, 8.0f));
float lastX = 1280.0f / 2.0f;
float lastY = 720.0f / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool captureMouse = false;

// Input callback variables
GLFWwindow* nativeWindow = nullptr;

// Simple AABB Collision check
bool CheckCollision(glm::vec3 playerPos, const std::vector<glm::vec3>& obstacles) {
    float playerSize = 0.5f; // Boundary around the player
    
    for (size_t i = 1; i < obstacles.size(); i++) {
        glm::vec3 obsPos = obstacles[i];
        glm::vec3 obsSize = glm::vec3(2.2f, 3.2f, 2.2f); // Slightly larger than visual for safety
        
        bool collisionX = playerPos.x + playerSize >= obsPos.x - obsSize.x / 2.0f &&
                         obsPos.x + obsSize.x / 2.0f >= playerPos.x - playerSize;
        bool collisionZ = playerPos.z + playerSize >= obsPos.z - obsSize.z / 2.0f &&
                         obsPos.z + obsSize.z / 2.0f >= playerPos.z - playerSize;
        
        if (collisionX && collisionZ) return true;
    }
    return false;
}

void ProcessInput(GLFWwindow* window, const std::vector<glm::vec3>& obstacles) {
    glm::vec3 oldPos = camera.Position;
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(0, deltaTime); // FORWARD
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(1, deltaTime); // BACKWARD
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(2, deltaTime); // LEFT
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(3, deltaTime); // RIGHT

    // If we hit a wall, move back
    if (CheckCollision(camera.Position, obstacles)) {
        camera.Position = oldPos;
    }
}

int main() {
    std::cout << "Starting Voidless Game..." << std::endl;

    Voidless::WindowProps props("Voidless", 1280, 720);
    Voidless::Window window(props);
    nativeWindow = (GLFWwindow*)window.GetNativeWindow();

    Voidless::Audio::Init();
    Voidless::Audio::PlayMusic("C:\\Games\\Voidless\\Voidless\\assets\\menu_music.mp3", true);

    Voidless::Network::Init();
    bool connected = Voidless::Network::Connect("192.168.1.9", 1234);

    // OpenGL depth testing for 3D
    glEnable(GL_DEPTH_TEST);

    // Cube vertices
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f,
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Voidless::Shader basicShader;
    if (!basicShader.Load("C:\\Games\\Voidless\\Voidless\\assets\\shaders\\basic.vert", "C:\\Games\\Voidless\\Voidless\\assets\\shaders\\basic.frag")) {
        std::cerr << "Failed to load shaders!" << std::endl;
    }

    // World layout (Floor and some obstacles)
    std::vector<glm::vec3> cubePositions = {
        glm::vec3( 0.0f, -0.5f,  0.0f), // Floor (scaled up later)
        glm::vec3( 2.0f,  1.0f, -5.0f), // Obstacle 1
        glm::vec3(-3.0f,  1.0f, -3.0f), // Obstacle 2
        glm::vec3( 1.0f,  1.0f,  4.0f)  // Obstacle 3
    };

    window.SetClearColor(0.05f, 0.02f, 0.1f, 1.0f);
    GameState currentState = GameState::MainMenu;

    while (!window.ShouldClose()) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        window.Clear();

        if (currentState == GameState::MainMenu) {
            captureMouse = false;
            glfwSetInputMode(nativeWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

            window.BeginUI();
            ImGui::SetNextWindowPos(ImVec2(540, 300), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(200, 150), ImGuiCond_Always);
            ImGui::Begin("Main Menu", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
            
            ImGui::SetCursorPosX(50);
            ImGui::Text("VOIDLESS");
            if (Voidless::Network::IsConnected()) {
                ImGui::TextColored(ImVec4(0, 1, 0, 1), "Server: ONLINE");
            } else {
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Server: OFFLINE");
            }
            ImGui::Separator();
            
            ImGui::SetCursorPosY(60);
            if (Voidless::Network::IsConnected()) {
                if (ImGui::Button("START GAME", ImVec2(100, 40))) {
                    currentState = GameState::Playing;
                    glfwSetInputMode(nativeWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                    captureMouse = true;
                    firstMouse = true;
                    camera.MovementSpeed = 8.0f; // Boosted speed
                }
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
                ImGui::Button("OFFLINE", ImVec2(100, 40));
                ImGui::PopStyleColor(3);
            }
            
            if (ImGui::Button("EXIT", ImVec2(100, 30))) {
                break;
            }
            ImGui::End();
            window.EndUI();
        } 
        else if (currentState == GameState::Playing) {
            if (captureMouse) {
                double xposIn, yposIn;
                glfwGetCursorPos(nativeWindow, &xposIn, &yposIn);
                
                float xpos = static_cast<float>(xposIn);
                float ypos = static_cast<float>(yposIn);

                if (firstMouse) {
                    lastX = xpos;
                    lastY = ypos;
                    firstMouse = false;
                }

                float xoffset = xpos - lastX;
                float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

                lastX = xpos;
                lastY = ypos;

                camera.ProcessMouseMovement(xoffset, yoffset);
            }

            // Process movement
            ProcessInput(nativeWindow, cubePositions);

            // Render 3D Scene
            basicShader.Bind();
            
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)window.GetWidth() / (float)window.GetHeight(), 0.1f, 100.0f);
            glm::mat4 view = camera.GetViewMatrix();
            basicShader.SetMat4("projection", projection);
            basicShader.SetMat4("view", view);

            glBindVertexArray(VAO);

            // Draw Floor
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[0]);
            model = glm::scale(model, glm::vec3(20.0f, 0.1f, 20.0f)); // Big flat floor
            basicShader.SetMat4("model", model);
            basicShader.SetVec3("color", glm::vec3(0.2f, 0.2f, 0.3f)); // Dark floor
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Draw Obstacles
            for (size_t i = 1; i < cubePositions.size(); i++) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                model = glm::scale(model, glm::vec3(2.0f, 3.0f, 2.0f)); // Tall box
                basicShader.SetMat4("model", model);
                basicShader.SetVec3("color", glm::vec3(0.8f, 0.1f, 0.4f)); // Neon pink/purple boxes
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            // Allow returning to menu with ESC
            if (glfwGetKey(nativeWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                currentState = GameState::MainMenu;
                captureMouse = false;
                glfwSetInputMode(nativeWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }

            // Render UI over 3D
            window.BeginUI();
            ImGui::SetNextWindowPos(ImVec2(10, 10));
            ImGui::Begin("Game Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
            ImGui::Text("FPS Mode - Press ESC for Menu");
            ImGui::Text("Pos: %.1f, %.1f, %.1f", camera.Position.x, camera.Position.y, camera.Position.z);
            ImGui::End();

            // Crosshair
            ImGui::SetNextWindowPos(ImVec2(window.GetWidth() / 2.0f - 5, window.GetHeight() / 2.0f - 5));
            ImGui::Begin("Crosshair", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs);
            ImGui::Text("+");
            ImGui::End();

            window.EndUI();
        }

        window.OnUpdate();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    Voidless::Audio::Shutdown();
    Voidless::Network::Shutdown();
    return 0;
}
