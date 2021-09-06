#pragma once

namespace mem {

	void PatchEx(BYTE* dst, BYTE* scr, unsigned int size, HANDLE hProc);

	void NopEx(BYTE* dst, unsigned int size, HANDLE hProc);

}

