/*
 * Copyright 2011 Jim Knowler
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */

#include <Windows.h>

// DLL exports
#include "JLibSpotify.h"

// Internal Implementation
#include "Spotify/Session.h"

#include <assert.h>

BOOL APIENTRY DllMain(
HANDLE hModule,	// Handle to DLL module
	DWORD ul_reason_for_call,	// Reason for calling function
	LPVOID lpReserved ) // Reserved
{		
	switch ( ul_reason_for_call )
	{
		case DLL_PROCESS_ATTACH:
		// A process is loading the DLL.
		break;
		case DLL_THREAD_ATTACH:
		// A process is creating a new thread.
		break;
		case DLL_THREAD_DETACH:
		// A thread exits normally.
		break;
		case DLL_PROCESS_DETACH:
		// A process unloads the DLL.
		break;
	}
	return TRUE;
}
