#include "Voidless/Input.hpp"
#include "Voidless/Window.hpp"
#include <GLFW/glfw3.h>

namespace Voidless {

    class WindowsInput : public Input {
    protected:
        virtual bool IsKeyPressedImpl(int keycode) override {
            // This is a bit of a hack, but we need access to the window.
            // In a real engine, we'd have an Application singleton.
            // For now, we'll assume there's one window and it's globally accessible or we find it.
            // Let's assume we can get it from somewhere. 
            // For now, I'll just use the first window I can find if I had a list, 
            // but let's just use the current context for now if possible.
            auto window = static_cast<GLFWwindow*>(glfwGetCurrentContext());
            if (!window) return false;
            auto state = glfwGetKey(window, keycode);
            return state == GLFW_PRESS || state == GLFW_REPEAT;
        }

        virtual bool IsMouseButtonPressedImpl(int button) override {
            auto window = static_cast<GLFWwindow*>(glfwGetCurrentContext());
            if (!window) return false;
            auto state = glfwGetMouseButton(window, button);
            return state == GLFW_PRESS;
        }

        virtual std::pair<float, float> GetMousePositionImpl() override {
            auto window = static_cast<GLFWwindow*>(glfwGetCurrentContext());
            if (!window) return {0, 0};
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            return { (float)xpos, (float)ypos };
        }

        virtual float GetMouseXImpl() override {
            auto[x, y] = GetMousePositionImpl();
            return x;
        }

        virtual float GetMouseYImpl() override {
            auto[x, y] = GetMousePositionImpl();
            return y;
        }
    };

    Input* Input::s_Instance = new WindowsInput();

    bool Input::IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
    bool Input::IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
    std::pair<float, float> Input::GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
    float Input::GetMouseX() { return s_Instance->GetMouseXImpl(); }
    float Input::GetMouseY() { return s_Instance->GetMouseYImpl(); }

}
