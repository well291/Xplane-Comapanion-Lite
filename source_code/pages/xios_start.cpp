//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// WINDOW
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void xios_class::xios_start()
{
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// MENUS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	xios_acf_menus(xfals);

	enum
	{
		menu_ramp=0	,
		menu_runway	,
		menu_dim
	};
	static xint menu_mode = menu_runway;

	if(RUNNING_1208)	// 1208 does not give us the ramp starts
	{
		menu_mode = menu_runway;
	}
	else
	{
		xchr names[menu_dim][name_dim80]={"RAMPS","RUNWAYS"};
		butn_menus(&menu_mode,menu_dim,names,2);
	}


//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// KEYBOARD
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	xflt y_keyboard		= butn_dy*8				+ui_sep_pix;
	xflt y_name_display	= y_keyboard+font_2_dy	+ui_sep_pix;

	static xint apt_sel	= 1;
	static vector <string> choices;
	static vector <xint>   indices;

	static xint											lookup=1;	// we need to look up on the very first call,
	if(get_keyboard(y_keyboard,apt_search,name_dim64))	lookup=1;	// even if no key touched yet and we just have the pref'd ID

	if(lookup)
	{
		lookup=0;
		apt_sel=1;
		choices.clear();
		indices.clear();

		string s1=				apt_search;	// search KCAE
		string s2="K"+(string)	apt_search;	// search  CAE

		if(	apt_icao_to_index.find(s1)!=
			apt_icao_to_index.end (	 )){
			choices.push_back(vec_apt[apt_icao_to_index[s1]].nam.apt_name	);
			indices.push_back(		  apt_icao_to_index[s1]					);
		}

		if(	apt_icao_to_index.find(s2)!=
			apt_icao_to_index.end (  )){
			choices.push_back(vec_apt[apt_icao_to_index[s2]].nam.apt_name	);
			indices.push_back(		  apt_icao_to_index[s2]					);}

		for(xint index=0;index<vec_apt.size();index++)
		if(find_string_no_case  (vec_apt[index].nam.apt_name,apt_search)	){	// search name as well!
			choices.push_back	(vec_apt[index].nam.apt_name				);
			indices.push_back	(		 index								);}
	}

	plot_text_box(	y_name_display-font_2_dy,
					y_name_display+font_2_dy,xfals);

	plot_string(t_font_2,graf.dev_cx_use(),y_name_display,"Start at "+(string)apt_search+(string)"|",col_text_back,1.0,just_ctr);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// APPROACH OPTIONS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	const xflt x1 = bx1()									;
	const xflt x2 = bx2()									;
	const xflt y1 = y_name_display+font_2_dy	+ui_sep_pix	;
	const xflt y2 = graf.dev_dy_use()			-ui_sep_pix	;
	const xflt yt = y2+butn_dy*0.5							;

	sci_start(0,y1,graf.dev_dx_use(),y2-y1);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// APT SEL
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(apt_sel)
	{
		const xflt x = x1;
			  xflt y = yt + butn_scroll_reg(choices.size()*butn_dy , y1,y2);

		for(xint c=0;c<choices.size();c++)
		if(fltrange(y-=butn_dy,y1-butn_dy,y2+butn_dy))
		if(butn_action(x,y,x2-x1,choices[c]))
		{
			apt_sel=0;
			C_eq_C(apt_ref_id,vec_apt[indices[c]].nam.apt_id,id_dim8);
		}
	}
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// RAMP OR RWY SEL
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	else
	if(	apt_icao_to_index.find(apt_ref_id	)!=
		apt_icao_to_index.end (				))
	{
		xint index	= apt_icao_to_index[apt_ref_id];
		xint stuff	= (menu_mode == menu_ramp) ? ((xint)vec_apt[index].vec_ram.size()/4 + 1) : ((xint)vec_apt[index].vec_rwy.size()*2 + 1);
		xflt y		= yt + butn_scroll_reg(stuff*butn_dy , y1,y2)-butn_dy;
	
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// RAMP SEL
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		if(menu_mode == menu_ramp)
		{
				xint i=0;
			for(xint r=0;r<vec_apt[index].vec_ram.size();r++)
			{
				if(butn_action(xos(i,4),y,xdx(4),vec_apt[index].vec_ram[r].ram_name,""))
				{
					PREL_struct	PREL;
								PREL.p_idx			= (is_pro_use()) ? acf_edit : 0;
								PREL.type_start		= loc_ram;
					C_eq_C(		PREL.apt_id			, apt_ref_id,8);
								PREL.apt_rwy_idx	= r;

					xios_send_PREL(acf_edit,PREL);
				}
				
					i++;
				if(	i==4){
					i=0;
					y-=butn_dy;}
			}
		}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// RWY SEL
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		if(menu_mode == menu_runway)
		for(xint r=0;r<vec_apt[index].vec_rwy.size();r++)
		{
			PREL_struct PREL;
						PREL.p_idx		=(is_pro_use()) ? acf_edit : 0;
						PREL.type_start	=loc_tak		;
				C_eq_C(	PREL.apt_id		,apt_ref_id,8)	;
						PREL.apt_rwy_idx= r				;
						PREL.apt_rwy_dir=-1				;

			const string s1 = (string)vec_apt[index].vec_rwy[r].rwy_num1+" takeoff";
			const string s2 = (string)vec_apt[index].vec_rwy[r].rwy_num2+" takeoff";

			if(butn_action(xos(0,7),y,xdx(7),s1				,"")){PREL.type_start = loc_tak		;	PREL.apt_rwy_dir=0;}
			if(butn_action(xos(1,7),y,xdx(7),"45-to-entry"	,"")){PREL.type_start = loc_entry	;	PREL.apt_rwy_dir=0;}
			if(butn_action(xos(2,7),y,xdx(7),"downwind"		,"")){PREL.type_start = loc_down	;	PREL.apt_rwy_dir=0;}
			if(butn_action(xos(3,7),y,xdx(7),"base"			,"")){PREL.type_start = loc_base	;	PREL.apt_rwy_dir=0;}
			if(butn_action(xos(4,7),y,xdx(7),"VFR final"	,"")){PREL.type_start = loc_vfr		;	PREL.apt_rwy_dir=0;}
			if(butn_action(xos(5,7),y,xdx(7),"IFR final"	,"")){PREL.type_start = loc_ifr		;	PREL.apt_rwy_dir=0;}
			if(butn_action(xos(6,7),y,xdx(7),"45-to-final"	,"")){PREL.type_start = loc_45final	;	PREL.apt_rwy_dir=0;}
			y-=butn_dy;

			if(butn_action(xos(0,7),y,xdx(7),s2				,"")){PREL.type_start = loc_tak		;	PREL.apt_rwy_dir=1;}
			if(butn_action(xos(1,7),y,xdx(7),"45-to-entry"	,"")){PREL.type_start = loc_entry	;	PREL.apt_rwy_dir=1;}
			if(butn_action(xos(2,7),y,xdx(7),"downwind"		,"")){PREL.type_start = loc_down	;	PREL.apt_rwy_dir=1;}
			if(butn_action(xos(3,7),y,xdx(7),"base"			,"")){PREL.type_start = loc_base	;	PREL.apt_rwy_dir=1;}
			if(butn_action(xos(4,7),y,xdx(7),"VFR final"	,"")){PREL.type_start = loc_vfr		;	PREL.apt_rwy_dir=1;}
			if(butn_action(xos(5,7),y,xdx(7),"IFR final"	,"")){PREL.type_start = loc_ifr		;	PREL.apt_rwy_dir=1;}
			if(butn_action(xos(6,7),y,xdx(7),"45-to-final"	,"")){PREL.type_start = loc_45final	;	PREL.apt_rwy_dir=1;}
			y-=butn_dy;

			if(PREL.apt_rwy_dir >= 0)
			{
				if(	(PREL.type_start == loc_tak		) ||
					(PREL.type_start == loc_entry	) ||
					(PREL.type_start == loc_down	) ||
					(PREL.type_start == loc_base	) ||
					(PREL.type_start == loc_45final	) )
				{
					xflt lon_tdz = 0.0;	// compared to runway center, in runway semi-lens
					xflt lat_tdz = 0.0;	// compared to runway center, in runway semi-lens
					xflt psi_tdz = 0.0;	// compared to runway center, rel to final
	
					if(PREL.type_start == loc_tak		){									psi_tdz = 			PREL.apt_rwy_dir*180.0;}
					if(PREL.type_start == loc_entry		){lon_tdz =-1.0;	lat_tdz = 2.0;	psi_tdz = 135.0 +	PREL.apt_rwy_dir*180.0;}
					if(PREL.type_start == loc_down		){					lat_tdz = 1.0;	psi_tdz = 180.0 +	PREL.apt_rwy_dir*180.0;}
					if(PREL.type_start == loc_base		){lon_tdz = 1.5;	lat_tdz = 1.0;	psi_tdz =  90.0 +	PREL.apt_rwy_dir*180.0;}
					if(PREL.type_start == loc_45final	){lon_tdz = 4.0;	lat_tdz = 1.0;	psi_tdz =  45.0 +	PREL.apt_rwy_dir*180.0;}
	
					const xdob sin_psi			= sin( (vec_apt[index].vec_rwy[r].rwy_psi_deg() + PREL.apt_rwy_dir*180.0)*degtorad );
					const xdob cos_psi			= cos( (vec_apt[index].vec_rwy[r].rwy_psi_deg() + PREL.apt_rwy_dir*180.0)*degtorad );
					const xdob mtr_to_lat_deg	= mtrtodeg;
					const xdob mtr_to_lon_deg	= mtrtodeg/cos(vec_apt[index].vec_rwy[r].rwy_lat_ctr()*degtorad);
					const xdob rwy_to_lat_deg	= vec_apt[index].vec_rwy[r].rwy_len_m() * 0.5 * mtr_to_lat_deg;
					const xdob rwy_to_lon_deg	= vec_apt[index].vec_rwy[r].rwy_len_m() * 0.5 * mtr_to_lon_deg;
	
					if(PREL.type_start == loc_tak)	// offset by the aircraft index to avoid overlapped planes in the lab case
					{
						xdob lon_os_mtr = 0.0;
						xdob lat_os_mtr = 0.0;

						if(is_lab_use())			// don't let planes jumble up
						{
							lon_os_mtr =  acf_edit * 25.0;
							lat_os_mtr = (acf_edit%2) ? -15.0 : 15.0;
						}

						PREL.dob_lon_deg = (PREL.apt_rwy_dir==0) ? vec_apt[index].vec_rwy[r].rwy_lon1 : vec_apt[index].vec_rwy[r].rwy_lon2;
						PREL.dob_lat_deg = (PREL.apt_rwy_dir==0) ? vec_apt[index].vec_rwy[r].rwy_lat1 : vec_apt[index].vec_rwy[r].rwy_lat2;

						PREL.dob_lon_deg += lon_os_mtr * mtr_to_lon_deg * sin_psi;
						PREL.dob_lat_deg += lon_os_mtr * mtr_to_lat_deg * cos_psi;

						PREL.dob_lon_deg += lat_os_mtr * mtr_to_lon_deg * cos_psi;
						PREL.dob_lat_deg -= lat_os_mtr * mtr_to_lat_deg * sin_psi;

						PREL.dob_ele_mtr = vec_apt[index].nam.apt_ele_mtr - 50.0;	// get us DOWN for sloping airport meshes so wew are on the ground all the time
						PREL.dob_psi_tru = fltwrap(vec_apt[index].vec_rwy[r].rwy_psi_deg()+psi_tdz , 0.0,360.0);
						PREL.dob_spd_msc = 0.0;
					}
					else							// all the start types we could want
					{
						PREL.dob_lon_deg = vec_apt[index].vec_rwy[r].rwy_lon_ctr() - lon_tdz*sin_psi*rwy_to_lon_deg - lat_tdz*cos_psi*rwy_to_lon_deg;
						PREL.dob_lat_deg = vec_apt[index].vec_rwy[r].rwy_lat_ctr() - lon_tdz*cos_psi*rwy_to_lat_deg + lat_tdz*sin_psi*rwy_to_lat_deg;
						PREL.dob_ele_mtr = vec_apt[index].nam.apt_ele_mtr + 1000.0*fttomtrs;
						PREL.dob_psi_tru = fltwrap(vec_apt[index].vec_rwy[r].rwy_psi_deg()+psi_tdz , 0.0,360.0);
						PREL.dob_spd_msc = xios.con.Vs_KIAS*1.3*ktstomsc/sqrt(atmo.get_ISA_sigma(PREL.dob_ele_mtr));
					}
	
					PREL.type_start	= loc_specify_lle;
				}

				xios_send_PREL(acf_edit,PREL);
			}
		}
	}

	sci_stop();
}
