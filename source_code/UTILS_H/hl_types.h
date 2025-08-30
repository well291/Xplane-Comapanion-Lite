#ifndef hl_types_h
#define hl_types_h

const char vers_string[]="Control Pad 1.03";

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// DEFS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
#define TEST_OZ_INIT	0	// my perf-testing: shows every call to test init
#define TEST_OZ_SHARK	0	// my perf-testing: shows on puff 1 to track perf or over time
#define NET_OZ_DIAG		0	// my network diagnostics

#define SUPPORT_1208	1

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SYSTEM INCLUDES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
#include <OpenGLES/ES1/gl.h>	// open-gl spec from 1.0 through 1.1
#include <OpenGLES/ES1/glext.h>	// open-gl spec for 1.2, which is defined in a different file (glext.h), beleive it or not. this is correct!
#include <iterator>
#include <memory>				// Ben sez: GCC is really tweaky on this!
#include <vector>				// for my dimensionless vectors, good for apt/nav/awy/fix so robin is not limited!
#include <map>					// "maps" - like a hash table, but sorted - let's us rapidly get all items BETWEEN two values.
#include <list>
#include <set>					// for stuff.
#include <time.h>				// for timing
#include <math.h>				// for, you know, math
#include <stdlib.h>				// for abs and rand
#include <stdio.h>				// file io
#include <fstream>				// for ifstream and ofstream				NO .H FOR THE NEW INCLUDE WE ARE USING NOW!
#include <iomanip>				// for numerical output formatting			NO .H FOR THE NEW INCLUDE WE ARE USING NOW!
#include <string>				// for "memset" call! (and strlen function)	NO .H FOR THE NEW INCLUDE WE ARE USING NOW!
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <iostream>

#ifdef __OBJC__
	#import <Foundation/Foundation.h>
	#import <UIKit/UIKit.h>
	#include <Availability.h>	// Check the iphone SDK version - it comes out as a macro. If we are not compiling against 2.0 (the OLDEST) version, yell and die.
#endif
using namespace std;

#if __GNUC__			// gnuc is the x-code compiler
	#define GCC_VERSION (__GNUC__			* 10000 \
						+ __GNUC_MINOR__	*	100 \
						+ __GNUC_PATCHLEVEL__)

	#if GCC_VERSION >= 40300
		#include <backward/hash_fun.h>
		#include <backward/hash_map>
	#else
		#include <unordered_map>
	#endif

	using namespace	std;	// DEC THIS TO GET THE NEW IOS FUNCTIONS IN fstream, iomanip, and string, which are all new, unlike the old fstream.h, iomanip.h, and string.h
#endif

#define new_achievements 1	// tell the cloud score saving to be turned on... chris needs this for that

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// OGL COMPATIBILITY MUCK
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
#include <OpenGLES/ES2/gl.h>
typedef float GLdouble;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// MACROS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void APP_alert(string s1,string s2="",string s3="",string s4="");

#include "hl_constants.h"
#include "hl_cross_app.h"
#include "hl_string.h"
#include "sock_TCP.h"			// TCP sockets for database updates:fast and guaranteed, used for init
#include "sock_UDP.h"			// UDP sockets for in-flight updates: small bytes that we do not wait for
#include "xtim.h"
#include "shar.h"
#include "xmap.h"
#include "colr.h"
#include "becn.h"

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// MY CONSTANTS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
const xflt font_1_dy=  20;				// art asset height
const xflt font_2_dy=  25;				// art asset height
const xflt font_3_dy=  30;				// art asset height
const xflt font_4_dy=  45;				// art asset height
const xflt  butn_hgt=  72;				// height of the button art asset. not used for spacing, but the height of the art asset itself
const xflt label_off=  60;				// this is simply scooting the text down underneath the circle direction and odometer mumerical interfaces

const xflt ui_sep_pix=  2;				// small separatation between keys and buttons and stuff
const xflt    butn_dy= 72+ui_sep_pix;	// and menu separation based on that

const xint  net_strDIM	= 150;			// used for sync with x-plane
const xint TEXT_DIM		=  92;			// NUMBER OF TEXT-FONT SLOTS to allocate spacing for in RAM in the color char-width look-up
const xint  TEX_DIM		=4096;			// 4096 for planet tex

