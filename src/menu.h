#pragma once

#include <string>
#include <vector>
#include <cstdint>

// Forward declarations
class AESCBC;
class FileUtils;
class StubGenerator;
class DragDropHandler;

// Menu display and handling functions
void displayMenu();
void handleMenuChoice(int choice);

// Encryption submenu
void displayEncryptionMenu();
void handleEncryptionChoice(int choice);

// File operations submenu
void displayFileMenu();
void handleFileChoice(int choice);

// Stub generator submenu
void displayStubMenu();
void handleStubChoice(int choice);

// Drag and drop submenu
void displayDragDropMenu();
void handleDragDropChoice(int choice);

// Utility functions
std::string getInputPath(const std::string& prompt);
std::vector<std::string> getMultipleInputPaths(const std::string& prompt);
std::string getHexSeed(const std::string& prompt);