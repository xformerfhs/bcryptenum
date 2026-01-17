//
// SPDX-FileCopyrightText: Copyright 2023-2025 Frank Schwab
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
// Version: 3.1.0
//
// Change history:
//    2023-11-18: V1.0.0: Created.
//    2025-11-12: V2.0.0: Print messages in console code page.
//    2025-12-22: V2.0.1: Corrected casing of function names.
//    2025-12-23: V2.0.2: Disable CRT locks.
//    2026-01-16: V3.0.0: Use own printing subsystem.
//    2026-01-17: V3.0.1: Get module handle for ntdll.dll only once.
//    2026-01-17: V3.1.0: Use system allocated message buffer; Try US English messages first.
//

#include <Windows.h>
#include <bcrypt.h>

#include "Printing.h"


// ******** Private variables ********

/// Module handle of ntdll.dll.
static HMODULE ntdllModuleHandle = NULL;

/// Pointer to message buffer (returned by FormatMessage).
static LPWSTR messageBufferPtr = NULL;


// ******** Private methods ********

/// <summary>
/// Get the module handle for ntdll.dll.
/// </summary>
/// <returns>Module handle for ntdll.dll.</returns>
static HMODULE GetNtdllModuleHandle() {
	if (ntdllModuleHandle == NULL)
		ntdllModuleHandle = GetModuleHandleW(L"ntdll.dll");

	return ntdllModuleHandle;
}

/// <summary>
/// Get the text of a message with various parameters.
/// </summary>
/// <param name="errorNumber">Error number to get the message for.</param>
/// <param name="module">Module handle (NULL for system message).</param>
/// <param name="flag">Where to get the message from.</param>
/// <param name="langId">Language id of message (0 for any message source).</param>
/// <returns>Message length (0, if an error occurred).</returns>
static DWORD TryGetMessage(
	const DWORD errorNumber,
	const LPCVOID module,
	const DWORD flag,
	const DWORD langId
) {
	return FormatMessageW(
		flag | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
		module,                    // Module handle (is NULL for system messages)
		errorNumber,               // Message identifier
		langId,                    // Language identifier
		(LPWSTR)&messageBufferPtr, // Pointer to variable that receives the allocated message buffer pointer
		0,                         // No minimum size for output buffer
		NULL                       // No insert values
	);
}

/// <summary>
/// Get the text for an NTSTATUS.
/// </summary>
/// <param name="errorNumber">NTSTATUS to get the text for.</param>
/// <returns>Length of message text. A value of 0 indicates that no message could be found.</returns>
static DWORD GetNtStatusErrorMessage(const DWORD errorNumber) {
	// The message texts for *all* NTSTATUS codes can only be found in "ntdll.dll"!
	const HMODULE ntdllModule = GetNtdllModuleHandle();

	// First try to get the message in US English.
	DWORD msgLen = TryGetMessage(
		errorNumber,
		ntdllModule,
		FORMAT_MESSAGE_FROM_HMODULE,
		MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US)
	);

	// If that did not work use any possible language.
	if (msgLen == 0)
		msgLen = TryGetMessage(
			errorNumber,
			ntdllModule,
			FORMAT_MESSAGE_FROM_HMODULE,
			0
		);

	return msgLen;
}

/// <summary>
/// Get the text for a Windows error code (GetLastError).
/// </summary>
/// <param name="errorNumber">Error code.</param>
/// <returns>Length of message text. A value of 0 indicates that no message could be found.</returns>
static DWORD GetSystemErrorMessage(const DWORD errorNumber) {
	// First try to get the message in US English.
	DWORD msgLen = TryGetMessage(
		errorNumber,
		NULL,
		FORMAT_MESSAGE_FROM_SYSTEM,
		MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US)
	);

	// If that did not work use any possible language.
	if (msgLen == 0)
		msgLen = TryGetMessage(
			errorNumber,
			NULL,
			FORMAT_MESSAGE_FROM_SYSTEM,
			0
		);

	return msgLen;
}

/// <summary>
/// Print an error message for a specified function and error number.
/// </summary>
/// <param name="functionName">Name of the function calling the failing Windows API function.</param>
/// <param name="apiName">Name of the failing Windows API function.</param>
/// <param name="errorNumber">Error number.</param>
/// <param name="isNtStatus">Is the error number an NTSTATUS.</param>
static void PrintError(const PCHAR functionName, const PCHAR apiName, const DWORD errorNumber, const BOOL isNtStatus) {
	DWORD msgLen;
	if (isNtStatus == FALSE)
		msgLen = GetSystemErrorMessage(errorNumber);
	else
		msgLen = GetNtStatusErrorMessage(errorNumber);

	DWORD le = 0;
	if (msgLen == 0)
		le = GetLastError();

	PrintByteStringStdErr(functionName);
	PrintByteStdErr('!');
	PrintByteStringStdErr(apiName);
	PrintByteBufferStdErr(" failed with error ", 19);
	PrintUint32StdErr(errorNumber);
	PrintByteBufferStdErr(" (0x", 4);
	PrintUpperHexStdErr(errorNumber, 8);
	PrintByteBufferStdErr("): ", 3);

	if (msgLen > 0) {
		PrintWcharStringStdErr(messageBufferPtr);
		LocalFree(messageBufferPtr);
	} else {
		PrintByteBufferStdErr("Could not get error message. FormatMessage error code = ", 56);
		PrintUint32StdErr(le);
		PrintByteBufferStdErr(" (0x", 4);
		PrintUpperHexStdErr(le, 8);
		PrintByteBufferStdErr(")\n", 2);
	}
}


// ******** Public methods ********

/// <summary>
/// Print the error message for a Windows error code.
/// </summary>
/// <param name="functionName">Name of the function calling the failing Windows API function.</param>
/// <param name="apiName">Name of the failing Windows API function.</param>
/// <param name="errorNumber">Error number.</param>
void PrintWinError(const PCHAR functionName, const PCHAR apiName, const DWORD errorNumber) {
	PrintError(functionName, apiName, errorNumber, FALSE);
}

/// <summary>
/// Print the error message for last faining Windows API function (GetLastError).
/// </summary>
/// <param name="functionName">Name of the function calling the failing Windows API function.</param>
/// <param name="apiName">Name of the failing Windows API function.</param>
void PrintLastError(const PCHAR functionName, const PCHAR apiName) {
	PrintWinError(functionName, apiName, GetLastError());
}

/// <summary>
/// Print the error message for an NTSTATUS.
/// </summary>
/// <param name="functionName">Name of the function calling the failing Windows API function.</param>
/// <param name="apiName">Name of the failing Windows API function.</param>
/// <param name="errorStatus">NTSTATUS of failing function.</param>
void PrintNtStatus(const PCHAR functionName, const PCHAR apiName, const NTSTATUS errorStatus) {
	PrintError(functionName, apiName, (DWORD)errorStatus, TRUE);
}
