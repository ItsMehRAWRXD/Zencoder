#pragma once

#ifdef _WIN32
#include <windows.h>
#endif
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
#ifdef _WIN32
        POINT position;
        DWORD keyState;
#else
        struct { int x, y; } position;
        unsigned int keyState;
#endif
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

#ifdef _WIN32
        static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#else
        static int windowProc(void* hwnd, int msg, void* wParam, void* lParam);
#endif

    private:
#ifdef _WIN32
        HWND m_hwnd;
        WNDPROC m_originalWndProc;
#else
        void* m_hwnd;
        void* m_originalWndProc;
#endif
        bool m_initialized;
        bool m_dragDropEnabled;
        
        DropCallback m_dropCallback;
        DropCallback m_dragEnterCallback;
        DropCallback m_dragLeaveCallback;
        DropCallback m_dragOverCallback;

#ifdef _WIN32
        void handleDrop(HDROP hDrop, POINT pt, DWORD keyState);
        void handleDragEnter(HDROP hDrop, POINT pt, DWORD keyState);
        void handleDragLeave();
        void handleDragOver(HDROP hDrop, POINT pt, DWORD keyState);
#else
        void handleDrop(void* hDrop, struct { int x, y; } pt, unsigned int keyState);
        void handleDragEnter(void* hDrop, struct { int x, y; } pt, unsigned int keyState);
        void handleDragLeave();
        void handleDragOver(void* hDrop, struct { int x, y; } pt, unsigned int keyState);
#endif
    };

} // namespace DragDrop