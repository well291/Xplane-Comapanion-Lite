void write_app_review();

void xios_class::xios_settings()
{
	enum
	{
		menu_connect=0	,
		menu_prefs		,
		menu_dim
	};

	static xint menu_mode = menu_connect;
	xchr names[menu_dim][name_dim80] = {"CONNECT X-PLANE 12","PREFS"};
	butn_menus(&menu_mode,menu_dim,names,1);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// TEXT DESCRIPS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	const xflt cx			= graf.dev_cx_use()							;
	const xflt y2_text_box	= graf.dev_dy_use()-ui_sep_pix				;
	const xflt y1_text_box	= graf.dev_dy_use()-ui_sep_pix-font_2_dy*12	;
	const xflt y1_cmnd_but	= butn_dy*7									;

	plot_text_box(y1_text_box,y2_text_box,xfals);

	string s1 = "None yet!";	if(becn.becn_rcvd	.size())s1 = becn.becn_rcvd		+" "+flt_to_str(xtim.real_time-becn.becn_time	,1,0)+" sec ago.";
	string s2 = "None yet!";	if(inet.udp_str_sent.size())s2 = inet.udp_str_sent	+" "+flt_to_str(xtim.real_time-inet.udp_tim_sent,1,0)+" sec ago.";
	string s3 = "None yet!";	if(inet.udp_str_rcvd.size())s3 = inet.udp_str_rcvd	+" "+flt_to_str(xtim.real_time-inet.udp_tim_rcvd,1,0)+" sec ago.";
	string s4 = "None yet!";	if(inet.tcp_str_sent.size())s4 = inet.tcp_str_sent	+" "+flt_to_str(xtim.real_time-inet.tcp_tim_sent,1,0)+" sec ago.";
	string s5 = "None yet!";	if(inet.tcp_str_rcvd.size())s5 = inet.tcp_str_rcvd	+" "+flt_to_str(xtim.real_time-inet.tcp_tim_rcvd,1,0)+" sec ago.";

	xflt					y =y2_text_box;
	plot_string(t_font_2,10,y-=font_2_dy,(string)vers_string,col_text_back);

	for(xint i=0;i<this_ip_dim;i++)
	if(strlen(inet.our_ip_addies[i])>0)
	plot_string(t_font_2,20,y-=font_2_dy,"IP address "+(string)inet.our_ip_addies[i],col_text_back);
	plot_string(t_font_2,20,y-=font_2_dy,(string)"Beacon rcvd: "+s1					,col_text_back);
	plot_string(t_font_2,20,y-=font_2_dy,(string)"UDP sent: "	+s2					,col_text_back);
	plot_string(t_font_2,20,y-=font_2_dy,(string)"UDP rcvd: "	+s3					,col_text_back);
	plot_string(t_font_2,20,y-=font_2_dy,(string)"TCP sent: "	+s4					,col_text_back);
	plot_string(t_font_2,20,y-=font_2_dy,(string)"TCP rcvd: "	+s5					,col_text_back);
	plot_string(t_font_2,10,y-=font_2_dy,"austin@X-Plane.com for support"			,col_text_back);

													y =y2_text_box;
						plot_string(t_font_2,cx+10,	y-=font_2_dy,"Battery "+int_to_str(my_battery_level_from_cpp())+"%"			,col_text_back);
						plot_string(t_font_2,cx+10,	y-=font_2_dy,int_to_str((xint)vec_fix.size())+" fixes in RAM."				,col_text_back);
						plot_string(t_font_2,cx+10,	y-=font_2_dy,int_to_str((xint)vec_nav.size())+" NAVAIDs in RAM."			,col_text_back);
						plot_string(t_font_2,cx+10,	y-=font_2_dy,int_to_str((xint)vec_awy.size())+" airways in RAM."			,col_text_back);
	if(!RUNNING_1208)	plot_string(t_font_2,cx+10,	y-=font_2_dy,int_to_str((xint)vec_hol.size())+" holding patterns in RAM."	,col_text_back);
						plot_string(t_font_2,cx+10,	y-=font_2_dy,int_to_str((xint)vec_apt.size())+" airports in RAM."			,col_text_back);
						plot_string(t_font_2,cx+10,	y-=font_2_dy,int_to_str((xint)vec_fal.size())+" failures in RAM."			,col_text_back);

	if(is_any_use())
	{
		if(is_pro_use())plot_string(t_font_2,cx+10,y1_text_box+font_2_dy,"ONE X-PLANE SELECTED. WE WILL CONTROL ALL THE AIRPLANES ON THAT COMPUTER."			,col_text_back,1.0,just_ctr);
		if(is_lab_use())plot_string(t_font_2,cx+10,y1_text_box+font_2_dy,"MULTIPLE X-PLANES SELECTED. WE WILL CONTROL THE PRIMARY AIRPLANE ON EACH COMPUTER."	,col_text_back,1.0,just_ctr);
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// CONNECTION OPTIONS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(menu_mode==menu_connect)
	{
		if(becn.other_becns.size()==0)
		{
			y = y1_text_box - butn_dy;

			plot_text_box(y-font_2_dy*12,y,xfals);
	
			plot_string(t_font_2,cx,y-=font_2_dy,"I have not heard from X-Plane 12.10 or later yet!"			,col_text_back,1.0,just_ctr);
			plot_string(t_font_2,cx,y-=font_2_dy,"Make sure Xavion is not running now on this device!"			,col_text_back,1.0,just_ctr);
									y-=font_2_dy;
			plot_string(t_font_2,cx,y-=font_2_dy,"Get X-Plane 12 from www.X-Plane.com."							,col_text_back,1.0,just_ctr);
			plot_string(t_font_2,cx,y-=font_2_dy,"Be sure that it is on the same network as this Pad or Phone."	,col_text_back,1.0,just_ctr);
									y-=font_2_dy;
			plot_string(t_font_2,cx,y-=font_2_dy,"Then in X-Plane go to Settings : Network."					,col_text_back,1.0,just_ctr);
			plot_string(t_font_2,cx,y-=font_2_dy,"Then check the box for X-Plane Control Pad."					,col_text_back,1.0,just_ctr);
		}
		else
		{
			const xflt y2_scroll = y1_text_box - ui_sep_pix;
			const xflt y1_scroll = y1_cmnd_but + ui_sep_pix;
		
			y =				y2_scroll - butn_dy + butn_scroll_reg(becn.other_becns.size()*butn_dy	, y1_scroll , y2_scroll);
			sci_start(0 ,	y1_scroll , graf.dev_dx_use()											, y2_scroll - y1_scroll);
			
			for(xint b=0;b<becn.other_becns.size();b++)
			{
				if(butn_on_off(	becn.other_becns[b].beacon_used,bx1(),y+(xflt)butn_dy*0.5,bdx(),(string)"Command X-Plane on "+
								becn.other_becns[b].other_name,(string)"IP address "+
								becn.other_becns[b].other_addy.toString())){
								becn.other_becns[b].beacon_used=!
								becn.other_becns[b].beacon_used;	// now the "CON1" message below will go to the selected copy of X-Plane, turning on the output from X-Plane to IOS!
							if(	becn.other_becns[b].beacon_used)
								inet.inet_send("CON1",NULL,0);}		// this turns on IOS in X-Plane so it sends all the stuff it needs to

				if(xtim.puff1)
				if(becn.other_becns[b].beacon_used)
					inet.inet_send("CON1",NULL,0);					// this turns on IOS in X-Plane so it sends all the stuff it needs to

				y-=butn_dy;
			}

			sci_stop();
		}

		plot_string(t_font_2,cx,y-=font_2_dy,"If on MacOS 15 or later, go to System Settings > Privacy > Local Network.",col_text_back,1.0,just_ctr);
		plot_string(t_font_2,cx,y-=font_2_dy,"Then give X-Plane permission to access your network."						,col_text_back,1.0,just_ctr);
		plot_string(t_font_2,cx,y-=font_2_dy,"You have to do this for Apple to allow a network connection."				,col_text_back,1.0,just_ctr);
		
		if(becn.other_becns.size())
		{
			if(is_any_use())
			{
				y = y1_cmnd_but;

				static xint paused=0;
				
				if(butn_on_off(paused,xos(0,1),y-=butn_dy,xdx(1),"Pause X-Plane"))
				{
					paused=!paused;
	
					const string pause_str = (paused) ? "sim/operation/pause_on" : "sim/operation/pause_off";
		
					inet.inet_send("CMND",(xchr*)pause_str.c_str(),(xint)pause_str.size());
				}

				if(butn_action(bx1(),y-=butn_dy,bdx(),"Quit X-Plane"		)){inet.inet_send("QUIT","",0);	if(prefs_save_armed)apps.apps_save();}
				if(butn_action(bx1(),y-=butn_dy,bdx(),"Shut Down Computer"	)){inet.inet_send("SHUT","",0);	if(prefs_save_armed)apps.apps_save();}

				if(!apps.review_done)
				if(butn_action(bx1(),y-=butn_dy,bdx(),"Write App Review"	))
				{
					write_app_review();
					apps.review_done = 1;
					pref.prefs_txt(IO_write);
				}
			}
		}
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// UI OPTIONS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(menu_mode==menu_prefs)
	{
		y = y1_text_box - butn_dy;

		if(graf.is_phone)
		{
			graf.dev_orient=orient_portrait;
		}
		else
		{
			if(butn_on_off((graf.dev_orient==orient_portrait),xos(0,4),y,xdx(4),"Portrait"			)){graf.dev_orient=orient_portrait	;	pref.prefs_txt(IO_write);}
			if(butn_on_off((graf.dev_orient==orient_invert	),xos(1,4),y,xdx(4),"Inverted"			)){graf.dev_orient=orient_invert	;	pref.prefs_txt(IO_write);}
			if(butn_on_off((graf.dev_orient==orient_land_lft),xos(2,4),y,xdx(4),"Landscape Left"	)){graf.dev_orient=orient_land_lft	;	pref.prefs_txt(IO_write);}
			if(butn_on_off((graf.dev_orient==orient_land_rgt),xos(3,4),y,xdx(4),"Landscape Right"	)){graf.dev_orient=orient_land_rgt	;	pref.prefs_txt(IO_write);}
		}
	
		y-=butn_dy;
		if(butn_on_off((colr.ui_sel==ui_race ),xos(0,2),y,xdx(2),"Dark Mode"	)){colr.ui_sel=ui_race	;	colr.init_color();	pref.prefs_txt(IO_write);}
		if(butn_on_off((colr.ui_sel==ui_apple),xos(1,2),y,xdx(2),"Bright Mode"	)){colr.ui_sel=ui_apple	;	colr.init_color();	pref.prefs_txt(IO_write);}

							y-=butn_dy;
		if(butn_slide(bx1(),y-=butn_dy,bdx(),"Brightness"	,&apps.brite_0_100 ,"%",5 , 0,100)){my_set_brightness_from_cpp	(apps.brite_0_100*0.01	);	pref.prefs_txt(IO_write);}
		if(butn_slide(bx1(),y-=butn_dy,bdx(),"Volume"		,&soun.volume_0_100,"%",5 , 0,100)){soun.play_sound				(soun_click				);	pref.prefs_txt(IO_write);}
							y-=butn_dy;

													y-=butn_dy;
		if(butn_on_off(becn.auto_connect,xos(0,1),	y,xdx(1),"Auto-Connect to All Stations"	)){becn.auto_connect=!becn.auto_connect;	pref.prefs_txt(IO_write);	for(xint o=0;o<becn.other_becns.size();o++)becn.other_becns[o].beacon_used = becn.auto_connect;}
													y-=butn_dy;
													y-=butn_dy;
		if(butn_on_off(!vis_meters		,xos(0,2),	y,xdx(2),"Visibility Feet / SM"			)){vis_meters=0;				pref.prefs_txt(IO_write);}
		if(butn_on_off( vis_meters		,xos(1,2),	y,xdx(2),"Visibility Meters / KM"		)){vis_meters=1;				pref.prefs_txt(IO_write);}
													y-=butn_dy;
		if(butn_on_off(!pres_hectopas	,xos(0,2),	y,xdx(2),"Pressure Inches HG"			)){pres_hectopas=0;				pref.prefs_txt(IO_write);}
		if(butn_on_off( pres_hectopas	,xos(1,2),	y,xdx(2),"Pressure Hecto Pascals"		)){pres_hectopas=1;				pref.prefs_txt(IO_write);}
													y-=butn_dy;
		if(butn_on_off(!weight_kg		,xos(0,2),	y,xdx(2),"Weight lb"					)){weight_kg=0;					pref.prefs_txt(IO_write);}
		if(butn_on_off( weight_kg		,xos(1,2),	y,xdx(2),"Weight kg"					)){weight_kg=1;					pref.prefs_txt(IO_write);}
	}
}

