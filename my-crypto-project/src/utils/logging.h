#include <iostream>
#include "menu.h"

int main() {
    // Initialize the application
    std::cout << "Welcome to the File Encryption and Packing Tool!" << std::endl;

    // Display the menu and handle user input
    while (true) {
        int choice = displayMenu();
        if (choice == 0) {
            break; // Exit the application
        }
        handleUserSelection(choice);
    }

    return 0;
}