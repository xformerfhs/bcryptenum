#pragma once

#include <Windows.h>

/// <summary>
/// Function prototype for write functions.
/// </summary>
typedef BOOL(*WRITE_FN)(HANDLE h, const wchar_t* s, DWORD len);

/// <summary>
/// Output stream structure.
/// </summary>
typedef struct {
   HANDLE   handle;
   WRITE_FN write;
} OUT_STREAM;

/// <summary>
/// Initialize the output streams.
/// </summary>
/// <param name="outStream">Pointer to standard output stream.</param>
/// <param name="errStream">Pointer to standard error stream.</param>
void OutStreamInit(OUT_STREAM* outStream, OUT_STREAM* errStream);

/// <summary>
/// Write to an out stream.
/// </summary>
#define OutStreamWrite(os, buf, count) os.write(os.handle, buf, count)

/// <summary>
/// Write to the pointer of an out stream.
/// </summary>
#define OutStreamPointerWrite(os, buf, count) os->write(os->handle, buf, count)
