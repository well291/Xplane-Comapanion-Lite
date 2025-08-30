//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void xios_class::xios_aircraft()
{
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// CONS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	xios_acf_menus(is_pro_use());	// set the number of craft in th pro-ise case: we set the number of airplanes on that machine!
									// but for the lab case, the number of airplanes is... the number of PCs we hear from!
	const xflt xd = xdx(  2);
	const xflt x1 = xos(0,1);
	const xflt x2 = xos(1,2);

	const xflt y1 = butn_dy*2			+ ui_sep_pix*2										;
	const xflt y2 = graf.dev_dy_use()	- ui_sep_pix										;
	const xflt yt = y2 - butn_dy*0.5	+ butn_scroll_reg(acf_paths.size()*butn_dy , y1,y2)	;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ACF LIST
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	xint send=0;

	sci_start(0,y1,graf.dev_dx_use(),y2-y1);

	xflt y = yt;

	for(xint a=0;a<acf_paths.size();a++)
	{
		string	disp_name=acf_paths[a];
				disp_name.erase(0										,strlen("Aircraft/"	));
				disp_name.erase(strlen(disp_name.c_str())-strlen(".acf"),strlen(".acf"		));

		if(butn_on_off((acf_paths[a] == acft_p(acf_edit)->acfpath) , x1,y,xd,disp_name))
		{
			send=1;
			C_eq_C(acft_p(acf_edit)->acfpath,(xchr*)acf_paths[a].c_str(),net_strDIM);
		}

		y-=butn_dy;
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// OPTIONS LIST
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	y = yt+butn_dy;

	if(!RUNNING_1208)	// 1208 does not have a message for this
	if(acft_p(acf_edit)->wpn_num_stations && acft_p_size()>1)	// x-plane will only allow team assignment if there is more than 1 player there!
	{
		if(butn_on_off(acft_p(acf_edit)->team_now	== team_none		,x2,	y-=butn_dy,xd,"no combat"	)){send=1;	acft_p(acf_edit)->team_now		= team_none						;}
		if(butn_on_off(acft_p(acf_edit)->team_now	== team_red			,x2,	y-=butn_dy,xd,"team red"	)){send=1;	acft_p(acf_edit)->team_now		= team_red						;}
		if(butn_on_off(acft_p(acf_edit)->team_now	== team_blue		,x2,	y-=butn_dy,xd,"team blue"	)){send=1;	acft_p(acf_edit)->team_now		= team_blue						;}
		if(butn_on_off(acft_p(acf_edit)->team_now	== team_green		,x2,	y-=butn_dy,xd,"team green"	)){send=1;	acft_p(acf_edit)->team_now		= team_green					;}
		if(butn_on_off(acft_p(acf_edit)->team_now	== team_gold		,x2,	y-=butn_dy,xd,"team gold"	)){send=1;	acft_p(acf_edit)->team_now		= team_gold						;}
																				y-=butn_dy;
		if(butn_on_off(acft_p(acf_edit)->skill_now	== skill_very_easy	,x2,	y-=butn_dy,xd,"very easy"	)){send=1;	acft_p(acf_edit)->skill_now		= skill_very_easy				;}
		if(butn_on_off(acft_p(acf_edit)->skill_now	== skill_easy		,x2,	y-=butn_dy,xd,"easy"		)){send=1;	acft_p(acf_edit)->skill_now		= skill_easy					;}
		if(butn_on_off(acft_p(acf_edit)->skill_now	== skill_medium		,x2,	y-=butn_dy,xd,"medium"		)){send=1;	acft_p(acf_edit)->skill_now		= skill_medium					;}
		if(butn_on_off(acft_p(acf_edit)->skill_now	== skill_hard		,x2,	y-=butn_dy,xd,"hard"		)){send=1;	acft_p(acf_edit)->skill_now		= skill_hard					;}
		if(butn_on_off(acft_p(acf_edit)->skill_now	== skill_very_hard	,x2,	y-=butn_dy,xd,"very hard"	)){send=1;	acft_p(acf_edit)->skill_now		= skill_very_hard				;}
																				y-=butn_dy;
		if(butn_on_off(acft_p(acf_edit)->ai_flies_now					,x2,	y-=butn_dy,xd,"AI flies"	)){send=1;	acft_p(acf_edit)->ai_flies_now	=!acft_p(acf_edit)->ai_flies_now;}
																				y-=butn_dy;
	}

	for(xint i=0;i<acft_p(acf_edit)->livery_dim;i++)
		if(butn_on_off((acft_p(acf_edit)->livery_now==i), x2,y-=butn_dy,xd,"livery #"+int_to_str(i+1))){send=1;	acft_p(acf_edit)->livery_now=i;}
	
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SEND!
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(send)xios_send_COMB(acf_edit);

	sci_stop();
}
