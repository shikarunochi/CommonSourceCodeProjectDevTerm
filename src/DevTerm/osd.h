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

#ifndef _DEVTERM_OSD_H_
#define _DEVTERM_OSD_H_

#define  LOG_TAG    "commonProject"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

#define ENABLE_SOUND 1

#include "../windows.h"
#include "../vm/vm.h"
//#include "../emu.h"
#include "../common.h"
#include "../config.h"
#include "../vm/vm_template.h"
#include <SDL.h>
//#include <SDL/SDL_gfxPrimitives.h>
//#include <SDL/SDL_gfxPrimitives_font.h>
#ifdef USE_ZLIB
// relative path from *.vcproj/*.vcxproj, not from this directory :-(
	#if defined(_MSC_VER) && (_MSC_VER >= 1800)
		#ifdef _DEBUG
			#pragma comment(lib, "../src/zlib-1.2.11/vc++2013/debug/zlibstat.lib")
		#else
			#pragma comment(lib, "../src/zlib-1.2.11/vc++2013/release/zlibstat.lib")
		#endif
	#else
		#ifdef _DEBUG
			#pragma comment(lib, "../src/zlib-1.2.11/vc++2008/debug/zlibstat.lib")
		#else
			#pragma comment(lib, "../src/zlib-1.2.11/vc++2008/release/zlibstat.lib")
		#endif
	#endif
#endif

#ifdef USE_SOCKET
//#include <winsock.h>
//#pragma comment(lib, "wsock32.lib")
#endif

#if defined(USE_MOVIE_PLAYER) || defined(USE_VIDEO_CAPTURE)
#pragma comment(lib, "strmiids.lib")
#include <dshow.h>
//#include <qedit.h>
EXTERN_C const CLSID CLSID_SampleGrabber;
EXTERN_C const CLSID CLSID_NullRenderer;
EXTERN_C const IID IID_ISampleGrabberCB;
MIDL_INTERFACE("0579154A-2B53-4994-B0D0-E773148EFF85")
ISampleGrabberCB : public IUnknown {
public:
	virtual HRESULT STDMETHODCALLTYPE SampleCB( double SampleTime,IMediaSample *pSample) = 0;
	virtual HRESULT STDMETHODCALLTYPE BufferCB( double SampleTime,BYTE *pBuffer,long BufferLen) = 0;
};
EXTERN_C const IID IID_ISampleGrabber;
MIDL_INTERFACE("6B652FFF-11FE-4fce-92AD-0266B5D7C78F")
ISampleGrabber : public IUnknown {
public:
	virtual HRESULT STDMETHODCALLTYPE SetOneShot( BOOL OneShot) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetMediaType( const AM_MEDIA_TYPE *pType) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetConnectedMediaType( AM_MEDIA_TYPE *pType) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetBufferSamples( BOOL BufferThem) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetCurrentBuffer( /* [out][in] */ long *pBufferSize,/* [out] */ long *pBuffer) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetCurrentSample( /* [retval][out] */ IMediaSample **ppSample) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetCallback( ISampleGrabberCB *pCallback,long WhichMethodToCallback) = 0;
};
#endif
#ifdef USE_MOVIE_PLAYER
class CMySampleGrabberCB : public ISampleGrabberCB {
private:
	VM_TEMPLATE *vm;
public:
	CMySampleGrabberCB(VM_TEMPLATE *vm_ptr)
	{
		vm = vm_ptr;
	}
	STDMETHODIMP_(ULONG) AddRef()
	{
		return 2;
	}
	STDMETHODIMP_(ULONG) Release()
	{
		return 1;
	}
	STDMETHODIMP QueryInterface(REFIID riid, void **ppv)
	{
		if(riid == IID_ISampleGrabberCB || riid == IID_IUnknown) {
			*ppv = (void *) static_cast<ISampleGrabberCB*>(this);
			return NOERROR;
		}
		return E_NOINTERFACE;
	}
	STDMETHODIMP SampleCB(double SampleTime, IMediaSample *pSample)
	{
		return S_OK;
	}
	STDMETHODIMP BufferCB(double dblSampleTime, BYTE *pBuffer, long lBufferSize)
	{
		vm->movie_sound_callback(pBuffer, lBufferSize);
		return S_OK;
	}
};
#endif
//#include<string>

