#include "Voidless/Window.hpp"
#include "Voidless/Input.hpp"
#include <iostream>

int main() {
    std::cout << "Starting Voidless Game..." << std::endl;

    Voidless::WindowProps props("Voidless", 1280, 720);
    Voidless::Window window(props);

    window.SetClearColor(0.1f, 0.1f, 0.15f, 1.0f); // Sleek dark blue background

    while (!window.ShouldClose()) {
        window.Clear();

        if (Voidless::Input::IsKeyPressed(256)) { // GLFW_KEY_ESCAPE is 256
             break;
        }
        
        window.OnUpdate();
    }

    return 0;
}
