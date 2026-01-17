//
// SPDX-FileCopyrightText: Copyright 2023-2025 Frank Schwab
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
// Version: 2.2.0
//
// Change history:
//    2023-12-01: V1.0.0: Created.
//    2024-12-14: V1.1.0: Sorted algorithm name lists.
//    2025-10-21: V1.2.0: Corrected error message, if heap allocation failed.
//                        Get output file pointer only once.
//    2025-10-21: V1.2.1: Simplified header output.
//    2025-10-21: V1.3.0: Use shellsort, instead of quicksort.
//    2025-10-22: V1.4.0: List all types and collect overall result.
//    2025-11-12: V1.4.1: Removed unnecessary compare in shell sort.
//    2025-11-12: V2.0.0: Print to console in console code page.
//    2025-11-14: V2.1.0: Removed wide character functions.
//    2025-12-22: V2.1.1: Corrected casing of function names.
//    2025-12-22: V2.1.2: More compact list.
//    2025-12-22: V2.1.3: Handle empty algorithm list.
//    2025-12-22: V2.1.4: Simplified list processing.
//    2025-12-23: V2.1.5: Corrected loop counter of list output.
//    2026-01-16: V2.2.0: Use own printing subsystem.
//

#include <Windows.h>
#include <bcrypt.h>

#include "ApiErrorHandler.h"
#include "Printing.h"
#include "PrintModVersion.h"
#include "Strings.h"

// ******** Private methods ********

/// <summary>
/// Sort the list of algorithm names with the shell sort algorithm.
/// </summary>
/// <param name="pAlgorithmNames">Pointer to list of algorithm names.</param>
/// <param name="algorithmCount">Count of algorithm names.</param>
static void ShellSort(LPWSTR* const pAlgorithmNames, const USHORT algorithmCount) {
	USHORT stepSize[] = {7, 4, 1};

	for (USHORT s = 0; s < sizeof(stepSize) / sizeof(stepSize[0]); s++) {
		USHORT step = stepSize[s];

		for (USHORT i = step; i < algorithmCount; i++) {
			LPWSTR insertionName = pAlgorithmNames[i];
			USHORT insertionIndex = i;
			
			while (insertionIndex >= step &&
					 CompareWcharStringsOrdinal(insertionName, pAlgorithmNames[insertionIndex - step]) < 0) {
				pAlgorithmNames[insertionIndex] = pAlgorithmNames[insertionIndex - step];
				insertionIndex -= step;
			}

			// This may be the same as the original position. It is faster to always assign, than to compare first.
			pAlgorithmNames[insertionIndex] = insertionName;
		}
	}
}

/// <summary>
/// Print the type of the elements in the list.
/// </summary>
/// <param name="algorithmType">BCrypt algorithm type.</param>
static void PrintAlgorithmTypeName(const ULONG algorithmType) {
	PrintByteStdOut('\n');

	char* algorithmTypeDescription;

	switch (algorithmType) {
	case BCRYPT_CIPHER_OPERATION:
		algorithmTypeDescription = "Symmetric ciphers";
		break;

	case BCRYPT_HASH_OPERATION:
		algorithmTypeDescription = "Hashes";
		break;

	case BCRYPT_ASYMMETRIC_ENCRYPTION_OPERATION:
		algorithmTypeDescription = "Asymmetric ciphers";
		break;

	case BCRYPT_SECRET_AGREEMENT_OPERATION:
		algorithmTypeDescription = "Secret agreements";
		break;

	case BCRYPT_SIGNATURE_OPERATION:
		algorithmTypeDescription = "Signatures";
		break;

	case BCRYPT_RNG_OPERATION:
		algorithmTypeDescription = "Pseudorandom Number Generators";
		break;

	case BCRYPT_KEY_DERIVATION_OPERATION:
		algorithmTypeDescription = "Key derivation";
		break;

	default:
		PrintByteBufferStdErr("Unknown algorithm type 0x", 25);
		PrintLowerHexStdErr(algorithmType, 2);
		PrintByteStdErr('\n');
		return;
	}

	PrintByteStringStdOut(algorithmTypeDescription);
	PrintByteBufferStdOut(":\n", 2);
}