#define WM_RESIZE  (WM_USER + 1)
#define WM_SOCKET0 (WM_USER + 2)
#define WM_SOCKET1 (WM_USER + 3)
#define WM_SOCKET2 (WM_USER + 4)
#define WM_SOCKET3 (WM_USER + 5)

#ifdef USE_SOCKET
#define SOCKET_MAX 4
#define SOCKET_BUFFER_MAX 0x100000
#endif

#ifdef USE_VIDEO_CAPTURE
#define MAX_CAPTURE_DEVS 8
#endif

//#define SUPPORT_WIN32_DLL

#define SCREEN_FILTER_NONE	0
#define SCREEN_FILTER_RGB	1
#define SCREEN_FILTER_RF	2

// check memory leaks
#ifdef _DEBUG
// _malloca is defined in typeinfo.h
#ifdef _malloca
#undef _malloca
#endif
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

// osd common

#define OSD_CONSOLE_BLUE	1 // text color contains blue
#define OSD_CONSOLE_GREEN	2 // text color contains green
#define OSD_CONSOLE_RED		4 // text color contains red
#define OSD_CONSOLE_INTENSITY	8 // text color is intensified

typedef struct bitmap_s {
	// common
	inline bool initialized()
	{
		return (lpBmp != NULL);
	}
	inline scrntype_t* get_buffer(int y)
	{
		return lpBmp + width * (height - y - 1);
	}
	int width, height;

	scrntype_t* lpBmp;

} bitmap_t;

typedef struct font_s {
	// common
	inline bool initialized()
	{
		//TODO:
		return true;
		//return (hFont != NULL);
	}
	_TCHAR family[64];
	int width, height, rotate;
	bool bold, italic;
	// win32 dependent
	//TODO:shikarunochi
	//HFONT hFont;
} font_t;

typedef struct pen_s {
	// common
	inline bool initialized()
	{
		//TODO
		return true;
		//return (hPen != NULL);
	}
	int width;
	uint8_t r, g, b;
	//TODO:
	// win32 dependent
	//HPEN hPen;
} pen_t;

struct AudioPlayContext 
{
    Uint8 * data; // pcm buffer
    int offset; // current read pos
    int data_len; // left data length
    bool is_exit; // is audio play buffer empty?
};
class FIFO;
class FILEIO;

class OBOESOUND;

class OSD
{
private:
	int lock_count;

	// input
	void initialize_input();
	void release_input();

	uint8_t keycode_conv[256];
	uint8_t key_status[256];	// windows key code mapping
	uint8_t key_dik[256];
	uint8_t key_dik_prev[256];
	bool key_shift_pressed, key_shift_released;
	bool key_caps_locked;
	bool lost_focus;
	
#ifdef USE_JOYSTICK
	// bit0-3	up,down,left,right
	// bit4-19	button #1-#16
	// bit20-21	z-axis pos
	// bit22-23	r-axis pos
	// bit24-25	u-axis pos
	// bit26-27	v-axis pos
	// bit28-31	pov pos
	uint32_t joy_status[4];
	int joy_num;
	struct {
		UINT wNumAxes;
		DWORD dwXposLo, dwXposHi;
		DWORD dwYposLo, dwYposHi;
		DWORD dwZposLo, dwZposHi;
		DWORD dwRposLo, dwRposHi;
		DWORD dwUposLo, dwUposHi;
		DWORD dwVposLo, dwVposHi;
		DWORD dwButtonsMask;
	} joy_caps[4];
	bool joy_to_key_status[256];
	
	SDL_Joystick *joy;
#endif
	
#ifdef USE_MOUSE
	int32_t mouse_status[3];	// x, y, button (b0 = left, b1 = right)
	bool mouse_enabled;
#endif
	
	
#ifdef USE_FLOPPY_DISK
	uint32_t fd_status;
#endif
#ifdef USE_QUICK_DISK
	uint32_t qd_status;
#endif
#ifdef USE_HARD_DISK
	uint32_t hd_status;
#endif
#ifdef USE_COMPACT_DISC
	uint32_t cd_status;
#endif
#ifdef USE_LASER_DISC
	uint32_t ld_status;
#endif
#if defined(USE_TAPE) && !defined(TAPE_BINARY_ONLY)
	_TCHAR tape_status[1024];
#endif	
	
