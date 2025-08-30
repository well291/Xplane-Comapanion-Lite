#ifndef hl_constants_h
#define hl_constants_h

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// TYPEDEF VARIABLE TYPES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
typedef int		xint;				// always 32-bits.. gotta do it this way since apple has a type xint in xpc.h! yup!
#define xdob	double				// good enough to store adeqate precision with lat/lon only, not x/y/z
#define xflt	float				// standard float
#define xsho	unsigned short		// 0 to 65535
#define xssh	short				// -32768 to 32767
#define xchr	char				// -127 to 128, though i can use it 0->256, it will print -127 to 128
#define xbyt	unsigned char		// 0-255
#define xuin	unsigned int		// always 32 bits
#define xu16	uint16_t			// unsigned
#define xu64	unsigned long long	// always 64 bits
#define WORD	unsigned short		// a few windows formats like .wav and .dds need this.
#define DWORD	unsigned int		// a few windows formats like .wav and .dds need this.
#define xfals	0					// there are no booleans since they are different size mac and windows, befuddling internet comms and file-trading,
#define xtrue	1					// so we use ints and compare to values zero or one here.

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// STRING DIMS THAT NEED TO BE THE SAME BETWEEN APPS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
const xint   ICAO_dim	=  8;	// XAVION!
const xint     id_dim8	=  8;	// 6 chars+null. EH0001, for example, is 6 chars! it has no ICAO, but has a local code, so we still use it! ALSO WE NEED A MULTIPLE OF 8 FOR THE CUSTOM AIRPORT PREFS FILE TO WORK, WHICH ALIGNS TO 8 BYTE BOUNDS!
const xint   name_dim32	= 32;	// ramp-starts
const xint   name_dim40	= 40;	// misc-use names, locked to 40 by failure name
const xint   name_dim80	= 80;	// misc-use names, locked to 80 by failure name
const xint   name_dim64	= 64;	// misc-use names, locked to 64 by Xavion custom airport name
const xint    str_dim	=256;	// misc-use strings
const xint ip_str_dim	= 16;	// IP address in string form

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PHYSICS CONSTANTS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
const xdob dob_radtodeg		= (xdob)180.0/(xdob)3.14159265358979	;
const xdob dob_degtorad		= (xdob)3.14159265358979/(xdob)180.0	;
const xdob dob_pi2			= (xdob)3.14159265358979*(xdob)  2.0	;
const xdob dob_pi			= (xdob)3.14159265358979				;

const xdob cel_to_kel		=	  273.15;
const xdob earth_sl_T_cel	=	   15.0;
const xdob earth_sl_P_pas	=  101325.0;
const xdob earth_sl_P_hg	=	   29.92;
const xdob earth_sl_r_met	=		1.225;
const xdob earth_sl_a_msc	=     340.29;
const xdob con_pi			=       3.14159265358979;
const xdob con_pid			=       3.14159265358979;
const xdob con_pim2			=       3.14159265358979*2.0;	// you know, pi, but times 2!
const xdob con_pim2d		=       3.14159265358979*2.0;	// you know, pi, but times 2!
const xdob con_pid2			=       3.14159265358979/2.0;	// you know, pi, but times 2!
const xdob con_pid2d		=       3.14159265358979/2.0;	// you know, pi, but times 2!
const xdob con_R			=     287.058;
const xdob con_g			=       3.986012e14/(6378145.0*6378145.0);	// used for conversions & load factor... based on earth standard 9.81...

