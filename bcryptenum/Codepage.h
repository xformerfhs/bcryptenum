#pragma once

#include <windows.h>
#include <stdint.h>

/// <summary>
/// Copy a wchar buffer to the given codepage in a newly allocated buffer.
/// </summary>
/// <param name="wcharBuffer">Source buffer.</param>
/// <param name="count">Number of characters in the source buffer.</param>
/// <param name="codepage">Result codepage.</param>
/// <returns>Pointer to byte buffer, or NULL.</returns>
uint8_t* CodepageCopy(
   const wchar_t* wcharBuffer,
   const uint32_t count,
   const UINT codepage
);

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
);
