#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
#include <windows.h>

// Forward declarations
class Z3Processor;

// Stub for KernelHookManager since it's referenced but not fully defined
class KernelHookManager {
public:
    bool isKernelHookAvailable() { return false; }
    bool initialize() { return false; }
    void cleanup() {}
};

// Menu display and handling functions
void displayMenu();
void handleMenuChoice(int choice);
void handleUserChoice(int choice);

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

#endif // MENU_H

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