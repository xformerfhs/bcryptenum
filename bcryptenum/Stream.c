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
// Version: 1.0.2
//
// Change history:
//    2026-10-23: V1.0.0: Created.
//    2026-10-26: V1.0.1: Removed unnecessary "const" modifier from parameter declaration.
//    2026-10-26: V1.0.2: Renamed "len" to "charCount".
//

#include <stdlib.h>
#include <Windows.h>

#include "Stream.h"


// ******** Constants ********

#define CHUNK_CHAR_LEN 256
#define CHUNK_BYTE_LEN (CHUNK_CHAR_LEN * 3)


// ******** Private functions ********

/// <summary>
/// Write wide characters to the console.
/// </summary>
/// <param name="h">Console handle.</param>
/// <param name="s">Wide characters to write.</param>
/// <param name="len">Number of characters.</param>
/// <returns>TRUE, if writing worked; false, if not.</returns>
static BOOL WriteToConsole(HANDLE h, const wchar_t* s, DWORD charCount) {
   return WriteConsoleW(h, s, charCount, NULL, NULL);
}

/// <summary>
/// Write wide characters to a file.
/// </summary>
/// <param name="h">File handle.</param>
/// <param name="s">Wide characters to write.</param>
/// <param name="len">Number of characters.</param>
/// <returns>TRUE, if writing worked; false, if not.</returns>
static BOOL WriteToFile(HANDLE h, const wchar_t* s, DWORD charCount) {
   char buf[CHUNK_BYTE_LEN];

   while (charCount > 0) {
      DWORD chunkLen = min(charCount, CHUNK_CHAR_LEN);

      // Ensure we do not split a surrogate pair.
      if (chunkLen < charCount && IS_HIGH_SURROGATE(s[chunkLen - 1]))
         chunkLen--;

      int n = WideCharToMultiByte(CP_UTF8, 0, s, (int)chunkLen, buf, sizeof buf, NULL, NULL);
      if (n <= 0)
         return FALSE;

      // Write to file may need to loop.
      const char* p = buf;
      DWORD rest = (DWORD)n;
      DWORD written;
      while (rest > 0) {
         if (!WriteFile(h, p, rest, &written, NULL) || written == 0)
            return FALSE;
         p += written;
         rest -= written;
      }

      s += chunkLen;
      charCount -= chunkLen;
   }

   return TRUE;
}

/// <summary>
/// Checks if the handle belongs to a console.
/// </summary>
/// <param name="h">Output handle.</param>
/// <returns>TRUE, if handle belongs to a console; FALSE, if not.</returns>
static BOOL IsConsole(HANDLE h) {
   DWORD mode;
   return h != NULL && h != INVALID_HANDLE_VALUE && GetConsoleMode(h, &mode);
}

/// <summary>
/// Initialize an output stream.
/// </summary>
/// <param name="stream"Pointer to output stream.</param>
/// <param name="stdId">Standard id of the handle.</param>
static void InitOutStream(OUT_STREAM* stream, DWORD stdId) {
   stream->handle = GetStdHandle(stdId);
   stream->write = IsConsole(stream->handle) ? WriteToConsole : WriteToFile;
}


// ******** Public functions ********

/// <summary>
/// Initialize the output streams.
/// </summary>
/// <param name="outStream">Pointer to standard output stream.</param>
/// <param name="errStream">Pointer to standard error stream.</param>
void OutStreamInit(OUT_STREAM* outStream, OUT_STREAM* errStream) {
   if (outStream != NULL)
      InitOutStream(outStream, STD_OUTPUT_HANDLE);

   if (errStream != NULL)
      InitOutStream(errStream, STD_ERROR_HANDLE);
}
