#ifndef hl_math_h
#define hl_math_h

inline xint sign		(const xint in											){													return (in<0)?-1:1;}
inline xflt sign		(const xflt in											){													return (in<0)?-1:1;}
inline xint intround	(const xflt in											){													return (in<0.0f)?(xint)(in-0.5f):(xint)(in+0.5f);}
inline xflt sqr			(const xflt in											){													return in*in;}
inline xflt signsqr		(const xflt in											){													return (in>=0.0f)?sqr  (in):-sqr  ( in);}
inline xflt signsqrt	(const xflt in											){													return (in>=0.0f)?sqrtf(in):-sqrtf(-in);}
inline xflt pythag		(const xflt p,const xflt q								){													return sqrtf(p*p+q*q	 );}
inline xflt pythag		(const xflt p,const xflt q,const xflt r					){													return sqrtf(p*p+q*q+r*r);}
inline xflt pythag_sqr  (const xflt p,const xflt q								){													return (p*p+q*q);}
inline xflt pythag_sqr	(const xflt p,const xflt q,const xflt r					){													return (p*p+q*q+r*r);}
inline xflt fltnear		(	   xflt in,const xflt nearest						){in/=nearest;	in=intround(in);					return in*nearest;}
inline xint intnear		(const xint in,const xint nearest						){													return (xint)fltnear((xflt)in,(xflt)nearest);}
inline xint intmin2		(const xint x1,const xint x2							){													return (x1<x2)?x1:x2;}
inline xint intmax2		(const xint x1,const xint x2							){													return (x1>x2)?x1:x2;}
inline xbyt bytmax2		(const xbyt x1,const xbyt x2							){													return (x1>x2)?x1:x2;}
inline xdob dobmin2		(const xdob x1,const xdob x2							){													return (x1<x2)?x1:x2;}
inline xdob dobmax2		(const xdob x1,const xdob x2							){													return (x1>x2)?x1:x2;}
inline xflt fltmin2		(const xflt x1,const xflt x2							){													return (x1<x2)?x1:x2;}
inline xflt fltmax2		(const xflt x1,const xflt x2							){													return (x1>x2)?x1:x2;}
inline xint intmin3		(const xint x1,const xint x2,const xint x3				){xint x4=(x1<x2)?x1:x2;							return (x3<x4)?x3:x4;}
inline xint intmax3		(const xint x1,const xint x2,const xint x3				){xint x5=(x1>x2)?x1:x2;							return (x3>x5)?x3:x5;}
inline xflt fltmin3		(const xflt x1,const xflt x2,const xflt x3				){xflt x4=(x1<x2)?x1:x2;							return (x3<x4)?x3:x4;}
inline xflt fltmax3		(const xflt x1,const xflt x2,const xflt x3				){xflt x5=(x1>x2)?x1:x2;							return (x3>x5)?x3:x5;}
inline xflt fltmin4		(const xflt x1,const xflt x2,const xflt x3,const xflt x4){xflt x5=(x1<x2)?x1:x2;	xflt x6=(x3<x4)?x3:x4;	return (x5<x6)?x5:x6;}
inline xflt fltmax4		(const xflt x1,const xflt x2,const xflt x3,const xflt x4){xflt x5=(x1>x2)?x1:x2;	xflt x6=(x3>x4)?x3:x4;	return (x5>x6)?x5:x6;}
inline xint intmin4		(const xint x1,const xint x2,const xint x3,const xint x4){xint x5=(x1<x2)?x1:x2;	xint x6=(x3<x4)?x3:x4;	return (x5<x6)?x5:x6;}
inline xint intmax4		(const xint x1,const xint x2,const xint x3,const xint x4){xint x5=(x1>x2)?x1:x2;	xint x6=(x3>x4)?x3:x4;	return (x5>x6)?x5:x6;}
inline xflt fltmin5		(const xflt x1,const xflt x2,const xflt x3,const xflt x4,const xflt x5	){xflt x6=fltmin3(x1,x2,x3);xflt x7=fltmin2(x4,x5);	return (x6<x7)?x6:x7;}
inline xflt fltmax5		(const xflt x1,const xflt x2,const xflt x3,const xflt x4,const xflt x5	){xflt x6=fltmax3(x1,x2,x3);xflt x7=fltmax2(x4,x5);	return (x6>x7)?x6:x7;}

