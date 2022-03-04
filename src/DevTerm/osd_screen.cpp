/*
	Skelton for retropc emulator

	Author : Takeda.Toshiya
	Date   : 2015.11.20-

	[ win32 screen ]

  	[for Android]
	Modify : @shikarunochi
	Date   : 2020.06.01-

  	[for DevTerm]
	Modify : @shikarunochi
	Date   : 2022.02.01-

*/
#include <sys/errno.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <linux/omapfb.h>

#include <SDL.h>

#include "osd.h"

#define REC_VIDEO_SUCCESS	1
#define REC_VIDEO_FULL		2
#define REC_VIDEO_ERROR		3

// フレームバッファ
#define FB_NAME "/dev/fb0"
#define DIV_BYTE 8

void OSD::initialize_screen()
{
//    LOGI("Initializing Graphic System ... ");

    vm_screen_width = SCREEN_WIDTH;
    vm_screen_height = SCREEN_HEIGHT;
    vm_window_width = WINDOW_WIDTH;
    vm_window_height = WINDOW_HEIGHT;
    vm_window_width_aspect = WINDOW_WIDTH_ASPECT;
    vm_window_height_aspect = WINDOW_HEIGHT_ASPECT;

    memset(&vm_screen_buffer, 0, sizeof(bitmap_t));
    //initialize_screen_buffer(&vm_screen_buffer, vm_screen_width , vm_screen_height, 0);
	initialize_screen_buffer(&vm_screen_buffer, vm_window_width , vm_window_height, 0);

    draw_screen_buffer = &vm_screen_buffer;

	printf("init sdl\n");
	SDL_Init(SDL_INIT_EVERYTHING);

	//適当判断で拡大
	int Y_scale = 1;
	int X_scale = 1;
	if(WINDOW_HEIGHT < 240){
		Y_scale = 2;
	}
	if(WINDOW_WIDTH <= 320){
		X_scale = 2;
	}

	printf("SCREEN_WIDTH:%d, SCREEN_HEIGHT:%d, WINDOW_WIDTH:%d, WINDOW_HEIGHT:%d, WINDOW_WIDTH_ASPECT:%d, WINDOW_HEIGHT_ASPECT:%d X_scale:%d Y_Scale:%d\n",
		SCREEN_WIDTH,SCREEN_HEIGHT,WINDOW_WIDTH,WINDOW_HEIGHT,WINDOW_WIDTH_ASPECT,WINDOW_HEIGHT_ASPECT, X_scale, Y_scale);
	
    sdlWindow = SDL_CreateWindow(CONFIG_NAME,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SCREEN_WIDTH * X_scale, WINDOW_HEIGHT * Y_scale, SDL_WINDOW_RESIZABLE);//
                                 //SDL_WINDOW_FULLSCREEN_DESKTOP);

	sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, 0);
	SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
    SDL_RenderClear(sdlRenderer);
    SDL_RenderPresent(sdlRenderer);
	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // 拡大縮小が滑らかになる
    SDL_RenderSetLogicalSize(sdlRenderer, WINDOW_WIDTH, WINDOW_HEIGHT);
	
	sdlTexture = SDL_CreateTexture(sdlRenderer,
                                   //SDL_PIXELFORMAT_ARGB8888,
									SDL_PIXELFORMAT_RGB888,
                                   SDL_TEXTUREACCESS_STREAMING,
                                   WINDOW_WIDTH, WINDOW_HEIGHT);
	fullScreen = false;
}




double OSD::get_window_mode_power(int mode)
{
    if(mode + WINDOW_MODE_BASE == 2) {
        return 1.5;
    } else if(mode + WINDOW_MODE_BASE > 2) {
        return mode + WINDOW_MODE_BASE - 1;
    }
    return mode + WINDOW_MODE_BASE;
}

int OSD::get_window_mode_width(int mode)
{
	return 640;
}

int OSD::get_window_mode_height(int mode)
{
	return 480;
}
void OSD::set_host_window_size(int window_width, int window_height, bool window_mode)
{
}

