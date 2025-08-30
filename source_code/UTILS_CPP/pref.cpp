
pref_class pref;

//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
// UTILS
//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
class scan_class
{
public:
   ~scan_class(									){pref_out.close();}
	scan_class(const string& path,xint in_opr	)
	{
		const xint scan_version=4;				// if i change this, the old files will be dis-regarded
		opr_typ=in_opr;							// memorize to use on the destructor when we close

		if(opr_typ==IO_write)
		{
			pref_out.open(path.c_str());
			pref_out<<setiosflags(ios::fixed);	// don't go to scientific notation
			pref_out<<scan_version<<" version\n";
		}
		if(opr_typ==IO_read)					// PREFS READ: READ THE ENTIRE FILE ALL AT ONCE INTO A BUNCH OF TAGS AND THEIR VALUES
		{
			prefs_map.clear();
			xmap_class m(path);

			if		(!m.exists		()				){if(DEV)APP_alert("Could not read prefs","File at path does not exist"						,path,"");	return;}
			if		( m.TXT_int_scan()!=scan_version){if(DEV)APP_alert("Could not read prefs","I found the file, but the path version is wrong"	,path,"");	return;}
			while	( m.in_file		()				)
			{
				string tag,value;
				m.TXT_str_scan_space(&tag);
				if( tag.find_first_of(" \t\r\n")!=tag.npos)APP_alert("ERROR: A prefs tag has whitespace or a newline!",tag,"","");
				if(!tag.empty()){
					m.TXT_str_scan_eoln(&value);
					prefs_map[tag]=value;
					if(prefs_map.count(tag)>1)APP_alert("ERROR: A prefs tag is being read twice!",tag,"","");}
			}
		}
	}

//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
	xint int_io(xint  * io,const string& tag			){string stl_s;	xchr cs[str_dim];	if(opr_typ==IO_write)sprintf(cs, "%d",*io);	else cs[0]=0;	if(fetch_or_get(cs,stl_s ,tag)){*io=atoi(stl_s.c_str());												return xtrue;}	return xfals;}
	xint flt_io(xflt  * io,const string& tag			){string stl_s;	xchr cs[str_dim];	if(opr_typ==IO_write)sprintf(cs, "%f",*io);	else cs[0]=0;	if(fetch_or_get(cs,stl_s ,tag)){*io=atof(stl_s.c_str());												return xtrue;}	return xfals;}
	xint dob_io(xdob  * io,const string& tag			){string stl_s;	xchr cs[str_dim];	if(opr_typ==IO_write)sprintf(cs, "%f",*io);	else cs[0]=0;	if(fetch_or_get(cs,stl_s ,tag)){*io=atof(stl_s.c_str());												return xtrue;}	return xfals;}
	xint stl_io(string* io,const string& tag			){if(opr_typ==IO_write && io->size()==0)return xfals;											if(fetch_or_get(*io,*io  ,tag)){																		return xtrue;}	return xfals;}
	xint str_io(xchr  * io,const string& tag,xint dim	){string stl_s;																					if(fetch_or_get( io,stl_s,tag)){if(stl_s.size()>=dim-1)stl_s.resize(dim-1); strcpy(io,stl_s.c_str());	return xtrue;}	return xfals;}
	xint tim_io(time_t* io,const string& tag			){string stl_s;	xchr cs[str_dim];	if(opr_typ==IO_write)sprintf(cs,"%ld",*io); else cs[0]=0;	if(fetch_or_get(cs,stl_s ,tag)){*io=atoi(stl_s.c_str());												return xtrue;}	return xfals;}

//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
private:
	ofstream pref_out;

	xint fetch_or_get(const string& input,string& output,const string& tag)
	{
		if(opr_typ==IO_write){
			pref_out<<tag<<" "<<input<<"\n";
			return xfals;}						// return false because we do not apply anything to the value when we are WRITING!

		if(opr_typ==IO_read){
			string cmd_key=string("--pref:")+tag;
				prefs_map_type::iterator i=prefs_map.find(tag);
				if(i==prefs_map.end())return xfals;
				output=i->second;}

		return xtrue;
	}

	typedef	map<string,string> prefs_map_type;	// prefs map is a map from string keys to string values.
	xint			opr_typ		;				// operation (read or write)... remember it here so we know to close the output file
	prefs_map_type	prefs_map	;				// prefs map
};

