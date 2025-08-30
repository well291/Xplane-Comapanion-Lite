//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void xios_class::xios_send_xWGT(const xint p)
{
	buff_class						buff;
	for(xint f=0;f<fueltankDIM;f++)	buff.com_xflt(acft_p(p)->crt_fuel_lb[f]);
	for(xint f=0;f<fixedDIM   ;f++)	buff.com_xflt(acft_p(p)->crt_payl_lb[f]);

	inet.inet_send("xWGT",(xchr*)&buff.m_data[0],buff.m_index,p);	// always goes to index 0 in SIM: we don't set the weight of AI planes in pro-use
}

void xios_class::xios_send_LWPN(const xint p,const xint s,const string name)
{
	buff_class	buff;
				buff.com_xint((is_pro_use()) ? p : 0);
				buff.com_xint(s);
				buff.com_xint(						(xint)name.size());
				buff.com_xdim((xchr*)name.c_str(),	(xint)name.size());
				inet.inet_send("LWPN",(xchr*)&buff.m_data[0],buff.m_index,p);	// goes to appropriate plane
}

void xios_class::xios_send_COMB(const xint p)
{
	if(RUNNING_1208)
	{
		struct ACFN_struct
		{
			ACFN_struct() { memset(this, 0, sizeof(*this)); }
		
			xint m_acfn_p;
			xchr m_acfn_path_rel[net_strDIM];
			xchr pad[2];
			xint m_acfn_live_ind;
		};
		ACFN_struct ACFN_var;

				ACFN_var.m_acfn_p			= (is_pro_use()) ? p : 0;
		C_eq_C(	ACFN_var.m_acfn_path_rel	, acft_p(p)->acfpath , net_strDIM);
				ACFN_var.m_acfn_live_ind	= acft_p(p)->livery_now;

		inet.inet_send("ACFN",(xchr*)&ACFN_var,sizeof(ACFN_struct),p);
	}
	else
	{
		buff_class	buff;
					buff.com_xint((is_pro_use()) ? p : 0	);	// for pro use, we are controlling this p on that machine. for lab use, it is p0 on each machine.
					buff.com_xdim(acft_p(p)->acfpath,160	);
					buff.com_xint(acft_p(p)->livery_now		);
					buff.com_xint(acft_p(p)->ai_flies_now	);
					buff.com_xint(acft_p(p)->team_now		);
					buff.com_xint(acft_p(p)->skill_now		);
	
		inet.inet_send("COMB",(xchr*)&buff.m_data[0],buff.m_index,p);
	}
}

