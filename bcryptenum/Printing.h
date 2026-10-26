#pragma once

#include <stdint.h>
#include <stdlib.h>

/// <summary>
/// Initialize the printing system.
/// </summary>
void InitializePrinting();

/// <summary>
/// Print a wide char buffer to standard output.
/// </summary>
/// <param name="buffer">Buffer to print.</param>
/// <param name="charCount">Count of characters in buffer.</param>
void PrintWcharBufferStdOut(const wchar_t* const buffer, const uint32_t charCount);

/// <summary>
/// Print a wide char buffer to standard error.
/// </summary>
/// <param name="buffer">Buffer to print.</param>
/// <param name="charCount">Count of characters in buffer.</param>
void PrintWcharBufferStdErr(const wchar_t* const buffer, const uint32_t charCount);

/// <summary>
/// Print a wchar string to standard output.
/// </summary>
/// <param name="str">String to print.</param>
void PrintWcharStringStdOut(const wchar_t* const str);

/// <summary>
/// Print a wchar string to standard error.
/// </summary>
/// <param name="str">String to print.</param>
void PrintWcharStringStdErr(const wchar_t* const str);

/// <summary>
/// Print a wide char to standard output.
/// </summary>
/// <param name="b">Wide char to print.</param>
void PrintWcharStdOut(const wchar_t b);

/// <summary>
/// Print a wide char to standard error.
/// </summary>
/// <param name="b">Wide char to print.</param>
void PrintWcharStdErr(const wchar_t b);

/// <summary>
/// Print a hexadecimal number in upper case to standard output.
/// </summary>
/// <param name="number">Number to print.</param>
/// <param name="size">Size of printed number.</param>
void PrintUpperHexStdOut(const uint32_t number, const uint8_t size);

/// <summary>
/// Print a hexadecimal number in lower case to standard output.
/// </summary>
/// <param name="number">Number to print.</param>
/// <param name="size">Size of printed number.</param>
void PrintLowerHexStdOut(const uint32_t number, const uint8_t size);

/// <summary>
/// Print a hexadecimal number in upper case to standard error.
/// </summary>
/// <param name="number">Number to print.</param>
/// <param name="size">Size of printed number.</param>
void PrintUpperHexStdErr(const uint32_t number, const uint8_t size);

/// <summary>
/// Print a hexadecimal number in lower case to standard error.
/// </summary>
/// <param name="number">Number to print.</param>
/// <param name="size">Size of printed number.</param>
void PrintLowerHexStdErr(const uint32_t number, const uint8_t size);

/// <summary>
/// Print an unsigned 32-bit integer to standard output.
/// </summary>
/// <param name="number">Integer to print.</param>
void PrintUint32StdOut(const uint32_t number);

/// <summary>
/// Print an unsigned 32-bit integer to standard error.
/// </summary>
/// <param name="number">Integer to print.</param>
void PrintUint32StdErr(const uint32_t number);


/// <summary>
/// Print a string literal to the specified stream.
/// </summary>
/// <remarks>
/// This will fail, if "str" is a string pointer instead of a string literal.
/// </remarks>
#define PrintWcharStringLiteral(stream, str) PrintWcharBuffer##stream(str, (const uint32_t)(_countof(str) - 1))

/// <summary>
/// Print a string literal to stdout.
/// </summary>
/// <remarks>
/// This will fail, if "str" is a string pointer instead of a string literal.
/// </remarks>
#define PrintWcharStringLiteralStdOut(str) PrintWcharStringLiteral(StdOut, str)

/// <summary>
/// Print a string literal to stderr.
/// </summary>
/// <remarks>
/// This will fail, if "str" is a string pointer instead of a string literal.
/// </remarks>
#define PrintWcharStringLiteralStdErr(str) PrintWcharStringLiteral(StdErr, str)
