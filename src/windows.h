/*
	Skelton for retropc emulator

	Qt Version : tanam
	Date   : 2013.05.18 -

 	[for Android]
	Modify : @shikarunochi
	Date   : 2020.06.01-
*/

#ifndef _WINDOWS_H_
#define _WINDOWS_H_

#define _tfopen fopen
#define __assume exit

#undef max
#undef min

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

#define FARPROC int
#define LRESULT int


#endif