inline xint dobrange(const xdob x,const xdob x1,const xdob x2){
if(x<x1)return xfals;
if(x>x2)return xfals;
		return xtrue;}

inline xint intrange(const xint x,const xint x1,const xint x2){
	if(x<x1)return xfals;
	if(x>x2)return xfals;
			return xtrue;}

inline xint intbox(const xint x,const xint y,const xint x1,const xint y1,const xint x2,const xint y2){
	if(x<x1)return xfals;
	if(x>x2)return xfals;
	if(y<y1)return xfals;
	if(y>y2)return xfals;
			return xtrue;}

inline xint fltrange(const xflt x ,const xflt x1,const xflt x2){
	if(x<x1)return xfals;
	if(x>x2)return xfals;
			return xtrue;}

inline xint fltbox(const xflt x,const xflt y,const xflt x1,const xflt y1,const xflt x2,const xflt y2){
	if(x<x1)return xfals;
	if(x>x2)return xfals;
	if(y<y1)return xfals;
	if(y>y2)return xfals;
			return xtrue;}

inline xflt fltabs(const xflt in){return (in>=0.0f	)?in:-in;}		// TYPICAL case: fabs() takes 6.3% of the CPU time, according to profiling!
inline xint intabs(const xint in){return (in>=0		)?in:-in;}		// going to this proc instead takes 0.1%! 6.2% speed boost with no performance loss! BAM!
inline void int_switch(xint*	a,xint*	b){xint	c=*a;	*a=*b;	*b=c;}
inline void flt_switch(xflt*	a,xflt*	b){xflt	c=*a;	*a=*b;	*b=c;}
inline void chr_switch(xchr*	a,xchr*	b){xchr	c=*a;	*a=*b;	*b=c;}
inline void str_switch(string* a,string* b){string c=*a;	*a=*b;	*b=c;}

inline xint intlim(const xint in,const xint min,const xint max){
	if(in<min)return min;
	if(in>max)return max;
	return in;}

inline xdob doblim(const xdob in,const xdob min,const xdob max){
	if(in<min)return min;
	if(in>max)return max;
	return in;}

inline xflt fltlim(const xflt in,const xflt min,const xflt max){
if(in<min)return min;
if(in>max)return max;
return in;}

inline xint intwrap(xint in,const xint min,const xint max){	// we DON'T send 0 and 360. That is REDUNDANT AT DUE
	xint del=max-min+1;										// NORTH! That is REALLY a range of 361. We send 0->359
	if(del<=0)return min;
	while(in<min)in+=del;
	while(in>max)in-=del;
	return in;}

inline xflt fltwrap(xflt in,const xflt min,const xflt max){	// FLOATING POINT MUST NOT TAKE 0->359, cause that
	xflt del=max-min;										// leaves a whole degree uncharted!!! Argh!
	if(del<=0.0f)return min;
	while(in<min)in+=del;
	while(in>max)in-=del;
	return in;}

inline xdob dobwrap(xdob in,const xdob min,const xdob max){	// FLOATING POINT MUST NOT TAKE 0->359, cause that
	xdob del=max-min;										// leaves a whole degree uncharted!!! Argh!
	if(del<=0.0)return min;
	while(in<min)in+=del;
	while(in>max)in-=del;
	return in;}

inline xdob xdob_extrap(const xdob x1,const xdob y1,const xdob x2,const xdob y2,const xdob x)
{
	if(x1==x2)	return (y1+y2)*0.5;
				return	y1+((y2-y1)/(x2-x1))*(x-x1);
}

