//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
const xflt UI_max_altitude_ft_msl	=45000.0;					// what sim uses
const xflt UI_min_altitude_ft_msl	=    0.0;
const xflt UI_min_thickness_ft		= 2000.0;
const xflt UI_max_wind_speed_kts	=  100.0;					// lower than SIM to let these sliders work with fine res
const xflt UI_max_vis_ter_sm		=  100.0;					// ACCORDING TO PETER COX (TRANSPORT-CANADA GUY): ALL VIS IN THE NORTHERN HEMISPHERE IS IN STATUTE, NOT NAUTICAL, MILES
const xflt UI_max_vis_RVR_km		=    1.0;					// a test engineer in the Navy at Pax River said this about Dr. Austin: "If she told me the sky was red, we just see it as blue, I'd believe her.". 100 meters is also the minimums for a cat-3
const xflt UI_max_vis_RVR_sm		=    1.0;					// a test engineer in the Navy at Pax River said this about Dr. Austin: "If she told me the sky was red, we just see it as blue, I'd believe her.". 100 meters is also the minimums for a cat-3
const xflt max_temp_sel_sfc_C		=   60.0;					// just above the highest we have seen at the surface
const xflt min_temp_sel_sfc_C		=  -80.0;					// the antarctica has recorded colder than this, really.
const xflt max_temp_off_sfc_C		= max_temp_sel_sfc_C-15.0;	// this for the offsets!
const xflt min_temp_off_sfc_C		= min_temp_sel_sfc_C-15.0;	// this for the offsets!
const xflt max_SLP_pas				= 107500;
const xflt min_SLP_pas				=  89000;

xflt get_max_layer_thickness_ft(				){return UI_max_altitude_ft_msl;}
xflt get_min_layer_thickness_ft(const xint c	){return UI_min_thickness_ft + ((c==0)?(xios.wxr.precip_rat*UI_max_altitude_ft_msl*0.5):0.0);}	// only layer 0 has the storm clouds

xflt x1_clds=0.0;
xflt x2_clds=0.0;
xflt y1_clds=0.0;
xflt y2_clds=0.0;

