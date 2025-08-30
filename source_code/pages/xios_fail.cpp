static xint edit_failures=0;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void xios_class::xios_failures_avail()
{
	const xflt y2 = graf.dev_dy_use()	;
	const xflt y1 = butn_dy*2.5			;

	sci_start(bx1(),y1,bx2()-bx1(),y2-y1);

	xflt y = graf.dev_dy_use()-ui_sep_pix-butn_dy*0.5+butn_scroll_reg(vec_fal.size()*butn_dy , y1,y2);

	for(xint f=0;f<vec_fal.size();f++)
	if(vec_fal[f].fail_name[0])
	if(vec_fal[f].gone_present_failed!=-1)	// if we have the part on the plane then xconsider it for selection... otherwise the list goes on FOREVER
	{
		const xint has_it=fail_names_we_show.find(vec_fal[f].fail_name)!=fail_names_we_show.end();

		if(butn_on_off(has_it,bx1(),y,bdx(),"SHOW "+(string)vec_fal[f].fail_name,""))
		{
			if(has_it)	fail_names_we_show.erase (vec_fal[f].fail_name);
			else		fail_names_we_show.insert(vec_fal[f].fail_name);
		}

		y-=butn_dy;
	}

	sci_stop();

	if(butn_action(bx1(),butn_dy*2,bdx(),"DONE"))
		edit_failures=0;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void xios_class::xios_failures_sel()
{
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// MENUS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	static xint menu2_mode[group_dim]={0};
	static xint menu1_mode			 = 0 ;

	xint g1_dim = 0;	for(xint g=0;g<group_dim;g++)if(group1_name			   [g][0])g1_dim = g+1;
	xint g2_dim = 0;	for(xint g=0;g<group_dim;g++)if(group2_name[menu1_mode][g][0])g2_dim = g+1;
	if(	 g1_dim<= 0)	// we don't have enough failures in to fill the screen yet. do not try to access a 0-len array below!
	{
		const xflt cx = graf.dev_cx_use();
		const xflt cy = graf.dev_cy_use();

		plot_string(t_font_2,cx,cy+font_2_dy*0.5,"Getting all the latest failure options from X-Plane!"	,col_text_back,1.0,just_ctr);
		plot_string(t_font_2,cx,cy-font_2_dy*0.5,"It takes a few minutes... check back in a bit!"		,col_text_back,1.0,just_ctr);
		return;
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// MENUS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	menu1_mode				=intlim(menu1_mode				,0,g1_dim-1);	// when we switch around to other failure menus
	menu2_mode[menu1_mode]	=intlim(menu2_mode[menu1_mode]	,0,g2_dim-1);	// make sure we always have some item selected

	if(g2_dim>0)butn_menus(&menu2_mode[menu1_mode]	,g2_dim,group2_name[menu1_mode]	,2);
	if(g1_dim>0)butn_menus(&menu1_mode				,g1_dim,group1_name				,1);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// MAIN FAILURES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
			xflt dx			=(bx2()-bx1()-ui_sep_pix*2.0)/3.0;
	const	xflt x1			= bx1()+dx*0+ui_sep_pix*0;
	const	xflt x2			= bx1()+dx*1+ui_sep_pix*1;
	const	xflt x3			= bx1()+dx*2+ui_sep_pix*2;
	const	xflt x4			= bx1()+dx*3+ui_sep_pix*3;
	const	xflt y_top		= graf.dev_dy_use()-ui_sep_pix-butn_dy*0.5;	// we plot to center of button

	static 	xflt ui_dy		= 0.0;
			xflt bar_y_hi	= graf.dev_dy_use()	-ui_sep_pix;
			xflt bar_y_lo	= butn_dy*3			+ui_sep_pix;

	xflt y2=y_top+butn_scroll_reg(ui_dy , bar_y_lo,bar_y_hi);
	xflt x =x1;	// x to start
	xflt y =y2;

	ui_dy=0;

	#define start_new_group  1
	#define start_new_column 2

	sci_start(0,bar_y_lo,graf.dev_dx_use(),bar_y_hi-bar_y_lo);

	for(xint f=0;f<vec_fal.size();f++)
	if(vec_fal[f].fail_name[0]										)
	if(vec_fal[f].g1==menu1_mode									)
	if(vec_fal[f].g2==menu2_mode[menu1_mode] || vec_fal[f].g2==-1	)		// g2 of -1 means there are no sub-menus, and g2 was limited to 0-1!
	if(fail_names_we_show.find(vec_fal[f].fail_name) != fail_names_we_show.end())
	{
		if(vec_fal[f].fail_ui==start_new_group )y-=butn_dy;					// spacer step
		if(vec_fal[f].fail_ui==start_new_column)							// new column
		{
				 if(x==x1){x=x2;	y=y2;	dx=x3-x2-ui_sep_pix;}
			else if(x==x2){x=x3;	y=y2;	dx=x4-x3-ui_sep_pix;}
		}

		if(vec_fal[f].gone_present_failed!=-1)								// we need to go in here even if the failure is not in existence so we do the new column heads above!
		{
			string s1,s2;
			half_name(vec_fal[f].fail_name,s1,s2);

			if(butn_on_off(vec_fal[f].gone_present_failed,x,y,dx,s1,s2))	// so we only plot the button if the system exists, but we must track new column heads either way!
			{
				const string s = int_to_str(f);

					vec_fal[f].gone_present_failed=
				   !vec_fal[f].gone_present_failed;
				if( vec_fal[f].gone_present_failed)inet.inet_send("FAIL",(xchr*)s.c_str(),(xint)s.size());
				if(!vec_fal[f].gone_present_failed)inet.inet_send("RECO",(xchr*)s.c_str(),(xint)s.size());
			}

			y-=butn_dy;
			ui_dy=fltmax2(ui_dy,y2-y);
		}
	}

	sci_stop();

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// FRONT PAGE: EXTRA STUFF
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(menu1_mode==0)
	{
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// FRONT PAGE: FAIL ARMED TEXT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		y=butn_dy*4;
		for(xint f=0;f<vec_fal.size();f++)
		if(vec_fal[f].fail_name[0])
		if(vec_fal[f].gone_present_failed==1)
		{
			plot_string(t_font_2 , (bx1()+bx2())*0.5,y , (string)vec_fal[f].fail_name+" failed",col_text_back,1.0,just_ctr);
			y+=font_2_dy;
		}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// FRONT PAGE: LOWER MENUS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		if(butn_action(x1,butn_dy*3,x3-x2-ui_sep_pix,"REPAIR ALL"))
		{
			inet.inet_send("RESE","",0);
			for(xint f=0;f<vec_fal.size();f++)
			if(	vec_fal[f].gone_present_failed==1)
				vec_fal[f].gone_present_failed= 0;
		}

		if(butn_action(x2,butn_dy*3,x3-x2-ui_sep_pix,"QUICK-START"))
		{
			const string str="sim/operation/quick_start";
			inet.inet_send("CMND",(xchr*)str.c_str(),(xint)str.size());
		}

		if(butn_action(x3,butn_dy*3,x3-x2-ui_sep_pix,"VISIBLE FAILURES","(set failures to display)"))
			edit_failures=1;
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// LAST PAGE: CUSTOM COMMANDS THAT ARE FAILURES, AS ROMAN AND PFC-MIKE WANT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if((string)group1_name[menu1_mode]=="Custom")
		xios_command_list(cmnd_failures);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void xios_class::xios_failures()
{
	if(!edit_failures)xios_failures_sel		();
	if( edit_failures)xios_failures_avail	();
}