//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
// TEXT PREFS
//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
void pref_class::prefs_txt(const IO_type opr_typ)
{
	scan_class p(file_path_prfs+"prefs_2.txt",opr_typ);

	p.int_io(&graf.dev_orient				,"dev_orient"			);
	p.int_io((xint*)&colr.ui_sel			,"ui_sel"				);
	p.flt_io(&apps.brite_0_100				,"brite_0_100"			);
	p.int_io(&apps.review_done				,"review_done"			);
	p.flt_io(&soun.volume_0_100				,"volume_0_100"			);
	p.str_io( xios.cmd_search				,"cmd_search", str_dim	);

	p.flt_io(&xios.tim.date_0_364			,"date_0_364"			);
	p.flt_io(&xios.tim.time_24hr			,"time_24hr"			);

	p.flt_io(&xios.wxr.regn_variation_pct	,"regn_variation_pct"	);
	p.int_io(&xios.wxr.regn_change_en		,"regn_change_en"		);

	p.flt_io(&xios.wxr.vis_sm				,"vis_sm"				);
	p.flt_io(&xios.wxr.ISA_offset_C			,"ISA_offset_C"			);
	p.flt_io(&xios.wxr.SL_press_baro		,"SL_press_baro"		);
	p.flt_io(&xios.wxr.precip_rat			,"precip_rat"			);

	for(xint c=0;c<cld_dim;c++)
	{
		p.int_io(&xios.wxr.cld_typ_enum		[c],"cld_typ_enum"		+int_to_str(c,3,str_lead_with_zeros));
		p.int_io(&xios.wxr.cld_cov_enum		[c],"cld_cov_enum"		+int_to_str(c,3,str_lead_with_zeros));
		p.flt_io(&xios.wxr.base_msl_ft		[c],"base_msl_ft"		+int_to_str(c,3,str_lead_with_zeros));
		p.flt_io(&xios.wxr.tops_msl_ft		[c],"tops_msl_ft"		+int_to_str(c,3,str_lead_with_zeros));
	}

	for(xint c=0;c<wnd_dim;c++)
	{
		p.flt_io(&xios.wxr.wind_alt_msl_ft	[c],"wind_alt_msl_ft"	+int_to_str(c,3,str_lead_with_zeros));
		p.flt_io(&xios.wxr.wind_spd_kt		[c],"wind_spd_kt"		+int_to_str(c,3,str_lead_with_zeros));
		p.flt_io(&xios.wxr.wind_dir_true	[c],"wind_dir_true"		+int_to_str(c,3,str_lead_with_zeros));
		p.flt_io(&xios.wxr.wind_inc_kt		[c],"wind_inc_kt"		+int_to_str(c,3,str_lead_with_zeros));
		p.flt_io(&xios.wxr.wind_shr_deg		[c],"wind_shr_deg"		+int_to_str(c,3,str_lead_with_zeros));
		p.flt_io(&xios.wxr.CAT_rat			[c],"CAT_rat"			+int_to_str(c,3,str_lead_with_zeros));
	}

	p.flt_io(&xios.wxr.therm_vvi_fpm	,"therm_vvi_fpm"	);
	p.int_io(&xios.wxr.ter_eff_net		,"ter_eff_net"		);
	p.flt_io(&xios.wxr.wave_hgt_ft		,"wave_hgt_ft"		);
	p.flt_io(&xios.wxr.wave_dir_deg		,"wave_dir_deg"		);

	p.int_io(&becn.auto_connect			,"auto_connect"		);
	p.int_io(&xios.pres_hectopas		,"pres_hectopas"	);
	p.int_io(&xios.vis_meters			,"vis_meters"		);
	p.int_io(&xios.weight_kg			,"weight_kg"		);
	p.flt_io(&xios.map_set_KIAS			,"map_set_KIAS"		);
	p.flt_io(&xios.map_set_alt			,"map_set_alt"		);

	p.flt_io(&xios.ai_rat_load_0_100	,"ai_rat_load_0_100");
	p.flt_io(&xios.ai_rat_fail_0_100	,"ai_rat_fail_0_100");
	p.flt_io(&xios.ai_rat_wexr_0_100	,"ai_rat_wexr_0_100");
	p.flt_io(&xios.ai_rat_rand_0_100	,"ai_rat_rand_0_100");

	xint fs = (xint)xios.vec_fal.size();
	p.int_io(&fs,"vec_fal_size");
	xios.vec_fal.resize(fs);

	for(xint c=0;c<	xios.vec_fal.size();c++)							// failures here not bin cause bin is saved when the tcp ip is complete...
	if(				xios.vec_fal[c].fail_name[0] || opr_typ==IO_read)	// failures come in by upd over time to allow for custom failures per-airplane, so they are not all here when we save the bin file!
	{
				  xios.vec_fal[c].f=c;
		p.int_io(&xios.vec_fal[c].g1					,"fail_g1"	+int_to_str(c,3,str_lead_with_zeros));
		p.int_io(&xios.vec_fal[c].g2					,"fail_g2"	+int_to_str(c,3,str_lead_with_zeros));
		p.int_io(&xios.vec_fal[c].fail_ui				,"fail_ui"	+int_to_str(c,3,str_lead_with_zeros));
		p.int_io(&xios.vec_fal[c].fail_rand				,"fail_rand"+int_to_str(c,3,str_lead_with_zeros));
		p.int_io(&xios.vec_fal[c].gone_present_failed	,"fail_is"	+int_to_str(c,3,str_lead_with_zeros));		// store this since this indicates if we HAVE the thing!
		p.str_io( xios.vec_fal[c].fail_name				,"fail_name"+int_to_str(c,3,str_lead_with_zeros),name_dim80);
	}

	if(opr_typ==IO_write)
	{
		std::set<std::string>::iterator it=xios.fail_names_we_show.begin();

		xint c=0;
		while(it!=xios.fail_names_we_show.end())
		{
			string s=(*it);
			p.stl_io(&s,"fail_names_we_show"+int_to_str(c,3,str_lead_with_zeros));
		    it++;
		    c++;
		}
	}

	if(opr_typ==IO_read)
	{
	 	xios.fail_names_we_show.erase(
	 	xios.fail_names_we_show.begin(),
	 	xios.fail_names_we_show.end  ());

		for(xint c=0;c<xios.vec_fal.size();c++)
		{
			string s;
			if(p.stl_io(&s,"fail_names_we_show"+int_to_str(c,3,str_lead_with_zeros)))
				xios.fail_names_we_show.insert(s);
		}
	}

	for(xint c1=0;c1<group_dim;c1++)
		p.str_io(xios.group1_name[c1],"group1"+int_to_str(c1,3,str_lead_with_zeros),name_dim80);

	for(xint c1=0;c1<group_dim;c1++)
	for(xint c2=0;c2<group_dim;c2++)
		p.str_io(xios.group2_name[c1][c2],"group2"+int_to_str(c1,3,str_lead_with_zeros)+int_to_str(c2,3,str_lead_with_zeros),name_dim80);


	if(opr_typ==IO_read)
		my_set_brightness_from_cpp(apps.brite_0_100*0.01);

	colr.init_color();	// after prefs so we set the right color for the UI type
}