inline xflt py_cld(const xflt ft){						return interp(0.0,y1_clds					,1.0,y2_clds				,xpow(interp(UI_min_altitude_ft_msl	,0.0 , UI_max_altitude_ft_msl	,1.0 , ft),1.0/3.0));}
inline xflt ft_cld(const xflt py){						return interp(0.0,UI_min_altitude_ft_msl	,1.0,UI_max_altitude_ft_msl	,xpow(interp(y1_clds				,0.0 , y2_clds					,1.0 , py),    3.0));}
inline xflt x1_cld(const xint in){const xflt cdx=150;	return x1_clds		+in*cdx			;}	// test on phone and pad, landscape and portrait
inline xflt x2_cld(const xint in){const xflt cdx=150;	return x2_clds-100	+in*cdx-cdx*2	;}	// test on phone and pad, landscape and portrait

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// BACKGROUND, SCALE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void sky_base()
{
	OGL_fillrect(col_butn_brt,0.25,	x1_clds											-ui_sep_pix		,
									y1_clds											-ui_sep_pix		,
									x2_clds									-x1_clds+ui_sep_pix*2	,
									y2_clds									-y1_clds+ui_sep_pix*2	);

	OGL_fillrect(col_butn_drk,0.25,	x1_clds											-ui_sep_pix		,
									y1_clds											-ui_sep_pix		,
									x2_clds									-x1_clds+ui_sep_pix*2	,
									py_cld(xios.acft_p(0)->ter_ele_ft())	-y1_clds+ui_sep_pix		);

	for(xint h=UI_min_altitude_ft_msl;h<UI_max_altitude_ft_msl;h+=(h<5000)?1000:5000)
	{
		OGL_line(col_text_back,1.0,	x1_clds-ui_sep_pix	,
									py_cld(h)			,
									x2_clds+ui_sep_pix*2,
									py_cld(h)			);

		plot_string(t_font_2,x2_clds-ui_sep_pix,py_cld(h)+font_2_dy*0.5+ui_sep_pix,int_to_str(h),col_text_back,1.0,just_rgt);
	}
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void xios_class::xios_weather()
{
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SUB-MODES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	enum
	{
		menu_over=0	,
		menu_time	,
		menu_surface,
		menu_atmo	,
		menu_clouds	,
		menu_wind	,
		menu_storms	,
		menu_dim
	};

	static xint menu_mode=menu_over;
	xchr names[menu_dim][name_dim80]={"QUICK","TIME","SURFACE","ATMOSPHERE","CLOUDS","WIND","MAP"};
	
	if(RUNNING_1208)
		names[menu_storms][0]=0;

	butn_menus(&menu_mode,(RUNNING_1208)?(menu_dim-1):(menu_dim),names,1);

	xint hard_clouds=0;

	const xflt y0 = butn_dy*2.0			;
	const xflt y1 = graf.dev_dy_use()	;
	const xflt yc = (y0+y1)*0.5			;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// DATE AND TIME
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(menu_mode==menu_over)
	{
		if(butn_on_off(wxr.regn_change_en != regn_down_real_weather,xos(0,2),yc+butn_dy*3,xdx(2),"REAL-WEATHER OFF"	,"")){wxr.regn_change_en=regn_static_wxr		;}
		if(butn_on_off(wxr.regn_change_en == regn_down_real_weather,xos(1,2),yc+butn_dy*3,xdx(2),"REAL-WEATHER ON"	,"")){wxr.regn_change_en=regn_down_real_weather	;}

		static xint																	preset=-1;
		if(butn_on_off(preset==0,xos(0,7),yc+butn_dy,xdx(7),"CAVOK"	,""			)){	preset= 0;	wxr.set_CAVOK();	wxr.vis_sm=25.0;}

		if(butn_on_off(preset==1,xos(1,7),yc+butn_dy,xdx(7),"VFR"	,""			)){	preset= 1;	wxr.set_CAVOK();	wxr.vis_sm= 7.0;	wxr.cld_typ_enum[0]=cld_cumulus	;	wxr.cld_cov_enum[0]=cld_sct;	wxr.base_msl_ft[0]= 8000.0;	wxr.tops_msl_ft[0]=12000.0;
																																		wxr.cld_typ_enum[1]=cld_cirrus	;	wxr.cld_cov_enum[1]=cld_sct;	wxr.base_msl_ft[1]=28000.0;	wxr.tops_msl_ft[1]=32000.0;}
 
		if(butn_on_off(preset==2,xos(2,7),yc+butn_dy,xdx(7),"MVFR"	,""			)){	preset= 2;	wxr.set_CAVOK();	wxr.vis_sm= 5.0;	wxr.cld_typ_enum[0]=cld_cumulus	;	wxr.cld_cov_enum[0]=cld_bkn;	wxr.base_msl_ft[0]= 2000.0;	wxr.tops_msl_ft[0]= 6000.0;
																																		wxr.cld_typ_enum[1]=cld_cirrus	;	wxr.cld_cov_enum[1]=cld_sct;	wxr.base_msl_ft[1]=28000.0;	wxr.tops_msl_ft[1]=32000.0;}
 
		if(butn_on_off(preset==3,xos(3,7),yc+butn_dy,xdx(7),"IFR"	,"Non-Prec"	)){	preset= 3;	wxr.set_CAVOK();	wxr.vis_sm= 1.0;	wxr.cld_typ_enum[0]=cld_stratus	;	wxr.cld_cov_enum[0]=cld_ovc;	wxr.base_msl_ft[0]=  500.0;	wxr.tops_msl_ft[0]= 5000.0;
																																		wxr.cld_typ_enum[1]=cld_cumulus	;	wxr.cld_cov_enum[1]=cld_bkn;	wxr.base_msl_ft[1]= 8000.0;	wxr.tops_msl_ft[1]=12000.0;}
 
		if(butn_on_off(preset==4,xos(4,7),yc+butn_dy,xdx(7),"IFR"	,"CAT-1"	)){	preset= 4;	wxr.set_CAVOK();	wxr.vis_sm= 0.5;	wxr.cld_typ_enum[0]=cld_stratus	;	wxr.cld_cov_enum[0]=cld_ovc;	wxr.base_msl_ft[0]=  200.0;	wxr.tops_msl_ft[0]= 5000.0;
																																		wxr.cld_typ_enum[1]=cld_cumulus	;	wxr.cld_cov_enum[1]=cld_bkn;	wxr.base_msl_ft[1]= 8000.0;	wxr.tops_msl_ft[1]=12000.0;}
 
		if(butn_on_off(preset==5,xos(5,7),yc+butn_dy,xdx(7),"Clear"	,"Windy"	)){	preset= 5;	wxr.set_CAVOK();	wxr.vis_sm=25.0;	wxr.cld_typ_enum[0]=cld_cumulus	;	wxr.cld_cov_enum[0]=cld_sct;	wxr.base_msl_ft[0]= 1000.0;	wxr.tops_msl_ft[0]=16000.0;
																																		wxr.cld_typ_enum[1]=cld_cirrus	;	wxr.cld_cov_enum[1]=cld_sct;	wxr.base_msl_ft[1]=20000.0;	wxr.tops_msl_ft[1]=24000.0;
			for(xint w=0;w<wnd_dim;w++)
			{
				wxr.wind_alt_msl_ft	[w]=5000.0*(1.0+xpow(w,2));
				wxr.wind_dir_true	[w]= 270.0;
				wxr.wind_spd_kt		[w]=  20.0*(1.0+xpow(w,2));
				wxr.wind_inc_kt		[w]=  10.0/(1.0+xpow(w,2));
				wxr.CAT_rat			[w]=  50.0/(1.0+xpow(w,2));
			}
		}

		if(butn_on_off(preset==6,xos(6,7),yc+butn_dy,xdx(7),"Thunder","Storms"	)){preset=6;	wxr.set_CAVOK();	wxr.vis_sm=15.0;

			wxr.cld_typ_enum[0]=cld_cnimbus;	wxr.cld_cov_enum[0]=cld_bkn;	wxr.base_msl_ft[0]=1000.0;	wxr.tops_msl_ft[0]=45000.0;
			wxr.cld_typ_enum[1]=cld_cumulus;	wxr.cld_cov_enum[1]=cld_sct;	wxr.base_msl_ft[1]=1000.0;	wxr.tops_msl_ft[1]= 5000.0;
			wxr.cld_typ_enum[2]=cld_cumulus;	wxr.cld_cov_enum[2]=cld_sct;	wxr.base_msl_ft[2]=5000.0;	wxr.tops_msl_ft[2]=10000.0;

			wxr.precip_rat=1.0;
			for(xint w=0;w<wnd_dim;w++)
			{
				wxr.wind_alt_msl_ft	[w]=5000.0*(1.0+xpow(w,2));
				wxr.wind_dir_true	[w]= 270.0;
				wxr.wind_spd_kt		[w]=  10.0*(1.0+xpow(w,2));
				wxr.wind_inc_kt		[w]=  10.0/(1.0+xpow(w,2));
				wxr.CAT_rat			[w]=  10.0/(1.0+xpow(w,2));
			}
		}

		if(	butn_on_off	(wxr.regn_change_en==regn_rapidly_improving			,xos(0,7)	,yc-butn_dy*0,xdx(7),"Rapidly"		,"Improving"	)){wxr.regn_change_en = regn_rapidly_improving		;}
		if(	butn_on_off	(wxr.regn_change_en==regn_improving					,xos(1,7)	,yc-butn_dy*0,xdx(7)," "			,"Improving"	)){wxr.regn_change_en = regn_improving				;}
		if(	butn_on_off	(wxr.regn_change_en==regn_gradually_improving		,xos(2,7)	,yc-butn_dy*0,xdx(7),"Gradually"	,"Improving"	)){wxr.regn_change_en = regn_gradually_improving	;}
		if(	butn_on_off	(wxr.regn_change_en==regn_static_wxr				,xos(3,7)	,yc-butn_dy*0,xdx(7),"Static"						)){wxr.regn_change_en = regn_static_wxr				;}
		if(	butn_on_off	(wxr.regn_change_en==regn_gradually_deteriorating	,xos(4,7)	,yc-butn_dy*0,xdx(7),"Gradually"	,"Deteriorating")){wxr.regn_change_en = regn_gradually_deteriorating;}
		if(	butn_on_off	(wxr.regn_change_en==regn_deteriorating				,xos(5,7)	,yc-butn_dy*0,xdx(7)," "			,"Deteriorating")){wxr.regn_change_en = regn_deteriorating			;}
		if(	butn_on_off	(wxr.regn_change_en==regn_rapidly_deteriorating		,xos(6,7)	,yc-butn_dy*0,xdx(7),"Rapidly"		,"Deteriorating")){wxr.regn_change_en = regn_rapidly_deteriorating	;}

			butn_slide	(													 bx1()		,yc-butn_dy*2,bdx( ),"Weather Variation Over Space",&wxr.regn_variation_pct,"%",1.0,0.0,100.0);
		if(butn_action	(													 xos(0,1)	,yc-butn_dy*3,xdx(1),"RANDOMIZE WEATHER PATTERN","")){const string s="sim/operation/regen_weather";	inet.inet_send("CMND",(xchr*)s.c_str(),(xint)s.size());}
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// DATE AND TIME
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(menu_mode==menu_time)
	{
		butn_slide(bx1(),yc+butn_dy*2	,bdx(),"Time",&tim.time_24hr	,"hr"	,0.1,0.0, 24.0);
		butn_slide(bx1(),yc				,bdx(),"Date",&tim.date_0_364	,"day"	,1.0,0.0,364.0);

		for(xint mo=0;mo<12;mo++)
			plot_string(t_font_1,interp( 0,bx1(),
										12,bx2(),(xflt)mo+0.5) , yc + butn_dy*0.5 + font_2_dy*0.5,month_name(mo),col_text_butt,1.0,just_ctr);

		static xint	   lites=-1;
		if(butn_on_off(lites==0,xos(0,4),yc-butn_dy*2,xdx(4),"Runway Lights","Off"		)){lites=0;	const string s="sim/operation/rwy_lights_off"	;	inet.inet_send("CMND",(xchr*)s.c_str(),(xint)s.size());}
		if(butn_on_off(lites==1,xos(1,4),yc-butn_dy*2,xdx(4),"Runway Lights","Low"		)){lites=1;	const string s="sim/operation/rwy_lights_lo"	;	inet.inet_send("CMND",(xchr*)s.c_str(),(xint)s.size());}
		if(butn_on_off(lites==2,xos(2,4),yc-butn_dy*2,xdx(4),"Runway Lights","Medium"	)){lites=2;	const string s="sim/operation/rwy_lights_med"	;	inet.inet_send("CMND",(xchr*)s.c_str(),(xint)s.size());}
		if(butn_on_off(lites==3,xos(3,4),yc-butn_dy*2,xdx(4),"Runway Lights","High"		)){lites=3;	const string s="sim/operation/rwy_lights_hi"	;	inet.inet_send("CMND",(xchr*)s.c_str(),(xint)s.size());}
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SURFACE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(menu_mode==menu_surface)
	{
		xflt y=yc+butn_dy*4;
		if(butn_on_off(wxr.ter_eff_net==ter_eff_dry			,xos(0,3),y,xdx(3),"Dry"				,""))wxr.ter_eff_net=ter_eff_dry			;

		y-=butn_dy;
		if(butn_on_off(wxr.ter_eff_net==ter_eff_wet1		,xos(0,3),y,xdx(3),"Wet 1"				,""))wxr.ter_eff_net=ter_eff_wet1			;
		if(butn_on_off(wxr.ter_eff_net==ter_eff_wet2		,xos(1,3),y,xdx(3),"Wet 2"				,""))wxr.ter_eff_net=ter_eff_wet2			;
		if(butn_on_off(wxr.ter_eff_net==ter_eff_wet3		,xos(2,3),y,xdx(3),"Wet 3"				,""))wxr.ter_eff_net=ter_eff_wet3			;

		y-=butn_dy;
		if(butn_on_off(wxr.ter_eff_net==ter_eff_puddly1		,xos(0,3),y,xdx(3),"Puddles 1"			,""))wxr.ter_eff_net=ter_eff_puddly1		;
		if(butn_on_off(wxr.ter_eff_net==ter_eff_puddly2		,xos(1,3),y,xdx(3),"Puddles 2"			,""))wxr.ter_eff_net=ter_eff_puddly2		;
		if(butn_on_off(wxr.ter_eff_net==ter_eff_puddly3		,xos(2,3),y,xdx(3),"Puddles 3"			,""))wxr.ter_eff_net=ter_eff_puddly3		;

		y-=butn_dy;
		if(butn_on_off(wxr.ter_eff_net==ter_eff_snowy1		,xos(0,3),y,xdx(3),"Snowy 1"			,""))wxr.ter_eff_net=ter_eff_snowy1			;
		if(butn_on_off(wxr.ter_eff_net==ter_eff_snowy2		,xos(1,3),y,xdx(3),"Snowy 2"			,""))wxr.ter_eff_net=ter_eff_snowy2			;
		if(butn_on_off(wxr.ter_eff_net==ter_eff_snowy3		,xos(2,3),y,xdx(3),"Snowy 3"			,""))wxr.ter_eff_net=ter_eff_snowy3			;

		y-=butn_dy;
		if(butn_on_off(wxr.ter_eff_net==ter_eff_icy1		,xos(0,3),y,xdx(3),"Icy 1"				,""))wxr.ter_eff_net=ter_eff_icy1			;
		if(butn_on_off(wxr.ter_eff_net==ter_eff_icy2		,xos(1,3),y,xdx(3),"Icy 2"				,""))wxr.ter_eff_net=ter_eff_icy2			;
		if(butn_on_off(wxr.ter_eff_net==ter_eff_icy3		,xos(2,3),y,xdx(3),"Icy 3"				,""))wxr.ter_eff_net=ter_eff_icy3			;

		y-=butn_dy;
		if(butn_on_off(wxr.ter_eff_net==ter_eff_snowy_icy1	,xos(0,3),y,xdx(3),"Icy and Snowy 1"	,""))wxr.ter_eff_net=ter_eff_snowy_icy1		;
		if(butn_on_off(wxr.ter_eff_net==ter_eff_snowy_icy2	,xos(1,3),y,xdx(3),"Icy and Snowy 2"	,""))wxr.ter_eff_net=ter_eff_snowy_icy2		;
		if(butn_on_off(wxr.ter_eff_net==ter_eff_snowy_icy3	,xos(2,3),y,xdx(3),"Icy and Snowy 3"	,""))wxr.ter_eff_net=ter_eff_snowy_icy3		;

		y-=butn_dy;
		y-=butn_dy;		butn_slide(bx1(),y,bdx(),"Wave Direction (from!)"	,&wxr.wave_dir_deg	,"deg"	,1.0	,0,360);
		y-=butn_dy;		butn_slide(bx1(),y,bdx(),"Wave Height"				,&wxr.wave_hgt_ft	,"ft"	,0.1	,0,6);
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ATMO
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(menu_mode==menu_atmo)
	{
		xflt y = yc+butn_dy*3;

		static xflt rvr = 0.0;	// ok this gets weird. the sliders are identified for sliding by address of the variable, so we need to pass in the same varible address every time! so these need to be static
		static xflt vis = 0.0;	// ok this gets weird. the sliders are identified for sliding by address of the variable, so we need to pass in the same varible address every time! so these need to be static

		if(vis_meters)
		{
			rvr = fltlim(	wxr.vis_sm*smtomtrs,0.0,UI_max_vis_RVR_km*1000.0+0.1);	// +0.1 to get the + when we exceeed that limit
			vis =			wxr.vis_sm*smtomtrs/1000.0;
	
							if(	butn_slide(bx1(),y,bdx(),"RVR"						,&rvr,(rvr>UI_max_vis_RVR_km*1000.0)	?"+m":"m"	,100.0 , 0.0					,UI_max_vis_RVR_km*1000.0	))wxr.vis_sm = rvr*mtrstosm			;
			y-=butn_dy;		if(	butn_slide(bx1(),y,bdx(),"Vis"						,&vis									,"km"		,  0.1 , 0.1					,UI_max_vis_ter_sm			))wxr.vis_sm = vis*mtrstosm*1000.0	;
		}
		else
		{
			rvr = fltlim(	wxr.vis_sm*5280.0,0.0,UI_max_vis_RVR_sm*5280.0+0.1);	// +0.1 to get the + when we exceeed that limit
			vis =			wxr.vis_sm;
	
							if(	butn_slide(bx1(),y,bdx(),"RVR"						,&rvr,(rvr>UI_max_vis_RVR_sm*5280.0)	?"+ft":"ft"	,100.0 , 0.0					,UI_max_vis_RVR_sm*5280.0	))wxr.vis_sm = rvr/5280.0	;
			y-=butn_dy;		if(	butn_slide(bx1(),y,bdx(),"Vis"						,&vis									,"sm"		,  0.1 , 0.1					,UI_max_vis_ter_sm			))wxr.vis_sm = vis			;
		}

			y-=butn_dy;			butn_slide(bx1(),y,bdx(),"Rain"						,&wxr.precip_rat						,"%"		,  0.01 , 0.0					,1.0 , 100.0				);
			y-=butn_dy;
			y-=butn_dy;			butn_slide(bx1(),y,bdx(),"Temperature ISA Offset"	,&wxr.ISA_offset_C						,"C"		,  1.0	, min_temp_off_sfc_C	,max_temp_off_sfc_C			);
			y-=butn_dy;
			if(!pres_hectopas)	butn_slide(bx1(),y,bdx(),"Baro Press"				,&wxr.SL_press_baro						,"inch HG"	,  0.01	, min_SLP_pas*pasto_hg	,max_SLP_pas*pasto_hg					);
			if( pres_hectopas)	butn_slide(bx1(),y,bdx(),"Baro Press"				,&wxr.SL_press_baro						,"hPa"		,  0.01	, min_SLP_pas*pasto_hg	,max_SLP_pas*pasto_hg,hg_topas/100.0	);
			y-=butn_dy;
			y-=butn_dy;			butn_slide(bx1(),y,bdx(),"Thermal Strength"			,&wxr.therm_vvi_fpm						,"FPM"		, 10.0	, 0.0,2500.0);
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// CLOUDS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(menu_mode==menu_clouds)
	{
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// DIMS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		x1_clds = bx1()					+ ui_sep_pix;
		x2_clds = bx2()					- ui_sep_pix;
		y1_clds = butn_dy*11			+ ui_sep_pix;
		y2_clds = graf.dev_dy_use()		- ui_sep_pix;

		if(graf.in_landscape())
		{
			x1_clds = graf.dev_cx_use()	+ ui_sep_pix;
			x2_clds = bx2()				- ui_sep_pix;
			y1_clds = butn_dy*2			+ ui_sep_pix;
			y2_clds = graf.dev_dy_use()	- ui_sep_pix;
		}

		sky_base();

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// QUICK LIMIT-SET
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		for(xint c=0;c<cld_dim;c++)
		{
			wxr.tops_msl_ft[c]=fltlim(
			wxr.tops_msl_ft[c],
			wxr.base_msl_ft[c]+get_min_layer_thickness_ft(c),
			wxr.base_msl_ft[c]+get_max_layer_thickness_ft());
		}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SORT ORDER TO GRAB TOP LAYERS WITH THE MOUSE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		static xint  c_grabbed=-1;	if(apps.mou_released(0)) c_grabbed=-1;
		static xint bt_grabbed=-1;	if(apps.mou_released(0))bt_grabbed=-1;

		xflt closes_dis=9999;

		if(apps.mou_clicked(0))		// on the click...
		for(xint c=0;c<cld_dim;c++)	// mouse-click grabs the CLOSES EDGE we find!
		{
			const xflt cy1=py_cld(wxr.base_msl_ft[c])	;
			const xflt cy2=py_cld(wxr.tops_msl_ft[c])	;
			const xflt cyc=(cy1+cy2)*0.5				;

			if(apps.mouse_box_drag(x1_cld(c),cy1-50 , x2_cld(c),cyc))	// use drag to not reset the click in the proc, so we can keep checking layers!
			{
				const xflt dis=fltabs(apps.mou_drag_y[0]-cy1);

				if(dis<closes_dis)
				{
					closes_dis=dis;	c_grabbed=c;	bt_grabbed=0;
				}
			}

			if(apps.mouse_box_drag(x1_cld(c),cyc , x2_cld(c),cy2+50))	// use drag to not reset the click in the proc, so we can keep checking layers!
			{
				const xflt dis=fltabs(apps.mou_drag_y[0]-cy2);

				if(dis<closes_dis)
				{
					closes_dis=dis;	c_grabbed=c;	bt_grabbed=1;
				}
			}
		}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SORT ORDER TO DRAW BIGGEST LAYERS BELOW, THINNER LAYERS ON TOP
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		xint ctr_to_c[cld_dim]={0,1,2};			// draw biggest layers as base, smaller layers on top!
		for(xint itc=0;itc<=cld_dim  ;itc++)
		for(xint ctr=0;ctr< cld_dim-1;ctr++)
		if(	wxr.tops_msl_ft[ctr_to_c[ctr+1]]-
			wxr.base_msl_ft[ctr_to_c[ctr+1]]>
			wxr.tops_msl_ft[ctr_to_c[ctr  ]]-
			wxr.base_msl_ft[ctr_to_c[ctr  ]])	// about to plot a bigger layer on top of a smaller one? then don't.
			int_switch(	   &ctr_to_c[ctr+1],
						   &ctr_to_c[ctr  ]);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// DRAW THE CLOUDS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		for(xint ctr=0;ctr<cld_dim;ctr++)
		{
			const xint c=ctr_to_c[ctr];

			string								 cld_type				;
			if(wxr.cld_typ_enum[c]==cld_cirrus	)cld_type="Cirrus"		;
			if(wxr.cld_typ_enum[c]==cld_stratus	)cld_type="Stratus"		;
			if(wxr.cld_typ_enum[c]==cld_cumulus	)cld_type="Cumulus"		;
			if(wxr.cld_typ_enum[c]==cld_cnimbus	)cld_type="Cumulonimbus";

			string								 cld_covr				;
			if(wxr.cld_cov_enum[c]==cld_clr		)cld_covr="CLR"			;	// only have room for abbrev in landscape mode
			if(wxr.cld_cov_enum[c]==cld_few		)cld_covr="FEW"			;	// only have room for abbrev in landscape mode
			if(wxr.cld_cov_enum[c]==cld_sct		)cld_covr="SCT"			;	// only have room for abbrev in landscape mode
			if(wxr.cld_cov_enum[c]==cld_bkn		)cld_covr="BKN"			;	// only have room for abbrev in landscape mode
			if(wxr.cld_cov_enum[c]==cld_ovc		)cld_covr="OVC"			;	// only have room for abbrev in landscape mode

			if(wxr.cld_cov_enum[c]!=cld_clr		)
			{
				const xflt cy1=py_cld(wxr.base_msl_ft[c]);
				const xflt cy2=py_cld(wxr.tops_msl_ft[c]);

				const string str_tops=cld_covr+" "+cld_type+", "+int_to_str(intnear(wxr.tops_msl_ft[c]							,10))+" MSL";
				const string str_base=cld_covr+" "+cld_type+", "+int_to_str(intnear(wxr.base_msl_ft[c]-acft_p(0)->ter_ele_ft()	,10))+" AGL";

				butn_2d(x1_cld(c),cy1,x2_cld(c),cy2,0.85);
				plot_string(t_font_2,(x1_cld(c)+x2_cld(c))*0.5,cy2-font_2_dy*0.5-ui_sep_pix,str_tops,col_text_butt,1.0,just_ctr);
				plot_string(t_font_2,(x1_cld(c)+x2_cld(c))*0.5,cy1+font_2_dy*0.5+ui_sep_pix,str_base,col_text_butt,1.0,just_ctr);

				if( c_grabbed==c)
				if(bt_grabbed==0)
				{
					wxr.base_msl_ft[c]=ft_cld(apps.mou_drag_y[0]);
					wxr.tops_msl_ft[c]=fltlim(
					wxr.tops_msl_ft[c],
					wxr.base_msl_ft[c]+get_min_layer_thickness_ft(c),
					wxr.base_msl_ft[c]+get_max_layer_thickness_ft());
				}

				if( c_grabbed==c)
				if(bt_grabbed==1)
				{
					wxr.tops_msl_ft[c]=ft_cld(apps.mou_drag_y[0]);
					wxr.base_msl_ft[c]=fltlim(
					wxr.base_msl_ft[c],
					wxr.tops_msl_ft[c]-get_max_layer_thickness_ft(),
					wxr.tops_msl_ft[c]-get_min_layer_thickness_ft(c));
				}
			}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PLOT THE CLOUD COMMAND BUTTONS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
			const xflt y = butn_dy*2 + butn_dy*3*c;			// BE SUER TO TEST IN LADSCAPE AND PORTRAIT BEFORE I CHANGE THIS!
			const xflt k = graf.in_landscape()?0.5:1.0;

			if(butn_on_off((wxr.cld_cov_enum[c]!=cld_clr && wxr.cld_typ_enum[c]==cld_cirrus	) , xos(1,5)*k , y+butn_dy*2	, xdx(5)*k,"Cirrus"			," ")){wxr.cld_typ_enum[c]=cld_cirrus	;	wxr.cld_cov_enum[c]=intmax2(wxr.cld_cov_enum[c],cld_few);	hard_clouds=1;}
			if(butn_on_off((wxr.cld_cov_enum[c]!=cld_clr && wxr.cld_typ_enum[c]==cld_stratus) , xos(2,5)*k , y+butn_dy*2	, xdx(5)*k,"Stratus"		," ")){wxr.cld_typ_enum[c]=cld_stratus	;	wxr.cld_cov_enum[c]=intmax2(wxr.cld_cov_enum[c],cld_few);	hard_clouds=1;}
			if(butn_on_off((wxr.cld_cov_enum[c]!=cld_clr && wxr.cld_typ_enum[c]==cld_cumulus) , xos(3,5)*k , y+butn_dy*2	, xdx(5)*k,"Cumulus"		," ")){wxr.cld_typ_enum[c]=cld_cumulus	;	wxr.cld_cov_enum[c]=intmax2(wxr.cld_cov_enum[c],cld_few);	hard_clouds=1;}
			if(butn_on_off((wxr.cld_cov_enum[c]!=cld_clr && wxr.cld_typ_enum[c]==cld_cnimbus) , xos(4,5)*k , y+butn_dy*2	, xdx(5)*k,"Cumulonimbus"	," ")){wxr.cld_typ_enum[c]=cld_cnimbus	;	wxr.cld_cov_enum[c]=intmax2(wxr.cld_cov_enum[c],cld_few);	hard_clouds=1;}

			if(butn_on_off((wxr.cld_cov_enum[c]==cld_clr									) , xos(0,5)*k , y+butn_dy*1.5	, xdx(5)*k,"Clear"			," ")){										wxr.cld_cov_enum[c]=cld_clr								;	hard_clouds=1;}
			if(butn_on_off((wxr.cld_cov_enum[c]==cld_few									) , xos(1,5)*k , y+butn_dy*1	, xdx(5)*k,"Few"			," ")){										wxr.cld_cov_enum[c]=cld_few								;	hard_clouds=1;}
			if(butn_on_off((wxr.cld_cov_enum[c]==cld_sct									) , xos(2,5)*k , y+butn_dy*1	, xdx(5)*k,"Scattered"		," ")){										wxr.cld_cov_enum[c]=cld_sct								;	hard_clouds=1;}
			if(butn_on_off((wxr.cld_cov_enum[c]==cld_bkn									) , xos(3,5)*k , y+butn_dy*1	, xdx(5)*k,"Broken"			," ")){										wxr.cld_cov_enum[c]=cld_bkn								;	hard_clouds=1;}
			if(butn_on_off((wxr.cld_cov_enum[c]==cld_ovc									) , xos(4,5)*k , y+butn_dy*1	, xdx(5)*k,"Overcast"		," ")){										wxr.cld_cov_enum[c]=cld_ovc								;	hard_clouds=1;}
		}
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// WIND
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(menu_mode==menu_wind)
	{
		x1_clds = bx1()							+ ui_sep_pix;
		x2_clds = bx2()							- ui_sep_pix;
		y1_clds = butn_dy*3						+ ui_sep_pix;
		y2_clds = graf.dev_dy_use() - ui_sep_pix- ui_sep_pix;

		sky_base();

		for(xint w=0;w<wnd_dim;w++)
		{
			const xflt x2=x1_clds+125;
			const xflt x3=x1_clds+250;
			const xflt y =py_cld(wxr.wind_alt_msl_ft[w]);

			xint grabbed=0;

			static xflt	gust[3]={0};	// has to be 3 vars for the lsider to work rite so it knows this is a diff var for each slider icne it works on address-location!
						gust[w]=wxr.wind_spd_kt[w]+
								wxr.wind_inc_kt[w];

				butn_dir	(x2,y							,"Dir, True"													,&wxr.wind_dir_true	[w]			,xtrue						);
			if(	butn_slide	(x3,y+butn_dy	,x2_clds-x3-100	,"Wind@"+int_to_str(intnear(wxr.wind_alt_msl_ft[w],100))+" ft"	,&wxr.wind_spd_kt	[w]	,"kt"	,1,0,UI_max_wind_speed_kts	)){grabbed=1;	gust[w]				=fltmax2(gust[w]			,wxr.wind_spd_kt[w]	);}
			if(	butn_slide	(x3,y			,x2_clds-x3-100	,"Gust"															,&gust				[w]	,"kt"	,1,0,UI_max_wind_speed_kts	)){grabbed=2;	wxr.wind_spd_kt[w]	=fltmin2(wxr.wind_spd_kt[w]	,gust[w]			);}
			if(	butn_slide	(x3,y-butn_dy	,x2_clds-x3-100	,"Turbulence"													,&wxr.CAT_rat		[w]	,"%"	,1,0,100					)){grabbed=3;}

			wxr.wind_inc_kt[w]=gust[w]-
			wxr.wind_spd_kt[w];

			if(grabbed==1)wxr.wind_alt_msl_ft[w]=ft_cld(apps.mou_drag_y[0]-butn_dy	);
			if(grabbed==2)wxr.wind_alt_msl_ft[w]=ft_cld(apps.mou_drag_y[0]			);
			if(grabbed==3)wxr.wind_alt_msl_ft[w]=ft_cld(apps.mou_drag_y[0]+butn_dy	);

			if(grabbed)
			for(xint sw=0;sw<wnd_dim;sw++)
			{
				if(sw>w)wxr.wind_alt_msl_ft[sw]=fltmax2(
						wxr.wind_alt_msl_ft[sw],ft_cld(py_cld(wxr.wind_alt_msl_ft[w])+butn_dy*3*(sw-w)));

				if(sw<w)wxr.wind_alt_msl_ft[sw]=fltmin2(
						wxr.wind_alt_msl_ft[sw],ft_cld(py_cld(wxr.wind_alt_msl_ft[w])+butn_dy*3*(sw-w)));
			}
		}
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// STORMS / FRONTS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(menu_mode==menu_storms)
		xios_map_start(xtrue);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// NET
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(!apps.mou_dragging(0) && !apps.mou_dragging(1))
	{
		static net_wxr_struct wxr_prev = wxr;
		static net_tim_struct tim_prev = tim;

		if(memcmp(&wxr_prev,&wxr,sizeof(net_wxr_struct))!=0)
		{
			const xflt max_cloud=fltmax3(	wxr.cld_cov_enum[0],
											wxr.cld_cov_enum[1],
											wxr.cld_cov_enum[2]);
			if(hard_clouds)
			{
				if(max_cloud<cld_sct)wxr.precip_rat=0.0;
			}
			else
			{
				if(max_cloud<cld_sct && wxr.precip_rat>0.0)
					wxr.cld_cov_enum[0]=fltmax2(
					wxr.cld_cov_enum[0],cld_sct);
			}
	
			wxr.apt_ele_m	=acft_p(0)->ele_mtr			// philipp uses this to go from ATIS baro to sea-level baro
							-acft_p(0)->ele_agl_mtr;	// philipp uses this to go from ATIS baro to sea-level baro
	
			wxr_prev=wxr;
			inet.inet_send("XWXR",(xchr*)&wxr,sizeof(wxr));
		}
		else if(memcmp(&tim_prev,&tim,sizeof(net_tim_struct))!=0)
		{
			tim_prev=tim;
			inet.inet_send("XTIM",(xchr*)&tim,sizeof(tim));
		}
	}
}
