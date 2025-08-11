#include "drag_drop.h"
#include <shellapi.h>
#include <map>
#include <iostream>

namespace DragDrop {

    // Global map to store window handles to their DragDropHandler instances
    static std::map<HWND, DragDropHandler*> g_handlerMap;

    DragDropHandler::DragDropHandler() 
        : m_hwnd(nullptr)
        , m_initialized(false)
        , m_dragDropEnabled(false)
        , m_originalWndProc(nullptr) {
    }

    DragDropHandler::~DragDropHandler() {
        cleanup();
    }

    bool DragDropHandler::initialize(HWND hwnd) {
        if (m_initialized) {
            cleanup();
        }

        m_hwnd = hwnd;
        
        // Store this instance in the global map
        g_handlerMap[hwnd] = this;
        
        // Set the window procedure
        m_originalWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)windowProc);
        
        if (!m_originalWndProc) {
            g_handlerMap.erase(hwnd);
            return false;
        }

        m_initialized = true;
        return true;
    }

    void DragDropHandler::cleanup() {
        if (m_initialized && m_hwnd) {
            // Restore original window procedure
            SetWindowLongPtr(m_hwnd, GWLP_WNDPROC, (LONG_PTR)m_originalWndProc);
            
            // Remove from global map
            g_handlerMap.erase(m_hwnd);
            
            // Disable drag and drop
            if (m_dragDropEnabled) {
                DragAcceptFiles(m_hwnd, FALSE);
            }
        }

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
        if (!m_initialized || !m_hwnd) {
            return false;
        }

        m_dragDropEnabled = enable;
        DragAcceptFiles(m_hwnd, enable ? TRUE : FALSE);
        return true;
    }

    bool DragDropHandler::isDragDropEnabled() const {
        return m_dragDropEnabled && m_initialized;
    }

    LRESULT CALLBACK DragDropHandler::windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        auto it = g_handlerMap.find(hwnd);
        if (it == g_handlerMap.end()) {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }

        DragDropHandler* handler = it->second;

        switch (msg) {
        case WM_DROPFILES: {
            HDROP hDrop = (HDROP)wParam;
            POINT pt;
            DragQueryPoint(hDrop, &pt);
            DWORD keyState = GetKeyState(VK_CONTROL) | GetKeyState(VK_SHIFT) | GetKeyState(VK_ALT);
            handler->handleDrop(hDrop, pt, keyState);
            return 0;
        }
        case WM_DRAGENTER: {
            HDROP hDrop = (HDROP)wParam;
            POINT pt;
            DragQueryPoint(hDrop, &pt);
            DWORD keyState = GetKeyState(VK_CONTROL) | GetKeyState(VK_SHIFT) | GetKeyState(VK_ALT);
            handler->handleDragEnter(hDrop, pt, keyState);
            return 0;
        }
        case WM_DRAGLEAVE: {
            handler->handleDragLeave();
            return 0;
        }
        case WM_DRAGOVER: {
            HDROP hDrop = (HDROP)wParam;
            POINT pt;
            DragQueryPoint(hDrop, &pt);
            DWORD keyState = GetKeyState(VK_CONTROL) | GetKeyState(VK_SHIFT) | GetKeyState(VK_ALT);
            handler->handleDragOver(hDrop, pt, keyState);
            return 0;
        }
        default:
            break;
        }

        // Call original window procedure
        return CallWindowProc(handler->m_originalWndProc, hwnd, msg, wParam, lParam);
    }

    void DragDropHandler::handleDrop(HDROP hDrop, POINT pt, DWORD keyState) {
        if (!m_dropCallback) {
            return;
        }

        DropEvent event;
        event.type = DropEventType::FileDrop;
        event.position = pt;
        event.keyState = keyState;

        // Get file count
        UINT fileCount = DragQueryFile(hDrop, 0xFFFFFFFF, nullptr, 0);
        
        // Get each file path
        for (UINT i = 0; i < fileCount; ++i) {
            UINT pathLen = DragQueryFile(hDrop, i, nullptr, 0);
            if (pathLen > 0) {
                std::wstring filePath(pathLen + 1, L'\0');
                DragQueryFile(hDrop, i, &filePath[0], pathLen + 1);
                event.files.push_back(filePath);
            }
        }

        m_dropCallback(event);
    }

    void DragDropHandler::handleDragEnter(HDROP hDrop, POINT pt, DWORD keyState) {
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

    void DragDropHandler::handleDragOver(HDROP hDrop, POINT pt, DWORD keyState) {
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