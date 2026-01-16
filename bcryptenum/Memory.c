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
// Version: 1.1.0
//
// Change history:
//    2026-01-15: V1.0.0: Created.
//    2026-01-15: V1.1.0: Do not overflow buffer list.
//

#include <Windows.h>
#include <stdint.h>


// ******** Private constants ********

/// <summary>
/// Maximum number of allocated buffers tracked.
/// </summary>
#define BUFFER_LIST_SIZE 8


// ******** Private variables ********

/// <summary>
/// List of allocated buffers.
/// </summary>
static void* bufferList[BUFFER_LIST_SIZE];

/// <summary>
/// Count of allocated buffers.
/// </summary>
static uint16_t bufferCount = 0;


// ******** Public functions ********

/// <summary>
/// Allocate a buffer of the given size and track it for later freeing.
/// </summary>
/// <param name="size">Requested size.</param>
/// <returns>Pointer to buffer, or NULL if none could be allocated.</returns>
void* AllocateBuffer(const uint32_t size) {
	if (bufferCount >= BUFFER_LIST_SIZE)
		return NULL;

	void* result = (void*)LocalAlloc(LMEM_FIXED, size);

	bufferList[bufferCount++] = result;

	return result;
}

/// <summary>
/// Free all allocated buffers.
/// </summary>
void FreeBuffers() {
	for (uint16_t i = 0; i < bufferCount; i++) {
		void* actBuffer = bufferList[i];
		if (actBuffer != NULL) {
			LocalFree(actBuffer);
			bufferList[i] = NULL;
		}
	}

	bufferCount = 0;
}
