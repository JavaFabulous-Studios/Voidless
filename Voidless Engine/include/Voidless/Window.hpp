#pragma once

#include <string>
#include <memory>

struct GLFWwindow;

namespace Voidless {

    struct WindowProps {
        std::string Title;
        unsigned int Width;
        unsigned int Height;

        WindowProps(const std::string& title = "Voidless Engine",
                    unsigned int width = 1280,
                    unsigned int height = 720)
            : Title(title), Width(width), Height(height) {}
    };

    class Window {
    public:
        Window(const WindowProps& props);
        ~Window();

        void OnUpdate();
        void Clear();
        void SetClearColor(float r, float g, float b, float a);

        // UI
        void BeginUI();
        void EndUI();

        unsigned int GetWidth() const { return m_Data.Width; }
        unsigned int GetHeight() const { return m_Data.Height; }

        bool ShouldClose() const;
        void* GetNativeWindow() const { return m_Window; }

    private:
        void Init(const WindowProps& props);
        void Shutdown();

    private:
        GLFWwindow* m_Window;

        struct WindowData {
            std::string Title;
            unsigned int Width, Height;
        };

        WindowData m_Data;
    };

}
