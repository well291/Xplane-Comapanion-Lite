//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xflt delta(const xflt in)
{
	if(in<      5)return      0.1;
	if(in<     10)return      0.2;
	if(in<     20)return      0.5;
	if(in<     50)return      1;
	if(in<    100)return      2;
	if(in<    200)return      5;
	if(in<    500)return     10;
	if(in<   1000)return     20;
	if(in<   2000)return     50;
	if(in<   5000)return    100;
	if(in<  10000)return    200;
	if(in<  20000)return    500;
	if(in<  50000)return   1000;
	if(in< 100000)return   2000;
	if(in< 200000)return   5000;
	if(in< 500000)return  10000;
	if(in<1000000)return  20000;
	if(in<2000000)return  50000;
	if(in<5000000)return 100000;
				  return 200000;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xflt x1_wb=0;
xflt x2_wb=0;
xflt y1_wb=0;
xflt y2_wb=0;

xflt px_wb(const xflt in){return extrap(xios.acft_p(xios.acf_edit)->for_cg_in_enter		,x1_wb , xios.acft_p(xios.acf_edit)->aft_cg_in_enter	,x2_wb , in);}
xflt py_wb(const xflt in){return extrap(xios.acft_p(xios.acf_edit)->min_wgt_lb_enter()	,y1_wb , xios.acft_p(xios.acf_edit)->max_wgt_lb_enter()	,y2_wb , in);}

void cg_line(const xflt w1,const xflt cg1,
			 const xflt w2,const xflt cg2,const enum_col color,const xflt alpha){OGL_line(color,alpha,px_wb(cg1),py_wb(w1),px_wb(cg2),py_wb(w2));}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void xios_class::xios_weight()
{
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// MENUS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	enum
	{
		menu_payload=0	,
		menu_weapons	,
		menu_dim
	};

	static xint menu_mode = menu_payload;

	if(SUPPORT_1208)
	{
		acf_edit = 0;
		menu_mode = menu_payload;
	}
	else
	{
		xios_acf_menus(xfals);
		xchr names[menu_dim][name_dim80] = {"PAYLOAD","WEAPONS"};
		butn_menus(&menu_mode,menu_dim,names,2);
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PAYLOAD
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(menu_mode==menu_payload)
	{
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// CONS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		x1_wb = bx1();
		x2_wb = bx2();
		y1_wb = butn_dy*11			+ ui_sep_pix*3;
		y2_wb = graf.dev_dy_use()	- ui_sep_pix*2;
	
		const xflt x1_book = px_wb(acft_p(acf_edit)->for_cg_in_book	);
		const xflt x2_book = px_wb(acft_p(acf_edit)->aft_cg_in_book	);
		const xflt y1_book = py_wb(acft_p(acf_edit)->min_wgt_lb_book);
		const xflt y2_book = py_wb(acft_p(acf_edit)->max_wgt_lb_book);
	
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SLIDERS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		xint send=0;
	
		xflt	y =y1_wb-ui_sep_pix*2;		// enevelope down to bottom of background and then a border below that for the buttons
				y-=butn_dy*0.5;				// top of button to center of button
		
		for(xint t=0;t<fueltankDIM;t++)
		if(acft_p(acf_edit)->max_fuel_lb[t]>0.0)
		{
			if(t%2==0)y-=butn_dy;
	
			xflt	dx =(bx2()-bx1()-ui_sep_pix)*0.5;
			xflt	 x =bx1()+dx*(t%2);
			if(t%2)	 x+=ui_sep_pix*0.5;
	
			if(!weight_kg && butn_slide(x,y,dx,"Fuel Tank "+int_to_str(t+1),&acft_p(acf_edit)->crt_fuel_lb[t],"lb",delta(acft_p(acf_edit)->max_fuel_lb[t]),0,acft_p(acf_edit)->max_fuel_lb[t]			))send=1;
			if( weight_kg && butn_slide(x,y,dx,"Fuel Tank "+int_to_str(t+1),&acft_p(acf_edit)->crt_fuel_lb[t],"kg",delta(acft_p(acf_edit)->max_fuel_lb[t]),0,acft_p(acf_edit)->max_fuel_lb[t],lbstokgs	))send=1;
		}
	
		y-=butn_dy;

		for(xint t=0;t<fixedDIM;t++)
		if(acft_p(acf_edit)->max_payl_lb[t]>0.0)
		{
			if(t%2==0)y-=butn_dy;
		
			xflt	dx =(bx2()-bx1()-ui_sep_pix)*0.5;
			xflt	 x =bx1()+dx*(t%2);
			if(t%2)	 x+=ui_sep_pix*0.5;
		
			if(!weight_kg && butn_slide(x,y,dx,acft_p(acf_edit)->chr_payl_name[t],&acft_p(acf_edit)->crt_payl_lb[t],"lb",delta(acft_p(acf_edit)->max_payl_lb[t]),0,acft_p(acf_edit)->max_payl_lb[t]				))send=1;
			if( weight_kg && butn_slide(x,y,dx,acft_p(acf_edit)->chr_payl_name[t],&acft_p(acf_edit)->crt_payl_lb[t],"kg",delta(acft_p(acf_edit)->max_payl_lb[t]),0,acft_p(acf_edit)->max_payl_lb[t],lbstokgs	))send=1;
		}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// W & B DISPLAY
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		OGL_fillrect(col_butn_drk,1.0 , x1_wb,y1_wb,x2_wb-x1_wb,y2_wb-y1_wb);
		OGL_fillrect(col_butn_med,1.0 , x1_book ,y1_book ,x2_book -x1_book ,y2_book -y1_book );
	
		cg_line(acft_p(acf_edit)->min_wgt_lb_book,acft_p(acf_edit)->for_cg_in_book , acft_p(acf_edit)->max_wgt_lb_book,acft_p(acf_edit)->for_cg_in_book , col_black,1.0);	// box border
		cg_line(acft_p(acf_edit)->max_wgt_lb_book,acft_p(acf_edit)->for_cg_in_book , acft_p(acf_edit)->max_wgt_lb_book,acft_p(acf_edit)->aft_cg_in_book , col_black,1.0);
		cg_line(acft_p(acf_edit)->max_wgt_lb_book,acft_p(acf_edit)->aft_cg_in_book , acft_p(acf_edit)->min_wgt_lb_book,acft_p(acf_edit)->aft_cg_in_book , col_black,1.0);
		cg_line(acft_p(acf_edit)->min_wgt_lb_book,acft_p(acf_edit)->aft_cg_in_book , acft_p(acf_edit)->min_wgt_lb_book,acft_p(acf_edit)->for_cg_in_book , col_black,1.0);
	
		const xflt mult		= (weight_kg)?lbstokgs:1.0;
		const xflt del_wgt	= delta	((acft_p(acf_edit)->max_wgt_lb_enter()-acft_p(acf_edit)->min_wgt_lb_enter())*4.0*mult)/mult;
		const xflt wgt1		= fltnear(acft_p(acf_edit)->min_wgt_lb_enter(),del_wgt);
		const xflt wgt2		= fltnear(acft_p(acf_edit)->max_wgt_lb_enter(),del_wgt);
	
		sci_start(0,y1_wb,graf.dev_dx_use(),y2_wb-y1_wb);
		for(xflt w=wgt1;w<=wgt2;w+=del_wgt)
		{
			cg_line(w,acft_p(acf_edit)->for_cg_in_book , w,acft_p(acf_edit)->aft_cg_in_book , col_text_back,1.0);
	
			if(!weight_kg)plot_string(t_font_2,x2_book+10.0,py_wb(w),int_to_str(intround(w		),1,str_show_with_commas)+" lb",col_text_back,1.0,just_lft);
			if( weight_kg)plot_string(t_font_2,x2_book+10.0,py_wb(w),int_to_str(intround(w*mult	),1,str_show_with_commas)+" kg",col_text_back,1.0,just_lft);
		}
		sci_stop();
	
		xflt								crt_arm = acft_p(acf_edit)->crt_cg_in		;
		xflt								crt_wgt = acft_p(acf_edit)->min_wgt_lb_book	;
		for(xint t=0;t<fueltankDIM	;t++)	crt_wgt+= acft_p(acf_edit)->crt_fuel_lb[t]	;
		for(xint t=0;t<fixedDIM		;t++)	crt_wgt+= acft_p(acf_edit)->crt_payl_lb[t]	;
	
		glLineWidth(2);
		cg_line(acft_p(acf_edit)->min_wgt_lb_book	,crt_arm							, acft_p(acf_edit)->max_wgt_lb_book	,crt_arm							, col_butn_brt,1.0);	// ref lines
		cg_line(crt_wgt		 						,acft_p(acf_edit)->for_cg_in_book	, crt_wgt							,acft_p(acf_edit)->aft_cg_in_book	, col_butn_brt,1.0);
		glLineWidth(1);
	
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// W & B DRAG
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		const xflt cgx = px_wb(crt_arm);
		const xflt cgy = py_wb(crt_wgt);
		butn_action(cgx-butn_dy*0.5,cgy,butn_dy,"CG","");

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// NET
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		if(send)
		{
			xios_send_xWGT(acf_edit);
			real_time_mod_wgt = xtim.real_time;
		}
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// WEAPONS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(menu_mode==menu_weapons)
	{
		const xflt y2		= graf.dev_dy_use();
		const xflt y1		= butn_dy*3;
		const xflt scroll	= butn_scroll_reg(wpn_files.size()*butn_dy , y1,y2);

		sci_start(0,y1,graf.dev_dx_use(),y2-y1);

		for(xint s=0;s<acft_p(acf_edit)->wpn_num_stations;s++)
		{
			const	xflt x1 = xos(s,acft_p(acf_edit)->wpn_num_stations);
			const	xflt dx = xdx(	acft_p(acf_edit)->wpn_num_stations);
					xflt y  = y2 - butn_dy*0.5 + scroll;

			for(xint w=0;w<wpn_files.size();w++)
			{
				string	disp_name=wpn_files[w];	// i deal in files
						disp_name.erase(strlen(wpn_files[w].c_str())-strlen(".wpn"),strlen(".wpn"));

				for(xint d=0;d<	disp_name.size();d++)
				if(				disp_name[d]=='_' ||
								disp_name[d]=='-')
								disp_name[d]= ' ';

				string s1,s2;
				half_name(disp_name,s1,s2);

				const xint hilited = (wpn_files[w] == string(acft_p(acf_edit)->wpn_chr_now[s]));

				if(butn_on_off(hilited , x1,y,dx,s1,s2))
				{
					const string name = (hilited) ? "" : wpn_files[w];
					xios_send_LWPN(acf_edit,s,name);
				}
				y-=butn_dy;
			}
		}

		sci_stop();
	}
}
