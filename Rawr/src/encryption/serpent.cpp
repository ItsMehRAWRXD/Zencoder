#include <iostream>
#include "menu.h"

int main() {
    while (true) {
        displayMenu();
        int choice;
        std::cin >> choice;

        if (choice == 0) {
            break; // Exit the application
        }

        handleUserSelection(choice);
    }
    return 0;
}