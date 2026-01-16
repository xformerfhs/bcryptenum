#pragma once

#include <stdint.h>

/// <summary>
/// Allocate a buffer of the given size and track it for later freeing.
/// </summary>
/// <param name="size">Requested size.</param>
/// <returns>Pointer to buffer, or NULL if none could be allocated.</returns>
void* AllocateBuffer(const uint32_t size);

/// <summary>
/// Free all allocated buffers.
/// </summary>
void FreeBuffers();
