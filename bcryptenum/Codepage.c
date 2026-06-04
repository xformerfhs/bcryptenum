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
// Author: Frank Schwab
//
// Version: 2.0.0
//
// Change history:
//    2026-01-15: V1.0.0: Created.
//    2026-01-16: V2.0.0: Scan wchar buffer only once for CodepageCopy.
//

#include <Windows.h>
#include <stdint.h>

#include "Strings.h"


// ******** Private constants ********

/// <summary>
/// Maximum number of wchars to convert in one chunk.
/// </summary>
#define MAX_WCHAR_CHUNK_COUNT 256

/// <summary>
/// Maximum number of bytes per wchar.
/// </summary>
#define MAX_BYTES_PER_WCHAR 5

/// <summary>
/// Size of byte buffer for conversion.
/// </summary>
#define BYTE_BUFFER_SIZE (MAX_WCHAR_CHUNK_COUNT * MAX_BYTES_PER_WCHAR)


// ******** Private variables ********

/// <summary>
/// Byte buffer for conversion.
/// </summary>
static uint8_t byteBuffer[BYTE_BUFFER_SIZE];


// ******** Public functions ********

/// <summary>
/// Write a wchar buffer to the given output handle using the given codepage.
/// </summary>
/// <param name="hOut">File handle of output file.</param>
/// <param name="codepage">Codepage for output.</param>
/// <param name="wcharBuffer">Buffer of wchars to write.</param>
/// <param name="count">Count of wchar characters in the wchar buffer.</param>
void WriteWithCodepage(
	const HANDLE hOut,
	const UINT codepage,
	const wchar_t* wcharBuffer,
	const uint32_t count
) {
	// This works by using a pointer into the wchar buffer and converting chunks of wchars.
	const wchar_t* p = wcharBuffer;
	uint32_t remaining = count;

	while (remaining > 0) {
	  // 1. Write at most MAX_WCHAR_CHUNK_COUNT wchars at a time.
		uint32_t chunkCount = remaining;
		if (chunkCount > MAX_WCHAR_CHUNK_COUNT)
			chunkCount = MAX_WCHAR_CHUNK_COUNT;

	  // 2. Ensure we do not split a surrogate pair.
		if (chunkCount > 1 && IS_HIGH_SURROGATE(p[chunkCount - 1]))
			chunkCount--;

	  // 3. Convert the chunk to the target codepage.
		int byteLen = WideCharToMultiByte(
			codepage,
			0,  // Substitute characters not present in the target codepage, no error on invalid chars, no composite checks.
			p,
			chunkCount,
			byteBuffer,
			sizeof(byteBuffer),
			NULL,
			NULL
		);

	  // 4. If that did not work, stop.
		if (byteLen <= 0)
			break;

	  // 5. Write the converted bytes to the output.
		if (WriteFile(hOut, byteBuffer, (DWORD)byteLen, NULL, NULL) == FALSE)
			break;

	  // 6. Advance the pointer and decrease the count.
		p += chunkCount;
		remaining -= chunkCount;
	}
}
