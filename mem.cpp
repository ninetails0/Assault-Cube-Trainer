#include "pch.h"
#include "mem.h"
#include <windows.h>

void mem::PatchEx(BYTE* dst, BYTE* scr, unsigned int size, HANDLE hProc) {
	DWORD oldProtect;
	VirtualProtectEx(hProc, dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	WriteProcessMemory(hProc, dst, scr, size, nullptr);
	VirtualProtectEx(hProc, dst, size, oldProtect, &oldProtect);
}

void mem::NopEx(BYTE* dst, unsigned int size, HANDLE hProc) {
	
	BYTE* nopArray = new BYTE[size];
	memset(nopArray, 0x90, size);

	PatchEx(dst, nopArray, size, hProc);
	delete[] nopArray;
}
