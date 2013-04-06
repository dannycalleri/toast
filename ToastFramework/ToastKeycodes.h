/*
=========================================================================

Toast 2D game framework
Copyright (C) 2013 Danny Calleri

==========================================================================
*/

/*
 *
 *      Keycode constants.
 */


#ifndef TOASTKEYCODES_H
#define TOASTKEYCODES_H



/* Note these values are deliberately the same as in Allegro 4.1.x */
enum
{
   TOAST_KEY_A		= 1,
   TOAST_KEY_B		= 2,
   TOAST_KEY_C		= 3,
   TOAST_KEY_D		= 4,
   TOAST_KEY_E		= 5,
   TOAST_KEY_F		= 6,
   TOAST_KEY_G		= 7,
   TOAST_KEY_H		= 8,
   TOAST_KEY_I		= 9,
   TOAST_KEY_J		= 10,
   TOAST_KEY_K		= 11,
   TOAST_KEY_L		= 12,
   TOAST_KEY_M		= 13,
   TOAST_KEY_N		= 14,
   TOAST_KEY_O		= 15,
   TOAST_KEY_P		= 16,
   TOAST_KEY_Q		= 17,
   TOAST_KEY_R		= 18,
   TOAST_KEY_S		= 19,
   TOAST_KEY_T		= 20,
   TOAST_KEY_U		= 21,
   TOAST_KEY_V		= 22,
   TOAST_KEY_W		= 23,
   TOAST_KEY_X		= 24,
   TOAST_KEY_Y		= 25,
   TOAST_KEY_Z		= 26,

   TOAST_KEY_0		= 27,
   TOAST_KEY_1		= 28,
   TOAST_KEY_2		= 29,
   TOAST_KEY_3		= 30,
   TOAST_KEY_4		= 31,
   TOAST_KEY_5		= 32,
   TOAST_KEY_6		= 33,
   TOAST_KEY_7		= 34,
   TOAST_KEY_8		= 35,
   TOAST_KEY_9		= 36,

   TOAST_KEY_PAD_0		= 37,
   TOAST_KEY_PAD_1		= 38,
   TOAST_KEY_PAD_2		= 39,
   TOAST_KEY_PAD_3		= 40,
   TOAST_KEY_PAD_4		= 41,
   TOAST_KEY_PAD_5		= 42,
   TOAST_KEY_PAD_6		= 43,
   TOAST_KEY_PAD_7		= 44,
   TOAST_KEY_PAD_8		= 45,
   TOAST_KEY_PAD_9		= 46,

   TOAST_KEY_F1		= 47,
   TOAST_KEY_F2		= 48,
   TOAST_KEY_F3		= 49,
   TOAST_KEY_F4		= 50,
   TOAST_KEY_F5		= 51,
   TOAST_KEY_F6		= 52,
   TOAST_KEY_F7		= 53,
   TOAST_KEY_F8		= 54,
   TOAST_KEY_F9		= 55,
   TOAST_KEY_F10		= 56,
   TOAST_KEY_F11		= 57,
   TOAST_KEY_F12		= 58,

   TOAST_KEY_ESCAPE	= 59,
   TOAST_KEY_TILDE		= 60,
   TOAST_KEY_MINUS		= 61,
   TOAST_KEY_EQUALS	= 62,
   TOAST_KEY_BACKSPACE	= 63,
   TOAST_KEY_TAB		= 64,
   TOAST_KEY_OPENBRACE	= 65,
   TOAST_KEY_CLOSEBRACE	= 66,
   TOAST_KEY_ENTER		= 67,
   TOAST_KEY_SEMICOLON	= 68,
   TOAST_KEY_QUOTE		= 69,
   TOAST_KEY_BACKSLASH	= 70,
   TOAST_KEY_BACKSLASH2	= 71, /* DirectInput calls this DIK_OEM_102: "< > | on UK/Germany keyboards" */
   TOAST_KEY_COMMA		= 72,
   TOAST_KEY_FULLSTOP	= 73,
   TOAST_KEY_SLASH		= 74,
   TOAST_KEY_SPACE		= 75,

