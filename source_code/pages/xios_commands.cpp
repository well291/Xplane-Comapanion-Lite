//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void xios_class::xios_command_list(const xint command_type)
{
	const xflt y_keyboard		=butn_dy*8				+ui_sep_pix;
	const xflt y_name_display	=y_keyboard+font_2_dy	+ui_sep_pix;

	get_keyboard (	y_keyboard				,cmd_search,name_dim64);
	plot_text_box(	y_name_display-font_2_dy,
					y_name_display+font_2_dy,xfals);

	plot_string(t_font_2,graf.dev_cx_use(),y_name_display,"COMMAND: "+(string)cmd_search+(string)"|",col_text_back,1.0,just_ctr);

	static vector <string>	choices;
							choices.clear();

	xint found_cus_fail=0;

	for(auto& str: ((command_type==cmnd_default)?set_cmd:set_cus))
	{
		string s1 = str;
		string s2 = cmd_search;
		string s3 = "custom_failure/";	// secret hand-shake for custom commands that are failures

		transform(s1.begin(), s1.end(), s1.begin(), ::toupper);
		transform(s2.begin(), s2.end(), s2.begin(), ::toupper);
		transform(s3.begin(), s3.end(), s3.begin(), ::toupper);

 		std::size_t found12=s1.find(s2);	// the string we are typing in to search
 		std::size_t found13=s1.find(s3);	// the string we are typing in to search PLUS THE SECRET HANDSHAKE FOR CUSTOM COMMANDS THAT ARE FAILURES!
 
 		if(found12!=std::string::npos)
 		{
			if(command_type!=cmnd_failures && (found13==std::string::npos))choices.push_back(str);	// not failures? then dont take secret failure strings!
			if(command_type==cmnd_failures && (found13!=std::string::npos))choices.push_back(str);	// failures? then only take secret failure strings!
		}
			if(command_type==cmnd_failures && (found13!=std::string::npos))found_cus_fail=1;		// failures? then only take secret failure strings!
	}
	
	if(!found_cus_fail)
	if(command_type==cmnd_failures)
	{
		plot_string(t_font_2 , graf.dev_cx_use(),graf.dev_cy_use()+font_2_dy*13 , "MAKE ANY NUMBER OF CUSTOM COMMANDS IN X-PLANE"	,col_text_back,1.0,just_ctr);
		plot_string(t_font_2 , graf.dev_cx_use(),graf.dev_cy_use()+font_2_dy*12 , "THAT START WITH 'custom_failure/'"				,col_text_back,1.0,just_ctr);
		plot_string(t_font_2 , graf.dev_cx_use(),graf.dev_cy_use()+font_2_dy*11 , "THEN I WILL PUT THEM IN THIS LIST"				,col_text_back,1.0,just_ctr);
		plot_string(t_font_2 , graf.dev_cx_use(),graf.dev_cy_use()+font_2_dy*10 , "FOR X-PLANE TO RECEIVE THEM AS CUSTOM FAILURES."	,col_text_back,1.0,just_ctr);
	}

	const xflt x1 = bx1()									;
	const xflt x2 = bx2()									;
	const xflt y1 = y_name_display+font_2_dy	+ui_sep_pix	;
	const xflt y2 = graf.dev_dy_use()			-ui_sep_pix	;
		  xflt y  = y2+butn_dy*0.5+butn_scroll_reg(choices.size()*butn_dy , y1,y2);

	sci_start(0,y1,graf.dev_dx_use(),y2-y1);

	for(xint c=0;c<choices.size();c++)
	if(fltrange(y-=butn_dy,y1-butn_dy,y2+butn_dy))
	if(butn_action(x1,y,x2-x1,choices[c]))
		inet.inet_send("CMND",(xchr*)choices[c].c_str(),(xint)choices[c].size());

	sci_stop();
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void xios_class::xios_commands()
{
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SUB-MODES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	static xint menu_mode = cmnd_default;
	xchr names[cmnd_dim][name_dim80] = {"DEFAULT","CUSTOM","CUSTOM FAILURES"};
	butn_menus(&menu_mode,cmnd_dim,names,1);

	xios_command_list(menu_mode);
}
