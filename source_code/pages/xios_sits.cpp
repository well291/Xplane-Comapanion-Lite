#include <dirent.h>

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// REMOVE A FILE FROM THE DRIVE!
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline void nuke_a_path(const string s){remove(s.c_str());}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SIT SIZE IN RAM... BE SURE TO UPDATE THE VERSION NUNBER IF I EVER CHANGE THIS... AND LOSE ALL THE SITS!
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
const	xint file_version		= 1;
		xint file_num_acf		= 0;
		xint file_size_saved	= 0;

xint xios_class::xios_sits_size(const xint num_acf)
{
	return	sizeof(file_version		)				+
			sizeof(file_num_acf		)				+
			sizeof(file_size_saved	)				+
			sizeof(tim				)				+
			sizeof(wxr				)				+
			sizeof(vec_fal			)				+
			acft_PX_struct::len_to_disk() * num_acf	;
}

void xios_class::xios_sits_save(const string path)
{
	ofstream ouf;

		ouf.open(path,ios::binary|ios::out);
	if(	ouf.good())
	{
		file_num_acf	= 					acft_p_size()	;
		file_size_saved	= xios_sits_size(	acft_p_size())	;

											ouf.write((xchr*)&file_version		,sizeof(file_version	));
											ouf.write((xchr*)&file_num_acf		,sizeof(file_num_acf	));
											ouf.write((xchr*)&file_size_saved	,sizeof(file_size_saved	));
											ouf.write((xchr*)&tim				,sizeof(tim				));
											ouf.write((xchr*)&wxr				,sizeof(wxr				));
											ouf.write((xchr*)&vec_fal			,sizeof(vec_fal			));
		for(xint p=0;p<file_num_acf;p++)	ouf.write((xchr*)&acft_p_var[p]		,acft_p(p)->len_to_disk());
	}
	ouf.close();
}

