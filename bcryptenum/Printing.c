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
// Version: 4.1.0
//
// Change history:
//    2026-01-13: V1.0.0: Created.
//    2026-01-15: V2.0.0: Completely reworked to use wide char input and output with code pages.
//    2026-01-15: V3.0.0: Offer byte and wchar functions, make output codepage available.
//    2026-10-23: V4.0.0: Converted to wchar and streams.
//    2026-10-26: V4.1.0: Corrected writing of error messages to wrong stream.
//

#include <Windows.h>

#include <stdint.h>

#include "Stream.h"
#include "Strings.h"


// ******** Private constants ********

/// <summary>
/// Lower hex digits.
/// </summary>
static wchar_t* lowerHexDigits = L"0123456789abcdef";

/// <summary>
/// Upper hex digits.
/// </summary>
static wchar_t* upperHexDigits = L"0123456789ABCDEF";


// ******** Private variables ********

/// <summary>
/// Standard output stream.
/// </summary>
static OUT_STREAM outStream;

/// <summary>
/// Standard error stream.
/// </summary>
static OUT_STREAM errStream;


// ******** Private functions ********

/// <summary>
/// Print a hexadecimal number to the given output handle.
/// </summary>
/// <param name="stream">Output stream.</param>
/// <param name="number">Number to print.</param>
/// <param name="size">Size of number.</param>
/// <param name="hexDigits">List of hex digits.</param>
static void PrintHex(
	const OUT_STREAM* stream,
	uint32_t number, 
	const uint8_t size, 
	const wchar_t* const hexDigits
) {
	// Maximum 8 hex digits for uint32_t.
	wchar_t hexBuffer[8];
	wchar_t* p = &hexBuffer[8];  // Initialize pointer to be one past the end of the buffer.
	uint8_t length = 0;

	// Convert number to hex digits (at least one digit).
	do {
		*--p = hexDigits[number & 0x0F];
		number >>= 4;
		length++;
	} while (number != 0);

	// Prepend leading zeros, if necessary.
	while (length < size) {
		*--p = L'0';
		length++;
	}

	// Write the string to the output.
	stream->write(stream->handle, p, (DWORD)length);
}

/// <summary>
/// Print an integer to the given output handle.
/// </summary>
/// <param name="stream">Output stream.</param>
/// <param name="number">Integer to print.</param>
static void PrintUint32(const OUT_STREAM* stream, uint32_t number) {
	wchar_t numberBuffer[10];  // Maximum 10 decimal digits for uint32_t.
	wchar_t* p = &numberBuffer[10]; // Initialize pointer to be one past the end of the buffer.

	// Convert the number to string in reverse order.
	uint8_t length = 0;
	do {
		uint16_t digit = number % 10;
		*--p = (L'0' + digit);
		number /= 10;
		length++;
	} while (number != 0);

	// Write the string to the output.
	stream->write(stream->handle, p, (DWORD)length);
}


// ******** Public functions ********

/// <summary>
/// Initialize the printing system.
/// </summary>
void InitializePrinting() {
	OutStreamInit(&outStream, &errStream);
}

/// <summary>
/// Print a wide char buffer to standard output.
/// </summary>
/// <param name="buffer">Buffer to print.</param>
/// <param name="charCount">Count of characters in buffer.</param>
void PrintWcharBufferStdOut(const wchar_t* const buffer, const uint32_t charCount) {
	if (charCount != 0)
		outStream.write(outStream.handle, buffer, charCount);
}

/// <summary>
/// Print a wide char buffer to standard error.
/// </summary>
/// <param name="buffer">Buffer to print.</param>
/// <param name="charCount">Count of characters in buffer.</param>
void PrintWcharBufferStdErr(const wchar_t* const buffer, const uint32_t charCount) {
	if (charCount != 0)
		errStream.write(errStream.handle, buffer, charCount);
}

/// <summary>
/// Print a wchar string to standard output.
/// </summary>
/// <param name="str">String to print.</param>
void PrintWcharStringStdOut(const wchar_t* const str) {
	outStream.write(outStream.handle, str, WcharStringLength(str));
}

/// <summary>
/// Print a wchar string to standard error.
/// </summary>
/// <param name="str">String to print.</param>
void PrintWcharStringStdErr(const wchar_t* const str) {
	errStream.write(errStream.handle, str, WcharStringLength(str));
}

/// <summary>
/// Print a wide char to standard output.
/// </summary>
/// <param name="b">Wide char to print.</param>
void PrintWcharStdOut(const wchar_t b) {
	wchar_t temp[1];
	temp[0] = b;
	outStream.write(outStream.handle, temp, 1);
}

/// <summary>
/// Print a wide char to standard error.
/// </summary>
/// <param name="b">Wide char to print.</param>
void PrintWcharStdErr(const wchar_t b) {
	wchar_t temp[1];
	temp[0] = b;
	errStream.write(errStream.handle, temp, 1);
}

/// <summary>
/// Print a hexadecimal number in upper case to standard output.
/// </summary>
/// <param name="number">Number to print.</param>
/// <param name="size">Size of printed number.</param>
void PrintUpperHexStdOut(const uint32_t number, const uint8_t size) {
	PrintHex(&outStream, number, size, upperHexDigits);
}

/// <summary>
/// Print a hexadecimal number in lower case to standard output.
/// </summary>
/// <param name="number">Number to print.</param>
/// <param name="size">Size of printed number.</param>
void PrintLowerHexStdOut(const uint32_t number, const uint8_t size) {
	PrintHex(&outStream, number, size, lowerHexDigits);
}

/// <summary>
/// Print a hexadecimal number in upper case to standard error.
/// </summary>
/// <param name="number">Number to print.</param>
/// <param name="size">Size of printed number.</param>
void PrintUpperHexStdErr(const uint32_t number, const uint8_t size) {
	PrintHex(&errStream, number, size, upperHexDigits);
}

/// <summary>
/// Print a hexadecimal number in lower case to standard error.
/// </summary>
/// <param name="number">Number to print.</param>
/// <param name="size">Size of printed number.</param>
void PrintLowerHexStdErr(const uint32_t number, const uint8_t size) {
	PrintHex(&errStream, number, size, lowerHexDigits);
}

/// <summary>
/// Print an unsigned 32-bit integer to standard output.
/// </summary>
/// <param name="number">Integer to print.</param>
void PrintUint32StdOut(const uint32_t number) {
	PrintUint32(&outStream, number);
}

/// <summary>
/// Print an unsigned 32-bit integer to standard error.
/// </summary>
/// <param name="number">Integer to print.</param>
void PrintUint32StdErr(const uint32_t number) {
	PrintUint32(&errStream, number);
}
