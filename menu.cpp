#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "./include/Credentials.h"

using namespace std;

struct Menu {
    std::vector<std::string> items;
    int selectedItem = -1;
    bool isVisible = false;
    float x, y;
};

Menu contextMenu;

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        contextMenu.isVisible = true;
        contextMenu.x = static_cast<float>(xpos);
        contextMenu.y = HEIGHT - static_cast<float>(ypos);

        contextMenu.items = { "Option 1", "Option 2", "Option 3" };
        contextMenu.selectedItem = -1;
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (contextMenu.isVisible) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            float y = HEIGHT - static_cast<float>(ypos);
            for (int i = 0; i < contextMenu.items.size(); ++i) {
                float itemY = contextMenu.y - i * 20;
                if (y > itemY - 10 && y < itemY + 10) {
                    contextMenu.selectedItem = i;
                    contextMenu.isVisible = false;
                    std::cout << "Selected: " << contextMenu.items[i] << std::endl;
                    break;
                }
            }
        }
    }
}
