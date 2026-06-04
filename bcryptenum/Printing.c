//
// SPDX-FileCopyrightText: Copyright 2026 Frank Schwab
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
// Purpose: Get pseudo-random bytes.
//
// Author: Frank Schwab
//
// Version: 3.0.0
//
// Change history:
//    2026-01-13: V1.0.0: Created.
//    2026-01-15: V2.0.0: Completely reworked to use wide char input and output with code pages.
//    2026-01-15: V3.0.0: Offer byte and wchar functions, make output codepage available.
//

#include <Windows.h>

#include <stdint.h>

#include "Codepage.h"
#include "Strings.h"


// ******** Private variables ********

/// <summary>
/// Handle to standard output.
/// </summary>
HANDLE hStdOut;

/// <summary>
/// Handle to standard error.
/// </summary>
HANDLE hStdErr;

/// <summary>
/// Code page for standard output.
/// </summary>
UINT cpStdOut;

/// <summary>
/// Code page for standard error.
/// </summary>
UINT cpStdErr;

static uint8_t* lowerHexDigits = "0123456789abcdef";
static uint8_t* upperHexDigits = "0123456789ABCDEF";


// ******** Private functions ********

/// <summary>
/// Get the code page for the given output handle.
/// </summary>
/// <param name="hFile">File handle.</param>
/// <param name="consoleCodepage">Console code page.</param>
static UINT GetCodePageForHandle(const HANDLE hFile, const UINT consoleCodepage) {
	DWORD mode;
	// This just checks if the handle is a console handle.
	if (GetConsoleMode(hFile, &mode))
		return consoleCodepage;  // Return console code page, if the handle refers to a console.
	else
		return CP_UTF8;			 // Otherwise, return UTF-8.
}

/// <summary>
/// Print a hexadecimal number to the given output handle.
/// </summary>
/// <param name="hOutput">Output handle.</param>
/// <param name="number">Number to print.</param>
/// <param name="size">Size of number.</param>
/// <param name="hexDigits">List of hex digits.</param>
static void PrintHex(const HANDLE hOutput, uint32_t number, const uint8_t size, const uint8_t* const hexDigits) {
	// Maximum 8 hex digits for uint32_t.
	uint8_t hexBuffer[8];
	uint8_t* p = &hexBuffer[8];  // Initialize pointer to be one past the end of the buffer.
	uint8_t length = 0;

	// Convert number to hex digits (at least one digit)
	do {
		*--p = hexDigits[number & 0x0F];
		number >>= 4;
		length++;
	} while (number != 0);

	// Prepend leading zeros, if necessary.
	while (length < size) {
		*--p = '0';
		length++;
	}

	// Write the string to the output.
	WriteFile(hOutput, p, (DWORD)length, NULL, NULL);
}

/// <summary>
/// Print an integer to the given output handle.
/// </summary>
/// <param name="hOutput">Output handle.</param>
/// <param name="number">Integer to print.</param>
static void PrintUint32(const HANDLE hOutput, const UINT codePage, uint32_t number) {
	uint8_t numberBuffer[10];  // Maximum 10 decimal digits for uint32_t.
	uint8_t* p = &numberBuffer[10]; // Initialize pointer to be one past the end of the buffer.

	// Convert the number to string in reverse order.
	uint8_t length = 0;
	do {
		uint8_t digit = number % 10;
		*--p = ('0' + digit);
		number /= 10;
		length++;
	} while (number != 0);

	// Write the string to the output. This is
	WriteFile(hOutput, p, (DWORD)length, NULL, NULL);
}


// ******** Public functions ********

/// <summary>
/// Initialize the printing system.
/// </summary>
void InitializePrinting() {
	// 1. Get file handles to standard output and standard error.
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	hStdErr = GetStdHandle(STD_ERROR_HANDLE);

	// 2. Get code pages for standard output and standard error.
	UINT consoleCodepage = GetConsoleOutputCP();
	cpStdOut = GetCodePageForHandle(hStdOut, consoleCodepage);
	cpStdErr = GetCodePageForHandle(hStdErr, consoleCodepage);
}

/// <summary>
/// Get the code page for standard output.
/// </summary>
/// <returns></returns>
uint32_t GetCodePageStdOut() {
	return (uint32_t)cpStdOut;
}

/// <summary>
/// Get the code page for standard error.
/// </summary>
/// <returns></returns>
uint32_t GetCodePageStdErr() {
	return (uint32_t)cpStdErr;
}

