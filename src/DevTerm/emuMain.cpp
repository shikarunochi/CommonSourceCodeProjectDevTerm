/*
/*
	Skelton for retropc emulator

	Author : Takeda.Toshiya
	Date   : 2006.08.18 -
	[ win32 main ]

	emuMain
  	[for DevTerm]
	Modify : @shikarunochi
	Date   : 2022.02.01-
*/

#include<stdio.h>

#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <dirent.h>

#include <vector>
#include<string>

#include <unistd.h>
#include <gtk/gtk.h>


#include "../emu.h"

EMU *emu;
static int64_t start_ms;

DWORD timeGetTime() {
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    //return ts.tv_sec * 1000000L + ts.tv_nsec / 1000;
    //return ts.tv_sec * 1000L + ts.tv_nsec / 1000000;
	//return  (((int64_t) ts.tv_sec) * 1000000000LL + ts.tv_nsec) / 1000000;
	 return ts.tv_sec + (double)ts.tv_nsec*1e-6;
}

int main(int argc,char *argv[]){
	
	// load config
	load_config(create_local_path(_T("%s.ini"), _T(CONFIG_NAME)));
	
	emu = new EMU();
	emu->reset();
	struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    //start_ms = (((int64_t) now.tv_sec) * 1000000000LL + now.tv_nsec) / 1000000;
	start_ms =  now.tv_sec + (double)now.tv_nsec*1e-6;
	int total_frames = 0, draw_frames = 0, skip_frames = 0;
    DWORD next_time = 0;
    bool prev_skip = false;
    DWORD update_fps_time = 0;
    DWORD update_status_bar_time = 0;
    DWORD disable_screen_saver_time = 0;
    bool needDraw = false;
    while (1) {
        if (emu) {
            // drive machine
            int run_frames = emu->run();
            total_frames += run_frames;

            // timing controls
            int sleep_period = 0;
            bool now_skip = (config.full_speed || emu->is_frame_skippable()) &&
                            !emu->is_video_recording() && !emu->is_sound_recording();

            if ((prev_skip && !now_skip) || next_time == 0) {
                next_time = timeGetTime();
            }
            if (!now_skip) {
                static int accum = 0;
                accum += emu->get_frame_interval();
                int interval = accum >> 10;
                accum -= interval << 10;
                next_time += interval;
            }
            prev_skip = now_skip;

            if (next_time > timeGetTime()) {
                // update window if enough time
                draw_frames += emu->draw_screen();
                needDraw = true;
                skip_frames = 0;
                // sleep 1 frame priod if need
                DWORD current_time = timeGetTime();
                if ((int) (next_time - current_time) >= 10) {
                    sleep_period = next_time - current_time;
                }
            } else if (++skip_frames > (int) emu->get_frame_rate()) {
                // update window at least once per 1 sec in virtual machine time
                draw_frames += emu->draw_screen();
                needDraw = true;
                skip_frames = 0;
                next_time = timeGetTime();
            }
            usleep(sleep_period);
        	//SDL_Delay(sleep_period);

            // calc frame rate
            DWORD current_time = timeGetTime();
            if (update_fps_time <= current_time) {
                if (update_fps_time != 0) {
                    if (emu->message_count > 0) {
                        //        SetWindowText(hWnd, create_string(_T("%s - %s"), _T(DEVICE_NAME), emu->message));
                        emu->message_count--;
                    } else if (now_skip) {
                        //        int ratio = (int)(100.0 * (double)total_frames / emu->get_frame_rate() + 0.5);
                        //        SetWindowText(hWnd, create_string(_T("%s - Skip Frames (%d %%)"), _T(DEVICE_NAME), ratio));
                    } else {
                        //       int ratio = (int)(100.0 * (double)draw_frames / (double)total_frames + 0.5);
                        //       SetWindowText(hWnd, create_string(_T("%s - %d fps (%d %%)"), _T(DEVICE_NAME), draw_frames, ratio));
                    }
                    update_fps_time += 1000;
                    total_frames = draw_frames = 0;
                }
                update_fps_time = current_time + 1000;
            }
        	if(emu->is_exit() == true){
        		save_config(create_local_path(_T("%s.ini"), _T(CONFIG_NAME)));
				delete emu;
				emu = NULL;
        		break;
        	}

        }
    }
	
	return 0;	
}