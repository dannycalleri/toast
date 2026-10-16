/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

#ifndef TOASTERROR_H
#define TOASTERROR_H

namespace Toast
{
	void reportError(const char* msg);

	// logs the error and shows it in a message box,
	// for failures the game cannot continue after
	void fatalError(const char* msg);
}

#define TOAST_ERROR(msg) ::Toast::reportError(msg)

#endif // TOASTERROR_H