	// screen
	void initialize_screen();
	void release_screen();
	void initialize_screen_buffer(bitmap_t *buffer, int width, int height, int mode);
	void release_screen_buffer(bitmap_t *buffer);
#ifdef USE_SCREEN_FILTER
	void apply_rgb_filter_to_screen_buffer(bitmap_t *source, bitmap_t *dest);
	void apply_rgb_filter_x3_y3(bitmap_t *source, bitmap_t *dest);
	void apply_rgb_filter_x3_y2(bitmap_t *source, bitmap_t *dest);
	void apply_rgb_filter_x2_y3(bitmap_t *source, bitmap_t *dest);
	void apply_rgb_filter_x2_y2(bitmap_t *source, bitmap_t *dest);
	void apply_rgb_filter_x1_y1(bitmap_t *source, bitmap_t *dest);
#endif
//#ifdef USE_SCREEN_ROTATEinitialize_screen_buffer
	void rotate_screen_buffer(bitmap_t *source, bitmap_t *dest);
//#endif
	void stretch_screen_buffer(bitmap_t *source, bitmap_t *dest);
	bool initialize_d3d9();
	bool initialize_d3d9_surface(bitmap_t *buffer);
	void release_d3d9();
	void release_d3d9_surface();
	void copy_to_d3d9_surface(bitmap_t *buffer);
	int add_video_frames();

	bitmap_t vm_screen_buffer;
#ifdef USE_SCREEN_FILTER
	bitmap_t filtered_screen_buffer;
	bitmap_t tmp_filtered_screen_buffer;
#endif
//#ifdef USE_SCREEN_ROTATE
	bitmap_t rotated_screen_buffer;
//#endif
	bitmap_t stretched_screen_buffer;
	bitmap_t shrinked_screen_buffer;
	bitmap_t video_screen_buffer;
	
	bitmap_t* draw_screen_buffer;
	
	int host_window_width, host_window_height;
	bool host_window_mode;
	int vm_screen_width, vm_screen_height;
	int vm_window_width, vm_window_height;
	int vm_window_width_aspect, vm_window_height_aspect;
	int draw_screen_width, draw_screen_height;
	
	_TCHAR video_file_path[_MAX_PATH];
	int rec_video_fps;
	double rec_video_run_frames;
	double rec_video_frames;


	bool first_draw_screen;
	bool first_invalidate;
	bool self_invalidate;
	
	// sound
	void initialize_sound(int rate, int samples);
	void release_sound();
	
	int sound_rate, sound_samples;
	bool sound_available, sound_started, sound_muted;
	
	bool sound_first_half;
	
	_TCHAR sound_file_path[_MAX_PATH];
	FILEIO* rec_sound_fio;
	int rec_sound_bytes;
	int rec_sound_buffer_ptr;
	

	// video device
#if defined(USE_MOVIE_PLAYER) || defined(USE_VIDEO_CAPTURE)
	void initialize_video();
	void release_video();
	
	IGraphBuilder *pGraphBuilder;
	IBaseFilter *pVideoBaseFilter;
	IBaseFilter *pCaptureBaseFilter;
	ICaptureGraphBuilder2 *pCaptureGraphBuilder2;
	ISampleGrabber *pVideoSampleGrabber;
	IBaseFilter *pSoundBaseFilter;
	ISampleGrabber *pSoundSampleGrabber;
	CMySampleGrabberCB *pSoundCallBack;
	IMediaControl *pMediaControl;
	IMediaSeeking *pMediaSeeking;
	IMediaPosition *pMediaPosition;
	IVideoWindow *pVideoWindow;
	IBasicVideo *pBasicVideo;
	IBasicAudio *pBasicAudio;
	bool bTimeFormatFrame;
	bool bVerticalReversed;
	
	bitmap_t direct_show_screen_buffer;
	bitmap_t direct_show_stretch_buffer;
	int direct_show_width, direct_show_height;
	bool direct_show_mute[2];
#endif
#ifdef USE_MOVIE_PLAYER
	double movie_frame_rate;
	int movie_sound_rate;
#endif
#ifdef USE_VIDEO_CAPTURE
	void enum_capture_devs();
	bool connect_capture_dev(int index, bool pin);
	int cur_capture_dev_index;
	int num_capture_devs;
	_TCHAR capture_dev_name[MAX_CAPTURE_DEVS][256];
#endif
	
	// socket
#ifdef USE_SOCKET
	void initialize_socket();
	void release_socket();
	