   TOAST_KEY_INSERT	= 76,
   TOAST_KEY_DELETE	= 77,
   TOAST_KEY_HOME		= 78,
   TOAST_KEY_END		= 79,
   TOAST_KEY_PGUP		= 80,
   TOAST_KEY_PGDN		= 81,
   TOAST_KEY_LEFT		= 82,
   TOAST_KEY_RIGHT		= 83,
   TOAST_KEY_UP		= 84,
   TOAST_KEY_DOWN		= 85,

   TOAST_KEY_PAD_SLASH	= 86,
   TOAST_KEY_PAD_ASTERISK	= 87,
   TOAST_KEY_PAD_MINUS	= 88,
   TOAST_KEY_PAD_PLUS	= 89,
   TOAST_KEY_PAD_DELETE	= 90,
   TOAST_KEY_PAD_ENTER	= 91,

   TOAST_KEY_PRINTSCREEN	= 92,
   TOAST_KEY_PAUSE		= 93,

   TOAST_KEY_ABNT_C1	= 94,
   TOAST_KEY_YEN		= 95,
   TOAST_KEY_KANA		= 96,
   TOAST_KEY_CONVERT	= 97,
   TOAST_KEY_NOCONVERT	= 98,
   TOAST_KEY_AT		= 99,
   TOAST_KEY_CIRCUMFLEX	= 100,
   TOAST_KEY_COLON2	= 101,
   TOAST_KEY_KANJI		= 102,

   TOAST_KEY_PAD_EQUALS	= 103,	/* MacOS X */
   TOAST_KEY_BACKQUOTE	= 104,	/* MacOS X */
   TOAST_KEY_SEMICOLON2	= 105,	/* MacOS X -- TODO: ask lillo what this should be */
   TOAST_KEY_COMMAND	= 106,	/* MacOS X */
   TOAST_KEY_UNKNOWN      = 107,

   /* All codes up to before TOAST_KEY_MODIFIERS can be freely
    * assignedas additional unknown keys, like various multimedia
    * and application keys keyboards may have.
    */

   TOAST_KEY_MODIFIERS	= 215,

   TOAST_KEY_LSHIFT	= 215,
   TOAST_KEY_RSHIFT	= 216,
   TOAST_KEY_LCTRL	= 217,
   TOAST_KEY_RCTRL	= 218,
   TOAST_KEY_ALT		= 219,
   TOAST_KEY_ALTGR	= 220,
   TOAST_KEY_LWIN		= 221,
   TOAST_KEY_RWIN		= 222,
   TOAST_KEY_MENU		= 223,
   TOAST_KEY_SCROLLLOCK = 224,
   TOAST_KEY_NUMLOCK	= 225,
   TOAST_KEY_CAPSLOCK	= 226,

   TOAST_KEY_MAX
};



enum
{
   TOAST_KEYMOD_SHIFT       = 0x00001,
   TOAST_KEYMOD_CTRL        = 0x00002,
   TOAST_KEYMOD_ALT         = 0x00004,
   TOAST_KEYMOD_LWIN        = 0x00008,
   TOAST_KEYMOD_RWIN        = 0x00010,
   TOAST_KEYMOD_MENU        = 0x00020,
   TOAST_KEYMOD_ALTGR       = 0x00040,
   TOAST_KEYMOD_COMMAND     = 0x00080,
   TOAST_KEYMOD_SCROLLLOCK  = 0x00100,
   TOAST_KEYMOD_NUMLOCK     = 0x00200,
   TOAST_KEYMOD_CAPSLOCK    = 0x00400,
   TOAST_KEYMOD_INALTSEQ	 = 0x00800,
   TOAST_KEYMOD_ACCENT1     = 0x01000,
   TOAST_KEYMOD_ACCENT2     = 0x02000,
   TOAST_KEYMOD_ACCENT3     = 0x04000,
   TOAST_KEYMOD_ACCENT4     = 0x08000
};



#endif // TOASTKEYCODES_H
