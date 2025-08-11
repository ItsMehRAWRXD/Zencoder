#include "drag_drop.h"
#include <iostream>

namespace DragDrop {

    DragDropHandler::DragDropHandler() 
        : m_initialized(false)
        , m_dragDropEnabled(false)
        , m_hwnd(nullptr)
        , m_originalWndProc(nullptr) {
    }

    DragDropHandler::~DragDropHandler() {
        cleanup();
    }

    bool DragDropHandler::initialize(void* hwnd) {
        if (m_initialized) {
            cleanup();
        }

        m_hwnd = hwnd;
        m_initialized = true;
        
        std::cout << "Drag and drop initialized (cross-platform stub)" << std::endl;
        return true;
    }

    void DragDropHandler::cleanup() {
        m_hwnd = nullptr;
        m_initialized = false;
        m_dragDropEnabled = false;
        m_originalWndProc = nullptr;
    }

    void DragDropHandler::setDropCallback(DropCallback callback) {
        m_dropCallback = callback;
    }

    void DragDropHandler::setDragEnterCallback(DropCallback callback) {
        m_dragEnterCallback = callback;
    }

    void DragDropHandler::setDragLeaveCallback(DropCallback callback) {
        m_dragLeaveCallback = callback;
    }

    void DragDropHandler::setDragOverCallback(DropCallback callback) {
        m_dragOverCallback = callback;
    }

    bool DragDropHandler::enableDragDrop(bool enable) {
        if (!m_initialized) {
            return false;
        }

        m_dragDropEnabled = enable;
        std::cout << "Drag and drop " << (enable ? "enabled" : "disabled") << std::endl;
        return true;
    }

    bool DragDropHandler::isDragDropEnabled() const {
        return m_dragDropEnabled && m_initialized;
    }

#ifdef _WIN32
    LRESULT CALLBACK DragDropHandler::windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        // Windows-specific implementation would go here
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
#else
    int DragDropHandler::windowProc(void* hwnd, int msg, void* wParam, void* lParam) {
        // Cross-platform stub implementation
        return 0;
    }
#endif

    void DragDropHandler::handleDrop(void* hDrop, struct { int x, y; } pt, unsigned int keyState) {
        if (!m_dropCallback) {
            return;
        }

        DropEvent event;
        event.type = DropEventType::FileDrop;
        event.position = pt;
        event.keyState = keyState;
        event.files.push_back(L"example.txt"); // Placeholder

        m_dropCallback(event);
    }

    void DragDropHandler::handleDragEnter(void* hDrop, struct { int x, y; } pt, unsigned int keyState) {
        if (!m_dragEnterCallback) {
            return;
        }

        DropEvent event;
        event.type = DropEventType::DragEnter;
        event.position = pt;
        event.keyState = keyState;

        m_dragEnterCallback(event);
    }

    void DragDropHandler::handleDragLeave() {
        if (!m_dragLeaveCallback) {
            return;
        }

        DropEvent event;
        event.type = DropEventType::DragLeave;
        event.position = {0, 0};
        event.keyState = 0;

        m_dragLeaveCallback(event);
    }

    void DragDropHandler::handleDragOver(void* hDrop, struct { int x, y; } pt, unsigned int keyState) {
        if (!m_dragOverCallback) {
            return;
        }

        DropEvent event;
        event.type = DropEventType::DragOver;
        event.position = pt;
        event.keyState = keyState;

        m_dragOverCallback(event);
    }

} // namespace DragDrop