	//int soc[SOCKET_MAX];
	//bool is_tcp[SOCKET_MAX];
	//struct sockaddr_in udpaddr[SOCKET_MAX];
	//int socket_delay[SOCKET_MAX];
	//char recv_buffer[SOCKET_MAX][SOCKET_BUFFER_MAX];
	//int recv_r_ptr[SOCKET_MAX], recv_w_ptr[SOCKET_MAX];
#endif
	
	static char *fbptr;
	unsigned int getFuncVmKeyCode(int keyCode);
	
	//Sound
										// reset buffer
	SDL_sem *audio_sem;
										// semaphore object
	int num_of_devices;
										// number of devices
	char *name_of_devices;
										// name of devices
	SDL_AudioDeviceID device_id;
										// opened device id
	SDL_AudioSpec device_spec;
										// opened device spec
	bool device_pause;
										// pause status (device)
	bool play_pause;
										// pause status (play-stop)
	Uint8 *sample_buffer;
										// sample buffer
	int sample_num;
										// number of samples (current)
	int sample_size;
										// size of sample buffer
	int sample_read;
										// head of sample buffer
	int sample_write;
										// tail of sample buffer
	int sample_per;

	
	
	
	
public:
	OSD()
	{
		lock_count = 0;
	}
	~OSD() {}
	
	// common
	VM_TEMPLATE* vm;
	
	void initialize(int rate, int samples);
	void release();
	void power_off();
	void suspend();
	void restore();
	void lock_vm();
	void unlock_vm();
	bool is_vm_locked()
	{
		return (lock_count != 0);
	}
	void force_unlock_vm();
	void sleep(uint32_t ms);
	
	// common debugger
#ifdef USE_DEBUGGER
	void start_waiting_in_debugger();
	void finish_waiting_in_debugger();
	void process_waiting_in_debugger();
#endif
	
	// common console
	void open_console(const _TCHAR* title);
	void close_console();
	unsigned int get_console_code_page();
	bool is_console_active();
	void set_console_text_attribute(unsigned short attr);
	void write_console(const _TCHAR* buffer, unsigned int length);
	int read_console_input(_TCHAR* buffer, unsigned int length);
	bool is_console_key_pressed(int vk);
	void close_debugger_console();
	
	// common input
	void update_input();
	void key_down(int code, bool extended, bool repeat);
	void key_up(int code, bool extended);
	void key_down_native(int code, bool repeat);
	void key_up_native(int code);
	void key_lost_focus()
	{
		lost_focus = true;
	}
#ifdef USE_MOUSE
	void enable_mouse();
	void disable_mouse();
	void toggle_mouse();
	bool is_mouse_enabled()
	{
		return mouse_enabled;
	}
#endif
	uint8_t* get_key_buffer()
	{
		return key_status;
	}
#ifdef USE_JOYSTICK
	uint32_t* get_joy_buffer()
	{
		return joy_status;
	}
#endif
#ifdef USE_MOUSE
	int32_t* get_mouse_buffer()
	{
		return mouse_status;
	}
#endif
#ifdef USE_AUTO_KEY
	bool now_auto_key;
#endif
	int exchangeUStoJIS(int code, bool shift);

	// common screen
	double get_window_mode_power(int mode);
	int get_window_mode_width(int mode);
	int get_window_mode_height(int mode);
	void set_host_window_size(int window_width, int window_height, bool window_mode);
	void set_vm_screen_size(int screen_width, int screen_height, int window_width, int window_height, int window_width_aspect, int window_height_aspect);
	void set_vm_screen_lines(int lines);
	int get_vm_window_width()
	{
		return vm_window_width;
	}
	int get_vm_window_height()
	{
		return vm_window_height;
	}
	int get_vm_window_width_aspect()
	{
		return vm_window_width_aspect;
	}
	int get_vm_window_height_aspect()
	{
		return vm_window_height_aspect;
	}
	scrntype_t* get_vm_screen_buffer(int y);
	int draw_screen();
#ifdef ONE_BOARD_MICRO_COMPUTER
	void reload_bitmap()
	{
		first_invalidate = true;
	}
#endif
	void capture_screen();
	bool start_record_video(int fps);
	void stop_record_video();
	void restart_record_video();
	void add_extra_frames(int extra_frames);
	bool now_record_video;
#ifdef USE_SCREEN_FILTER
	bool screen_skip_line;
#endif
	
	// common sound
	void update_sound(int* extra_frames);
	void mute_sound(){};
	void stop_sound(){};
	void start_record_sound(){};
	void stop_record_sound(){};
	void restart_record_sound(){};
	bool now_record_sound;
	void reset_sound();

