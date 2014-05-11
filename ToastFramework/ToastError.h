/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef TOASTERROR_H
#define TOASTERROR_H

#ifdef WIN32
#include <Windows.h>
#include <Winuser.h>
#endif

namespace Toast
{

#ifdef WIN32

// Check if we are in debug build
#ifdef DEBUG
#define TOAST_DEBUG
#endif

#ifdef _DEBUG
#define TOAST_DEBUG
#endif

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