const xdob sm_to_ft = 5280.0;								const xdob ft_to_sm = 1.0/sm_to_ft;
const xdob degtonm  = 60.0;									const xdob nmtodeg  = 1.0/60.0    ;
const xdob kmtosm   = 0.621371;								const xdob smtokm   = 1.0/kmtosm  ;
const xdob galtolit = 3.785412;								const xdob littogal = 1.0/galtolit;	// gallons to liters for g1000
const xdob lbstokgs = 0.4535925;							const xdob kgstolbs = 1.0/lbstokgs;	// pounds to kilograms in one g		for weights/masses
const xdob fttomtrs = 0.3048;								const xdob mtrstoft = 1.0/fttomtrs;	// feet to meters					for arms
const xdob hptowatt = 745.7;								const xdob watttohp = 1.0/hptowatt;	// horsepower to watts				for engine
const xdob radtodeg = 180.0/con_pi;							const xdob degtorad = 1.0/radtodeg;	// degrees to radians				for trig
const xdob ktstofps = 6076.1155/3600.0;						const xdob fpstokts = 1.0/ktstofps;	// knots to feet per sec			for shuttle
const xdob ktstomsc = ktstofps*fttomtrs;					const xdob msctokts = 1.0/ktstomsc;	// knots to meters per sec			for airspeed
const xdob rpmtorsc = 360.0/60.0*degtorad;					const xdob rsctorpm = 1.0/rpmtorsc;	// rpm to radians per sec			for propellor
const xdob fpmtomsc = fttomtrs/60.0;						const xdob msctofpm = 1.0/fpmtomsc;	// feet per min to meters per sec	for vertical speed
const xdob nmtomtrs = 6076.1155*fttomtrs;					const xdob mtrstonm = 1.0/nmtomtrs;	// nm to meters						for plate and enrt
const xdob smtomtrs = 5280.0000*fttomtrs;					const xdob mtrstosm = 1.0/smtomtrs;	// sm to meters						for plate and enrt
const xdob sftosmtr = fttomtrs*fttomtrs;					const xdob smtrtosf = 1.0/sftosmtr;	// square feet to square meters		for wing areas
const xdob lbstontn = lbstokgs*con_g;						const xdob ntntolbs = 1.0/lbstontn;	// pounds to newtons				for forces
const xdob ftlbtonm = lbstontn*fttomtrs;					const xdob nmtoftlb = 1.0/ftlbtonm;	// foot pounds to newton meters		for torques
const xdob endtomed = con_g*lbstokgs/pow(fttomtrs,(xdob)4);	const xdob medtoend = 1.0/endtomed;	// english density to met den		for densities
const xdob enptomep = lbstontn/sftosmtr;					const xdob meptoenp = 1.0/enptomep;	// english press to metric pres		for dynamic pressure
const xdob enItomtI = sftosmtr*lbstokgs;					const xdob mtltoenl = 1.0/enItomtI;	// square ft lb to square meter kg	for prop
const xdob pasto_hg = earth_sl_P_hg/earth_sl_P_pas;			const xdob hg_topas = 1.0/pasto_hg;	// nautical to statute miles		for airspeed kt/mph
const xdob milto_hg = earth_sl_P_hg/earth_sl_P_pas*100.0;	const xdob hg_tomil = 1.0/milto_hg;	// pascals to hg
const xdob nm_to_sm = 6076.1155/5280.0;						const xdob sm_to_nm = 1.0/nm_to_sm;	// millibars to hg
const xdob lbhtokgs = lbstokgs/3600.0;						const xdob kgstolbh = 1.0/lbhtokgs;	// kg.sec to lb/hr					for fuel flow
const xdob psitopas = 6895.0;								const xdob pastopsi = 1.0/psitopas;	// psi to pascals
const xdob ktstomph = 6076.1155/5280.0;						const xdob mphtokts = 1.0/ktstomph;
const xdob ktstokph = nmtomtrs*0.001;						const xdob kphtokts = 1.0/ktstokph;
const xdob msctomph = msctokts*nm_to_sm;					const xdob mphtomsc = 1.0/msctomph;
															const xdob kgstopph = kgstolbs*3600.0;// for jet engine fuel burn
const xdob degtomtr = degtonm*nmtomtrs;
const xdob mtrtodeg = 1.0/degtomtr;

const xdob earth_diameter_m = 360.0*degtonm*nmtomtrs/con_pi;

inline xdob C_to_F(const xdob C){return (C     )*1.8+32.0	;}
inline xdob F_to_C(const xdob F){return (F-32.0)/1.8		;}

const xflt lb_per_gallon_prp = 5.87;	// 5.87 for props, 6.8 for jets... cessna swears this is 5.87 lb per gallon!
const xflt lb_per_gallon_jet = 6.80;	// 5.87 for props, 6.8 for jets
const xflt kg_per_liter_prp	 = 0.70;
const xflt kg_per_kiter_jet	 = 0.79;

enum port_enum
{
	port_NULL												=    0,

	port_beacon_sends_from									=49706,
	port_beacon_listens_on									=49707,

	port_PX4_sends_from										= 3009,
	port_PX4_listens_on										= 3009,

	port_ConPad_sends_from									=49001,
	port_ConPad_listens_on									=48003,

	port_SkyOS_sends_from									=30002,
	port_SkyOS_listens_on									=30001,

	port_XPlane_listens_on									=49000,

