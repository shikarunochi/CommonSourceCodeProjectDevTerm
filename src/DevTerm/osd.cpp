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