void OSD::set_vm_screen_size(int screen_width, int screen_height, int window_width, int window_height, int window_width_aspect, int window_height_aspect)
{
}

void OSD::set_vm_screen_lines(int lines)
{
//	set_vm_screen_size(vm_screen_width, lines, vm_window_width, vm_window_height, vm_window_width_aspect, vm_screen_height);
}

scrntype_t* OSD::get_vm_screen_buffer(int y)
{
	return vm_screen_buffer.get_buffer(y);
}




int OSD::draw_screen()
{
    vm->draw_screen();
	draw_screen_buffer = &vm_screen_buffer;
	scrntype_t* lpBmp =  draw_screen_buffer->lpBmp;

#ifdef USE_TAPE
	if(vm->is_tape_playing(0)){
		int tapePosition = vm->get_tape_position(0);
		if(tapePosition > 0){
			scrntype_t* screenBuffer = get_vm_screen_buffer(vm_window_height-1);
			for(int x = 0;x < vm_window_width * sizeof (scrntype_t);x++){
				if(x * 100 / vm_window_width * sizeof (scrntype_t) < tapePosition){
					*(screenBuffer + x) = 0x11FF11;
				}
			}
		}
	}
#endif	
	SDL_UpdateTexture(sdlTexture, NULL, lpBmp, vm_screen_width * sizeof (scrntype_t));
	SDL_RenderClear(sdlRenderer);
    SDL_RenderCopyEx(sdlRenderer, sdlTexture, NULL, NULL,0,0,SDL_FLIP_VERTICAL);
    SDL_RenderPresent(sdlRenderer);

	
	//const char *text ="8x8 Primitive Font Test";
	//stringColor(sdlWindow, 50, 50, text, 0xff0000ff);
	//SDL_Flip(sdlWindow);
   return 1;
}

bitmap_t* OSD::getScreenBuffer(){
	draw_screen_buffer = &vm_screen_buffer;
	return draw_screen_buffer;
}

void OSD::invalidate_screen()
{
}

void OSD::initialize_screen_buffer(bitmap_t *buffer, int width, int height, int mode)
{
    release_screen_buffer(buffer);
    buffer->height = height;
    buffer->width = width;
    buffer->lpBmp = (scrntype_t*)malloc(width * height * sizeof(scrntype_t));
}

void OSD::release_screen(){
	SDL_DestroyRenderer(sdlRenderer);
	SDL_Quit();
}

void OSD::release_screen_buffer(bitmap_t *buffer)
{
    free(buffer->lpBmp);
}

#ifdef USE_SCREEN_FILTER
#define _3_8(v) (((((v) * 3) >> 3) * 180) >> 8)
#define _5_8(v) (((((v) * 3) >> 3) * 180) >> 8)
#define _8_8(v) (((v) * 180) >> 8)

static uint8_t r0[2048], g0[2048], b0[2048], t0[2048];
static uint8_t r1[2048], g1[2048], b1[2048];

void OSD::apply_rgb_filter_to_screen_buffer(bitmap_t *source, bitmap_t *dest)
{
}

void OSD::apply_rgb_filter_x3_y3(bitmap_t *source, bitmap_t *dest)
{
}

void OSD::apply_rgb_filter_x3_y2(bitmap_t *source, bitmap_t *dest)
{
}

void OSD::apply_rgb_filter_x2_y3(bitmap_t *source, bitmap_t *dest)
{
}

void OSD::apply_rgb_filter_x2_y2(bitmap_t *source, bitmap_t *dest)
{
}

void OSD::apply_rgb_filter_x1_y1(bitmap_t *source, bitmap_t *dest)
{
}
#endif

//#ifdef USE_SCREEN_ROTATE
void OSD::rotate_screen_buffer(bitmap_t *source, bitmap_t *dest)
{
}
//#endif

void OSD::stretch_screen_buffer(bitmap_t *source, bitmap_t *dest)
{
}