	// common video device
#if defined(USE_MOVIE_PLAYER) || defined(USE_VIDEO_CAPTURE)
	void get_video_buffer();
	void mute_video_dev(bool l, bool r);
#endif
#ifdef USE_MOVIE_PLAYER
	bool open_movie_file(const _TCHAR* file_path);
	void close_movie_file();
	void play_movie();
	void stop_movie();
	void pause_movie();
	double get_movie_frame_rate()
	{
		return movie_frame_rate;
	}
	int get_movie_sound_rate()
	{
		return movie_sound_rate;
	}
	void set_cur_movie_frame(int frame, bool relative);
	uint32_t get_cur_movie_frame();
	bool now_movie_play, now_movie_pause;
#endif
#ifdef USE_VIDEO_CAPTURE
	int get_cur_capture_dev_index()
	{
		return cur_capture_dev_index;
	}
	int get_num_capture_devs()
	{
		return num_capture_devs;
	}
	_TCHAR* get_capture_dev_name(int index)
	{
		return capture_dev_name[index];
	}
	void open_capture_dev(int index, bool pin);
	void close_capture_dev();
	void show_capture_dev_filter();
	void show_capture_dev_pin();
	void show_capture_dev_source();
	void set_capture_dev_channel(int ch);
#endif
	
	// common printer
#ifdef USE_PRINTER
	void create_bitmap(bitmap_t *bitmap, int width, int height);
	void release_bitmap(bitmap_t *bitmap);
	void create_font(font_t *font, const _TCHAR *family, int width, int height, int rotate, bool bold, bool italic);
	void release_font(font_t *font);
	void create_pen(pen_t *pen, int width, uint8_t r, uint8_t g, uint8_t b);
	void release_pen(pen_t *pen);
	void clear_bitmap(bitmap_t *bitmap, uint8_t r, uint8_t g, uint8_t b);
	int get_text_width(bitmap_t *bitmap, font_t *font, const char *text);
	void draw_text_to_bitmap(bitmap_t *bitmap, font_t *font, int x, int y, const char *text, uint8_t r, uint8_t g, uint8_t b);
	void draw_line_to_bitmap(bitmap_t *bitmap, pen_t *pen, int sx, int sy, int ex, int ey);
	void draw_rectangle_to_bitmap(bitmap_t *bitmap, int x, int y, int width, int height, uint8_t r, uint8_t g, uint8_t b);
	void draw_point_to_bitmap(bitmap_t *bitmap, int x, int y, uint8_t r, uint8_t g, uint8_t b);
	void stretch_bitmap(bitmap_t *dest, int dest_x, int dest_y, int dest_width, int dest_height, bitmap_t *source, int source_x, int source_y, int source_width, int source_height);
#endif
	void write_bitmap_to_file(bitmap_t *bitmap, const _TCHAR *file_path);
	
	// common socket
#ifdef USE_SOCKET
	int get_socket(int ch)
	{
		//return soc[ch];
		return 0;
	}
	void notify_socket_connected(int ch);
	void notify_socket_disconnected(int ch);
	void update_socket();
	bool initialize_socket_tcp(int ch);
	bool initialize_socket_udp(int ch);
	bool connect_socket(int ch, uint32_t ipaddr, int port);
	void disconnect_socket(int ch);
	bool listen_socket(int ch);
	void send_socket_data_tcp(int ch);
	void send_socket_data_udp(int ch, uint32_t ipaddr, int port);
	void send_socket_data(int ch);
	void recv_socket_data(int ch);
#endif
	
	// win32 dependent
	void invalidate_screen();
	bool vista_or_later;

	bitmap_t* getScreenBuffer();

	
	bool soundEnable = false;
	
	SDL_Window * sdlWindow = NULL;
	SDL_Renderer *sdlRenderer = NULL;
	SDL_Texture *sdlTexture = NULL;
	bool appQuit;
	void toggleWindowFullscreen();
	bool fullScreen;
	
	void selectFile(int fileType);
	void fileSelectDialog();

	bool get_status_bar_updated();

	
	//sound
	void Close();
										// write to sample buffer
	static void CommonCallback(void *userdata, Uint8 *stream, int len);
										// callback from SDL
	void Callback(Uint8 *steram, int len);
										// callback main
	void soundReset();

	int inputSoundBufferPos;
	int outputSoundBufferPos;
	//#define SOUND_BUFFER_LENGTH 4800*100*2
	//#define SOUND_BUFFER_LENGTH 4800

