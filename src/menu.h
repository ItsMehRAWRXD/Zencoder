#pragma once

#include <string>
#include <vector>

// Menu display and handling functions
void displayMenu();
void handleMenuChoice(int choice);

// Encryption submenu
void displayEncryptionMenu();
void handleEncryptionChoice(int choice);

// File operations submenu
void displayFileMenu();
void handleFileChoice(int choice);

// Utility functions
std::string getInputPath(const std::string& prompt);
std::vector<std::string> getMultipleInputPaths();
std::vector<uint8_t> getHexSeed();