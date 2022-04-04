/*
	Skelton for retropc emulator

	Author : Takeda.Toshiya
	Date   : 2015.11.20-

	[ win32 dependent ]

  	[for Android]
	Modify : @shikarunochi
	Date   : 2020.06.01-

  	[for DevTerm]
	Modify : @shikarunochi
	Date   : 2022.02.01-

*/

#include "osd.h"
#include <sys/types.h>
#include <dirent.h>
#include <gtk/gtk.h>


void OSD::initialize(int rate, int samples)
{
	appQuit = false;
	initialize_input();
	initialize_screen();
	initialize_sound(rate, samples);
	
		
#ifdef USE_FLOPPY_DISK
	fd_status = 0x80000000;
#endif
#ifdef USE_QUICK_DISK
	qd_status = 0x80000000;
#endif
#ifdef USE_HARD_DISK
	hd_status = 0x80000000;
#endif
#ifdef USE_COMPACT_DISC
	cd_status = 0x80000000;
#endif
#ifdef USE_LASER_DISC
	ld_status = 0x80000000;
#endif
#if defined(USE_TAPE) && !defined(TAPE_BINARY_ONLY)
	my_tcscpy_s(tape_status, array_length(tape_status), _T("uninitialized"));
#endif	
}

void OSD::release()
{
	printf("Release OSD\n");
	release_input();
	release_screen();
	release_sound();
}

void OSD::power_off()
{
}

void OSD::suspend()
{
}

void OSD::restore()
{
}

void OSD::lock_vm()
{
	lock_count++;
}

void OSD::unlock_vm()
{
	if(--lock_count <= 0) {
		force_unlock_vm();
	}
}

void OSD::force_unlock_vm()
{
	lock_count = 0;
}

void OSD::sleep(uint32_t ms)
{
	//Sleep(ms);
	//sleep(ms);
}

void convertUTF8(char *src,char *desc,int length);

#ifdef USE_DEBUGGER

void OSD::start_waiting_in_debugger()
{
}

void OSD::finish_waiting_in_debugger()
{
}

void OSD::process_waiting_in_debugger()
{
}

#endif

