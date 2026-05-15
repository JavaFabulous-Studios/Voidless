#include "Voidless/Window.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
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
    }

    void Window::Shutdown() {
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

    bool Window::ShouldClose() const {
        return glfwWindowShouldClose(m_Window);
    }

}
