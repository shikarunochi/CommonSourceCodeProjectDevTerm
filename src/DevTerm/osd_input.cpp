/*
	Skelton for retropc emulator

	Author : Takeda.Toshiya
	Date   : 2015.11.26-

	[ win32 input ]

  	[for Android]
	Modify : @shikarunochi
	Date   : 2020.06.01-

  	[for DevTerm]
	Modify : @shikarunochi
	Date   : 2022.02.01-

*/

#include "osd.h"
#include "../fifo.h"
#include <SDL.h>
#include "DevTermKeyCode.h"
#include "../emu.h"

void OSD::initialize_input()
{	// initialize status
	memset(key_status, 0, sizeof(key_status));
#ifdef USE_JOYSTICK
	memset(joy_status, 0, sizeof(joy_status));
	memset(joy_to_key_status, 0, sizeof(joy_to_key_status));
#endif
#ifdef USE_MOUSE
	memset(mouse_status, 0, sizeof(mouse_status));
#endif
	key_shift_pressed = key_shift_released = false;
	key_caps_locked = false;

	lost_focus = false;
	
#ifdef USE_JOYSTICK
	//http://sdl2referencejp.osdn.jp/SDL_JoystickOpen.html
	// initialize joysticks
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);

	// ジョイスティックが存在するかチェックする
	if (SDL_NumJoysticks() > 0) {
	    // ジョイスティックを開く
	    joy = SDL_JoystickOpen(0);
	
	    if (joy) {
        	printf("Open JoyStick 0\n");
        	printf("名前: %s\n", SDL_JoystickNameForIndex(0));
        	printf("軸の数: %d\n", SDL_JoystickNumAxes(joy));
        	printf("ボタンの数: %d\n", SDL_JoystickNumButtons(joy));
        	printf("ボールの数: %d\n", SDL_JoystickNumBalls(joy));
    	} else {
	        printf("Can't open JoySitck 0\n");
    	}
	}
#endif	
	
	//機種ごとのカスタマイズ
#if defined(_MZ700) || defined(_MZ80K)||defined(_MZ1200)|| defined(_MZ1500)
	DevTermKeyCode[0x8] = VK_DELETE; //BS → DEL
	DevTermKeyCode[0x1b] = VK_BACK; //ESC→BS(=BREAK)
#endif
#if defined(_MZ80K)||defined(_MZ1200)
	DevTermKeyCode[0x60] = VK_OEM_3; //'→=
#endif
	
}

void OSD::release_input()
{
#ifdef USE_JOYSTICK
   if (SDL_JoystickGetAttached(joy)) {
        SDL_JoystickClose(joy);
    }
#endif
}

