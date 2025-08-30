#include "hl_types.h"

xtim_class xtim;

void xtim_class::xtim_per_frame()
{
	time_t		zulu_time_t  =time(NULL)				;	// number seconds since Unix Epoch
	time_t		locl_time_t  =time(NULL)				;	// number seconds since Unix Epoch
	struct tm*	zulu_time_ptr=   gmtime(&zulu_time_t)	;	// turn this to local time.
	struct tm*	locl_time_ptr=localtime(&locl_time_t)	;	// turn this to local time.

	zulu_year		=1900+	zulu_time_ptr->tm_year		;	// raw data: years since 1900
	zulu_month		=   1+	zulu_time_ptr->tm_mon		;	// raw data: months since January [0-11]
	zulu_day		=		zulu_time_ptr->tm_mday		;	// raw data: day of the month [1-31]
	zulu_hour		=		zulu_time_ptr->tm_hour		;
	zulu_minute		=		zulu_time_ptr->tm_min		;
	zulu_second		=		zulu_time_ptr->tm_sec		;

	locl_year		=1900+	locl_time_ptr->tm_year		;	// raw data: years since 1900
	locl_month		=   1+	locl_time_ptr->tm_mon		;	// raw data: months since January [0-11]
	locl_day		=		locl_time_ptr->tm_mday		;	// raw data: day of the month [1-31]
	locl_hour		=		locl_time_ptr->tm_hour		;
	locl_minute		=		locl_time_ptr->tm_min		;
	locl_second		=		locl_time_ptr->tm_sec		;

	#if TEST_NEXT_MONTH
		zulu_month+=TEST_NEXT_MONTH;
		locl_month+=TEST_NEXT_MONTH;
	#endif

	zulu_date_int	=zulu_year	*10000	// data is good until midnight of the expiration day
					+zulu_month	*100	// this matters, because SA releases new data on the SAME DAY the old data expires!
					+zulu_day	*1;		// so use local not zulu... that is what the data expiration is based on!

	locl_date_int	=locl_year	*10000	// data is good until midnight of the expiration day
					+locl_month	*100	// this matters, because SA releases new data on the SAME DAY the old data expires!
					+locl_day	*1;		// so use local not zulu... that is what the data expiration is based on!

	const	xdob min_prd		=1.0/10000.0				;	// I've seen 1000 fps in the setup screen!
	const	xdob max_prd		=1.0/	 1.0				;
	static	xdob dob_prev_time	=time_sec()					;
			xdob dob_del		=time_sec()-dob_prev_time	;	dob_del=doblim(dob_del,min_prd,max_prd);	prd_act=closer_assymp_dob(prd_act,dob_del,0.05);
				 dob_prev_time	=time_sec()					;

	real_time += prd_act;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// APP-REFERENCE TIME
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	using namespace std::chrono;
	auto tp				=system_clock::now();
	Unix_sec=tp.time_since_epoch().count()/(xdob)1000000.0;

	{static xflt time01=0.0;	time01+=prd_act;	puff01=xfals;	if(time01>0.0){time01-=10.0;	puff01=xtrue;}}
	{static xflt time02=0.0;	time02+=prd_act;	puff02=xfals;	if(time02>0.0){time02-= 5.0;	puff02=xtrue;}}
	{static xflt time05=0.0;	time05+=prd_act;	puff05=xfals;	if(time05>0.0){time05-= 2.0;	puff05=xtrue;}}
	{static xflt time1 =0.0;	time1 +=prd_act;	puff1 =xfals;	if(time1 >0.0){time1 -= 1.0;	puff1 =xtrue;}}
	{static xflt time10=0.0;	time10+=prd_act;	puff10=xfals;	if(time10>0.0){time10-= 0.1;	puff10=xtrue;}}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// THIS TO KEEP COUT OUT OF SCI NOT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	static xint inited=0;
	if(!inited)
	{
		inited=1;
		cout << fixed << showpoint;
	}
}
