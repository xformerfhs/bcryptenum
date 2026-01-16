#pragma once

#include <stdint.h>

/// <summary>
/// Get the length of a null-terminated string.
/// </summary>
/// <param name="str">String.</param>
/// <returns>Length of string.</returns>
uint32_t ByteStringLength(const uint8_t* const str);

/// <summary>
/// Get the length of a null-terminated string.
/// </summary>
/// <param name="str">String.</param>
/// <returns>Length of string.</returns>
uint32_t WcharStringLength(const wchar_t* const str);

/// <summary>
/// Compare two null-terminated wide character strings.
/// </summary>
/// <param name="a">First string.</param>
/// <param name="b">Second string.</param>
/// <returns>< 0, if a < b; 0, if a == b; > 0, if a > b.</returns>
int WcharStringCompare(const wchar_t* a, const wchar_t* b);
