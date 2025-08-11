#include <iostream>
#include "menu.h"

int main() {
    // Initialize the application
    std::cout << "Welcome to the Encryption and Packing Tool!" << std::endl;

    // Display the menu and handle user input
    displayMenu();

    return 0;
}

#pragma once

#include <string>
#include <vector>

// Forward declaration of needed classes
class Z3Processor;
class KernelHookManager;

// Menu display and handling functions
void displayMenu();
void handleMenuChoice(int choice);

// Encryption submenu
void displayEncryptionMenu();
void handleEncryptionChoice(int choice, Z3Processor& processor);

// Packing submenu
void displayPackingMenu();
void handlePackingChoice(int choice, Z3Processor& processor);

// PE manipulation submenu
void displayPEMenu();
void handlePEManipulationChoice(int choice, Z3Processor& processor, KernelHookManager& hookManager);

// Security and hooks submenu
void displaySecurityMenu();
void handleSecurityChoice(int choice, KernelHookManager& hookManager);

// Utility functions
std::string getInputPath(const std::string& prompt);
std::vector<std::string> getMultipleInputPaths();
std::vector<uint8_t> getHexSeed();