void xios_class::xios_send_PREL(const xint p,const PREL_struct PREL)
{
	inet.inet_send("PREL",(xchr*)&PREL,sizeof(PREL),p);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void xios_class::xios_acf_menus(const xint allow_mod)
{
	const xint menu_dim = 22;

	xchr names[menu_dim][name_dim80]={0};	// when > 4 items, this gets auto-split into 2 lines!

	for(xint p=0;p<acft_p_size();p++)
	{
		const string n = acft_p(p)->acft_addy.toString();	// don't use acf index. nobody can keep that in sync with the fixed IP addies of the lab.

												C_eq_C	(names[p],(xchr*)n.c_str()		,name_dim80);
												app_C	(names[p],": "					,name_dim80);
//		if(acft_p(p)->team_now == team_red	)	app_C	(names[p],"red "				,name_dim80);
//		if(acft_p(p)->team_now == team_blue	)	app_C	(names[p],"blue "				,name_dim80);
//		if(acft_p(p)->team_now == team_green)	app_C	(names[p],"green "				,name_dim80);
//		if(acft_p(p)->team_now == team_gold	)	app_C	(names[p],"gold "				,name_dim80);
												app_C	(names[p],acft_p(p)->tailnum	,name_dim80);
	}

	xint											index=acft_p_size();
	if(allow_mod && acft_p_size()<20)C_eq_C(names[	index++],"ADD ACF",name_dim80);
	if(allow_mod && acft_p_size()> 1)C_eq_C(names[	index++],"DEL ACF",name_dim80);

	xint		   menu_sel = acf_edit;
	if(butn_menus(&menu_sel,index,names,1))
	{
				if((string)names[menu_sel]=="ADD ACF"){const xint n = acft_p_size()+1;		inet.inet_send("NACF",(xchr*)&n,sizeof(n));}
		else	if((string)names[menu_sel]=="DEL ACF"){const xint n = acft_p_size()-1;		inet.inet_send("NACF",(xchr*)&n,sizeof(n));}
		else	acf_edit = menu_sel;
	}
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void xios_class::xios_main()
{
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// BACKGROUND
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	OGL_fillrect(col_background,1.0,0,0,graf.dev_dx_use(),graf.dev_dy_use());

	if(colr.ui_sel!=ui_apple)
	{
		for(xflt x=10;x<graf.dev_dx_use();x+=20)
		{
			vector <xflt> vrt;

			for(xflt y=10;y<graf.dev_dy_use();y+=20)
			{
				const xint del=6.0;

				vrt.push_back(x);
				vrt.push_back(y);
				vrt.push_back(0.5);
				vrt.push_back(0.5);

				vrt.push_back(x);
				vrt.push_back(y+del);
				vrt.push_back(0.5);
				vrt.push_back(0.5);

				vrt.push_back(x+del);
				vrt.push_back(y);
				vrt.push_back(0.5);
				vrt.push_back(0.5);

				vrt.push_back(x+del);
				vrt.push_back(y);
				vrt.push_back(0.5);
				vrt.push_back(0.5);

				vrt.push_back(x);
				vrt.push_back(y+del);
				vrt.push_back(0.5);
				vrt.push_back(0.5);

				vrt.push_back(x+del);
				vrt.push_back(y+del);
				vrt.push_back(0.5);
				vrt.push_back(0.5);
			}

			xint dim=(xint)vrt.size();
			plot_geo(t_white,col_butn_drk,0.20,&vrt[0],2,&dim,GL_TRIANGLES , col_vert_fals,dep_read_fals,dep_writ_fals);
		}
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// MENUS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(!acft_p_size())
	{
		xios_settings();	// CRITICAL TO NOT GO TO MENUS UNTIL AN AIRCRAFT IS SELECTED, OR WE COULD TRY TO ACCESS A ZERO-LEN VECTOR WHEN LOOKING FOR OUR AIRPLANE!
	}
	else
	{
		enum menu_mode_enum
		{
			menu_set=0		,
			menu_aircraft	,
			menu_start		,
			menu_weight		,
			menu_failures	,
			menu_weather	,
			menu_map		,
			menu_commands	,
			menu_sits		,
			menu_dim
		};
	
		static	menu_mode_enum	menu_mode=menu_set;	// start at X-Plane selection settings
				xchr			names[10][name_dim80]={"SET","CRAFT","START","WGT","FAIL","WXR","MAP","CMD","SITS"};

		acf_edit = intlim(						// keep the acf sel in range here
		acf_edit , 0,(xint)acft_p_size()-1);	// as we change the number of aircraft with the UI

		if(menu_mode==menu_set		)xios_settings	();
		if(menu_mode==menu_aircraft	)xios_aircraft	();
		if(menu_mode==menu_start	)xios_start		();
		if(menu_mode==menu_weight	)xios_weight	();
		if(menu_mode==menu_failures	)xios_failures	();
		if(menu_mode==menu_weather	)xios_weather	();
		if(menu_mode==menu_map		)xios_map_start	(xfals);
		if(menu_mode==menu_commands	)xios_commands	();
		if(menu_mode==menu_sits		)xios_sits		();

		if(butn_menus((xint*)&menu_mode,menu_dim,names,0))
			pref.prefs_txt(IO_write);
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ALERTS: BUILD
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	string s1;
	string s2;
	string s3;
	string s4;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ALERTS: PLOT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	static xflt alert_t=0.0;
				alert_t-=xtim.prd_act;

	static string alert_old1;
	static string alert_old2;
	static string alert_old3;
	static string alert_old4;
	if(			 (alert_old1!=alert_message1)||
				 (alert_old2!=alert_message2)||
				 (alert_old3!=alert_message3)||
				 (alert_old4!=alert_message4)){
				  alert_old1 =alert_message1;
				  alert_old2 =alert_message2;
				  alert_old3 =alert_message3;
				  alert_old4 =alert_message4;	alert_t=10.0;}

	if(alert_t>0.0)
	{
		s1=alert_message1;
		s2=alert_message2;
		s3=alert_message3;
		s4=alert_message4;
	}

	xflt		 alert_dy=			0	;
	if(s1.size())alert_dy=font_1_dy*2+10;
	if(s2.size())alert_dy=font_1_dy*3+10;
	if(s3.size())alert_dy=font_1_dy*4+10;
	if(s4.size())alert_dy=font_1_dy*5+10;

	if(alert_dy>0.0)
	{
		const xflt cx=graf.dev_cx_use()						;
		const xflt y1=graf.dev_dy_use()-ui_sep_pix			;
		const xflt y0=graf.dev_dy_use()-ui_sep_pix-alert_dy	;

		OGL_fillrect(col_background,0.85,	0.0					,y0+10		,
											graf.dev_dx_use()	,alert_dy	);

		if(s1.size())plot_string(t_font_1,cx,y1-font_1_dy*1,s1,col_text_back,1.0,just_ctr);
		if(s2.size())plot_string(t_font_1,cx,y1-font_1_dy*2,s2,col_text_back,1.0,just_ctr);
		if(s3.size())plot_string(t_font_1,cx,y1-font_1_dy*3,s3,col_text_back,1.0,just_ctr);
		if(s4.size())plot_string(t_font_1,cx,y1-font_1_dy*4,s4,col_text_back,1.0,just_ctr);
	}
}
