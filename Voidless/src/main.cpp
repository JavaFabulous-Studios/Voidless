#include "Voidless/Window.hpp"
#include <iostream>

int main() {
    std::cout << "Starting Voidless Game..." << std::endl;

    Voidless::WindowProps props("Voidless", 1280, 720);
    Voidless::Window window(props);

    while (!window.ShouldClose()) {
        // Game Logic here
        
        window.OnUpdate();
    }

    return 0;
}