	//uint16_t soundBuffer[SOUND_BUFFER_LENGTH];
	int soundBufferLength;
	uint16_t *soundBuffer;
};

/*
 * Virtual Keys, Standard Set
 */
#define VK_LBUTTON        0x01
#define VK_RBUTTON        0x02
#define VK_CANCEL         0x03
#define VK_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */

/*
 * 0x07 : reserved
 */


#define VK_BACK           0x08
#define VK_TAB            0x09

/*
 * 0x0A - 0x0B : reserved
 */

#define VK_CLEAR          0x0C
#define VK_RETURN         0x0D

/*
 * 0x0E - 0x0F : unassigned
 */

#define VK_SHIFT          0x10
#define VK_CONTROL        0x11
#define VK_MENU           0x12
#define VK_PAUSE          0x13
#define VK_CAPITAL        0x14

#define VK_KANA           0x15
#define VK_HANGEUL        0x15  /* old name - should be here for compatibility */
#define VK_HANGUL         0x15

/*
 * 0x16 : unassigned
 */

#define VK_JUNJA          0x17
#define VK_FINAL          0x18
#define VK_HANJA          0x19
#define VK_KANJI          0x19

/*
 * 0x1A : unassigned
 */

#define VK_ESCAPE         0x1B

#define VK_CONVERT        0x1C
#define VK_NONCONVERT     0x1D
#define VK_ACCEPT         0x1E
#define VK_MODECHANGE     0x1F

#define VK_SPACE          0x20
#define VK_PRIOR          0x21
#define VK_NEXT           0x22
#define VK_END            0x23
#define VK_HOME           0x24
#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28
#define VK_SELECT         0x29
#define VK_PRINT          0x2A
#define VK_EXECUTE        0x2B
#define VK_SNAPSHOT       0x2C
#define VK_INSERT         0x2D
#define VK_DELETE         0x2E
#define VK_HELP           0x2F

/*
 * VK_0 - VK_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
 * 0x3A - 0x40 : unassigned
 * VK_A - VK_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
 */
#define VK_0           0x30
#define VK_1           0x31
#define VK_2           0x32
#define VK_3           0x33
#define VK_4           0x34
#define VK_5           0x35
#define VK_6           0x36
#define VK_7           0x37
#define VK_8           0x38
#define VK_9           0x39

#define VK_A           0x41
#define VK_B           0x42
#define VK_C           0x43
#define VK_D           0x44
#define VK_E           0x45
#define VK_F           0x46
#define VK_G           0x47
#define VK_H           0x48
#define VK_I           0x49
#define VK_J           0x4A
#define VK_K           0x4B
#define VK_L           0x4C
#define VK_M           0x4D
#define VK_N           0x4E
#define VK_O           0x4F
#define VK_P           0x50
#define VK_Q           0x51
#define VK_R           0x52
#define VK_S           0x53
#define VK_T           0x54
#define VK_U           0x55
#define VK_V           0x56
#define VK_W           0x57
#define VK_X           0x58
#define VK_Y           0x59
#define VK_Z           0x5A

#define VK_LWIN           0x5B
#define VK_RWIN           0x5C
#define VK_APPS           0x5D

/*
 * 0x5E : reserved
 */

#define VK_SLEEP          0x5F

#define VK_NUMPAD0        0x60
#define VK_NUMPAD1        0x61
#define VK_NUMPAD2        0x62
#define VK_NUMPAD3        0x63
#define VK_NUMPAD4        0x64
#define VK_NUMPAD5        0x65
#define VK_NUMPAD6        0x66
#define VK_NUMPAD7        0x67
#define VK_NUMPAD8        0x68
#define VK_NUMPAD9        0x69
#define VK_MULTIPLY       0x6A
#define VK_ADD            0x6B
#define VK_SEPARATOR      0x6C
#define VK_SUBTRACT       0x6D
#define VK_DECIMAL        0x6E
#define VK_DIVIDE         0x6F
#define VK_F1             0x70
#define VK_F2             0x71
#define VK_F3             0x72
#define VK_F4             0x73
#define VK_F5             0x74
#define VK_F6             0x75
#define VK_F7             0x76
#define VK_F8             0x77
#define VK_F9             0x78
#define VK_F10            0x79
#define VK_F11            0x7A
#define VK_F12            0x7B
#define VK_F13            0x7C
#define VK_F14            0x7D
#define VK_F15            0x7E
#define VK_F16            0x7F
#define VK_F17            0x80
#define VK_F18            0x81
#define VK_F19            0x82
#define VK_F20            0x83
#define VK_F21            0x84
#define VK_F22            0x85
#define VK_F23            0x86
#define VK_F24            0x87

