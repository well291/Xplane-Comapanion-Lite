//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ENUMS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
enum
{
	nav_flag=0,	// ntf types
	nav_to	=1,
	nav_from=2
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// UTILS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline xflt ang_to_pix(xflt ang_deg,xflt dx)							// THIS GEO ASSUME FLAT-PLANE PROJECTION!
{
	ang_deg=fltlim(ang_deg,-89.0,89.0);									// fltlim that angle since we take the tangent of it!
	return (dx*0.5)*tan(ang_deg*degtorad)/tan(fov_lat*0.5*degtorad);	// DO THE GEO ON THIS BY ASSUMING THE FLAT VIEWING PLANE IS 1 METER AWAY FROM YOU IN Z-DIR AND JUST DO THE FULL ALL-OUT GEO IN METERS.
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// TAPES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void acft_PX_struct::plot_asi_alt_tape(xflt x_tic,xflt y_cen,xflt y_siz,xflt val,xint is_asi)
{
	const	xflt min_KIAS_indi	=	30;									// indicator no good below this. it flickers.
	const	xflt min_gps_alt_ft	=-1500;									// lowest elevation is -1,388 ft
	const	xflt min_val		=(is_asi)?min_KIAS_indi:min_gps_alt_ft;	// min val
			xint too_slow		=(val<min_val);							// ASI flickers below this!

	val=fltmax2(val,min_val);

	xflt overlay_dx	=(is_asi)?-45.0: 45.0;
	xflt pix_per_v	=(is_asi)?  5.0:  0.5;
	xint dv_tic		=(is_asi)? 10  :100  ;
	xint dv_num		=(is_asi)? 20  :200  ;
	xflt v2			=fltnear(val+y_siz/pix_per_v+dv_tic,dv_tic);
	xflt v1			=fltnear(val-y_siz/pix_per_v-dv_tic,dv_tic);	v1=fltmax2(v1,min_val);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// BOXES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if( is_asi)plot_ins_overlay(x_tic+overlay_dx,y_cen,0,0,2,t_layers_tall_wide);
	if(!is_asi)plot_ins_overlay(x_tic+overlay_dx,y_cen,0,1,2,t_layers_tall_wide);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// V-SPEED ARCS ON THE SCROLLING TAPE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	sci_start(0,y_cen-y_siz,graf.dev_dx_use(),y_siz*2);

	enum
	{
		arc_low=0	,	// low-end redline... common on efis
		arc_green	,	// white last so white plots on top since it overlaps the green at a lateral offset!!!!
		arc_yellow	,
		arc_red		,
		arc_white	,	// white last so white plots on top since it overlaps the green at a lateral offset!!!!
		arc_dim
	};
	enum_col col=col_black;

	xflt arc_v1 =0.0;
	xflt arc_v2 =0.0;
	xflt xos	=0.0;

	if(is_asi)
	{
		for(xint en=0;en<arc_dim;en++)
		{
			if(en==arc_low		){col=col_red	;	arc_v1=min_val			;	arc_v2=xios.con.Vso_KIAS		;	xos=-6;}
			if(en==arc_white	){col=col_white	;	arc_v1=xios.con.Vso_KIAS;	arc_v2=xios.con.Vfe_KIAS_full	;	xos=-6;}
			if(en==arc_green	){col=col_green	;	arc_v1=xios.con.Vs_KIAS	;	arc_v2=xios.con.Vno_KIAS		;	xos=-1;}
			if(en==arc_yellow	){col=col_yellow;	arc_v1=xios.con.Vno_KIAS;	arc_v2=xios.con.Vne_KIAS		;	xos=-1;}
			if(en==arc_red		){col=col_red	;	arc_v1=xios.con.Vne_KIAS;	arc_v2=xios.con.Vne_KIAS+100	;	xos=-1;}

			OGL_fillrect(col,1.0,	x_tic+xos	,y_cen+	(arc_v1-val		)*pix_per_v,
									8			,		(arc_v2-arc_v1	)*pix_per_v);
		}
	}
	else
	{
		for(xint en=arc_yellow;en<=arc_red;en++)
		{
			const xflt terr_ft = xios.acft_p(0)->ter_ele_ft();

			if(en==arc_yellow	){col=col_yellow;	arc_v1=terr_ft	;	arc_v2=terr_ft+200.0;	xos=-7;}
			if(en==arc_red		){col=col_red	;	arc_v1=v1		;	arc_v2=terr_ft		;	xos=-7;}

			OGL_fillrect(col,1.0,	x_tic+xos	,y_cen+	(arc_v1-val		)*pix_per_v,
									8			,		(arc_v2-arc_v1	)*pix_per_v);
		}
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// FULL-HEIGHT SCROLLING TAPE IN THE BACKGROUND
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	for(xint v=v1;v<=v2;v+=dv_tic)
	{
		xflt y=y_cen+(v-val)*pix_per_v;
		if(intrange(y,y_cen-y_siz-20,y_cen+y_siz+20))
		{
				 if(v%dv_num==0)plot_ins_overlay(x_tic,y,0,(is_asi)?1:3,4,t_tickmarks);
			else if(v%dv_tic==0)plot_ins_overlay(x_tic,y,0,(is_asi)?0:2,4,t_tickmarks);
		}
	}

	for(xint v=v1;v<=v2;v+=dv_tic)
	if((is_asi && v>0) || (!is_asi))	// show neg alt, but not asi
	if(v%dv_num==0)
	{
		xflt y=y_cen+(v-val)*pix_per_v;
		if(intrange(y,y_cen-y_siz-20,y_cen+y_siz+20))
		{
			if(is_asi)
			{
				plot_number(t_font_3,x_tic-9,y,v				,3,0,col_white,1.0,str_lead_with_spaces	,just_rgt);
			}
			else
			{
				plot_number(t_font_3,x_tic+47,y,fltabs(v/1000)	,2,0,col_white,1.0,str_lead_with_spaces	,just_rgt);
				plot_number(t_font_2,x_tic+95,y,fltabs(v%1000)	,3,0,col_white,1.0,str_lead_with_zeros	,just_rgt);
			}
		}
	}

	sci_stop();

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// OVERLAY ON TOP OF THE BACKGROUND TAPE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if( is_asi)plot_ins_overlay(x_tic+overlay_dx,y_cen,0,0,2,t_layers_short_wide);	// overlay on top of the background tape
	if(!is_asi)plot_ins_overlay(x_tic+overlay_dx,y_cen,0,1,2,t_layers_short_wide);	// overlay on top of the background tape

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// NON-SCROLLING PART OF DIGITAL OVERLAY
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(is_asi)
	{
		if(too_slow)plot_string(t_font_3,x_tic-45,y_cen,"--");
		else		plot_number(t_font_3,x_tic-28,y_cen,(xint)(val+0.5)/10,2,0,col_white,1.0,str_lead_with_spaces,just_rgt);		// add 0.5 to switch the digit over at the half-knot point since the scrolling part has a label every knot
	}
	else
	{
		plot_number(t_font_3,x_tic+50,y_cen, (xint)(fltabs(val)+5.0)		/1000,2,0,col_white,1.0,str_lead_with_spaces,just_rgt);	// add 5.0 to switch the digit over at the 5-foot point since the scrolling part has a label 10 feet
		plot_number(t_font_2,x_tic+58,y_cen,((xint)(fltabs(val)+5.0)%1000)	/ 100,1,0,col_white,1.0,str_lead_with_spaces,just_ctr);	// add 5.0 to switch the digit over at the 5-foot point since the scrolling part has a label 10 feet

		if(val<0.0)plot_string(t_font_3,x_tic+25,y_cen,"-",col_white,1.0,just_rgt);
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SCROLLING PART OF DIGITAL OVERLAY
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(!too_slow)
	{
		sci_start(0,y_cen-36,graf.dev_dx_use(),36*2);
		if( is_asi)for(xflt v=intnear(val, 1)- 2;v<=intnear(val, 1)+ 2;v+= 1)if(v>=0)	plot_number(t_font_3,x_tic-19,y_cen+(v-val)*30.0, (xint)(       v )% 10		,1,0,col_white,1.0,str_lead_with_zeros,just_ctr);
		if(!is_asi)for(xflt v=intnear(val,10)-20;v<=intnear(val,10)+20;v+=10){			plot_number(t_font_2,x_tic+73,y_cen+(v-val)* 2.5,((xint)(fltabs(v))%100)/10	,1,0,col_white,1.0,str_lead_with_zeros,just_ctr);
																						plot_number(t_font_2,x_tic+88,y_cen+(v-val)* 2.5,				0			,1,0,col_white,1.0,str_lead_with_zeros,just_ctr);}
		sci_stop();
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SHADOW
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	plot_ins_overlay(x_tic+overlay_dx+((is_asi)?0:7),y_cen,0,(is_asi)?0:1,2,t_no_shadow);	// shadow on top of the digital display
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PFD
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void acft_PX_struct::plot_PFD(const xflt x1,const xflt y1,const xflt x2,const xflt y2)
{
	const xflt xc = (x1+x2)*0.5	;
	const xflt yc = (y1+y2)*0.5	;
	const xflt dx = (x2-x1)		;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// BACKGROUND SKY POLY
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	xflt the_ref=fltlim(the_deg,-25.0,25.0);						// limit to so the edge of the poly never goes off-screen, which looks really wrong.
	xflt horiz_x=xc+ang_to_pix(the_ref,dx)*sin(phi_deg*degtorad);
	xflt horiz_y=yc-ang_to_pix(the_ref,dx)*cos(phi_deg*degtorad);
	xflt rad	=pythag(x2-x1,y2-y1);

	sci_start(x1,y1,x2-x1,y2-y1);
	glPushMatrix();
	glTranslatef(horiz_x,horiz_y,0.0);
	glRotatef	(phi_deg,0.0,0.0,1.0);

	OGL_quad(t_sky,col_white,1.0,
		-rad,-rad , 0.75,-0.0,
		-rad, rad , 0.75, 1.0,
		 rad, rad , 0.75, 1.0,
		 rad,-rad , 0.75,-0.0);

	glPopMatrix();
	sci_stop();

	const xflt fov_top=atan( (y2-yc)/(x2-x1) * 2.0*tan(fov_lat*0.5*degtorad) )*radtodeg;	// since we are doing flat-plane projection,
	const xflt fov_bot=atan( (yc-y1)/(x2-x1) * 2.0*tan(fov_lat*0.5*degtorad) )*radtodeg;	// this is really the right answer

	OGL_set_viewport_2d(x1,y1,x2,y2);	// set the 2d pixels fo the viewport
	OGL_set_viewport_3d(-fov_lat*0.5,
						 fov_lat*0.5,
						-fov_bot	,
						 fov_top	,vis_deg*degtomtr);

	glEnable	(GL_DEPTH_TEST		);	// depth read
	glDepthMask	(GL_TRUE			);	// depth write
	glClear		(GL_DEPTH_BUFFER_BIT);	// depth buffer

	const xflt PEY_from_GPS=2.0;

	const xflt synvis_AGL=ele_mtr-synv.terr_ret_ele(lon_deg,lat_deg);
	
	xflt						os =0.0;
	if(synvis_AGL<PEY_from_GPS)	os+=PEY_from_GPS-synvis_AGL;

	glRotatef	( phi_deg		, 0.0,0.0,1.0);
	glRotatef	( the_deg		,-1.0,0.0,0.0);
	glRotatef	( psi_deg_true	, 0.0,1.0,0.0);
	glTranslatef(-lle_xyz.x_with_lon(lon_deg),
									-ele_mtr-PEY_from_GPS-os,	// add our viewpoint above the cg a bit
				 -lle_xyz.z_with_lat(lat_deg));

	synv.plot_terrain_3d(*this);
	OGL_set_viewport_2d(0,0,graf.dev_dx_use(),graf.dev_dy_use());

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PITCH LADDER
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	const xflt clip_x1=x1+ 85;
	const xflt clip_x2=x2-111;

	sci_start(clip_x1,y1,clip_x2-clip_x1,y2-y1);
	glPushMatrix();
	glTranslatef(xc,yc,0.0);
	glRotatef(phi_deg,0.0,0.0,1.0);

	for(xint t=-80;t<=80;t+=10)
	{
		xflt da=ang_to_pix((xflt)t-the_deg,dx);

			 if(t<-30)plot_ins_overlay(0,da,90,6,10,t_layers_tall_medium);	// emer pointer
		else if(t> 30)plot_ins_overlay(0,da,90,0,10,t_layers_tall_medium);	// emer pointer
		else if(t<  0)plot_ins_overlay(0,da,90,5,10,t_layers_tall_medium);
		else if(t>  0)plot_ins_overlay(0,da,90,1,10,t_layers_tall_medium);
		else if(t== 0)plot_ins_overlay(0,da,90,3,10,t_layers_tall_medium);

		if(intrange(t,-30,-10) || intrange(t,10,30))
		{
			plot_number(t_font_1_shad,-60,da,t,2,0,col_white,1.0,str_lead_with_spaces,just_rgt);
			plot_number(t_font_1_shad, 60,da,t,2,0,col_white,1.0,str_lead_with_spaces,just_lft);
		}
	}

	glPopMatrix();
	sci_stop();

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ROLL REFERENCE TICKS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	const xflt slip_pix=xc-ang_to_pix(slip_deg,dx)*0.4;	// 0.4 just seems about right... 1.0 is just way too much.

	for(xint phi=-45;phi<=45;phi+=45)			plot_ins_overlay(xc			,yc,phi-phi_deg	,2,24,t_layers_tall_narrow);	// roll pointer
	for(xint phi=-60;phi<=60;phi+=30)if(phi!=0)	plot_ins_overlay(xc			,yc,phi-phi_deg	,1,24,t_layers_tall_narrow);	// roll pointer
	for(xint phi=-20;phi<=20;phi+=10)if(phi!=0)	plot_ins_overlay(xc			,yc,phi-phi_deg	,0,24,t_layers_tall_narrow);	// roll pointer
												plot_ins_overlay(xc			,yc,0			,3,24,t_layers_tall_narrow);	// roll REF pointer
												plot_ins_overlay(slip_pix	,yc,0			,4,24,t_layers_tall_narrow);	// slip REF pointer

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ASI-ALT TAPES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	const xflt tape1=x1+75	;
	const xflt tape2=x2-75-26	;

	plot_asi_alt_tape(tape1,yc,168,AV_KEAS			,xtrue);
	plot_asi_alt_tape(tape2,yc,168,ele_mtr*mtrstoft	,xfals);

	if(xios.con.nav_gls_int)
	{
		const xflt cdi_dy=-xios.con.nav_CDI_vrt*39.0;

		plot_ins_overlay(xc-200,yc			,0, 6,24 , t_layers_tall_narrow);	// CDI scale
		plot_ins_overlay(xc+200,yc			,0, 9,24 , t_layers_tall_narrow);	// CDI scale
		plot_ins_overlay(xc-180,yc+cdi_dy	,0, 7,24 , t_layers_tall_narrow);	// CDI
		plot_ins_overlay(xc+180,yc+cdi_dy	,0, 8,24 , t_layers_tall_narrow);	// CDI
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// VVI
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	const xint vvi_y_now=interp(-4000.0,yc-190.0,
								 4000.0,yc+190.0,vvi_fpm);

	plot_ins_overlay(			tape2-48.0,vvi_y_now,0,5,6,t_pfd);
	plot_number		(t_font_2,	tape2-24.0,vvi_y_now,fltnear(vvi_fpm,10.0),1,0,col_white,1.0,str_lead_with_spaces,just_rgt);	// use fltabs here or the neg sign overdraws the vvi box and looks annoying

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// HSI
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	sci_start(x1,y1,x2-x1,y2-y1);

	const xint hsi_yc=y1;

	plot_ins_overlay(xc,hsi_yc,-psi_deg_mag,0,1,t_rose_pfd,col_white);

	for(xint psi=0;psi<360;psi+=30)
	{
		const xflt hsi_rad	=140.0;
		const xflt rel_psi	=psi-psi_deg_mag;
		const xflt x		=xc		+hsi_rad*sin(rel_psi*degtorad);
		const xflt y		=hsi_yc	+hsi_rad*cos(rel_psi*degtorad);
		const xflt scdx		= 7.0;	// character size for offsets
		const xflt  cdx		=11.0;

		if(psi==  0)plot_string(t_font_1,x-scdx,y, "N",col_white,1.0,just_lft,draw_buildup);
		if(psi== 90)plot_string(t_font_1,x-scdx,y, "E",col_white,1.0,just_lft,draw_buildup);
		if(psi==180)plot_string(t_font_1,x-scdx,y, "S",col_white,1.0,just_lft,draw_buildup);
		if(psi==270)plot_string(t_font_1,x-scdx,y, "W",col_white,1.0,just_lft,draw_buildup);
		if(psi== 30)plot_string(t_font_1,x-scdx,y, "3",col_white,1.0,just_lft,draw_buildup);
		if(psi== 60)plot_string(t_font_1,x-scdx,y, "6",col_white,1.0,just_lft,draw_buildup);
		if(psi==120)plot_string(t_font_1,x- cdx,y,"12",col_white,1.0,just_lft,draw_buildup);
		if(psi==150)plot_string(t_font_1,x- cdx,y,"15",col_white,1.0,just_lft,draw_buildup);
		if(psi==210)plot_string(t_font_1,x- cdx,y,"21",col_white,1.0,just_lft,draw_buildup);
		if(psi==240)plot_string(t_font_1,x- cdx,y,"24",col_white,1.0,just_lft,draw_buildup);
		if(psi==300)plot_string(t_font_1,x- cdx,y,"30",col_white,1.0,just_lft,draw_buildup);
		if(psi==330)plot_string(t_font_1,x- cdx,y,"33",col_white,1.0,just_lft,draw_buildup);
	}

	plot_string		(t_font_1,0,0,"",col_white,1.0	,just_lft,draw_finish);															// wrap up HSI headings
	plot_ins_overlay(				xc,hsi_yc+182	,0,0,6,t_pfd,col_white);														// HSI heading text-box
	plot_number		(t_font_3_shad,	xc,hsi_yc+184	,intwrap(psi_deg_mag,1,360),3,0,col_white,1.0,str_lead_with_zeros,just_ctr);	// HSI heading text

	const xflt ntf_dx= 75.0							*cos((xios.con.nav_OBS_deg-psi_deg_mag)*degtorad);
	const xflt ntf_dy=-75.0							*sin((xios.con.nav_OBS_deg-psi_deg_mag)*degtorad);
	const xflt cdi_dx= xios.con.nav_CDI_lat*39.0	*cos((xios.con.nav_OBS_deg-psi_deg_mag)*degtorad);
	const xflt cdi_dy=-xios.con.nav_CDI_lat*39.0	*sin((xios.con.nav_OBS_deg-psi_deg_mag)*degtorad);

										 plot_ins_overlay(xc		,hsi_yc			,xios.con.nav_OBS_deg-psi_deg_mag+90,  9,24 , t_layers_tall_narrow);	// CDI scale
										 plot_ins_overlay(xc		,hsi_yc			,xios.con.nav_OBS_deg-psi_deg_mag 	, 10,24 , t_layers_tall_narrow);	// CDI ref
	if(xios.con.nav_ntf_int!=nav_flag	)plot_ins_overlay(xc+cdi_dx	,hsi_yc+cdi_dy	,xios.con.nav_OBS_deg-psi_deg_mag 	, 11,24 , t_layers_tall_narrow);	// CDI
	if(xios.con.nav_ntf_int==nav_to		)plot_ins_overlay(xc+ntf_dx	,hsi_yc+ntf_dy	,xios.con.nav_OBS_deg-psi_deg_mag 	, 12,24 , t_layers_tall_narrow);	// to/from
	if(xios.con.nav_ntf_int==nav_from	)plot_ins_overlay(xc+ntf_dx	,hsi_yc+ntf_dy	,xios.con.nav_OBS_deg-psi_deg_mag 	, 13,24 , t_layers_tall_narrow);	// to/from
	sci_stop();

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// FLITE DIRECTOR WATERLINE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	plot_ins_overlay(xc,yc,90,9,10,t_layers_tall_medium);	// attitude reference waterline
}