/// <summary>
/// Copy the pointers to the algorithm names from the BCrypt algorithm list into a local memory area.
/// </summary>
/// <param name="hHeap">Handle of the local heap.</param>
/// <param name="pAlgoList">Pointer to the list of BCrypt algorithm identifiers.</param>
/// <param name="algoCount">Number of algorithms.</param>
/// <returns>Pointer to the local copy of the algorithm name pointers.</returns>
static LPWSTR* CopyAlgorithmNamePointers(
	const HANDLE hHeap, 
	BCRYPT_ALGORITHM_IDENTIFIER* const pAlgoList,
	const ULONG algoCount
) {
	LPWSTR* pNameList = HeapAlloc(hHeap, 0, algoCount * sizeof(LPWSTR));
	if (pNameList == NULL) {
		PrintByteBufferStdErr("CopyAlgorithmNamePointers:HeapAlloc for algorithm name list failed.\n", 68);
		return pNameList;
	}

	// Pointer to algorithm identifier.
	BCRYPT_ALGORITHM_IDENTIFIER* pActAlgo = pAlgoList;
	LPWSTR* pName = pNameList;
	for (ULONG i = algoCount; i > 0; i--)
		*pName++ = pActAlgo++->pszName;

	return pNameList;
}

/// <summary>
/// Print the list of algorithm names for the specified type.
/// </summary>
/// <param name="hHeap">Handle of the local heap.</param>
/// <param name="algorithmType">BCrypt algorithm type.</param>
static BOOL ListForType(const HANDLE hHeap, const ULONG algorithmType) {
	const PCHAR functionName = "ListForType";

	// 1. Print the algorithm type.
	PrintAlgorithmTypeName(algorithmType);

	// 2. Get the list of algorithms of this type.
	ULONG algoCount;
	BCRYPT_ALGORITHM_IDENTIFIER* pAlgoList = NULL;
	NTSTATUS nts = BCryptEnumAlgorithms(algorithmType, &algoCount, &pAlgoList, 0);
	if (nts < 0) {
		PrintNtStatus(functionName, "BCryptEnumAlgorithms", nts);
		return FALSE;
	}

	// 2.1 Check, if any algorithms were found.
	if (algoCount == 0) {
		PrintByteBufferStdOut("   <no algorithms found>\n", 25);
		// It is not necessary to free pAlgoList, since it is NULL if no algorithms were found.
		return TRUE;
	}

	// 3. Sort the algorithm names.

	// 3.1 Copy the pointers to the names into a local memory area that can be sorted.
	
	// Pointer to list of string pointers to algorithm names.
	LPWSTR* pSortedList = CopyAlgorithmNamePointers(hHeap, pAlgoList, algoCount);
	if (pSortedList == NULL) {
		BCryptFreeBuffer(pAlgoList);
		return FALSE;
	}

	// 3.2 Sort the string pointers in the list.
	ShellSort(pSortedList, (USHORT)algoCount);

	// 4. Print the sorted list of names.

	// Pointer to algorithm identifier.
	LPWSTR* pActAlgoName = pSortedList;
	for (ULONG i = 0; i < algoCount; i++) {
		PrintByteBufferStdOut("   ", 3);
		PrintWcharStringStdOut(*pActAlgoName++);
		PrintByteStdOut('\n');
	}

	// 5. Release memory.
	HeapFree(hHeap, 0, pSortedList);  // This must be freed *before* the algorithm list is freed.
	BCryptFreeBuffer(pAlgoList);      // This must be freed *after* the names have been printed.

	return TRUE;
}


// ******** Public methods ********

/// <summary>
/// Print the names of all BCrypt algorithms.
/// </summary>
BOOL ListAllTypes() {
	const PCHAR functionName = "ListAllTypes";

	// 1. Print header.
	PrintByteBufferStdOut("\nList of Bcrypt ", 16);
	PrintModuleVersion("bcrypt.dll");
	PrintByteBufferStdOut(" algorithms by type:\n", 21);
	
	// 2. Get the process heap to use in the list functions.
	HANDLE hHeap = GetProcessHeap();
	if (hHeap == NULL) {
		PrintLastError(functionName, "GetProcessHeap");
		return FALSE;
	}

	// 3. Print lists for each type.
	BOOL result = ListForType(hHeap, BCRYPT_CIPHER_OPERATION);
	result &= ListForType(hHeap, BCRYPT_ASYMMETRIC_ENCRYPTION_OPERATION);
	result &= ListForType(hHeap, BCRYPT_HASH_OPERATION);
	result &= ListForType(hHeap, BCRYPT_SECRET_AGREEMENT_OPERATION);
	result &= ListForType(hHeap, BCRYPT_SIGNATURE_OPERATION);
	result &= ListForType(hHeap, BCRYPT_RNG_OPERATION);
	result &= ListForType(hHeap, BCRYPT_KEY_DERIVATION_OPERATION);

	return result;
}
