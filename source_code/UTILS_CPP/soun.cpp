#include "hl_types.h"

soun_class soun;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PLAY/STOP
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void soun_class::play_sound(enum_soun en)
{
	static xflt	next_call_time[sound_dim]={0}								;
	if(			next_call_time[en		]>xtim.real_time)return				;
				next_call_time[en		]=xtim.real_time+sound_dur_sec[en]	;

	alSourcei	(al_source[en],AL_LOOPING,0					);
	alSourcePlay(al_source[en]								);
	alSourcef	(al_source[en],AL_PITCH	 ,1					);
	alSourcef	(al_source[en],AL_GAIN	 ,volume_0_100*0.01	);
}

void soun_class::stop_sound(enum_soun en)
{
	alSourceStop(al_source[en]);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// LOAD
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xint UTIL_find4(xmap_class* m,xchr c1,xchr c2,xchr c3,xchr c4)
{
	xchr r1=0;
	xchr r2=0;
	xchr r3=0;
	xchr r4=0;
	while((r1!=c1||r2!=c2||r3!=c3||r4!=c4) && m->can_scan(1)){
		r1=r2;
		r2=r3;
		r3=r4;
		m->BIN_buf_scan(&r4,1);}

	return (m->can_scan(0) && (r1==c1) && (r2==c2) && (r3==c3) && (r4==c4));
}

void soun_class::fill_sound(enum_soun en,string file,xflt extra_buffer_sec)
{
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// LOAD AND CHECK FILE TYPE AND CONTENTS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	string path="RESOURCE/"+file;
	xmap_class m(path);

	if(!check_ext(path,".wav")			){APP_alert("Calling fill_sound with a non-wav!"		,path,"","");	return;}
	if(!m.exists()						){APP_alert("Cannot find a certain WAV file!"			,path,"","");	return;}
	if(!UTIL_find4(&m,'R','I','F','F')	){APP_alert("This is not a RIFF file."					,path,"","");	return;}
	if(!UTIL_find4(&m,'W','A','V','E')	){APP_alert("This is not a WAVE file."					,path,"","");	return;}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// LOAD FORMAT CHUNK
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	#define DWORD	unsigned int	// 32 bits
	#define WORD	unsigned short	// 16 bits

	struct wave_fmt_type
	{
		WORD  wFormatTag;		// format type
		WORD  nChannels;		// number of channels (i.e. mono, stereo...)
		DWORD nSamplesPerSec;	// sample rate
		DWORD nAvgBytesPerSec;	// for buffer estimation
		WORD  nBlockAlign;		// block size of data
		WORD  wBitsPerSample;	// number of bits per sample of mono data
		WORD  cbSize;			// the count in bytes of the size of additional data after cbsize
	};

	if(!UTIL_find4(&m,'f','m','t',' ')	){APP_alert("WAV file has no format chunk."				,path,"","");	return;}
	DWORD dwFmtSize=m.BIN_int_scan();			// format chunk size to come in a moment
	wave_fmt_type		   Format={0};			// format chunk itself of the size listed above
	m.BIN_buf_scan((xchr*)&Format,dwFmtSize);	// scan the format chunk
	if(Format.wFormatTag	!= 1		){APP_alert("The WAV file is not a PCM file!"			,path,"","");	return;}
	if(Format.wBitsPerSample!=16		){APP_alert("The WAV file is not 16-bit."				,path,"","");	return;}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// LOAD DATA CHUNK
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(!UTIL_find4(&m,'d','a','t','a')	){APP_alert("WAVE file has no data chunk."				,path,"","");	return;}
	DWORD dwDataSize=m.BIN_int_scan();			// data chunk size
	if(	  dwDataSize==0L				){APP_alert("WAVE file data chunk has no data."			,path,"","");	return;}
	unsigned char* data_buffer=(unsigned char*)malloc(dwDataSize);
	if(!data_buffer						){APP_alert("Could not allocate memory for data chunk!"	,path,"","");	return;}
	m.BIN_buf_scan((xchr*)data_buffer,dwDataSize);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// LOAD BUFFER INTO XAVION
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	xflt sample_rate	=(xflt)Format.nSamplesPerSec;		// float type-coercion needed to handle samples smaller than one second!
	xflt sample_size	=(xflt)Format.wBitsPerSample/8.0;	// float type-coercion needed to handle samples smaller than one second!
	xflt num_channels	=(xflt)Format.nChannels;			// float type-coercion needed to handle samples smaller than one second!
	sound_dur_sec[en]	=(xflt)dwDataSize/(sample_rate*sample_size*num_channels)+extra_buffer_sec;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// LOAD BUFFER INTO OPENAL
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	ALuint				al_buffer=0		 ;
	alGenBuffers	(1,&al_buffer		);
	alGenSources	(1,&al_source[en]	);
	alBufferData	(al_buffer,(Format.nChannels==1)?((Format.wBitsPerSample==8)?AL_FORMAT_MONO8	:AL_FORMAT_MONO16	):
													 ((Format.wBitsPerSample==8)?AL_FORMAT_STEREO8	:AL_FORMAT_STEREO16	),data_buffer,(ALsizei)dwDataSize,(ALsizei)Format.nSamplesPerSec);

	ALfloat defsourcepos[3]={0,0,1};	// defaults
	ALfloat defsourcevel[3]={0,0,0};	// defaults
	alSourcei (al_source[en],AL_BUFFER	,al_buffer);
	alSourcef (al_source[en],AL_PITCH	,1.0);
	alSourcef (al_source[en],AL_GAIN	,1.0);
	alSourcefv(al_source[en],AL_POSITION,defsourcepos);
	alSourcefv(al_source[en],AL_VELOCITY,defsourcevel);

	free(data_buffer);						// free our data buffer now that it is loaded into openal
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// INIT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void soun_class::init_sound()
{
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// IPAD INIT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	#if IPAD
		#define HW_FREQ	22050

		OSStatus result=AudioSessionInitialize(NULL,NULL,NULL,NULL);
		if(result)
		{
			//printf("Error initializing audio session! %d\n",(xint)result);
		}
		else
		{
			UInt32 category=kAudioSessionCategory_AmbientSound;
			result=AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(category), &category);
			if(result)
			{
				//printf("Error setting audio session category! %d\n",(xint)result);
			}
			else
			{
				Float64 srate=HW_FREQ;
				result=AudioSessionSetProperty(kAudioSessionProperty_PreferredHardwareSampleRate, sizeof(srate), &srate);
				//if(result)printf("Error setting audio rate! %d\n",(xint)result);

				result=AudioSessionSetActive(true);
				//if(result)printf("Error setting audio session active! %d\n",(xint)result);

				UInt32 prop_size=sizeof(srate);
				result=AudioSessionGetProperty(kAudioSessionProperty_PreferredHardwareSampleRate, &prop_size, &srate);
				//printf("Result: %d.  HW at: %lf.\n",(xint)result,srate);
			}
		}

		alcMacOSXMixerOutputRateProcPtr	alcMacOSXMixerOutputRate_=(alcMacOSXMixerOutputRateProcPtr) alGetProcAddress("alcMacOSXMixerOutputRate");
		if(	alcMacOSXMixerOutputRate_)
			alcMacOSXMixerOutputRate_(HW_FREQ);
	#endif

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// OPENAL INIT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	device =alcOpenDevice(NULL);			if(device ==NULL)					{																						return;}
	context=alcCreateContext(device,NULL);	if(context==NULL)					{								alcCloseDevice(device);					device=NULL;	return;}
											if(!alcMakeContextCurrent(context))	{alcDestroyContext(context);	alcCloseDevice(device);	context=NULL;	device=NULL;	return;}

	ALCint major_version,minor_version;
	alcGetIntegerv(NULL,ALC_MAJOR_VERSION,sizeof(major_version),&major_version);
	alcGetIntegerv(NULL,ALC_MINOR_VERSION,sizeof(minor_version),&minor_version);
	alGetError();	// Clear any waiting errors

	ALfloat listenerpos[]={0.0,0.0,0.0};	// 3-d sound options
	ALfloat listenervel[]={0.0,0.0,0.0};	// 3-d sound options
	ALfloat listenerori[]={0.0,0.0,0.0,		// First three are "at"
						   0.0,1.0,0.0};	// Second three are which direction is up!

	alListenerfv(AL_POSITION	,listenerpos);
	alListenerfv(AL_VELOCITY	,listenervel);
	alListenerfv(AL_ORIENTATION	,listenerori);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// CLOSE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void soun_class::done_sound()
{
	for(xint i=0;i<sound_dim;i++){
		ALuint j=al_source[i];
		alDeleteSources(1,&j);}
}
