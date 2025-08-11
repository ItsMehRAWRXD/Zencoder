#pragma once

#ifdef _KERNEL_MODE
#include <ntddk.h>
#include <wdm.h>
#else
#include <windows.h>
#endif

// Driver communication IOCTLs
#define IOCTL_INSTALL_HOOK CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_REMOVE_HOOK  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)

// Hook data structures
typedef struct _HOOK_INSTALL_DATA {
    PVOID TargetAddress;
    PVOID HookFunction;
    PVOID TrampolineOut;
} HOOK_INSTALL_DATA, *PHOOK_INSTALL_DATA;

typedef struct _HOOK_REMOVE_DATA {
    PVOID TargetAddress;
} HOOK_REMOVE_DATA, *PHOOK_REMOVE_DATA;

// Dispatch routines
NTSTATUS DispatchCreate(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS DispatchClose(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS DispatchDeviceControl(PDEVICE_OBJECT DeviceObject, PIRP Irp);
VOID DriverUnload(PDRIVER_OBJECT DriverObject);

// Hook management functions
NTSTATUS InstallHook(PHOOK_INSTALL_DATA HookData);
NTSTATUS RemoveHook(PHOOK_REMOVE_DATA HookData);