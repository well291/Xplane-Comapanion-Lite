//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void xios_class::xios_per_frame()
{
	const xflt rand_lo = interp(0.0,0.75 , 100.0,0.0 , xios.ai_rat_rand_0_100);
	const xflt rand_hi = interp(0.0,1.25 , 100.0,2.0 , xios.ai_rat_rand_0_100);

	static xflt prev_ai_rat_load_0_100 = 0.0;	// 0 means leave you alone
	static xflt prev_ai_rat_wexr_0_100 = 0.0;	// 0 means leave you alone
	static xflt prev_ai_rat_rand_0_100 = 0.0;	// 0 means leave you alone

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(	prev_ai_rat_load_0_100 != xios.ai_rat_load_0_100 ||
		prev_ai_rat_rand_0_100 != xios.ai_rat_rand_0_100 )
	for(xint p=0;p<acft_p_size();p++)
	{
		for(xint t=0;t<fixedDIM;t++)
			acft_p(p)->crt_payl_lb[t] = extrap(	 0.0,acft_p(p)->max_payl_lb[t]*0.20,
												75.0,acft_p(p)->max_payl_lb[t]*0.75,xios.ai_rat_load_0_100*fltrand(rand_lo,rand_hi));

		xios_send_xWGT(p);
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	const xflt MTBF_minutes=interp(	  0.0,60.0,
									100.0, 1.0,xios.ai_rat_fail_0_100*fltrand(rand_lo,rand_hi));

	if(xtim.puff1						)
	if(xios.ai_rat_fail_0_100	 >0.0	)
	if(fltrand(0.0,60.0			)<1.0	)
	if(fltrand(0.0,MTBF_minutes	)<1.0	)
	for(xint i=0;i<10;i++				)		// try a bunch of times if needed to find a failure that we can really exercise!
	{
		const xint f = intrand(1,(xint)vec_fal.size()-1);

		if(vec_fal[f].fail_name[0]			)	// if it is a failure we really have
		if(vec_fal[f].gone_present_failed==0)	// if it is not type -1, which is a system we do not have
		if(vec_fal[f].fail_rand				)	// if it is not a wing or something
		{
			vec_fal[f].gone_present_failed = 1;
			const string s = int_to_str(f);
			inet.inet_send("FAIL",(xchr*)s.c_str(),(xint)s.size());
			break;
		}
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(	acft_p_size())										// we come in here is the AI instructor is on,  including the first frame, before any aircraft are ready!
	if(	prev_ai_rat_wexr_0_100!=xios.ai_rat_wexr_0_100 ||
		prev_ai_rat_rand_0_100!=xios.ai_rat_rand_0_100 )
	{
		const xflt challenge_rat_tim		=xios.ai_rat_wexr_0_100*0.01*fltrand(rand_lo,rand_hi);
		const xflt challenge_rat_vis		=xios.ai_rat_wexr_0_100*0.01*fltrand(rand_lo,rand_hi);
		const xflt challenge_rat_storm		=xios.ai_rat_wexr_0_100*0.01*fltrand(rand_lo,rand_hi);
		const xflt challenge_rat_cld_typ	=xios.ai_rat_wexr_0_100*0.01*fltrand(rand_lo,rand_hi);
		const xflt challenge_rat_cld_hgt	=xios.ai_rat_wexr_0_100*0.01*fltrand(rand_lo,rand_hi);
		const xflt challenge_rat_wind		=xios.ai_rat_wexr_0_100*0.01*fltrand(rand_lo,rand_hi);
		const xflt challenge_rat_gust		=xios.ai_rat_wexr_0_100*0.01*fltrand(rand_lo,rand_hi);
		const xflt challenge_rat_turb		=xios.ai_rat_wexr_0_100*0.01*fltrand(rand_lo,rand_hi);

		tim.time_24hr		= interp(0.0,12.0 , 1.0, 24.0 ,		challenge_rat_tim	);
		wxr.vis_sm			= interp(0.0,20.0 , 1.0,  0.1 ,sqrt(challenge_rat_vis)	);
		wxr.ISA_offset_C	= interp(0.0, 0.0 , 1.0,-15.0 ,		challenge_rat_storm	);
		wxr.SL_press_baro	= 29.92 + 0.5*xios.ai_rat_rand_0_100*0.01*fltrand(-1.0,1.0);
		wxr.precip_rat		= interp(0.0, 0.0 , 1.0,  1.0 ,		challenge_rat_storm	);

		const xflt terr_ft=synv.terr_ret_ele(	xios.acft_p(0)->lon_deg,
				 								xios.acft_p(0)->lat_deg)*mtrstoft;

		xint c=0;					wxr.cld_typ_enum	[c] = interp(0.0,cld_cirrus	, 1.0,cld_cnimbus	,challenge_rat_cld_typ);
		for(xint c=1;c<cld_dim;c++)	wxr.cld_typ_enum	[c] = interp(0.0,cld_cirrus	, 1.0,cld_cumulus	,challenge_rat_cld_typ);
		c=0;						wxr.cld_cov_enum	[c] = interp(0.0,cld_clr		, 1.0,cld_bkn		,challenge_rat_cld_typ);
		for(xint c=0;c<cld_dim;c++)	wxr.cld_cov_enum	[c] = interp(0.0,cld_clr		, 1.0,cld_ovc		,challenge_rat_cld_typ);

		for(xint c=0;c<cld_dim;c++)	wxr.base_msl_ft		[c] = interp(0.0,12000.0 , 1.0,0.0 , sqrt(challenge_rat_cld_hgt)) + fltrand(8000.0,12000.0)*c + terr_ft;
		for(xint c=0;c<cld_dim;c++)	wxr.tops_msl_ft		[c] = wxr.base_msl_ft[c]+fltrand(2000.0,6000.0);

		for(xint w=0;w<wnd_dim;w++)	wxr.wind_alt_msl_ft	[w] = 3000.0+3000.0*w;
		for(xint w=0;w<wnd_dim;w++)	wxr.wind_spd_kt		[w] =  50.0*challenge_rat_wind * interp(0.0,1.0 , wnd_dim,2.0 , w);
		for(xint w=0;w<wnd_dim;w++)	wxr.wind_dir_true	[w] = fltrand(0.0,360.0);
		for(xint w=0;w<wnd_dim;w++)	wxr.wind_inc_kt		[w] =  50.0*challenge_rat_gust * interp(0.0,1.0 , wnd_dim,0.0 , w);
		for(xint w=0;w<wnd_dim;w++)	wxr.wind_shr_deg	[w] =  45.0*challenge_rat_gust * interp(0.0,1.0 , wnd_dim,0.0 , w);
		for(xint w=0;w<wnd_dim;w++)	wxr.CAT_rat			[w] = 100.0*challenge_rat_turb * interp(0.0,1.0 , wnd_dim,0.5 , w);

		wxr.therm_vvi_fpm	=0.0;
		wxr.ter_eff_net		=interp(0.0,0.0 , 1.0,2.0 , challenge_rat_storm);
		wxr.wave_hgt_ft		=interp(0.0,0.0 , 1.0,6.0 , challenge_rat_storm);
		wxr.wave_dir_deg	=0.0;

		inet.inet_send("XWXR",(xchr*)&wxr,sizeof(wxr));
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	prev_ai_rat_load_0_100 = xios.ai_rat_load_0_100;
	prev_ai_rat_wexr_0_100 = xios.ai_rat_wexr_0_100;
	prev_ai_rat_rand_0_100 = xios.ai_rat_rand_0_100;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	for(xint w=0;w<vec_wpn.size();w++)
	{
			vec_wpn[w].time_left-=xtim.prd_act;
		if(	vec_wpn[w].time_left<0.0)
			vec_wpn.erase(
			vec_wpn.begin()+w);
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(xtim.puff1)
	for(xint p=0;p<acft_p_size();p++)
		acft_p(p)->acft_path.path_per_sec(	acft_p(p)->lat_deg,
											acft_p(p)->lon_deg,
											acft_p(p)->ele_mtr,
											acft_p(p)->flap_req,
											acft_p(p)->sbrk_req,
											acft_p(p)->gear_req);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// KEEP STUFF UP TO DATE THAT MIGHT BE NEEDED WHEN WE FIRST START THE APP, START X-PLANE FIRST THEN THE APP SECOND, ETC
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	static xint  init_sync_sent=0;
	if(			!init_sync_sent){
				 init_sync_sent=1;
		inet.inet_send("XWXR",(xchr*)&xios.wxr,sizeof(xios.wxr));}
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void apps_class::apps_frame()
{
	if(!init_is_done)return;	// iOS starts calling per-frame before the init is done! really!!! so we have to use this check to avoid that.

	shar.shar_per_init(".");

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// TRACK APP VARIABLES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	mou_drag_dx[0]=0;
	mou_drag_dy[0]=0;
	mou_drag_dx[1]=0;
	mou_drag_dy[1]=0;

	{static xflt omx=0.0;	static xint od=0;	if(mou_dragging(0) && od)mou_drag_dx[0]=mou_drag_x[0]-omx;	od=mou_dragging(0);	omx=mou_drag_x[0];}
	{static xflt omy=0.0;	static xint od=0;	if(mou_dragging(0) && od)mou_drag_dy[0]=mou_drag_y[0]-omy;	od=mou_dragging(0);	omy=mou_drag_y[0];}
	{static xflt omx=0.0;	static xint od=0;	if(mou_dragging(1) && od)mou_drag_dx[1]=mou_drag_x[1]-omx;	od=mou_dragging(1);	omx=mou_drag_x[1];}
	{static xflt omy=0.0;	static xint od=0;	if(mou_dragging(1) && od)mou_drag_dy[1]=mou_drag_y[1]-omy;	od=mou_dragging(1);	omy=mou_drag_y[1];}

	dragged_pix[0]+=pythag(mou_drag_dx[0],mou_drag_dy[0]);
	dragged_pix[1]+=pythag(mou_drag_dx[1],mou_drag_dy[1]);

	mou_dis_rat=1.0;

	if( mou_dragging(0) &&
		mou_dragging(1) )
	{
		const xflt new_dist=fltmax2(pythag(	mou_drag_x[0]-mou_drag_x[1],
											mou_drag_y[0]-mou_drag_y[1]),20.0);

		static xflt							old_dist=	 20.0;
		if(mou_clicked(1) || mou_clicked(0))old_dist=new_dist;

		mou_dis_rat=new_dist/old_dist;
					old_dist=new_dist;
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// CLEAN OUT OLD ACF
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	for(xint b=0;b<becn.other_becns.size();b++)
	if(!becn.other_becns[b].beacon_used)
	for(xint p=0;p<xios.acft_p_size();p++)
	if(	xios.acft_p(p)->acft_addy == becn.other_becns[b].other_addy)
		xios.acft_p_var.erase(
		xios.acft_p_var.begin()+p);

	if(is_pro_use())
	for(xint p=1;p<xios.acft_p_size();p++)
	if(	xios.acft_p(p)->acft_time_net_recv < xtim.real_time-5.0)	// if we reduced the # of acf, drop out the extras
		xios.acft_p_var.erase(
		xios.acft_p_var.begin()+p);

	static xint old_pro = is_pro_use();
	static xint old_lab = is_lab_use();

	if(old_pro != is_pro_use())xios.acft_p_var.clear();	// we might have just switched from pro use to lab use... clear so we can set the # acf accordingly
	if(old_lab != is_lab_use())xios.acft_p_var.clear();	// we might have just switched from pro use to lab use... clear so we can set the # acf accordingly

	old_pro = is_pro_use();
	old_lab = is_lab_use();

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PER-FRAME CALLS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	OGL_set_viewport_2d	(0,0,graf.dev_dx_use(),graf.dev_dy_use());	shar.shar_per_check("view");	// this is done for all app modes
	xtim.xtim_per_frame	(										);	shar.shar_per_check("xtim");	// keep this updated all topwins for the UI, etc
	inet.inet_per_frame	(										);	shar.shar_per_check("inet");	// keep this updated for all topwin for the net settings to come across from multiplayer to set track, weather, time, etc
	synv.synv_per_frame	(										);	shar.shar_per_check("synv");
	xios.xios_per_frame	(										);	shar.shar_per_check("xios");
	xios.xios_main		(										);	shar.shar_per_check("main");

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// RESET
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(!mou_dragging(0))mouse_btn_slide(0,0,0,0,NULL,xtrue);

	set_mou_clicked	(0,xfals);	// before next event-handler
	set_mou_clicked	(1,xfals);	// before next event-handler
	set_mou_released(0,xfals);	// before next event-handler
	set_mou_released(1,xfals);	// before next event-handler

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// TEST-CODE FOR SIM
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	#define test_VEHX 0

	#if test_VEHX
		if(xtim.real_time>30)
		{
			buff_class	buff;
	
			const xint p				=   0.0;								buff.com_xint(p					);
			const xdob dat_lat			=  32.0+xtim.real_time/60.0;			buff.com_xdob(dat_lat			);
			const xdob dat_lon			= -84.0;								buff.com_xdob(dat_lon			);
			const xdob dat_ele			=5000.0;								buff.com_xdob(dat_ele			);
			const xflt veh_psi_loc_true	=  45.0*sin(xtim.real_time*con_pi);		buff.com_xflt(veh_psi_loc_true	);	// local-horizon
			const xflt veh_the_loc		=  15.0*sin(xtim.real_time*con_pi);		buff.com_xflt(veh_the_loc		);	// local-horizon
			const xflt veh_phi_loc		=  30.0*sin(xtim.real_time*con_pi);		buff.com_xflt(veh_phi_loc		);	// local-horizon

			inet.inet_send("VEHX",(xchr*)&buff.m_data[0],buff.m_index);
		}
	#endif
}