xint xios_class::xios_sits_load(const string path,const xint just_check_file)
{
	xmap_class m(path);

	if(m.exists	())
	if(m.in_file())
	{
		const xint scan_file_version	= m.BIN_int_scan();
		const xint scan_file_num_acf	= m.BIN_int_scan();
		const xint scan_file_size_saved	= m.BIN_int_scan();

		if(scan_file_version	== file_version)
		if(scan_file_num_acf	>=  1)
		if(scan_file_num_acf	<= 20)
		if(scan_file_size_saved	== xios_sits_size(scan_file_num_acf))
		if(m.size()				== xios_sits_size(scan_file_num_acf))
		{
			if(just_check_file)
				return xtrue;

			con.num_acf_this_pc	=scan_file_num_acf ;
			acft_p_var.resize	(scan_file_num_acf);			inet.inet_send("NACF",(xchr*)&con.num_acf_this_pc	,sizeof(con.num_acf_this_pc	));
			m.BIN_buf_scan((xchr*)&tim		,sizeof(tim		));	inet.inet_send("XTIM",(xchr*)&tim					,sizeof(tim					));
			m.BIN_buf_scan((xchr*)&wxr		,sizeof(wxr		));	inet.inet_send("XWXR",(xchr*)&wxr					,sizeof(wxr					));
			m.BIN_buf_scan((xchr*)&vec_fal	,sizeof(vec_fal));

			inet.inet_send("RESE","",0);
			for(xint f=0;f<vec_fal.size();f++)
			if(vec_fal[f].gone_present_failed)
			{
				const string s = int_to_str(f);
				inet.inet_send("FAIL",(xchr*)s.c_str(),(xint)s.size());
			}

			for(xint p=0;p<con.num_acf_this_pc;p++)
			{
				m.BIN_buf_scan((xchr*)&acft_p_var[p],acft_p(p)->len_to_disk());

				PREL_struct PREL;
							PREL.p_idx			= (is_pro_use()) ? p : 0;	// for lab use, it is always 0 on that machine!
							PREL.type_start		= loc_specify_lle;
							PREL.dob_lon_deg	= acft_p(p)->lon_deg;
							PREL.dob_lat_deg	= acft_p(p)->lat_deg;
							PREL.dob_ele_mtr	= acft_p(p)->ele_mtr;
							PREL.dob_psi_tru	= atmo.get_psi_true(acft_p(p)->psi_deg_mag,acft_p(p)->lon_deg,acft_p(p)->lat_deg);
							PREL.dob_spd_msc	= acft_p(p)->AV_KEAS*ktstomsc/sqrt(atmo.get_ISA_sigma(acft_p(p)->ele_mtr));

																		xios_send_COMB(p);										// this loads the acf
																		xios_send_xWGT(p);										// set the weight
				for(xint s=0;s<acft_p(acf_edit)->wpn_num_stations;s++)	xios_send_LWPN(p,s,acft_p(acf_edit)->wpn_chr_now[s]);	// set the weapons
																		xios_send_PREL(p,PREL);									// set the location
			}
		}
	}

	return xfals;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// GET ITEMS IN A DIRECTORY
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline void opensave_getitems(const string dir_path,vector <string>& file_names,const string ext)
{
	file_names.clear();

	DIR* dir=opendir(dir_path.c_str());
	if (!dir)return;

	struct dirent* ent;
	while((ent=readdir(dir)))
	{
		#if DEV
			if(!ent			)printf("WHOAH! No ent! NOT COOL!\n"		);
			if(!ent->d_name	)printf("WHOAH! No ent->d_name! NOT COOL!\n");
		#endif

		string path_rel=dir_path+ent->d_name;

		struct stat ss;
		if(stat(path_rel.c_str(),&ss)>=0)
		{
			if(S_ISDIR(ss.st_mode))
			{
				// folder
			}
			else
			{
				const string s=ent->d_name;
				if(strlen(ext.c_str())==0 || s.find(ext)!=std::string::npos)file_names.push_back(s);
			}
		}
	}

	closedir(dir);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PROC
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void xios_class::xios_sits()
{
	enum
	{
		sit_ai	=0,
		sit_save=1,
		sit_load=2,
		sit_dim
	};

	static xint sit_mode=sit_ai;
	xchr names[sit_dim][name_dim80]={"AI INSTRUCTOR","SAVE","LOAD"};
	butn_menus(&sit_mode,sit_dim,names,1);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// AI
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(sit_mode==sit_ai)
	{
		xflt x=graf.dev_cx_use()			;
		xflt y=graf.dev_dy_use()-ui_sep_pix	;

		plot_text_box(y-font_2_dy*4.0,y,xfals);

		plot_string(t_font_2,x,y-=font_2_dy,"Here, let the AI instructor challenge you."		,col_text_back,1.0,just_ctr);
		plot_string(t_font_2,x,y-=font_2_dy,"Set to 0% to have the instructor leave you alone."	,col_text_back,1.0,just_ctr);
		plot_string(t_font_2,x,y-=font_2_dy,"Set to 100% to have the instructor be devilish."	,col_text_back,1.0,just_ctr);

		butn_slide(bx1(),graf.dev_cy_use()+butn_dy*3.0,bdx(),"Aircraft Weight & Balance Challenge"	,&xios.ai_rat_load_0_100,"%",1 , 0,100);
		butn_slide(bx1(),graf.dev_cy_use()+butn_dy*2.0,bdx(),"Aircraft Failure Rate"				,&xios.ai_rat_fail_0_100,"%",1 , 0,100);
		butn_slide(bx1(),graf.dev_cy_use()+butn_dy*1.0,bdx(),"Weather Challenge"					,&xios.ai_rat_wexr_0_100,"%",1 , 0,100);
		butn_slide(bx1(),graf.dev_cy_use()-butn_dy*1.0,bdx(),"AI Unpredictability"					,&xios.ai_rat_rand_0_100,"%",1 , 0,100);
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SAVE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(sit_mode==sit_save)
	{
		const xflt y_keyboard = butn_dy*7 + ui_sep_pix;

		static xchr save_name[name_dim64];

		if(strlen(save_name)>0)
		if(butn_action(bx1(),y_keyboard+ui_sep_pix*2+font_2_dy*2.0+butn_hgt*0.5,bdx(),"SAVE AS: "+(string)save_name+(string)"|"))
		{
			xios_sits_save(file_path_sits+save_name);
			memset(save_name,0,sizeof(save_name));
		}

		get_keyboard(y_keyboard,save_name,name_dim64);
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// LOAD
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(sit_mode==sit_load)
	{
		vector <string>					 file_names		;
		opensave_getitems(file_path_sits,file_names,"")	;

		for(xint s=(xint)file_names.size()-1;s>=0;s--)	// delete old file-format versions, working down from the end of the list to not skip files when we shorten the list while traversing it!
		if(!xios_sits_load(file_path_sits+file_names[s],xtrue))
			nuke_a_path(file_path_sits+file_names[s]);

		opensave_getitems(file_path_sits,file_names,"");	// updated list after old formats deleted

		const	xflt x1=bx1()				;
		const	xflt x2=bx2()-butn_dy-bx1()	;
		const	xflt x3=bx2()				;

		const	xflt y1=butn_dy*2.0			+ui_sep_pix;
		const	xflt y2=graf.dev_dy_use()	-ui_sep_pix;
				xflt y =y2-butn_dy*0.5+butn_scroll_reg(file_names.size()*butn_dy , y1,y2);

		sci_start(0,y1,graf.dev_dx_use(),y2-y1);

		for(xint s=0;s<file_names.size();s++)
		{
			if(butn_action(x2,y,x3-x2-ui_sep_pix,"X"			))nuke_a_path	(file_path_sits+file_names[s]);
			if(butn_action(x1,y,x2-x1-ui_sep_pix,file_names[s]	))xios_sits_load(file_path_sits+file_names[s],xfals);

			y-=butn_dy;
		}

		sci_stop();
	}
}
