#include "Voidless/Window.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <iostream>

namespace Voidless {

    static bool s_GLFWInitialized = false;

    Window::Window(const WindowProps& props) {
        Init(props);
    }

    Window::~Window() {
        Shutdown();
    }

    void Window::Init(const WindowProps& props) {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        if (!s_GLFWInitialized) {
            int success = glfwInit();
            if (!success) {
                std::cerr << "Could not initialize GLFW!" << std::endl;
                return;
            }
            s_GLFWInitialized = true;
        }

        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);

        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (!status) {
            std::cerr << "Failed to initialize Glad!" << std::endl;
            return;
        }

        std::cout << "Window created: " << props.Title << " (" << props.Width << "x" << props.Height << ")" << std::endl;

        // Initialize ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
        ImGui_ImplOpenGL3_Init("#version 410");

        // Custom Premium Theme
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 12.0f;
        style.FrameRounding = 8.0f;
        style.GrabRounding = 8.0f;
        style.PopupRounding = 12.0f;
        style.ScrollbarRounding = 12.0f;
        style.WindowBorderSize = 1.0f;
        style.FrameBorderSize = 0.0f;
        
        ImVec4* colors = style.Colors;
        colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.02f, 0.1f, 0.95f);
        colors[ImGuiCol_Border] = ImVec4(0.55f, 0.36f, 0.96f, 0.4f);
        colors[ImGuiCol_Button] = ImVec4(0.55f, 0.36f, 0.96f, 0.6f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.55f, 0.36f, 0.96f, 1.0f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.45f, 0.26f, 0.86f, 1.0f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.1f, 0.1f, 0.2f, 1.0f);
        colors[ImGuiCol_Header] = ImVec4(0.55f, 0.36f, 0.96f, 0.4f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.05f, 0.02f, 0.1f, 1.0f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.55f, 0.36f, 0.96f, 0.2f);
    }

    void Window::Shutdown() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(m_Window);
    }

    void Window::OnUpdate() {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void Window::Clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Window::SetClearColor(float r, float g, float b, float a) {
        glClearColor(r, g, b, a);
    }

    void Window::BeginUI() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Window::EndUI() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    bool Window::ShouldClose() const {
        return glfwWindowShouldClose(m_Window);
    }

}