//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
// BIN DAT
//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
/*
WE SAVE THE BINARY DATA IN PREFS HERE IN CASE THE TCP IP DOES NOT WORK!
I HAVE SEEN IT FAIL A NUMBER OF TIMES, AND SO HAVE CUSTOMERS!
AND NOBODY EVER KNOWS WHY!
SO LET'S STORE THE DATA HERE IN PREFS SO WE HAVE IT IN CASE THERE IS A FAILURE!
*/
//ееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
xint prefs_bin_path(const IO_type opr_typ,const string path)
{
	long vec_fix_size = vec_fix.size();
	long vec_nav_size = vec_nav.size();
	long vec_awy_size = vec_awy.size();
	long vec_hol_size = vec_hol.size();
	long vec_apt_size = vec_apt.size();

	if(opr_typ==IO_write)
	{
		ofstream ouf;

			ouf.open(path,ios::binary|ios::out);
		if(	ouf.good())
		{
			ouf.write((xchr*)&vec_fix_size,sizeof(vec_fix_size));
			ouf.write((xchr*)&vec_nav_size,sizeof(vec_nav_size));
			ouf.write((xchr*)&vec_awy_size,sizeof(vec_awy_size));
			ouf.write((xchr*)&vec_hol_size,sizeof(vec_hol_size));
			ouf.write((xchr*)&vec_apt_size,sizeof(vec_apt_size));

			for(xint f=0;f<vec_fix_size	;f++)	ouf.write((xchr*)&vec_fix		[f]				,sizeof(vec_fix			[f]				));
			for(xint n=0;n<vec_nav_size	;n++)	ouf.write((xchr*)&vec_nav		[n]				,sizeof(vec_nav			[n]				));
			for(xint n=0;n<vec_awy_size	;n++)	ouf.write((xchr*)&vec_awy		[n]				,sizeof(vec_awy			[n]				));
			for(xint n=0;n<vec_hol_size	;n++)	ouf.write((xchr*)&vec_hol		[n]				,sizeof(vec_hol			[n]				));
			for(xint a=0;a<vec_apt_size	;a++){	ouf.write((xchr*)&vec_apt		[a].nam			,sizeof(vec_apt			[a].nam			));	long vec_rwy_size=vec_apt[a].vec_rwy.size();
												ouf.write((xchr*)&vec_rwy_size					,sizeof(vec_rwy_size					));
			for(xint r=0;r<vec_rwy_size	;r++)	ouf.write((xchr*)&vec_apt		[a].vec_rwy[r]	,sizeof(vec_apt			[a].vec_rwy[r]	));}

			return xtrue;
		}
	}

	if(opr_typ==IO_read)
	{
		xmap_class	m(path);
		if(			m.exists())
		{
			m.BIN_buf_scan((xchr*)&vec_fix_size,sizeof(vec_fix_size));		vec_fix.resize(vec_fix_size);
			m.BIN_buf_scan((xchr*)&vec_nav_size,sizeof(vec_nav_size));		vec_nav.resize(vec_nav_size);
			m.BIN_buf_scan((xchr*)&vec_awy_size,sizeof(vec_awy_size));		vec_awy.resize(vec_awy_size);
			m.BIN_buf_scan((xchr*)&vec_hol_size,sizeof(vec_hol_size));		vec_hol.resize(vec_hol_size);
			m.BIN_buf_scan((xchr*)&vec_apt_size,sizeof(vec_apt_size));		vec_apt.resize(vec_apt_size);

			for(xint f=0;f<vec_fix_size;f++)	m.BIN_buf_scan((xchr*)&vec_fix		[f]				,sizeof(vec_fix			[f]				));
			for(xint n=0;n<vec_nav_size;n++)	m.BIN_buf_scan((xchr*)&vec_nav		[n]				,sizeof(vec_nav			[n]				));
			for(xint n=0;n<vec_awy_size;n++)	m.BIN_buf_scan((xchr*)&vec_awy		[n]				,sizeof(vec_awy			[n]				));
			for(xint n=0;n<vec_hol_size;n++)	m.BIN_buf_scan((xchr*)&vec_hol		[n]				,sizeof(vec_hol			[n]				));
			for(xint a=0;a<vec_apt_size;a++){	m.BIN_buf_scan((xchr*)&vec_apt		[a].nam			,sizeof(vec_apt			[a].nam			));	apt_icao_to_index[vec_apt[a].nam.apt_id]=a;
			long vec_rwy_size=0;				m.BIN_buf_scan((xchr*)&vec_rwy_size					,sizeof(vec_rwy_size					));	vec_apt[a].vec_rwy.resize(vec_rwy_size);
			for(xint r=0;r<vec_rwy_size;r++)	m.BIN_buf_scan((xchr*)&vec_apt		[a].vec_rwy[r]	,sizeof(vec_apt			[a].vec_rwy[r]	));}

			return xtrue;
		}
	}

	return xfals;
}

void pref_class::prefs_bin(const IO_type opr_typ)
{
	const string XPlane_sent_TCP_IP_path = file_path_prfs+"wrl6.bin"	;	// this is sent from X-Plane by TCP-IP, but might not always make it!
	const string saved_ship_with_it_path = "RESOURCE/x-plane-data.bin"	;	// so i just ship the app with a copy of the data so we have it anyway
																			// just toss the app container to the desktop and drag the prefs file into resources witha re-name.. takes 5 seconds
	if(!prefs_bin_path(opr_typ,XPlane_sent_TCP_IP_path) && opr_typ==IO_read)
		prefs_bin_path(opr_typ,saved_ship_with_it_path);
}