inline xflt extrap(const xflt x1,const xflt y1,const xflt x2,const xflt y2,const xflt x)
{
	if(x1==x2)	return (y1+y2)*0.5f;
				return	y1+((y2-y1)/(x2-x1))*(x-x1);
}

inline xflt interp(const xflt x1,const xflt y1,const xflt x2,const xflt y2,const xflt x)
{
	if(x1==x2)	return (y1+y2)*0.5f;
	if(y1< y2)	return fltlim(y1+((y2-y1)/(x2-x1))*(x-x1),y1,y2);	// remember x1 may be greater than x2! happens all the time!
				return fltlim(y1+((y2-y1)/(x2-x1))*(x-x1),y2,y1);	// remember x1 may be greater than x2! happens all the time!
}

inline xflt interp360(const xflt x1,const xflt y1,const xflt x2,const xflt y2,const xflt x)
{
	xflt ratio=interp (x1,0.0 , x2,1.0 , x);	// this lets us do an interp on heading
	xflt delta=fltwrap(y2-y1,-180.0,180.0);		// without a stupid wrap-around problem going from 350 to 10!
	return fltwrap(y1+delta*ratio,0.0,360.0);
}

inline xflt extrap2(const xflt rat_x,
					const xflt rat_y,
					const xflt ya,		// lower left
					const xflt yb,		// lower right
					const xflt yc,		// upper left
					const xflt yd)		// upper right
{
	xflt y2=yc+(yd-yc)*rat_x;
	xflt y1=ya+(yb-ya)*rat_x;
	return  y1+(y2-y1)*rat_y;
}

inline xflt extrap2_360(const xflt rat_x,
						const xflt rat_y,const xflt ya,const xflt yb,const xflt yc,const xflt yd){
	xflt y2=		yc+fltwrap(yd-yc,-180.0,180.0)*rat_x			;
	xflt y1=		ya+fltwrap(yb-ya,-180.0,180.0)*rat_x			;
	return  fltwrap(y1+fltwrap(y2-y1,-180.0,180.0)*rat_y,0.0,360.0)	;}

inline xflt fallout(const xflt in,const xflt lo,const xflt hi){
	if(in<			  lo)return in;
	if(in>			  hi)return in;
	if(in<((lo+hi)*0.5f))return lo;
						 return hi;}

inline xflt closer_assymp(const xflt nowval,const xflt termval,const xflt speedrat){return nowval*(1.0f-speedrat)+termval*speedrat;}
inline xdob closer_assymp_dob(const xdob nowval,const xdob termval,const xdob speedrat){return nowval*(1.0f-speedrat)+termval*speedrat;}

inline xflt closer_360(const xflt nowval,const xflt termval,const xflt ratnew)
{
	xflt delta=	fltwrap(termval-nowval		,-180.0,180.0);
	return		fltwrap(nowval+delta*ratnew	,	0.0,360.0);	// this gives a proper 359->1 wrap
}

inline xflt closer_180(const xflt nowval,const xflt termval,const xflt ratnew)
{
	xflt delta=	fltwrap(termval-nowval		,-180.0,180.0);
	return		fltwrap(nowval+delta*ratnew	,-180.0,180.0);	// this gives a proper -180->180 wrap
}

inline xint fltrange_180(xflt lon,xflt lon1,xflt lon2)		// gotta do this to work where -179/+179 get close
{
	xflt extra_lon=fltwrap( lon-lon1,-180.0,180.0);
	xflt width_lon=fltwrap(lon2-lon1,	0.0,360.0);
	return fltrange(extra_lon,0,width_lon);
}

