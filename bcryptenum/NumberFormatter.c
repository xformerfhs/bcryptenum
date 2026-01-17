//
// SPDX-FileCopyrightText: Copyright 2025 Frank Schwab
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
// Version: 1.1.0
//
// Change history:
//    2025-11-13: V1.0.0: Created.
//    2025-12-22: V1.0.1: Corrected function naming.
//    2025-12-22: V1.1.0: Removed unnecessary "fast" divmod function which in reality is much slower than normal divmod.
//

#include <stdint.h>


// ******** Private constants ********

/// Default thousands group separator character.
#define DEFAULT_SEPARATOR ','

/// Size of the buffer for formatting numbers with thousands grouping separators.
#define BUFFER_SIZE 20


// ******** Private variables ********

/// Buffer for formatting numbers with thousands grouping separators.
char numberBuffer[BUFFER_SIZE];


// ******** Public methods ********

/// <summary>
/// Print an unsigned 16-bit integer with thousands groups.
/// </summary>
/// <param name="aNumber">Number to proint.</param>
/// <param name="separator">Thousands group separator character.</param>
/// <returns>Pointer to string of number.</returns>
char* FormatUint16WithSeparator(uint16_t const aNumber, char const separator) {
	uint16_t last = aNumber;
	char* pBuffer = &numberBuffer[BUFFER_SIZE - 1];  // Start at the end of the buffer.
	*pBuffer-- = 0;  // Set terminating zero.

	uint8_t groupCount = 0;
	do {
		if (groupCount >= 3) {
			*pBuffer-- = separator;
			groupCount = 0;
		}

		uint16_t div10 = last / 10;
		uint16_t mod10 = last % 10;

		*pBuffer-- = (char)mod10 + '0';
		groupCount++;
		last = div10;
	} while (last != 0);

	return ++pBuffer;
}

/// <summary>
/// Print an unsigned 16-bit integer with thousands groups separated by the default separator character.
/// </summary>
/// <param name="aNumber">Number to proint.</param>
/// <returns>Pointer to string of number.</returns>
char* FormatUint16NumberWithDefaultSeparator(const uint16_t aNumber) {
	return FormatUint16WithSeparator(aNumber, DEFAULT_SEPARATOR);
}

/// <summary>
/// Print an unsigned 16-bit integer.
/// </summary>
/// <param name="aNumber">Number to proint.</param>
/// <returns>Pointer to string of number.</returns>
char* FormatUint16Number(const uint16_t aNumber) {
	uint16_t last = aNumber;
	char* pBuffer = &numberBuffer[BUFFER_SIZE - 1];  // Start at the end of the buffer.
	*pBuffer-- = 0;  // Set terminating zero.

	do {
		 uint16_t div10 = last / 10;
		 uint16_t mod10 = last % 10;

		*pBuffer-- = (char)mod10 + '0';
		last = div10;
	} while (last != 0);

	return ++pBuffer;
}
