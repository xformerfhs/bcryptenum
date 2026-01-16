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
// Version: 3.0.0
//
// Change history:
//    2023-11-18: V1.0.0: Created.
//    2025-11-12: V2.0.0: Print messages in console code page.
//    2025-12-22: V2.0.1: Corrected casing of function names.
//    2025-12-23: V2.0.2: Disable CRT locks.
//    2026-01-16: V3.0.0: Use own printing subsystem.
//

#include <Windows.h>
#include <bcrypt.h>

#include "Printing.h"

// ******** Private constants ********

#define MESSAGE_BUFFER_LENGTH 512


// ******** Private variables ********

/// Buffer for wide character error message text.
static WCHAR messageBuffer[MESSAGE_BUFFER_LENGTH];


// ******** Private methods ********

/// <summary>
/// Get the text for an NTSTATUS.
/// </summary>
/// <param name="errorNumber">NTSTATUS to get the text for.</param>
/// <returns>Length of message text. A value of 0 indicates that no message could be found.</returns>
static DWORD GetNtStatusErrorMessage(const DWORD errorNumber) {
   // For *all* NTSTATUS codes it is necessary to look them up in "ntdll.dll"!
   HMODULE ntdllModule = GetModuleHandleW(L"ntdll.dll");
   DWORD msgLen = FormatMessageW(FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 ntdllModule,
                                 errorNumber,
                                 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                                 messageBuffer,
                                 MESSAGE_BUFFER_LENGTH,
                                 NULL);
   return msgLen;
}

/// <summary>
/// Get the text for a Windows error code (GetLastError).
/// </summary>
/// <param name="errorNumber">Error code.</param>
/// <returns>Length of message text. A value of 0 indicates that no message could be found.</returns>
static DWORD GetSystemErrorMessage(const DWORD errorNumber) {
   DWORD msgLen = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL,
                                 errorNumber,
                                 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                                 messageBuffer,
                                 MESSAGE_BUFFER_LENGTH,
                                 NULL);
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
   PrintByteStdErr(':');
   PrintByteStringStdErr(apiName);
	PrintByteBufferStdErr(" failed with error ", 19);
	PrintUint32StdErr(errorNumber);
	PrintByteBufferStdErr(" (0x", 4);
	PrintUpperHexStdErr(errorNumber, 8);
   PrintByteBufferStdErr("): ", 3);

   if (msgLen > 0)
      PrintWcharStringStdErr(messageBuffer);
   else {
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
   PrintError(functionName, apiName, (DWORD) errorStatus, TRUE);
}