inline xflt xpow(xflt input,xflt power)
{
		 if(input>0.0f)	return  pow( input,power);	// when do we get negative or zero values? airspeed at zero, rpm at negative as the prop kicks back, stuff like that!
	else if(input<0.0f)	return -pow(-input,power);	// windows machines cannot handle negative input! so i handle it this way... this is a physics sim after all.
	else				return  0.0f;				// windows machines cannot handle 0 input! amazing and strange but true!
}

inline xint get_pow2(xint b)
{
	xint		r =1;
	while(r<b)	r*=2;
	return		r	;
}

inline xint inside_cw_reg(	const xdob x1,const xdob z1,	// we need xdob not xflt to have enough precision!
							const xdob x2,const xdob z2,	// otherwise it fails sometimes on the edge-cases!
							const xdob x ,const xdob z )
{
	xdob dx=x2-x1;
	xdob dz=z2-z1;
	if(dx==0.0 && dz==0.0)return xfals;						// 0-len line? then we are not really inside of it, are we? the math below, though fast, is fooled to think we are always inside a 0-len line... duh-oh!
															// though i test against this very well for terrain, it can still happen with airplane geometry and who-knows-what-else!
	return ( ((x-x1)*dz - (z-z1)*dx) <=0.0 );
}

inline xint inside_cw_reg_lon_lat(	const xdob lon ,const xdob lat ,
									const xdob lon1,const xdob lat1,	// we need xdob not xflt to have enough precision!
									const xdob lon2,const xdob lat2)	// otherwise it fails sometimes on the edge-cases!
{
	return inside_cw_reg(	lon ,-lat ,
							lon1,-lat1,
							lon2,-lat2);
}

inline void normalize(xflt vec[3])
{
	xflt    ilen=1.0/pythag(vec[0],vec[1],vec[2]);
	vec[0]*=ilen;
	vec[1]*=ilen;
	vec[2]*=ilen;
}

inline xdob crs_deg_with_dll(const xdob lon2,const xdob lon1,const xdob lat2,const xdob lat1)
{
	const xdob phi1=		 	 lat1				*degtorad;
	const xdob phi2=		lat2					*degtorad;
	const xdob lonD=fltwrap(lon2-lon1,-180.0,180.0)	*degtorad;

	return atan2( sin(lonD) , (cos(phi1)*tan(phi2) - sin(phi1)*cos(lonD)) )*radtodeg;	// great-circle heading!
}

inline xdob dis_mtr_with_dll(const xdob lon2,const xdob lon1,const xdob lat2,const xdob lat1)
{
	const xdob d_lat=abs(lat1-lat2)*degtorad;
	const xdob d_lon=abs(lon1-lon2)*degtorad;
	const xdob a	=										sqr(sin(d_lat*0.5))
					+cos(lat1*degtorad)*cos(lat2*degtorad)*	sqr(sin(d_lon*0.5));

	return earth_diameter_m * asin(sqrt(a));	// great-circle distance!
}

inline xflt pow10(xint i){	// a little faster than pow
	xflt			ret = 1.0;
	while(i<0){i++; ret/=10.0;}
	while(i>0){i--; ret*=10.0;}
	return ret;}

inline void wrl_to_any(	const xdob  x_wrl,const	xdob  y_wrl,const	xdob  z_wrl,
							  xdob* x_any,		xdob* y_any,		xdob* z_any,
						const xdob sin_psi,const xdob cos_psi,
						const xdob sin_the,const xdob cos_the,
						const xdob sin_phi,const xdob cos_phi)
{
	xdob x_psi=x_wrl*cos_psi + z_wrl*sin_psi;
	xdob y_psi=y_wrl						;
	xdob z_psi=z_wrl*cos_psi - x_wrl*sin_psi;
	xdob x_the=x_psi						;
	xdob y_the=y_psi*cos_the + z_psi*sin_the;
	xdob z_the=z_psi*cos_the - y_psi*sin_the;
		*x_any=x_the*cos_phi - y_the*sin_phi;
		*y_any=y_the*cos_phi + x_the*sin_phi;
		*z_any=z_the						;
}

#endif
