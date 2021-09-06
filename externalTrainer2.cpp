#include "pch.h"
#include <iostream>
#include <windows.h>
#include <vector>
#include "proc.h"
#include "mem.h"

int main(){
    
    DWORD procId = 0;
    HANDLE hProcess = NULL;
    uintptr_t moduleBase = 0, localPlayer = 0, healthAddr = 0;

    const int newValue = 1337;
    bool bHealth = false, bAmmo = false, bRecoil = false;
    
    procId = getProcId(L"ac_client.exe");

    if (procId) {
        std::cout << "--------AC trainer by 9tails--------" << std::endl;
        std::cout << "Process found sucessfully with ID: " << procId << std::endl;

        hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
        moduleBase = getModuleBaseAddr(procId, L"ac_client.exe");
        localPlayer = moduleBase + 0x10F4F4;

        if (hProcess != NULL) {
            std::cout << "Process opened. " << std::endl;
        }
        healthAddr = findMAAddyAddy(hProcess, localPlayer, { 0xf8 });

    }
    else {
        std::cout << "Process not found. " << std::endl;
        getchar();
        return 0;
    }

    DWORD dwExit = 0;

    while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE) {
        
        if (GetAsyncKeyState(VK_PRIOR) & 1) {
            bHealth = !bHealth;
        }

        if (GetAsyncKeyState(VK_NEXT) & 1) {
            bAmmo = !bAmmo;
            
            if (bAmmo) {
                mem::PatchEx((BYTE*)(moduleBase + 0x637e9), (BYTE*)"\xFF\x06", 2, hProcess);
            }
            else {
                mem::PatchEx((BYTE*)(moduleBase + 0x637e9), (BYTE*)"\xFF\x0E", 2, hProcess);
            }
        }

        if (GetAsyncKeyState(0x32) & 1) {
            bRecoil = !bRecoil;

            if (bRecoil) {
                mem::NopEx((BYTE*)(moduleBase + 0x63786), 10, hProcess);
            }
            else {
                mem::PatchEx((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10, hProcess);
            }
        }


        if (GetAsyncKeyState(VK_INSERT) & 1) {
            return 0;
        }

        if (bHealth) {
            mem::PatchEx((BYTE*)(healthAddr), (BYTE*)&newValue, sizeof(newValue), hProcess);
        }

        Sleep(10);

    }
    
    std::cout << "process not found. " << std::endl;
    getchar();
    return 0;
}