#if defined(_RGB555)
	#define D3DFMT_TMP D3DFMT_X1R5G5B5
#elif defined(_RGB565)
	#define D3DFMT_TMP D3DFMT_R5G6B5
#elif defined(_RGB888)
	#define D3DFMT_TMP D3DFMT_X8R8G8B8
#endif

bool OSD::initialize_d3d9()
{
	return true;
}

bool OSD::initialize_d3d9_surface(bitmap_t *buffer)
{
	return true;
}

void OSD::release_d3d9()
{
}

void OSD::release_d3d9_surface()
{
}

void OSD::copy_to_d3d9_surface(bitmap_t *buffer)
{
}

void OSD::capture_screen()
{
}

bool OSD::start_record_video(int fps)
{
	return true;
}

void OSD::stop_record_video()
{
}

void OSD::restart_record_video()
{
}

void OSD::add_extra_frames(int extra_frames)
{
}


int OSD::add_video_frames()
{
	return 0;
}
#ifdef USE_PRINTER
void OSD::create_bitmap(bitmap_t *bitmap, int width, int height)
{
	//initialize_screen_buffer(bitmap, width, height, HALFTONE);
	initialize_screen_buffer(bitmap, width, height, 0);
}

void OSD::release_bitmap(bitmap_t *bitmap)
{
	release_screen_buffer(bitmap);
}

void OSD::create_font(font_t *font, const _TCHAR *family, int width, int height, int rotate, bool bold, bool italic)
{
	/*
	LOGFONT logfont;
	memset(&logfont, 0, sizeof(logfont));
	logfont.lfEscapement = font->rotate = rotate;
	logfont.lfOrientation = 0;
	logfont.lfWeight = (font->bold = bold) ? FW_BOLD : FW_NORMAL;
	logfont.lfItalic = (font->italic = italic) ? TRUE : FALSE;
	logfont.lfUnderline = FALSE;
	logfont.lfStrikeOut = FALSE;
	logfont.lfCharSet = SHIFTJIS_CHARSET;
	logfont.lfOutPrecision = OUT_TT_PRECIS;
	logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logfont.lfQuality = DEFAULT_QUALITY; 
	logfont.lfPitchAndFamily = FIXED_PITCH | FF_DONTCARE;
	if(_tcsicmp(family, _T("Gothic")) == 0) {
		my_tcscpy_s(logfont.lfFaceName, LF_FACESIZE, _T("MS Gothic"));
		my_tcscpy_s(font->family, 64, _T("Gothic"));
	} if(_tcsicmp(family, _T("PGothic")) == 0) {
		my_tcscpy_s(logfont.lfFaceName, LF_FACESIZE, _T("MS PGothic"));
		my_tcscpy_s(font->family, 64, _T("Gothic"));
	} else if(_tcsicmp(family, _T("Mincho")) == 0) {
		my_tcscpy_s(logfont.lfFaceName, LF_FACESIZE, _T("MS Mincho"));
		my_tcscpy_s(font->family, 64, _T("Mincho"));
	} else if(_tcsicmp(family, _T("PMincho")) == 0) {
		my_tcscpy_s(logfont.lfFaceName, LF_FACESIZE, _T("MS PMincho"));
		my_tcscpy_s(font->family, 64, _T("Mincho"));
	} else {
		my_tcscpy_s(logfont.lfFaceName, LF_FACESIZE, _T("MS Gothic"));
		my_tcscpy_s(font->family, 64, _T("Gothic"));
	}
	logfont.lfHeight = font->height = height;
	logfont.lfWidth = font->width = width;
	font->hFont = CreateFontIndirect(&logfont);
	*/
}

void OSD::release_font(font_t *font)
{
	/*
	if(font->initialized()) {
		DeleteObject(font->hFont);
		font->hFont = NULL;
	}
	*/
}

