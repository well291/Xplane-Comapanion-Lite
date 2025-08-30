#ifndef colr_h
#define colr_h

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// CONS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
const xflt gray_rect_alpha = 0.75;	// standard opacity for overlays

#define spectrumDIM 1000

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ENUMS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
enum enum_ui
{
	ui_race	=0,
	ui_apple=2
};

enum enum_col
{
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// BASE COLORS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	col_black=0			,
	col_dgray			,
	col_mgray			,
	col_lgray			,
	col_white			,

	col_red				,	// do not change order. used for hoops logic.
	col_orange			,	// do not change order. used for hoops logic.
	col_yellow			,	// do not change order. used for hoops logic.
	col_green			,	// do not change order. used for hoops logic.
	col_blue			,	// do not change order. used for hoops logic.
	col_violet			,	// do not change order. used for hoops logic.

	col_cyan			,
	col_magenta			,
	col_lgt_cyan		,
	col_lgt_magenta		,

	col_purple			,
	col_pink			,
	col_peach			,

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// UI
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	col_NAS_blue		,	// airspace blue
	col_NAS_magenta		,	// airspace magenta

	ui_background		,
	ui_background_text	,
	ui_not_ready		,

	ui_tab_off			,
	ui_tab_off_text		,
	ui_tab_on			,
	ui_tab_on_text		,

	ui_button_off		,
	ui_button_off_text	,
	ui_button_on		,
	ui_button_on_text	,

	ui_app_menu_off		,
	ui_app_menu_off_text,
	ui_app_menu_on		,
	ui_app_menu_on_text	,

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// NEXRAD COLORS NEED THEIR OWN SET SO THEY GO IN ORDER FROM LOWEST TO HIGHEST PRECIP LEVELS
// THIS IS BECAUSE WE OFTEN TAKE THE HIGHEST PRECIP LEVEL IN A REGION TO COLOR THAT REGION,
// SO THE NEXRAD COLORS HAVE TO GO IN ORDER
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	nexrad_black		,	// lev 0
	nexrad_lgt_green	,	// lev 1
	nexrad_yellow_green	,	// lev 2
	nexrad_yellow		,	// lev 3
	nexrad_yellow_orange,	// lev 4
	nexrad_orange		,	// lev 5
	nexrad_orange_red	,	// lev 6
	nexrad_red			,	// lev 7

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// CONTROL PAD UI
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	col_background		,	// background
	col_text_reg		,	// text region on the background
	col_text_back		,	// text on background
	col_text_butt		,	// text on button

	col_butn_drk		,	// button off
	col_butn_med		,	// button that cannot be on or off
	col_butn_brt		,	// button on

	col_menu_drk		,	// menu off
	col_menu_brt		,	// menu on

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// MISC SET PER FRAME
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	col_pulse_orange	,	// data entry color for entering glide data
	col_variable		,
	col_dim
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class colr_class
{
public:
   ~colr_class(){}
	colr_class()
	{
		memset(this,0,sizeof(*this));
		ui_sel=ui_apple;
		init_color();
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// VARS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
enum_ui ui_sel;
xflt private_col_rgb[col_dim][3];	// we store alpha in last byte just for faster setting to fcol is all

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SPECTRUM
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xflt spectrum_min;
xflt spectrum_max;

void spectrum_set_scale(const xflt min_elev,const xflt max_elev);
void spectrum_set_col(const xflt val,xflt brite=1.0);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PROCS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void init_color();
void colr_per_frame();

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ACCESSORS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline void init_col(const enum_col en,const xflt r,const xflt g,const xflt b)
{
	private_col_rgb[en][0]=r;
	private_col_rgb[en][1]=g;
	private_col_rgb[en][2]=b;
}

inline void set_index(const xint index,const xflt a=1.0)
{
	glColor4f(	private_col_rgb[index][0],
				private_col_rgb[index][1],
				private_col_rgb[index][2],a);	// ALL UI COLORS ARE UNLIT... JUST KEEP THEM THE SAME NO MATTER WHAT OR WE LOSE UI INTERFACE AT NITE OR SOMETHING.
}

inline void set_rbga(const xflt r,const xflt g,const xflt b,const xflt a)
{
	glColor4f(r,g,b,a);
}

inline xflt rgba_byt_to_flt(const xbyt r,const xbyt g,const xbyt b,const xbyt a)
{
	static xbyt		 rgba[4]={0};
					 rgba[0]=r;
					 rgba[1]=g;
					 rgba[2]=b;
					 rgba[3]=a;
	return *((xflt*)(rgba));
}

inline xflt gray_byt_to_flt(const xbyt rgb,const xbyt a)
{
	static xbyt		 rgba[4]={0};
					 rgba[0]=rgb;
					 rgba[1]=rgb;
					 rgba[2]=rgb;
					 rgba[3]=a;
	return *((xflt*)(rgba));
}

inline xflt colr_enum_to_flt(const enum_col col,const xflt alpha)
{
	return rgba_byt_to_flt
	(
		private_col_rgb[col][0]	*255.0,
		private_col_rgb[col][1]	*255.0,
		private_col_rgb[col][2]	*255.0,
		alpha					*255.0
	);
}
};
extern class colr_class colr;

const xflt f_white		= colr.rgba_byt_to_flt(255,255,255,255);
const xflt f_magenta	= colr.rgba_byt_to_flt(255, 50,255,255);
#endif
