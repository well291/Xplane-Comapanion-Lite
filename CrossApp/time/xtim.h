#ifndef xtim_h
#define xtim_h

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class xtim_class
{
public:
   ~xtim_class(){}
	xtim_class()
	{
		memset(this,0,sizeof(*this));
		prd_act = 0.05;
		xtim_per_frame();	// do this at construct so the local and zulu time are avail during init! this is used for seeding the random numbers and checking map currency!
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xint zulu_date_int	,locl_date_int	;	// my 2014 10 20 just run together into an int
xint zulu_year		,locl_year		;	// calendar year to run together into megadate
xint zulu_month		,locl_month		;	// start 1 to run together into megadate
xint zulu_day		,locl_day		;	// start 1 to run together into megadate
xint zulu_hour		,locl_hour		;
xint zulu_minute	,locl_minute	;
xdob zulu_second	,locl_second	;		// from midnight

inline xint zulu_time_min_since_midnight	(){return zulu_hour*60.0 + zulu_minute	;}	// from midnight
inline xint zulu_time_hour_since_midnight	(){return zulu_hour						;}	// from midnight

xdob prd_act	;	// real period... will never be 0, and should be double since used to integrate double alt and lon
xdob real_time	;	// real time elapsed since application start
xdob Unix_sec	;	// floating-point seconds since Jan 1 1970, 00:00 UTC

xint puff01	;
xint puff02	;
xint puff05	;
xint puff1	;
xint puff10	;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// HI-ACCURATE INTER-FRAME TIMING
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline xu64 microsecs_tot()
{
	#if IPAD					// Xavion!!!
		xu64 sec_millionths=0;	// millionths of a second since computer-start

		struct timeval tv;
		gettimeofday(&tv,NULL);
		sec_millionths=(tv.tv_sec*1000000LL+tv.tv_usec);

		return sec_millionths;
	#else
		xu64 now;
		Microseconds((UnsignedWide*)&now);
		return now;
	#endif
}

inline xu64 microsecs(){							// this returns microseconds (millionths of a second)
	static xu64 init_call=microsecs_tot();			// the first call to this proc could be anything... so we find it statically
	return		microsecs_tot()-init_call;}			// and look at the delta so we always start at 0 with this proc.

inline xdob time_sec(){
	const xu64 del_micro_xu64	=microsecs();
	const xdob del_micro_xdob	=del_micro_xu64;
	const xdob ret				=del_micro_xdob/1000000.0;	// go from millionths of a second integer to floating-point seconds
	return ret;}

void xtim_per_frame();
};
extern class xtim_class xtim;

#endif