	port_Xavion_sends_from									=49003,
	port_Xavion_listens_on_AHRS_G							=46400,	// hardware sends to.
	port_Xavion_listens_on_iLevil							=43211,	// hardware sends to.
	port_Xavion_listens_on_Clarity_Stratux_Stratus_uAvionix	= 4000,	// hardware sends to. X-Plane sends to this as well for ADSB weather.
	port_Xavion_listens_on_Xplorer							=49100,	// hardware sends to.
	port_Xavion_listens_on_XPlane							=48002,	// X-Plane sends to this. this is built into X-Plane.
	port_Xavion_listens_on_PX4								=45000,	// hardware sends to.
	port_Xavion_listens_on_test								=45010,	// hardware sends to.

	port_Stratus_listens_on									=41500,	// hardware listens on to change modes between foreflight and public
	port_iLevil_listens_on									= 2000,	// hardware listens on to run autopilot when doing iLevil autopilot

	port_TECH_sends_from									= 2004,
	port_TECH_listens_on									=port_Xavion_listens_on_XPlane	// emulate Xavion
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// THESE ARE AUTOPILOT MODE BUTTON COLOR ENUMS THAT MUST BE THE SAME FOR XAVION, PFD-MAP, AND AUTOPILOT, SINCE THEY ARE AUTOPILOT MODES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
enum alert_enum
{
	alert_not_ready=0		,

	alert_button_off		,
	alert_button_on			,

	alert_app_menu_off		,
	alert_app_menu_on		,

	alert_tab_menu_off		,
	alert_tab_menu_on		,

	alert_otto_off			,
	alert_otto_on			,
	alert_alt_arm			,

	alert_red_button_on		,	// emer on
	alert_red_button_miss	,	// miss the approach

	alert_runway_number		,
	alert_weather_scale
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// TYPEDEF ALERTS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void _APP_alert(const xchr* func,const xchr* file,const xint line,const string& s1="",const string& s2="",const string& s3="",const string& s4="");

#define APP_alert(__A,__B,__C,__D) _APP_alert(__func__,__FILE__,__LINE__,__A,__B,__C,__D)

inline xint check_nan(const xflt val){return(isnan(val) || isinf(val));}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// CHECKS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
#if DEV
	inline void _dev_alert_OGL(const xchr* from_str,const xchr* func,const xchr* file,const xint line)	// WE NEED CHR* NOT STR* FOR SPEED! ANY TEXT I TYPE IS BORN AS A CHR*! CONVERTINT TO AN STR* IS SLOW!
	{
		const xint	oglerr =glGetError();	// THIS ROUTINE ALWAYS RETURNS ERROR IF IT IS INSIDE A QUAD-LOOP OR SIMILAR! SO WRAP EVERYTHING UP BEFORE CALLING IT!
		if(			oglerr!=GL_NO_ERROR)
			_APP_alert(func,file,line,"dev_alert_OGL!","","","");
	}

	inline void _dev_alert_nan(const xflt val,const xchr* func,const xchr* file,const xint line)
	{
		if(isnan(val) || isinf(val))	// can't return a value here os we can't compile opt builds that don't have this
			_APP_alert(func,file,line,"dev_alert_nan!","","","");
	}

	inline void _dev_assert(const xint val,const xchr* func,const xchr* file,const xint line)
	{
		if(!val)						// can't return a value here os we can't compile opt builds that don't have this
			_APP_alert(func,file,line,"dev_assert!","","","");
	}

	#define dev_alert_OGL(__X) _dev_alert_OGL(__X,__func__,__FILE__,__LINE__)	// I CAN NOT PUT ANY SPACES IN THE #DEFINE OR IT WON'T COMPILE! AMAZING BUT TRUE!
	#define dev_alert_nan(__X) _dev_alert_nan(__X,__func__,__FILE__,__LINE__)	// I CAN NOT PUT ANY SPACES IN THE #DEFINE OR IT WON'T COMPILE! AMAZING BUT TRUE!
	#define dev_assert(__X) _dev_assert(__X,__func__,__FILE__,__LINE__)			// I CAN NOT PUT ANY SPACES IN THE #DEFINE OR IT WON'T COMPILE! AMAZING BUT TRUE!
#else
	#define dev_alert_OGL(__X)													// I CAN NOT PUT ANY SPACES IN THE #DEFINE OR IT WON'T COMPILE! AMAZING BUT TRUE!
	#define dev_alert_nan(__X)													// I CAN NOT PUT ANY SPACES IN THE #DEFINE OR IT WON'T COMPILE! AMAZING BUT TRUE!
	#define dev_assert(__X)														// I CAN NOT PUT ANY SPACES IN THE #DEFINE OR IT WON'T COMPILE! AMAZING BUT TRUE!
#endif

#endif

