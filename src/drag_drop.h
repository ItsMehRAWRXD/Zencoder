#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <functional>

namespace DragDrop {

    enum class DropEventType {
        FileDrop,
        DragEnter,
        DragLeave,
        DragOver
    };

    struct DropEvent {
        DropEventType type;
        std::vector<std::wstring> files;
        POINT position;
        DWORD keyState;
    };

    class DragDropHandler {
    public:
        using DropCallback = std::function<void(const DropEvent&)>;

        DragDropHandler();
        ~DragDropHandler();

        bool initialize(HWND hwnd);
        void cleanup();
        
        void setDropCallback(DropCallback callback);
        void setDragEnterCallback(DropCallback callback);
        void setDragLeaveCallback(DropCallback callback);
        void setDragOverCallback(DropCallback callback);

        bool enableDragDrop(bool enable = true);
        bool isDragDropEnabled() const;

        static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    private:
        HWND m_hwnd;
        bool m_initialized;
        bool m_dragDropEnabled;
        
        DropCallback m_dropCallback;
        DropCallback m_dragEnterCallback;
        DropCallback m_dragLeaveCallback;
        DropCallback m_dragOverCallback;

        WNDPROC m_originalWndProc;

        void handleDrop(HDROP hDrop, POINT pt, DWORD keyState);
        void handleDragEnter(HDROP hDrop, POINT pt, DWORD keyState);
        void handleDragLeave();
        void handleDragOver(HDROP hDrop, POINT pt, DWORD keyState);
    };

} // namespace DragDrop