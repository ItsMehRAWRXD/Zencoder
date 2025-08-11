#include <iostream>
#include "menu.h"
#include "packer.h"
#include <mutex>
#include <map>
#include <set>

// Global variables needed by the menu
std::mutex g_protectedRegionsMutex;
std::map<DWORD, std::set<std::pair<uintptr_t, uintptr_t>>> g_protectedRegions;

// Register a memory region to be protected
void registerProtectedMemoryRegion(DWORD processId, void* startAddress, size_t size) {
    std::lock_guard<std::mutex> lock(g_protectedRegionsMutex);
    g_protectedRegions[processId].insert(std::make_pair(
        reinterpret_cast<uintptr_t>(startAddress),
        reinterpret_cast<uintptr_t>(startAddress) + size
    ));
    
    std::cout << "[PROTECTION] Registered memory region " << std::hex 
              << startAddress << "-" << (uintptr_t)startAddress + size 
              << std::dec << " for process " << processId << std::endl;
}

int main() {
    std::cout << "==================================\n";
    std::cout << "      Advanced Crypto Utility     \n";
    std::cout << "==================================\n";
    std::cout << "This tool provides encryption, packing, and PE manipulation capabilities.\n";
    
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