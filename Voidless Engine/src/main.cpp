#include "Voidless/Window.hpp"
#include <iostream>

int main() {
    std::cout << "Voidless Engine Initializing..." << std::endl;

    Voidless::WindowProps props("Voidless Engine v0.1", 1280, 720);
    Voidless::Window window(props);

    while (!window.ShouldClose()) {
        window.OnUpdate();
    }

    return 0;
}
