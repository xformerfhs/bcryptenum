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
// Version: 2.2.0
//
// Change history:
//    2026-01-14: V1.0.0: Created.
//    2026-01-14: V2.0.0: Convert command line correctly to console code page.
//    2026-01-15: V2.1.0: Use memory manager.
//    2026-01-17: V2.2.0: Simplified.
//

#include "Printing.h"
#include "Strings.h"


/// <summary>
/// Main function prototype.
/// </summary>
int __cdecl main();

/// <summary>
/// This is the program entry point. It is jumped to from the OS loader, not called.
/// </summary>
void Startup() {
	// 1. Initialize printing subsystem.
	InitializePrinting();

	// 2. Call main and exit process with its return code. The program does not use commandline arguments.
	ExitProcess(main());
}