void OSD::create_pen(pen_t *pen, int width, uint8_t r, uint8_t g, uint8_t b)
{
	/*
	pen->hPen = CreatePen(PS_SOLID, (pen->width = width), RGB((pen->r = r), (pen->g = g), (pen->b = b)));
	*/
}

void OSD::release_pen(pen_t *pen)
{
	/*
	if(pen->initialized()) {
		DeleteObject(pen->hPen);
		pen->hPen = NULL;
	}
	*/
}

void OSD::clear_bitmap(bitmap_t *bitmap, uint8_t r, uint8_t g, uint8_t b)
{
	draw_rectangle_to_bitmap(bitmap, 0, 0, bitmap->width, bitmap->height, r, g, b);
}

int OSD::get_text_width(bitmap_t *bitmap, font_t *font, const char *text)
{
	/*
	HFONT hFontOld = (HFONT)SelectObject(bitmap->hdcDib, font->hFont);
	SIZE size;
#ifdef _UNICODE
	_TCHAR unicode[1024];
	MultiByteToWideChar(CP_ACP, 0, text, -1, unicode, 1024);
	GetTextExtentPoint32(bitmap->hdcDib, unicode, (int)wcslen(unicode), &size);
#else
	GetTextExtentPoint32(bitmap->hdcDib, text, (int)strlen(text), &size);
#endif
	SelectObject(bitmap->hdcDib, hFontOld);
	return (int)size.cx;
	*/
	return 16;
}

void OSD::draw_text_to_bitmap(bitmap_t *bitmap, font_t *font, int x, int y, const char *text, uint8_t r, uint8_t g, uint8_t b)
{
	/*
	HFONT hFontOld = (HFONT)SelectObject(bitmap->hdcDib, font->hFont);
	SetBkMode(bitmap->hdcDib, TRANSPARENT);
	SetTextColor(bitmap->hdcDib, RGB(r, g, b));
#ifdef _UNICODE
	_TCHAR unicode[1024];
	MultiByteToWideChar(CP_ACP, 0, text, -1, unicode, 1024);
	ExtTextOut(bitmap->hdcDib, x, y, NULL, NULL, unicode, (UINT)wcslen(unicode), NULL);
#else
	ExtTextOut(bitmap->hdcDib, x, y, NULL, NULL, text, (UINT)strlen(text), NULL);
#endif
	SelectObject(bitmap->hdcDib, hFontOld);
	*/
}

void OSD::draw_line_to_bitmap(bitmap_t *bitmap, pen_t *pen, int sx, int sy, int ex, int ey)
{
	/*
	HPEN hPenOld = (HPEN)SelectObject(bitmap->hdcDib, pen->hPen);
	MoveToEx(bitmap->hdcDib, sx, sy, NULL);
	LineTo(bitmap->hdcDib, ex, ey);
	SelectObject(bitmap->hdcDib, hPenOld);
	*/
}

void OSD::draw_rectangle_to_bitmap(bitmap_t *bitmap, int x, int y, int width, int height, uint8_t r, uint8_t g, uint8_t b)
{
	for(int yy = 0; yy < height; yy++) {
		for(int xx = 0; xx < width; xx++) {
			draw_point_to_bitmap(bitmap, x + xx, y + yy, r, g, b);
		}
	}
}

void OSD::draw_point_to_bitmap(bitmap_t *bitmap, int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
	if(x >= 0 && x < bitmap->width && y >= 0 && y < bitmap->height) {
		scrntype_t *dest = bitmap->get_buffer(y);
		dest[x] = RGB_COLOR(r, g, b);
	}
}

void OSD::stretch_bitmap(bitmap_t *dest, int dest_x, int dest_y, int dest_width, int dest_height, bitmap_t *source, int source_x, int source_y, int source_width, int source_height)
{
	/*
	if(dest_width == source_width && dest_height == source_height) {
		BitBlt(dest->hdcDib, dest_x, dest_y, dest_width, dest_height, source->hdcDib, source_x, source_y, SRCCOPY);
	} else {
		StretchBlt(dest->hdcDib, dest_x, dest_y, dest_width, dest_height, source->hdcDib, source_x, source_y, source_width, source_height, SRCCOPY);
	}
	*/
}
#endif


