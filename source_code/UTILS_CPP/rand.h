#ifndef rand_h
#define rand_h

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SIMPLE RAND
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline xint intrand(const xint min,const xint max)
{
	dev_alert_nan(min);
	dev_alert_nan(max);
	if(max <min)APP_alert("int rand range negative!","","","");
	if(min==max)return min;	// %0 is the same as /0... so if the range is 0 then just return the single number we can

	xuin randval=rand()%(max-min+1);
	xint ret=min+randval;
	return ret;
}

inline xflt fltrand(const xflt min,const xflt max)
{
	dev_alert_nan(min);
	dev_alert_nan(max);
	return min+rand()/(xflt)RAND_MAX*(max-min);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PERLIN NOISE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
const  xuin randim1	=100			;	// use unsigned here to avoid negative array access when % by this
const  xuin randim2	=randim1*randim1;	// use unsigned here to avoid negative array access when % by this
extern xflt rand_rat[randim2]		;	// seed random numbers

inline xflt interp_rat	(const xflt a,const xflt b,const xflt rat){return a*(1.0f-rat)+b*rat;}
inline xflt interp_noise(xflt x,xflt y,const xint seed)
{
	dev_assert(seed>=0);

	x=fltabs(x);														dev_alert_nan(x);		// we pass in locations here from time to time, which may be negative, so straighten that out.
	y=fltabs(y);														dev_alert_nan(y);

	const xint integer_X=(xint)x;										dev_alert_nan(integer_X);
	const xint integer_Y=(xint)y;										dev_alert_nan(integer_Y);

	const xflt fractional_X=x-integer_X;								dev_alert_nan(fractional_X);
	const xflt fractional_Y=y-integer_Y;								dev_alert_nan(fractional_Y);
																								// CANNOT USE <<12 HERE INSTEAD OF *8932! IT MESSES SOMETHING UP... MAY BE SIMPLY A NEED FOR BETTER () OR SOMETHING, BUT IT DID NOT MAKE ANY NOTICEABLE SPEED-UP IN GENERATION-TIME TO DO IT!
	const xuin seed1=(integer_X  +(integer_Y  )*randim1+seed)%randim2;	dev_alert_nan(seed1);	// THESE SEED GUYS MUST BE UNSIGNED!!!!!!
	const xuin seed2=(integer_X+1+(integer_Y  )*randim1+seed)%randim2;	dev_alert_nan(seed2);	// THE EXPRESSIONS WE USE CAN OVERFLOW AND EVALUATE TO NEGATIVE NUMBERS!
	const xuin seed3=(integer_X  +(integer_Y+1)*randim1+seed)%randim2;	dev_alert_nan(seed3);	// IF I PASS IN NEGATIVE INPUT TO THE RANDRAT [] THEN OF COURSE WE CRASH!
	const xuin seed4=(integer_X+1+(integer_Y+1)*randim1+seed)%randim2;	dev_alert_nan(seed4);	// SO DO NOT PASS THIS INPUT DIRECTLY INTO RANDRAT OR WE CRASH ON A LARGE INPUT!
																								// APPLY THIS TO AN UNSIGNED INT SO WE ALWAYS HAVE POSITIVE ARRAY INDEXING EVEN IF WE OVERFLOW!
	const xflt v1=rand_rat[seed1];	dev_assert(fltrange(v1,0.0,1.0));	dev_alert_nan(v1);
	const xflt v2=rand_rat[seed2];	dev_assert(fltrange(v2,0.0,1.0));	dev_alert_nan(v2);
	const xflt v3=rand_rat[seed3];	dev_assert(fltrange(v3,0.0,1.0));	dev_alert_nan(v3);
	const xflt v4=rand_rat[seed4];	dev_assert(fltrange(v4,0.0,1.0));	dev_alert_nan(v4);

	const xflt i0=interp_rat(v1,v2,fractional_X);						dev_alert_nan(i0);
	const xflt i1=interp_rat(v3,v4,fractional_X);						dev_alert_nan(i1);
	const xflt i2=interp_rat(i0,i1,fractional_Y);						dev_alert_nan(i2);	

	return i2;	// 0.0->1.0
}

inline xflt interp_noise_smooth_2d(const xflt x,const xflt y,const xint seed)
{
	return	interp_noise(x* 1.0,y* 1.0 , seed   )*1.0+
			interp_noise(x* 2.0,y* 2.0 , seed* 2)*0.5+
			interp_noise(x* 4.0,y* 4.0 , seed* 4)*0.25+
			interp_noise(x* 8.0,y* 8.0 , seed* 8)*0.125+
			interp_noise(x*16.0,y*16.0 , seed*16)*0.0625+
			interp_noise(x*32.0,y*32.0 , seed*32)*0.03125+
			interp_noise(x*64.0,y*64.0 , seed*64)*0.015625;
}

#endif
