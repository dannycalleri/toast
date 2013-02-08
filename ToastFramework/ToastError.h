/*
=========================================================================

Toast 2D game framework
Copyright (C)2012 Danny Calleri

Toast 2D game framework is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

==========================================================================
*/

#ifndef TOASTERROR_H
#define TOASTERROR_H

#include <Windows.h>
#include <Winuser.h>

namespace Toast
{

// Check if we are in debug build
#ifdef DEBUG
#define TOAST_DEBUG
#endif

#ifdef _DEBUG
#define TOAST_DEBUG
#endif

#ifdef TOAST_DEBUG
	#define TOAST_ERROR(msg) \
	do \
	{ \
		MessageBox(0, msg, "Toast Error", MB_OK | MB_ICONSTOP | MB_TASKMODAL); \
		{__asm {int 3}} \
	} \
	while(0); 
#else
	#define TOAST_ERROR(msg) {}
#endif

}

#endif // TOASTERROR_H