void OSD::write_bitmap_to_file(bitmap_t *bitmap, const _TCHAR *file_path)
{
	
	//DevTermプリンタに出力する。
	//https://forum.clockworkpi.com/t/printing-images-with-tmp-devterm-printer-in/7650
	//GS(\x1d) v 0 p wL wH hL hH, where:
	//Width is (wL+256*wH)*8 pixels (because it’s in bytes)
	//Height is (hL+256*hH) pixels (because it’s in lines)
	//色が 0 以上の物を黒として出力
	int width = bitmap->width;
	int height = bitmap->height;
	printf("test-----:X:%d, Y:%d\n",width,height);
	scrntype_t *lpBmp = bitmap->lpBmp;
	
	//データが出る（値0が出る）までは読み飛ばす。
	int startY = 0;
	bool found = false;
	for(startY = 0;startY < height;startY++){
		for(int x = 0;x < width;x++){
			if(lpBmp[startY * width + x] == 0){
				found = true;
				break;
			}
		}
		if(found == true){
			break;
		}
	}

	int printerWidth = 384;
	
	
	//mz1p17.hで、#define PIXEL_PER_INCH	180 にした上で…
	//3倍で出力されてるっぽいのでサイズは1/3にして印刷
	int realWidth = width / 3;
	int realHeight = height / 3;
	int realStartY = startY / 3;
	
	int printHeight = realHeight - realStartY; //印刷する高さ
	
	//データは上下逆向きになってるので、realHeight から realStartY まで印刷する。
	
	//印刷用画像データ
	#define IMAGE_MAX 9224

	FILEIO* fio_d = fio_d = new FILEIO();
	
	uint8_t imageData[IMAGE_MAX];// // 48x192bytes(384x192 pixels) ,max
	memset(imageData, 0, sizeof(imageData));

	int bit = 0;
	int imageY = 0;
	for(int y = 0;y < printHeight;y++){
		//1列分のデータを作成
		for(int x = 0;x < realWidth;x++){
			int imageXIndex = x / 8;
			int imageXBit = x - imageXIndex * 8;
			if(imageXIndex >=48){
				break;
			}
			int imageIndex = imageY * 48 + imageXIndex;
			
			if(lpBmp[(realHeight - y) * 3 * width + x * 3] == 0 || lpBmp[(realHeight - y) * 3 * width + x * 3 + 1] == 0){ //2ドットでOR
				imageData[imageIndex] = imageData[imageIndex] | (1 << (7-imageXBit));
			}
		}
		//printf("\n");
		if(imageY > 190){
			//いったん出力
			//wL = 48
			//wH = 0
			uint8_t hH = (uint8_t)(imageY / 256);	
			uint8_t hL = (uint8_t)(imageY - hH * 256); 
			printf("imageY %d\n",imageY);
			//int hH = (int)(imageY / 256); = 192 / 256 = 0
			//\x1d\x76\x30\x70\x01\x00\x08\x00
			if(fio_d->Fopen(file_path, FILEIO_WRITE_BINARY))
			{
				fio_d->Fputc(0x1d);
				fio_d->Fputc(0x76);
				fio_d->Fputc(0x30);
				fio_d->Fputc(0x70);
				fio_d->Fputc(48);
				fio_d->Fputc(0);
				fio_d->Fputc(hL);
				fio_d->Fputc(hH);

				for(int i = 0;i < (imageY) * 48;i++){
					fio_d->Fputc(imageData[i]);
				}
				fio_d->Fclose();
			}else{
				printf("FileOpenError\n");
			}
			char command[_MAX_PATH * 2 + 256];
			sprintf(command, "cat %s > /tmp/DEVTERM_PRINTER_IN;rm %s", file_path,file_path);
			//sprintf(command, "echo \"\\x1d\\x0\\x \" > /tmp/DEVTERM_PRINTER_IN", file_path);
			printf("%s\n",command);
			int ret = system(command);
			//FILEIO::RemoveFile(file_path);
			memset(imageData, 0, sizeof(imageData));
			imageY = 0;
		}else{
			imageY++;
		}
	}
	if(imageY >= 0){
			//char file_path2[_MAX_PATH + 256];
			//wL = 0
			//wH = 48 / 8 = 6
			uint8_t hH = (uint8_t)(imageY / 256);	
			uint8_t hL = (uint8_t)(imageY - hH * 256); 
			//create_date_file_path(file_path, _MAX_PATH, _T("txt"));
			printf("imageY %d: hH %d: hL %d :%s\n",imageY,hH,hL,file_path);
			//\x1d\x76\x30\x70\x00\x06\x08\x00
		if(fio_d->Fopen(file_path, FILEIO_WRITE_BINARY)){
				fio_d->Fputc(0x1d);
				fio_d->Fputc(0x76);
				fio_d->Fputc(0x30);
				fio_d->Fputc(0x70);
				fio_d->Fputc(48);
				fio_d->Fputc(0);
				fio_d->Fputc(hL);
				fio_d->Fputc(hH);
				for(int i = 0;i < (imageY) * 48;i++){
					//printf("%d\n",i);
					fio_d->Fputc(imageData[i]);
				}
				fio_d->Fclose();
		}else{
			printf("FileOpenError\n");
		}
			char command[_MAX_PATH * 2 + 256];
			sprintf(command, "cat %s > /tmp/DEVTERM_PRINTER_IN;rm %s", file_path,file_path);
			//sprintf(command, "echo \"\\x1d\\x0\\x \" > /tmp/DEVTERM_PRINTER_IN", file_path);
			printf("%s\n",command);
			int ret = system(command);
			//FILEIO::RemoveFile(file_path);
	}
	printf("Print End\n");
	
	//if(check_file_extension(file_path, _T(".bmp"))) {
	//	// save as bmp file
	//	BITMAPFILEHEADER bmFileHeader = { (WORD)(TEXT('B') | TEXT('M') << 8) };
	//	bmFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	//	bmFileHeader.bfSize = bmFileHeader.bfOffBits + bitmap->lpDib->bmiHeader.biSizeImage;
	//	
	//	DWORD dwSize;
	//	HANDLE hFile = CreateFile(file_path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//	WriteFile(hFile, &bmFileHeader, sizeof(BITMAPFILEHEADER), &dwSize, NULL);
	//	WriteFile(hFile, bitmap->lpDib, sizeof(BITMAPINFOHEADER), &dwSize, NULL);
	//	WriteFile(hFile, bitmap->lpBmp, bitmap->lpDib->bmiHeader.biSizeImage, &dwSize, NULL);
	//	CloseHandle(hFile);
	//} else if(check_file_extension(file_path, _T(".png"))) {
		// save as png file
//		CLSID encoderClsid;
//		if(GetEncoderClsid(L"image/png", &encoderClsid)) {
//			Bitmap image(bitmap->hBmp, (HPALETTE)GetStockObject(DEFAULT_PALETTE));
//#ifdef _UNICODE
//			image.Save(file_path, &encoderClsid, NULL);
//#else
	//		WCHAR wszFilePath[_MAX_PATH];
	//		MultiByteToWideChar(CP_ACP, 0, file_path, -1, wszFilePath, _MAX_PATH);
	//		image.Save(wszFilePath, &encoderClsid, NULL);
//#endif
//		}
//	}
}

void OSD::toggleWindowFullscreen() {
	//u32 value = 0;
	//if(!fullscreen) value = SDL_WINDOW_FULLSCREEN_DESKTOP;
	//SDL_SetWindowFullscreen(sdlWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
	if(fullScreen == true){
		SDL_SetWindowFullscreen(sdlWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}else{
		SDL_SetWindowFullscreen(sdlWindow, 0);
	}
	fullScreen = !fullScreen;
}