void OSD::update_input()
{
	SDL_Event ev;
	SDL_PollEvent(&ev);
	int keyCode = ev.key.keysym.sym;

	
#ifdef USE_JOYSTICK
	//memset(joy_status, 0, sizeof(joy_status));
#endif
	bool resetJoyPad = true;
	switch (ev.type) {
	case SDL_QUIT:
		appQuit = true;
		break;
	case SDL_KEYDOWN:
		if(keyCode != 0){
			int vmKeyCode = 0;
			if(keyCode < 0xff){
				vmKeyCode = DevTermKeyCode[keyCode];
			}else{
				vmKeyCode = getFuncVmKeyCode(keyCode);
			}
			if(vmKeyCode > 0){
				key_down_native(vmKeyCode,false);
			}
			//printf("keyDown:%x:%x\n",keyCode,vmKeyCode);
		}
		break;
	case SDL_KEYUP:
		if(keyCode != 0){
			int vmKeyCode = 0;
			if(keyCode < 0xff){
				vmKeyCode = DevTermKeyCode[keyCode];
			}else{
				vmKeyCode = getFuncVmKeyCode(keyCode);
			}
			if(vmKeyCode > 0){
				key_up_native(vmKeyCode);
			}
			//printf("keyUp:%x:%x\n",keyCode,vmKeyCode);
		}
		break;
		
#ifdef USE_JOYSTICK
	//十字キー
		//http://lesson.ifdef.jp/C02.html
		case SDL_JOYAXISMOTION:
			//左右の動き

			if(ev.jaxis.axis == 0) 
			{
				if(ev.jaxis.value < -0x7000){
					joy_status[0] |= 0x00000004;	// left
					resetJoyPad = false;
				}else if(ev.jaxis.value >  0x7000){
					joy_status[0] |= 0x00000008;	// right
					resetJoyPad = false;
				}
			}
				//上下の動き
			if(ev.jaxis.axis == 1) 
			{
				if(ev.jaxis.value < -0x7000){
					joy_status[0] |= 0x00000001;	// up
					resetJoyPad = false;
				}else if(ev.jaxis.value >  0x7000){
					joy_status[0] |= 0x00000002;	// down
					resetJoyPad = false;
				}
			}
			if(resetJoyPad == true){
				joy_status[0] &= ~0x0000000F;	// 方向キーリセット
			}
			break;
/*
		case SDL_JOYHATMOTION:
			switch(ev.jhat.value)
			{
				case SDL_HAT_UP:
					printf("UP\n");
					break;
				case SDL_HAT_LEFT:
					printf("LEFT\n");
					break;
				case SDL_HAT_DOWN:
					printf("DOWN\n");
					break;
				case SDL_HAT_RIGHT:
					printf("RIGHT\n");
					break;
			}
		break;
*/
		//ボタン
		case SDL_JOYBUTTONDOWN:
		    //printf("joyButtonDown:%d\n",ev.jbutton.button);
			joy_status[0] |= 1 << (ev.jbutton.button + 4);
		
		break;
		case SDL_JOYBUTTONUP:
			//printf("joyButtonUp:%d\n",ev.jbutton.button);
		joy_status[0] &= ~(1 << (ev.jbutton.button + 4));
		break;
#endif		
	}	
	if(lost_focus) {
		// we lost key focus so release all pressed keys
		for(int i = 0; i < 256; i++) {
			if(key_status[i] & 0x80) {
				key_status[i] &= 0x7f;
				if(!key_status[i]) {
					vm->key_up(i);
				}
			}
		}
	} else {
		for(int i = 0; i < 256; i++) {
			if(key_status[i] & 0x7f) {
				key_status[i] = (key_status[i] & 0x80) | ((key_status[i] & 0x7f) - 1);
				if(!key_status[i]) {
					vm->key_up(i);
				}
			}
		}
	}
	lost_focus = false;

#ifdef USE_JOYSTICK

	if(config.use_joy_to_key) {
		int status[256] = {0};
		if(config.joy_to_key_type == 0) {
			// cursor key
			static const int vk[] = {VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT};
			for(int i = 0; i < 4; i++) {
				if(joy_status[0] & (1 << i)) {
					status[vk[i]] = 1;
				}
			}
		} else if(config.joy_to_key_type == 1) {
			// numpad key (4-directions)
			static const int vk[] = {VK_NUMPAD8, VK_NUMPAD2, VK_NUMPAD4, VK_NUMPAD6};
			for(int i = 0; i < 4; i++) {
				if(joy_status[0] & (1 << i)) {
					status[vk[i]] = 1;
				}
			}
		} else if(config.joy_to_key_type == 2) {
			// numpad key (8-directions)
			switch(joy_status[0] & 0x0f) {
			case 0x02 + 0x04: status[VK_NUMPAD1] = 1; break; // down-left
			case 0x02       : status[VK_NUMPAD2] = 1; break; // down
			case 0x02 + 0x08: status[VK_NUMPAD3] = 1; break; // down-right
			case 0x00 + 0x04: status[VK_NUMPAD4] = 1; break; // left
//			case 0x00       : status[VK_NUMPAD5] = 1; break;
			case 0x00 + 0x08: status[VK_NUMPAD6] = 1; break; // right
			case 0x01 + 0x04: status[VK_NUMPAD7] = 1; break; // up-left
			case 0x01       : status[VK_NUMPAD8] = 1; break; // up
			case 0x01 + 0x08: status[VK_NUMPAD9] = 1; break; // up-right
			}
		}
		if(config.joy_to_key_type == 1 || config.joy_to_key_type == 2) {
			// numpad key
			if(config.joy_to_key_numpad5 && !(joy_status[0] & 0x0f)) {
				status[VK_NUMPAD5] = 1;
			}
		}
		for(int i = 0; i < 16; i++) {
			if(joy_status[0] & (1 << (i + 4))) {
				if(config.joy_to_key_buttons[i] < 0 && -config.joy_to_key_buttons[i] < 256) {
					status[-config.joy_to_key_buttons[i]] = 1;
				}
			}
		}
		for(int i = 0; i < 256; i++) {
			if(status[i]) {
				if(!joy_to_key_status[i]) {
					if(!(key_status[i] & 0x80)) {
						key_down_native(i, false);
						// do not keep key pressed
						if(config.joy_to_key_numpad5 && (i >= VK_NUMPAD1 && i <= VK_NUMPAD9)) {
							key_status[i] = KEY_KEEP_FRAMES;
						}
					}
					joy_to_key_status[i] = true;
				}
			} else {
				if(joy_to_key_status[i]) {
					if(key_status[i]) {
						key_up_native(i);
					}
					joy_to_key_status[i] = false;
				}
			}
		}
	}
#endif		

	// VK_$00 should be 0
	key_status[0] = 0;
}

void OSD::key_down(int code, bool extended, bool repeat)
{
	key_down_native(code, repeat);
}

void OSD::key_up(int code, bool extended)
{
	key_up_native(code);
}

