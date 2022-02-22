#define PSAPI_VERSION 2
#include <processthreadsapi.h>
#include <psapi.h>
#include <stdio.h>
#include <windows.h>

int gEngineInitOffset = 0x227F5A;
BYTE value[] = {0xE8, 0x91, 0x3F, 0x31, 0x03, 0x90};

DWORD_PTR GetProcessBaseAddress(DWORD processID) {
  DWORD_PTR baseAddress = 0;
  HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
  HMODULE *moduleArray;
  LPBYTE moduleArrayBytes;
  DWORD bytesRequired;

  if (processHandle) {
    if (EnumProcessModules(processHandle, NULL, 0, &bytesRequired)) {
      if (bytesRequired) {
        moduleArrayBytes = (LPBYTE)LocalAlloc(LPTR, bytesRequired);

        if (moduleArrayBytes) {
          unsigned int moduleCount;

          moduleCount = bytesRequired / sizeof(HMODULE);
          moduleArray = (HMODULE *)moduleArrayBytes;

          if (EnumProcessModules(processHandle, moduleArray, bytesRequired,
                                 &bytesRequired)) {
            baseAddress = (DWORD_PTR)moduleArray[0];
          }

          LocalFree(moduleArrayBytes);
        }
      }
    }

    CloseHandle(processHandle);
  }

  return baseAddress;
}

void Payload() {
  DWORD pid = GetCurrentProcessId();
  if (pid) {
    OutputDebugString("pid:");
    char pidStr[63];
    sprintf(pidStr, "%d", pid);
    OutputDebugString(pidStr);
    HMODULE baseAddress = GetProcessBaseAddress(pid);
    char baseAddressStr[63];
    sprintf(baseAddressStr, "%p", baseAddress);
    OutputDebugString(baseAddressStr);

    uintptr_t *p = (uintptr_t *)((uintptr_t)baseAddress + gEngineInitOffset);

    HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    WriteProcessMemory(handle, p, &value, sizeof(value), 0);
    CloseHandle(handle);

    // MessageBoxA(NULL, "Address patched", "Success", 0);
  } else {
    // MessageBoxA(NULL, "PID NOT FOUND", "Error", 0);
  }
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
  switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
      Payload();
      break;
    case DLL_THREAD_ATTACH:
      break;
    case DLL_THREAD_DETACH:
      break;
    case DLL_PROCESS_DETACH:
      break;
  }
  return TRUE;
}
// E8913F310390
// 140227F5A original address
// 227F5A
// 14353bef0
// 353BEF0
