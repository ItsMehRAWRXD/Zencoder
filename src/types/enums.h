#include <iostream>
#include "menu.h"

int main() {
    while (true) {
        displayMenu();
        int choice;
        std::cin >> choice;

        if (choice == 0) {
            std::cout << "Exiting the application." << std::endl;
            break;
        }

        handleUserChoice(choice);
    }
    return 0;
}