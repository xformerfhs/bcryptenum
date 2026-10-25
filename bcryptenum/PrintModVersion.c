//
// SPDX-FileCopyrightText: Copyright 2024-2026 Frank Schwab
//
// SPDX-License-Identifier: Apache-2.0
//
// SPDX-FileType: SOURCE
//
// Licensed under the Apache License, Version 2.0 (the "License");
// You may not use this file except in compliance with the License.
//
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Author: Frank Schwab
//
// Version: 2.2.0
//
// Change history:
//    2024-11-12: V1.0.0: Created.
//    2024-11-13: V1.0.1: Small change.
//    2026-01-16: V1.0.0: Use own printing subystem.
//    2026-10-23: V2.0.0: Converted to wchar.
//    2026-10-24: V2.1.0: Fixed premature freeing of the version information blob.
//    2026-10-25: V2.2.0: Fixed wrong tests of return values.
//

#include <stdlib.h>
#include <Windows.h>

#include "ApiErrorHandler.h"
#include "Printing.h"


// ******** Public methods ********

/// <summary>
/// Print the version of the supplied module file.
/// </summary>
/// <param name="moduleName">Name of the module.</param>
void PrintModuleVersion(const PWCHAR moduleName) {
	// Name of this function for error messages.
	const PWCHAR functionName = L"PrintModuleVersion";

	// Getting the version of a loaded module is ridiciously complicated.
	// And each step can fail...

	// 1. Get the module handle from the module name.
	HMODULE hModule = GetModuleHandleW(moduleName);
	if (hModule == NULL) {
		PrintLastError(functionName, L"GetModuleHandle");
		return;
	}

	// 2. Get the file name from the module handle.   
	WCHAR fileName[MAX_PATH];
	DWORD fileNameLen = GetModuleFileNameW(hModule, fileName, _countof(fileName));
	if (fileNameLen == 0 || fileNameLen == _countof(fileName)) {
		PrintLastError(functionName, L"GetModuleFileName");
		return;
	}

	// 3. Get the size of the version information for the file.
	DWORD fileVersionInfoSize = GetFileVersionInfoSizeW(fileName, NULL);
	if (fileVersionInfoSize == 0) {
		PrintLastError(functionName, L"GetFileVersionInfoSize");
		return;
	}

	// 4. Allocate memory to store the opaque version information blob.
	HANDLE hHeap = GetProcessHeap();
	if (hHeap == NULL) {
		PrintLastError(functionName, L"GetProcessHeap");
		return;
	}

	// The pointer to the memory that will hold the opaque file version information blob.
	LPVOID pFileVersionInfo = HeapAlloc(hHeap, 0, fileVersionInfoSize);
	if (pFileVersionInfo == NULL) {
		PrintLastError(functionName, L"HeapAlloc");
		return;
	}

	// 5. Get the opaque version information blob.
	//    This is bizarre: The version information has no known structure.
	if (GetFileVersionInfoW(fileName, 0, fileVersionInfoSize, pFileVersionInfo) == FALSE) {
		PrintLastError(functionName, L"GetFileVersionInfo");
		HeapFree(hHeap, 0, pFileVersionInfo);
		return;
	}

	// 6. Copy the version numbers from the opaque version information blob.
	//    The call has a strange and bizarre interface.
	//    "\" means to query the root block of the version information.
	//    Why is this not just some structure?

	// This is where the file version numbers will be copied to.
	// It is part of the data pointed to by pFileVersionInfo.
	VS_FIXEDFILEINFO* pfi;
	UINT fiLength;
	if (VerQueryValueW(pFileVersionInfo, L"\\", &pfi, &fiLength) == FALSE) {
		PrintLastError(functionName, L"VerQueryValue");
		HeapFree(hHeap, 0, pFileVersionInfo);
		return;
	}

	// 7. Hooray! Done! We can print the version information.
	//    But wait... The version is hidden in DWORDs which we have to untangle ourselves.
	//    This is so bizarre...
	PrintWcharStdOut(L'V');
	PrintUint32StdOut((pfi->dwProductVersionMS >> 16) & 0xffff);
	PrintWcharStdOut(L'.');
	PrintUint32StdOut(pfi->dwProductVersionMS & 0xffff);
	PrintWcharStdOut(L'.');
	PrintUint32StdOut((pfi->dwProductVersionLS >> 16) & 0xffff);
	PrintWcharStdOut(L'.');
	PrintUint32StdOut(pfi->dwProductVersionLS & 0xffff);

	// 8. Release the memory of the version information blob after the information
	//    has been printed.
	HeapFree(hHeap, 0, pFileVersionInfo);
}