#define VK_NUMLOCK        0x90
#define VK_SCROLL         0x91

/*
 * NEC PC-9800 kbd definitions
 */
#define VK_OEM_NEC_EQUAL  0x92   // '=' key on numpad

/*
 * Fujitsu/OASYS kbd definitions
 */
#define VK_OEM_FJ_JISHO   0x92   // 'Dictionary' key
#define VK_OEM_FJ_MASSHOU 0x93   // 'Unregister word' key
#define VK_OEM_FJ_TOUROKU 0x94   // 'Register word' key
#define VK_OEM_FJ_LOYA    0x95   // 'Left OYAYUBI' key
#define VK_OEM_FJ_ROYA    0x96   // 'Right OYAYUBI' key

/*
 * 0x97 - 0x9F : unassigned
 */

/*
 * VK_L* & VK_R* - left and right Alt, Ctrl and Shift virtual keys.
 * Used only as parameters to GetAsyncKeyState() and GetKeyState().
 * No other API or message will distinguish left and right keys in this way.
 */
#define VK_LSHIFT         0xA0
#define VK_RSHIFT         0xA1
#define VK_LCONTROL       0xA2
#define VK_RCONTROL       0xA3
#define VK_LMENU          0xA4
#define VK_RMENU          0xA5

/*
 * 0xB8 - 0xB9 : reserved
 */

#define VK_OEM_1          0xBA   // ';:' for US     :
#define VK_OEM_PLUS       0xBB   // '+' any country ;
#define VK_OEM_COMMA      0xBC   // ',' any country ,
#define VK_OEM_MINUS      0xBD   // '-' any country -
#define VK_OEM_PERIOD     0xBE   // '.' any country .
#define VK_OEM_2          0xBF   // '/?' for US    /
#define VK_OEM_3          0xC0   // '`~' for US    @

/*
 * 0xC1 - 0xC2 : reserved
 */

#define VK_OEM_4          0xDB  //  '[{' for US  [
#define VK_OEM_5          0xDC  //  '\|' for US  \""
#define VK_OEM_6          0xDD  //  ']}' for US  ]
#define VK_OEM_7          0xDE  //  ''"' for US  ^
#define VK_OEM_8          0xDF

/*
 * 0xE0 : reserved
 */

/*
 * Various extended or enhanced keyboards
 */
#define VK_OEM_AX         0xE1  //  'AX' key on Japanese AX kbd
#define VK_OEM_102        0xE2  //  "<>" or "\|" on RT 102-key kbd. _
#define VK_ICO_HELP       0xE3  //  Help key on ICO
#define VK_ICO_00         0xE4  //  00 key on ICO


#define VK_ICO_CLEAR      0xE6

/*
 * 0xE8 : unassigned
 */

/*
 * Nokia/Ericsson definitions
 */
#define VK_OEM_RESET      0xE9
#define VK_OEM_JUMP       0xEA
#define VK_OEM_PA1        0xEB
#define VK_OEM_PA2        0xEC
#define VK_OEM_PA3        0xED
#define VK_OEM_WSCTRL     0xEE
#define VK_OEM_CUSEL      0xEF
#define VK_OEM_ATTN       0xF0
#define VK_OEM_FINISH     0xF1
#define VK_OEM_COPY       0xF2
#define VK_OEM_AUTO       0xF3
#define VK_OEM_ENLW       0xF4
#define VK_OEM_BACKTAB    0xF5

#define VK_ATTN           0xF6
#define VK_CRSEL          0xF7
#define VK_EXSEL          0xF8
#define VK_EREOF          0xF9
#define VK_PLAY           0xFA
#define VK_ZOOM           0xFB
#define VK_NONAME         0xFC
#define VK_PA1            0xFD
#define VK_OEM_CLEAR      0xFE


enum FileSelectType { FILE_SELECT_NONE ,FLOPPY_DISK_1 ,FLOPPY_DISK_2 , CASETTE_TAPE , CARTRIDGE_1, CARTRIDGE_2, QUICK_DISK, BINARY_FILE, FILE_SELECT_TYPE_MAX};

#endif