/// <summary>
/// Print a byte buffer to standard output.
/// </summary>
/// <param name="buffer">Buffer to print.</param>
/// <param name="bufferCount">Count of bytes in buffer.</param>
void PrintByteBufferStdOut(const uint8_t* const buffer, const uint32_t bufferCount) {
	if (bufferCount != 0)
		WriteFile(hStdOut, buffer, (DWORD)bufferCount, NULL, NULL);
}

/// <summary>
/// Print a byte buffer to standard error.
/// </summary>
/// <param name="buffer">Buffer to print.</param>
/// <param name="bufferCount">Count of bytes in buffer.</param>
void PrintByteBufferStdErr(const uint8_t* const buffer, const uint32_t bufferCount) {
	if (bufferCount != 0)
		WriteFile(hStdErr, buffer, (DWORD)bufferCount, NULL, NULL);
}

/// <summary>
/// Print a byte string to standard output.
/// </summary>
/// <param name="str">String to print.</param>
void PrintByteStringStdOut(const uint8_t* const str) {
	PrintByteBufferStdOut(str, ByteStringLength(str));
}

/// <summary>
/// Print a byte string to standard error.
/// </summary>
/// <param name="str">String to print.</param>
void PrintByteStringStdErr(const uint8_t* const str) {
	PrintByteBufferStdErr(str, ByteStringLength(str));
}

/// <summary>
/// Print a wchar string to standard output.
/// </summary>
/// <param name="str">String to print.</param>
void PrintWcharStringStdOut(const wchar_t* const str) {
	WriteWithCodepage(hStdOut, cpStdOut, str, WcharStringLength(str));
}

/// <summary>
/// Print a wchar string to standard error.
/// </summary>
/// <param name="str">String to print.</param>
void PrintWcharStringStdErr(const wchar_t* const str) {
	WriteWithCodepage(hStdErr, cpStdErr, str, WcharStringLength(str));
}

/// <summary>
/// Print a byte to standard output.
/// </summary>
/// <param name="b">Byte to print.</param>
void PrintByteStdOut(const uint8_t b) {
	uint8_t temp[1];
	temp[0] = b;
	WriteFile(hStdOut, temp, 1, NULL, NULL);
}

/// <summary>
/// Print a byte to standard error.
/// </summary>
/// <param name="b">Byte to print.</param>
void PrintByteStdErr(const uint8_t b) {
	uint8_t temp[1];
	temp[0] = b;
	WriteFile(hStdErr, temp, 1, NULL, NULL);
}

/// <summary>
/// Print a hexadecimal number in upper case to standard output.
/// </summary>
/// <param name="number">Number to print.</param>
/// <param name="size">Size of printed number.</param>
void PrintUpperHexStdOut(const uint32_t number, const uint8_t size) {
	PrintHex(hStdOut, number, size, upperHexDigits);
}

/// <summary>
/// Print a hexadecimal number in lower case to standard output.
/// </summary>
/// <param name="number">Number to print.</param>
/// <param name="size">Size of printed number.</param>
void PrintLowerHexStdOut(const uint32_t number, const uint8_t size) {
	PrintHex(hStdOut, number, size, lowerHexDigits);
}

/// <summary>
/// Print a hexadecimal number in upper case to standard error.
/// </summary>
/// <param name="number">Number to print.</param>
/// <param name="size">Size of printed number.</param>
void PrintUpperHexStdErr(const uint32_t number, const uint8_t size) {
	PrintHex(hStdErr, number, size, upperHexDigits);
}

/// <summary>
/// Print a hexadecimal number in lower case to standard error.
/// </summary>
/// <param name="number">Number to print.</param>
/// <param name="size">Size of printed number.</param>
void PrintLowerHexStdErr(const uint32_t number, const uint8_t size) {
	PrintHex(hStdErr, number, size, lowerHexDigits);
}

/// <summary>
/// Print an unsigned 32-bit integer to standard output.
/// </summary>
/// <param name="number">Integer to print.</param>
void PrintUint32StdOut(const uint32_t number) {
	PrintUint32(hStdOut, cpStdOut, number);
}

/// <summary>
/// Print an unsigned 32-bit integer to standard error.
/// </summary>
/// <param name="number">Integer to print.</param>
void PrintUint32StdErr(const uint32_t number) {
	PrintUint32(hStdErr, cpStdErr, number);
}
