#pragma once

#include <stdint.h>

/// <summary>
/// Initialize the printing system.
/// </summary>
void InitializePrinting();

/// <summary>
/// Get the code page for standard output.
/// </summary>
/// <returns></returns>
uint32_t GetCodePageStdOut();

/// <summary>
/// Get the code page for standard error.
/// </summary>
/// <returns></returns>
uint32_t GetCodePageStdErr();

/// <summary>
/// Print a byte buffer to standard output.
/// </summary>
/// <param name="buffer">Buffer to print.</param>
/// <param name="bufferCount">Count of bytes in buffer.</param>
void PrintByteBufferStdOut(const uint8_t* const buffer, const uint32_t bufferCount);

/// <summary>
/// Print a byte buffer to standard error.
/// </summary>
/// <param name="buffer">Buffer to print.</param>
/// <param name="bufferCount">Count of bytes in buffer.</param>
void PrintByteBufferStdErr(const uint8_t* const buffer, const uint32_t bufferCount);

/// <summary>
/// Print a byte string to standard output.
/// </summary>
/// <param name="str">String to print.</param>
void PrintByteStringStdOut(const uint8_t* const str);

/// <summary>
/// Print a byte string to standard error.
/// </summary>
/// <param name="str">String to print.</param>
void PrintByteStringStdErr(const uint8_t* const str);

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
/// Print a byte to standard output.
/// </summary>
/// <param name="b">Byte to print.</param>
void PrintByteStdOut(const uint8_t b);

/// <summary>
/// Print a byte to standard error.
/// </summary>
/// <param name="b">Byte to print.</param>
void PrintByteStdErr(const uint8_t b);
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
#define PrintByteStringLiteral(stream, str) PrintByteBuffer##stream(str, (const uint32_t)(sizeof(str) - 1))

/// <summary>
/// Print a string literal to stdout.
/// </summary>
/// <remarks>
/// This will fail, if "str" is a string pointer instead of a string literal.
/// </remarks>
#define PrintByteStringLiteralStdOut(str) PrintByteStringLiteral(StdOut, str)

/// <summary>
/// Print a string literal to stderr.
/// </summary>
/// <remarks>
/// This will fail, if "str" is a string pointer instead of a string literal.
/// </remarks>
#define PrintByteStringLiteralStdErr(str) PrintByteStringLiteral(StdErr, str)
