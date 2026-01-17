#pragma once

#include <windows.h>
#include <stdint.h>

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
