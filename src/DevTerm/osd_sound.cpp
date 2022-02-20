/*
	Skelton for retropc emulator

	Author : Takeda.Toshiya
	Date   : 2015.11.26-

	[ win32 sound ]

  	[for Android]
	Modify : @shikarunochi
	Date   : 2020.06.01-

  	[for DevTerm]
	Modify : @shikarunochi
	Date   : 2022.02.01-

*/

//https://propella.hatenablog.com/entry/20080217/p1 参考


#include "osd.h"

void OSD::update_sound(int* extra_frames){
#ifdef ENABLE_SOUND
	
	//printf("updateSound\n");
	
	if(soundEnable == false){
        return;
    }

	SDL_LockAudio();

    *extra_frames = 0;
    sound_muted = false;
    if (sound_available) {
        uint16_t *sound_buffer = vm->create_sound(extra_frames);
        int length = sound_samples;// * sizeof(uint16_t); // stereo
    	if(length > 0){
    		//printf("update:start inputSoundBufferPos:%d outputSoundBufferPos:%d\n",inputSoundBufferPos,outputSoundBufferPos);
    		int maxLength = 0;
    		if(inputSoundBufferPos >= outputSoundBufferPos){
    			maxLength = soundBufferLength - ( inputSoundBufferPos - outputSoundBufferPos) - 1;
    		}else{
    			maxLength = outputSoundBufferPos - inputSoundBufferPos - 1;
    		}
	        for(int index = 0;index < length;index++){
	            soundBuffer[inputSoundBufferPos] = sound_buffer[index];
	            inputSoundBufferPos++;
	        	if(inputSoundBufferPos >= soundBufferLength){
	        		inputSoundBufferPos = 0;
	        	}
	            if(index >= maxLength){
	                break;
	            }
	        }
    		//printf("update:end inputSoundBufferPos:%d outputSoundBufferPos:%d length:%d maxLength:%d\n",inputSoundBufferPos,outputSoundBufferPos,length,maxLength);
    	}
    }
	
	SDL_UnlockAudio();
#endif
}

void OSD::reset_sound(){
	for(int index= 0;index <  soundBufferLength;index++){
        soundBuffer[index] = 0;
    }
    inputSoundBufferPos = 0;
    outputSoundBufferPos = 0;
}


#define DEFAULT_AUDIO_BUFFER	120
#define DEFAULT_AUDIO_UNIT		15
void OSD::initialize_sound(int rate, int samples){
#ifdef ENABLE_SOUND
	inputSoundBufferPos = 0;
	outputSoundBufferPos = 0;
    sound_samples = samples;

	SDL_AudioSpec Desired;
	SDL_AudioSpec Obtained;
	
	SDL_memset(&Desired, 0, sizeof(Desired)); 
	
	printf("samples=%d\n",samples);
	soundBufferLength = samples * 2;
	soundBuffer = (uint16_t*)malloc(soundBufferLength * sizeof(uint16_t));
	memset(soundBuffer,0,soundBufferLength);
	
	// parameter
	Desired.freq = rate;
	Desired.format =  AUDIO_S16SYS;
	Desired.channels = 2;
	Desired.samples = samples;
	Desired.callback = CommonCallback;
	Desired.userdata = (void*)this;

	if(SDL_OpenAudio(&Desired, &Obtained)<0){
		printf("init Sound NG:rate:%d,samples:%d\n",rate,samples);
		return;
	}
  	SDL_PauseAudio(0);
	
	printf("init Sound OK:rate:%d,samples:%d\n",rate,samples);
	soundEnable = true;
	sound_available = true;
#endif
	return ;
}

//
// CommonCallback()
// common callback from SDL
//
void OSD::CommonCallback(void *userdata, Uint8 *stream, int len)
{
	OSD *osd;

	osd = (OSD*)userdata;

	if ((stream != NULL) && (len > 0)) {
		osd->Callback(stream, len);
	}
}

//
// Callback()
// callback to fill buffer
//
void OSD::Callback(Uint8 *stream, int len)
{
    if(inputSoundBufferPos == outputSoundBufferPos){
		return;
    }
		//printf("callbackStart inputSoundBufferPos:%d outputSoundBufferPos:%d\n",inputSoundBufferPos,outputSoundBufferPos);	
	int writeBufferSize = 0;
	if(inputSoundBufferPos > outputSoundBufferPos){
    	writeBufferSize = inputSoundBufferPos - outputSoundBufferPos;
	}else{
		writeBufferSize = soundBufferLength - (outputSoundBufferPos - inputSoundBufferPos);
	}
	//printf("WriteBufferSize:%d Len:%d\n",writeBufferSize,len);
	int zeroBufferSize = 0;
	int framesize = len / 2;
	if(writeBufferSize > framesize){
        writeBufferSize = framesize;
	}
	 Uint16 *frames = (Uint16 *)stream;
	//printf("writeBufferSize: %d inputSoundBufferPos:%d outputSoundBufferPos:%d\n",writeBufferSize, inputSoundBufferPos,outputSoundBufferPos);
    for (int i = 0; i < writeBufferSize; i++) {
        if(soundBuffer[outputSoundBufferPos] <= INT_MAX / 2) {
            frames[i] = (Uint16) soundBuffer[outputSoundBufferPos] / 2 ;
        }else{
           frames[i] = (Uint16)(-soundBuffer[outputSoundBufferPos] / 2);
        }
        outputSoundBufferPos++;
        if(outputSoundBufferPos >= soundBufferLength){
            outputSoundBufferPos = 0;
        }
    }

		
	//printf("callbackEnd inputSoundBufferPos:%d outputSoundBufferPos:%d\n",inputSoundBufferPos,outputSoundBufferPos);
}


void OSD::release_sound(){
	SDL_CloseAudio();
	// free buffer
	if (sample_buffer != NULL) {
		SDL_free(sample_buffer);
		sample_buffer = NULL;
	}
	if (soundBuffer != NULL) {
		free(soundBuffer);
		soundBuffer = NULL;
	}
}