const xint lat_secs		=  8					;
const xint lon_secs		= 12					;
const xint hires_dem_di	=300					;	// we currently have 4 gig of allowable app store size
const xint hires_dem_dj	=400					;	// halfway between cos(0) and cos(60)
const xint arr_i_DIM	=hires_dem_di*lon_secs	;	// use ZERO TO THIS DIM MINUS ONE
const xint arr_j_DIM	=hires_dem_dj*lat_secs	;	// use ZERO TO THIS DIM MINUS ONE

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ENUMS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
enum click_type_enum
{
	click_type_click=0	,
	click_type_drag		,
	click_type_release
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// VARIABLES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class apps_class
{
public:
   ~apps_class(){}
	apps_class()
	{
		memset(this,0,sizeof(*this));
		brite_0_100=100.0;
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// WINDOW AND MOUSE-CLICK GENERAL STUFF
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xint init_is_done,review_done,got_ram_warn;	// iOS starts calling per-frame before the init is done! really!!! so we have to use this check to avoid that.
xflt brite_0_100;

private:
xint private_mou_clicked	[2];
xint private_mou_dragging	[2];
xint private_mou_released	[2];

public:
inline void set_mou_clicked (const xint m,const xint tf	){		 private_mou_clicked	[m]=tf	;}
inline void set_mou_dragging(const xint m,const xint tf	){		 private_mou_dragging	[m]=tf	;}
inline void set_mou_released(const xint m,const xint tf	){		 private_mou_released	[m]=tf	;}
inline xint		mou_clicked (const xint m				){return private_mou_clicked	[m]		;}
inline xint		mou_dragging(const xint m				){return private_mou_dragging	[m]		;}
inline xint		mou_released(const xint m				){return private_mou_released	[m]		;}

xflt mou_drag_x	[2],mou_drag_y [2]	;	// mouse current x and y... allowing 2 mouse-clicks at once. This is where the finger is now AS we drag (or click)
xflt mou_drag_dx[2],mou_drag_dy[2]	;	// dragging d x and y per frame!
xflt dragged_pix[2],mou_dis_rat		;	// total drag pix since screen-touch and pinch-zoom distance ratio modded this frame

xint mouse_box_internal	(const xflt x1,const xflt y1,const xflt x2,const xflt y2,const click_type_enum click_type);
xint mouse_box_click	(const xflt x1,const xflt y1,const xflt x2,const xflt y2);
xint mouse_box_drag		(const xflt x1,const xflt y1,const xflt x2,const xflt y2);
xint mouse_box_release	(const xflt x1,const xflt y1,const xflt x2,const xflt y2);
xint mouse_btn_slide	(const xint x1,const xint y1,const xint x2,const xint y2,void* new_var,const xint reset=0);	// grab and hold for sliding

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// APP STUFF
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void apps_event	(xint et,xint m);
void apps_frame	();
void apps_save	();
void apps_init	(void* ref,xint var);

void* gAppDelegateRef;
};
extern apps_class apps;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
extern string file_path_prfs;
extern string file_path_sits;
extern string alert_message1;
extern string alert_message2;
extern string alert_message3;
extern string alert_message4;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// IPAD SENSORS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xflt my_battery_level_from_cpp	(						);
xflt my_get_brightness_from_cpp	(						);
void my_set_brightness_from_cpp	(const xflt brite_rat	);

void make_worker_thread_ogl_from_cpp		(void* self);
void my_set_pixel_spec_from_cpp				(void* self);

extern char* iPhoneDeviceName_from_cpp		();
extern char* hardwareString_from_cpp		();
extern char gUIDeviceName[1024];

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// MY INCLUDES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
#include "hl_math.h"
#include "tman.h"	// tman is needed up here for the graphics to access it
#include "iOS_graf.h"

inline xflt bx1(){return					ui_sep_pix	;}	// left side
inline xflt bx2(){return graf.dev_dx_use()-	ui_sep_pix	;}	// right side
inline xflt bdx(){return bx2()-bx1()					;}	// delta

inline xint xos(const xflt x,	const xflt d){return bx1()+	bdx()*x/d			;}
inline xint xdx(				const xflt d){return 		bdx()  /d-ui_sep_pix;}

#include "inet.h"
#include "pref.h"
#include "soun.h"
#include "rand.h"
#include "hl_buttons.h"
#include "xios.h"
#include "atmo.h"
#include "synvis.h"
#include "AIRS.h"

#endif
