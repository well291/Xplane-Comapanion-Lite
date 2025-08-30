#ifndef soun_h
#define soun_h

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// INCLUDES AND ENUMS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
#if IPAD
	#include <OpenAL/al.h>						// audio library
	#include <OpenAL/alc.h>						// audio library context
	#include <OpenAL/oalMacOSX_OALExtensions.h>
	#include <AudioToolbox/AudioToolbox.h>
#else
	#include <AL/al.h>							// audio library
	#include <AL/alc.h>							// audio library context
#endif

enum enum_soun
{
	soun_click=0,
	sound_dim
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// CLASS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class soun_class
{
public:
   ~soun_class(){}
	soun_class()
	{
		device =NULL;
		context=NULL;

		al_source.clear	(			);
		al_source.resize(sound_dim,0);

		memset(sound_dur_sec,0,sizeof(sound_dur_sec));
		volume_0_100=100.0;
	}

private:
	ALCdevice*		device					;
	ALCcontext*		context					;
	vector <ALuint>	al_source				;
	xflt			sound_dur_sec[sound_dim];

public:
	xflt volume_0_100;
	void play_sound(enum_soun en);
	void stop_sound(enum_soun en);
	void fill_sound(enum_soun en,string file,xflt extra_buffer_sec=0.0);
	void init_sound();
	void done_sound();

	#if IPAD
		inline void interrupt_start(){alcMakeContextCurrent(NULL	);}
		inline void interrupt_stop (){alcMakeContextCurrent(context	);}
	#endif
};
extern soun_class soun;
#endif
