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
//    2026-01-14: V2.0.0: Added Utf16StringLength.
//    2026-01-15: V3.0.0: Support byte and wchar strings.
//

#include <stdint.h>

/// <summary>
/// Get the length of a null-terminated string.
/// </summary>
/// <param name="str">String.</param>
/// <returns>Length of string.</returns>
uint32_t ByteStringLength(const uint8_t* const str) {
   const uint8_t* p = str;

   while (*p)
      p++;

   return (uint32_t)(p - str);
}

/// <summary>
/// Get the length of a null-terminated string.
/// </summary>
/// <param name="str">String.</param>
/// <returns>Length of string.</returns>
uint32_t WcharStringLength(const wchar_t* const str) {
   const wchar_t* p = str;

   while (*p)
      p++;

   return (uint32_t)(p - str);
}

/// <summary>
/// Compare two null-terminated wide character strings.
/// </summary>
/// <param name="a">First string.</param>
/// <param name="b">Second string.</param>
/// <returns>< 0, if a < b; 0, if a == b; > 0, if a > b.</returns>
int WcharStringCompare(const wchar_t* a, const wchar_t* b) {
   while (*a && (*a == *b)) {
      a++;
      b++;
   }

   return (int)(*a - *b);
}
