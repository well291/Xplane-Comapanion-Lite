#include "hl_types.h"

colr_class colr;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void colr_class::init_color()
{
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// BASE COLORS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	init_col(col_black				,0.0 ,0.0 ,0.0 );
	init_col(col_dgray				,0.2 ,0.2 ,0.2 );
	init_col(col_mgray				,0.4 ,0.4 ,0.4 );
	init_col(col_lgray				,0.7 ,0.7 ,0.7 );
	init_col(col_white				,1.0 ,1.0 ,1.0 );

	init_col(col_red				,1.0 ,0.4 ,0.4 );	// red must be very brite to show up as urgent traffic or it is dimmer than the yellow! pure red is near-impossible to see, and my optometrist said it is like having myopia to look at it... so we make it bright here!
	init_col(col_orange				,1.0 ,0.6 ,0.2 );	// orange must be very brite to show up as urgent traffic or it is dimmer than the yellow!
	init_col(col_yellow				,1.0 ,1.0 ,0.0 );
	init_col(col_green				,0.0 ,1.0 ,0.0 );
	init_col(col_blue				,0.5 ,0.5 ,1.0 );
	init_col(col_violet				,0.75,0.0 ,1.0 );

	init_col(col_cyan				,0.2 ,1.0 ,1.0 );	// this is only used for airways and diagnostics
	init_col(col_magenta			,1.0 ,0.2 ,1.0 );	// this is used in tec and maybe other apps
	init_col(col_lgt_cyan			,0.5 ,1.0 ,1.0 );	// this is only used for airways and diagnostics
	init_col(col_lgt_magenta		,1.0 ,0.5 ,1.0 );	// this is used in tec and maybe other apps

	init_col(col_purple				,0.8 ,0.0 ,1.0 );
	init_col(col_pink				,1.0 ,0.75,0.8 );
	init_col(col_peach				,1.0 ,0.9 ,0.7 );

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// UI
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	init_col(col_NAS_blue			,0.6 ,0.6 ,1.0 );	// this is as bright as i can make it while still looking blue
	init_col(col_NAS_magenta		,1.0 ,0.3 ,1.0 );	// a little darker to go with the blue airspace and so we can see our path over the airspace

	#define space_race	0

	init_col(ui_background			,0.0,0.0,0.0);
	init_col(ui_background_text		,1.0,1.0,1.0);
	init_col(ui_not_ready			,1.0,0.6,0.2);

	init_col(ui_tab_off				,0.0,0.0,0.0);
	init_col(ui_tab_off_text		,1.0,1.0,1.0);
	init_col(ui_tab_on				,0.2,0.2,0.2);
	init_col(ui_tab_on_text			,1.0,1.0,1.0);

	init_col(ui_button_off			,0.6,0.6,0.6);
	init_col(ui_button_off_text		,0.0,0.0,0.0);
	init_col(ui_button_on			,1.0,1.0,1.0);
	init_col(ui_button_on_text		,0.0,0.0,0.0);

	init_col(ui_app_menu_off		,0.2,0.2,0.2);
	init_col(ui_app_menu_off_text	,1.0,1.0,1.0);
	init_col(ui_app_menu_on			,0.3,0.3,0.3);
	init_col(ui_app_menu_on_text	,1.0,1.0,1.0);

	#if space_race
		init_col(ui_not_ready		,0.5,1.0,1.0);
	#endif

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// NEXRAD COLORS NEED THEIR OWN SET SO THEY GO IN ORDER FROM LOWEST TO HIGHEST PRECIP LEVELS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	init_col(nexrad_black			,0.0 ,0.0 ,0.0 );
	init_col(nexrad_lgt_green		,0.0 ,1.0 ,0.0 );
	init_col(nexrad_yellow_green	,0.75,1.0 ,0.0 );
	init_col(nexrad_yellow			,1.0 ,1.0 ,0.0 );
	init_col(nexrad_yellow_orange	,1.0 ,0.75,0.0 );
	init_col(nexrad_orange			,1.0 ,0.5 ,0.0 );
	init_col(nexrad_orange_red		,1.0 ,0.25,0.0 );
	init_col(nexrad_red				,1.0 ,0.0 ,0.0 );

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// CONTROL PAD UI
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(ui_sel==ui_race)
	{
		init_col(col_background	,0.1,0.1,0.1);
		init_col(col_text_reg	,0,0,0);
		init_col(col_text_back	,1,1,1);
		init_col(col_text_butt	,0,0,0);

		xflt r=0.35/0.55;
		xflt g=0.45/0.55;
		xflt b=0.55/0.55;

		init_col(col_butn_drk,r*0.5,g*0.5,b*0.5);	// mode: off
		init_col(col_butn_med,r*0.7,g*0.7,b*0.7);	// basic button
		init_col(col_butn_brt,r    ,g    ,b    );	// mode: on
	}

	if(ui_sel==ui_apple)
	{
		init_col(col_background	,1,1,1);
		init_col(col_text_reg	,0.2,0.2,0.2);
		init_col(col_text_back	,0,0,0);
		init_col(col_text_butt	,0,0,0);

		xflt r=0.8;
		xflt g=1.0;
		xflt b=1.0;

		init_col(col_butn_drk,r*0.5,g*0.5,b*0.5);	// mode: off
		init_col(col_butn_med,r*0.7,g*0.7,b*0.7);	// basic button
		init_col(col_butn_brt,r    ,g    ,b    );	// mode: on
	}

	init_col(col_menu_drk	,0.5,0.5,0.5);
	init_col(col_menu_brt	,1.0,1.0,1.0);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void colr_class::colr_per_frame()
{
	const xflt					enter_alpha=0.9+0.1*sin(xtim.real_time*10.0);
	init_col(col_pulse_orange,	enter_alpha*1.0,
								enter_alpha*0.6,
								enter_alpha*0.2);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void colr_class::spectrum_set_scale(const xflt min_elev,const xflt max_elev)
{
	spectrum_min = min_elev;
	spectrum_max = max_elev;
}

void colr_class::spectrum_set_col(const xflt val,xflt brite)
{
	const xflt rat = interp(spectrum_min,0 , spectrum_max,1, val);

	const xflt r = rat;//interp(0.5,1.0 , 1.0,0.0, rat);
	const xflt g = rat;//fltmin2(interp(0.0,0.0 , 1.0,1.0, rat) , interp(0.0,1.0 , 1.0,0.0, rat));
	const xflt b = rat;//interp(0.5,0.0 , 1.0,1.0, rat);

	glColor4f(	r*brite,
				b*brite,
				g*brite,1.0);
}