void OSD::key_down_native(int code, bool repeat)
{

	uint8_t prev_shift = key_status[VK_SHIFT];
	uint8_t prev_control = key_status[VK_CONTROL];
	uint8_t prev_menu = key_status[VK_MENU];

	if(code == VK_LSHIFT || code == VK_RSHIFT) {
		if(prev_shift == 0) {
			vm->key_down(VK_SHIFT, repeat);
		}
	} else if(code == VK_LCONTROL|| code == VK_RCONTROL) {
		if(prev_control == 0) {
			key_status[code] = 0x80;
			vm->key_down(VK_CONTROL, repeat);
		}
	} else if(code == VK_LMENU|| code == VK_RMENU) {
		if(prev_menu == 0) {
			key_status[code] = 0x80;
			vm->key_down(VK_MENU, repeat);
		}
	}
	
	if(key_status[VK_MENU] == 0x80){ //VK_MENU = ALT KEY
		if(code == VK_RETURN){
			toggleWindowFullscreen();
			return;
		}
		if(code == 0x51){ //Q
			EMU* emu = vm->getEmu();
			emu->request_exit();
			return;
		}
		
		#ifdef USE_FLOPPY_DISK
		if(code == 0x31){ //1
			SDL_PauseAudio(1);
			selectFile(FLOPPY_DISK_1);
			SDL_PauseAudio(0);
			return;
		}
		if(code == 0x32){ //2
			SDL_PauseAudio(1);
			selectFile(FLOPPY_DISK_2);
			SDL_PauseAudio(0);
			return;
		}
		#endif
		
		#ifdef USE_TAPE 
		if(code == 0x33){ //3
			SDL_PauseAudio(1);
			selectFile(CASETTE_TAPE);
			SDL_PauseAudio(0);
			return;
		}
		#endif

		#ifdef USE_CART
		if(code == 0x34){ //4
			SDL_PauseAudio(1);
			selectFile(CARTRIDGE);
			SDL_PauseAudio(0);
			return;
		}
		#endif

		#ifdef USE_QUICK_DISK
		if(code == 0x35){ //5
			SDL_PauseAudio(1);
			selectFile(QUICK_DISK);
			SDL_PauseAudio(0);
			return;
		}
		#endif
		
		if(code == 0x52){ //R
			vm->reset();
			return;
		}
		if(code == 0x46){ //F
			SDL_PauseAudio(1);
			fileSelectDialog();
			SDL_PauseAudio(0);
			return;
		}
		
		if(code == 0x50){ //P
			EMU* emu = vm->getEmu();
			emu->switchPCG();
			return;
		}
	}
	
	key_status[code] = 0x80;
    key_status[VK_SHIFT] = key_status[VK_LSHIFT] | key_status[VK_RSHIFT];
	key_status[VK_CONTROL] = key_status[VK_LCONTROL] | key_status[VK_RCONTROL];
	key_status[VK_MENU] = key_status[VK_LMENU] | key_status[VK_RMENU];
	
	vm->key_down(code, repeat);

}

void OSD::key_up_native(int code)
{
	if(key_status[code] == 0) {
		return;
	}
	if((key_status[code] &= 0x7f) != 0) {
		return;
	}
	vm->key_up(code);

	uint8_t prev_shift = key_status[VK_SHIFT];
	uint8_t prev_control = key_status[VK_CONTROL];
	uint8_t prev_menu = key_status[VK_MENU];

	key_status[VK_SHIFT] = key_status[VK_LSHIFT] | key_status[VK_RSHIFT];
	key_status[VK_CONTROL] = key_status[VK_LCONTROL] | key_status[VK_RCONTROL];
	key_status[VK_MENU] = key_status[VK_LMENU] | key_status[VK_RMENU];

	if(code == VK_LSHIFT || code == VK_RSHIFT) {
		if(prev_shift != 0 ) {
			vm->key_up(VK_SHIFT);
		}
	} else if(code == VK_LCONTROL|| code == VK_RCONTROL) {
		if(prev_control != 0) {
			vm->key_up(VK_CONTROL);
		}
	} else if(code == VK_LMENU || code == VK_RMENU) {
		if(prev_menu != 0) {
			vm->key_up(VK_MENU);
		}
	}
}


unsigned int OSD::getFuncVmKeyCode(int keyCode){
	int checkKeyCodeIndex = 0;
	while(1){
		if(DevTermFuncKeyCode[checkKeyCodeIndex][0] == keyCode){
			return DevTermFuncKeyCode[checkKeyCodeIndex][1];
		}
		checkKeyCodeIndex++;
		if(DevTermFuncKeyCode[checkKeyCodeIndex][0] == 0){
			break;
		}
	}
	return 0;
}


#ifdef USE_MOUSE
void OSD::enable_mouse()
{
}

void OSD::disable_mouse()
{
}

void OSD::toggle_mouse()
{
}
#endif

