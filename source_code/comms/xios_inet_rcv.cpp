//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// AIRWAY ENDPOINT FINDING
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
map			<string,xint> fix_to_index;
multimap	<string,xint> nav_to_index;	// lots of navaids with the same name. lots. like AST and many others. multimap handles that.

inline xint closest_nav_to_lat_lon(const string name,const xflt lat,const xflt lon)
{
	xint ret_min=0		;
	xflt dis_min=360.0	;

	multimap<string,int>::iterator it;
	for(it=nav_to_index.equal_range(name).first;it!=nav_to_index.equal_range(name).second;++it)
	{
		const xflt dis_nav = pythag(		lat-vec_nav[(*it).second].nav_lat,
									fltwrap(lon-vec_nav[(*it).second].nav_lon,-180.0,180.0));
		if(dis_nav<dis_min)
		{
			dis_min = dis_nav;
			ret_min = (*it).second;
		}
	}

	return ret_min;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// NUM DIR CHARS TO SORT BY DIRECTORY FIRST, ALPHABET SECOND, SO THE PLANES IN THE OUTER LEVEL ARE VIS AT THE TOP OF THE LIST!
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline xint num_dir_chars(const string s)
{
	xint num_dirs=0;
	for(xint c=0;c<s.size();c++)
	if(is_a_dir_char(s[c]))
		num_dirs++;

	return num_dirs;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// I NORMALLY WRITE UDP, BUT THIS IS STILL CALLED WITH BIG TCP-IP CHUNKS AS WELL!
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void xios_class::xios_inet_rcv(const IPAddress from_ip,xchr* label_read,xchr* data_read,const xint len)
{
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// FIXES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(strncmp(label_read,"xFIX",4)==0)
	{
		if(vec_fix.size()==0)shar.shar_per_init("vec_fix");

		if(len==sizeof(fix_struct))
		{
			fix_struct fix;
			memcpy(	  &fix,data_read,len);

			fix_to_index[fix.fix_name] = (xint)vec_fix.size();
			vec_fix.push_back(fix);
		}
		else APP_alert("Bad xFIX struct size.","Wanted "+int_to_str(sizeof(fix_struct)),"Got "+int_to_str(len),"");
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// NAVAIDS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	struct nav_struct_net
	{
		xchr nav_id[id_dim8];
		xint nav_type,nav_freq;
		xflt nav_lon,nav_lat,nav_ele_mtr;
		xflt psi_or_gls_deg_true;
	};

	if(strncmp(label_read,"xNAV",4)==0)
	{
		if(vec_nav.size()==0)shar.shar_per_init("vec_nav");

		if(len==sizeof(nav_struct_net))
		{
			nav_struct nav;
			memcpy(	  &nav,data_read,len);		// copy in the nav struct that comes across the net... this fills the FIRST part of the struct
					   nav.nav_is_failed=0;		// then fill in the rest that is used inside this app right here!

			nav_to_index.insert(pair <string, int> (nav.nav_id, (xint)vec_nav.size()));
			vec_nav.push_back(nav);
		}
		else APP_alert("Bad xNAV struct size.","Wanted "+int_to_str(sizeof(nav_struct_net)),"Got "+int_to_str(len),"");
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// AIRWAYS AFTER FIXES AND NAVAIDS SO WE CAN LOOK THOSE UP!
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// https://developer.x-plane.com/wp-content/uploads/2019/01/XP-AWY1101-Spec.pdf
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(strncmp(label_read,"xAWY",4)==0)	// airway
	{
		const string string_line=data_read;

		if(string_line.size()>36)	// room to look at string
		{
			string end1 = string_line.substr( 0,5	);	for(xint i=0;i<5;i++)if(end1[0]==' ')end1.erase(0,1);	// pull out leading spaces for navaids
			string typ1 = string_line.substr( 9,2	);	const xint isfix1=(typ1=="11");							// 11 for fix, something else for navaid
			string end2 = string_line.substr(12,5	);	for(xint i=0;i<5;i++)if(end2[0]==' ')end2.erase(0,1);	// pull out leading spaces for navaids
			string typ2 = string_line.substr(21,2	);	const xint isfix2=(typ2=="11");							// 11 for fix, something else for navaid
			string name = string_line.substr(36		);

			xint end1_ind = (isfix1)?fix_to_index[end1]:0;	// use the FIX as the endpoint if that is what it is... otherwise find the closest NAVAID with the right name to the fix. there are lots of NAVAIDs with the same name.
			xint end2_ind = (isfix2)?fix_to_index[end2]:0;	// use the FIX as the endpoint if that is what it is... otherwise find the closest NAVAID with the right name to the fix. there are lots of NAVAIDs with the same name.

			if(!isfix1 &&  isfix2)end1_ind = closest_nav_to_lat_lon(end1,vec_fix[end2_ind].fix_lat,vec_fix[end2_ind].fix_lon);
			if( isfix1 && !isfix2)end2_ind = closest_nav_to_lat_lon(end2,vec_fix[end1_ind].fix_lat,vec_fix[end1_ind].fix_lon);

			if(end1_ind && end2_ind)
			{
				if(vec_awy.size()==0)shar.shar_per_init("vec_awy");

				awy_struct			awy;
				C_eq_C(				awy.awy_name,(xchr*)name.c_str(),net_strDIM);
									awy.lat1 = (isfix1)?vec_fix[end1_ind].fix_lat:vec_nav[end1_ind].nav_lat;
									awy.lon1 = (isfix1)?vec_fix[end1_ind].fix_lon:vec_nav[end1_ind].nav_lon;
									awy.lat2 = (isfix2)?vec_fix[end2_ind].fix_lat:vec_nav[end2_ind].nav_lat;
									awy.lon2 = (isfix2)?vec_fix[end2_ind].fix_lon:vec_nav[end2_ind].nav_lon;

				if(awy.lon1>90.0 && awy.lon2<-90.0)awy.lon2 += 360.0;
				if(awy.lon2>90.0 && awy.lon1<-90.0)awy.lon1 += 360.0;

				vec_awy.push_back(awy);
			}
		}
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// AIRPORTS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(strncmp(label_read,"xAPT",4)==0)
	{
		if(vec_apt.size()==0)shar.shar_per_init("vec_apt");

		const xint num_rwy		=(len-sizeof(nam_struct))			/ sizeof(rwy_struct);
		const xint bytes_req	=	  sizeof(nam_struct)+num_rwy	* sizeof(rwy_struct);
		const xint bytes_act	=len;

		if(num_rwy>0)
		{
			if(bytes_req==bytes_act)
			{
				nam_struct nam;
				memcpy(	  &nam,	data_read, sizeof(nam));
								data_read+=sizeof(nam) ;

				apt_icao_to_index[nam.apt_id]=(xint)vec_apt.size();
													vec_apt.push_back(apt_struct());

				const xint	 index=apt_icao_to_index[nam.apt_id];	// ok this is now the index for this ID
				vec_apt		[index].nam=nam;						// update name and id
				vec_apt		[index].vec_rwy.clear();				// clear runways, since we may be updating an existing airport

				for(xint r=0;r<num_rwy;r++)
				{
					rwy_struct rwy;
					memcpy(	  &rwy,	data_read, sizeof(rwy));
									data_read+=sizeof(rwy) ;

					vec_apt[index].vec_rwy.push_back(rwy);
				}
			}
			else APP_alert("Bad xAPT struct size.","Wanted "+int_to_str(bytes_req),"Got "+int_to_str(bytes_act),"");
		}
		else APP_alert("xAPT with num_runways==0","","","");
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// RAMP STARTS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(strncmp(label_read,"xRAM",4)==0)
	{
		static xint done=0;
		if(!done){
			done=1;
			shar.shar_per_init("xRAM");}

		xint apt_index	=0;		memcpy(&apt_index	,data_read,sizeof(apt_index	));		data_read += sizeof(apt_index	);
		xint num_ram	=0;		memcpy(&num_ram		,data_read,sizeof(num_ram	));		data_read += sizeof(num_ram		);

		for(xint r=0;r<num_ram;r++)
		{
			ram_struct ram;
			memcpy(	  &ram,	data_read, sizeof(ram));
							data_read+=sizeof(ram) ;

			vec_apt[apt_index].vec_ram.push_back(ram);
		}
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// HOLDING PATTERNS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(strncmp(label_read,"HOLD",4)==0)
	{
		if(vec_hol.size()==0)shar.shar_per_init("vec_hol");

		hol_struct hol;

		buff_class			buff;
							buff.load_buff((xchr*)data_read,len);
		hol.lat			=	buff.ext_xdob();
		hol.lon			=	buff.ext_xdob();
		hol.inbnd_psi	=	buff.ext_xflt();	// psi
		hol.turn_dir	=	buff.ext_xchr();	// turn dir
		hol.time_min	=	buff.ext_xflt();	// size: time in minutes
		hol.len_nm		=	buff.ext_xflt();	// size: length in nm

		if(buff.perfect_scan())
			vec_hol.push_back(hol);
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// COMMANDS, WE WILL GET ALL THE DEFAULT SIM COMMANDS ONCE, THEN THEN THE CUSTOM COMMANDS OVER AND OVER AS CUSTOM PLANES SEND THEM IN FOR CUSTOM FAILURES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(	strncmp(label_read,"xCMD",4)==0 ||
		strncmp(label_read,"xCUS",4)==0 )
	{
		static xint done=0;
		if(!done){
			done=1;
			shar.shar_per_init("xCMD");}

		xchr    name[1024]={0};
		memcpy(&name,data_read,len);

		if(strncmp(label_read,"xCMD",4)==0)set_cmd.insert(name);	// sets won't let us add dups, so ok to send custom commands over and over
		if(strncmp(label_read,"xCUS",4)==0)set_cus.insert(name);	// sets won't let us add dups, so ok to send custom commands over and over
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// AVAILABLE FAILURES, WHICH KEEPS ON GOING TO GET CUSTOM FAILURES FOR EACH PLANE WE LOAD!
// THIS IS A NEEDED PRO-FEATURE: BRINGING IN CUSTOM FAILURES! FOR EACH AIRPLANE!
// THAT MEANS WE CANNOT JUST SPLAT THEM OVER BY TCP-IP
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(acft_p_size()>0)					// only take this from p0 in those lab setups (the ONLY use in the prop setups)
	if(acft_p(0)->acft_addy == from_ip)	// only take this from p0 in those lab setups (the ONLY use in the prop setups)
	if(strncmp(label_read,"xFAL",4)==0)
	if(len==sizeof(fail_struct))
	{
		fail_struct	net_fail;
		memcpy(&	net_fail,data_read,len);

		if(net_fail.f >= (xint)vec_fal.size())
			vec_fal.resize(net_fail.f+1);

		if(net_fail.g1>=group_dim)APP_alert("Failure index from X-Plane too large!","Update Control Pad to latest version to handle all X-Plane failures!","","");
		if(net_fail.g2>=group_dim)APP_alert("Failure index from X-Plane too large!","Update Control Pad to latest version to handle all X-Plane failures!","","");

		if(intrange(net_fail.f , 0,(xint)vec_fal.size()	-1))	// if not menu data, but a real failure!
		if(intrange(net_fail.g1,-1,group_dim			-1))	// if in range
		if(intrange(net_fail.g2,-1,group_dim			-1))	// if in range
		{
			if(	(vec_fal[net_fail.f].g1				!=net_fail.g1			) ||	// if anything is different, create new failure
				(vec_fal[net_fail.f].g2				!=net_fail.g2			) ||	// if anything is different, create new failure
				(vec_fal[net_fail.f].fail_name[0]	!=net_fail.fail_name[0]	) )		// if anything is different, create new failure
			{
				fail_names_we_show.insert(net_fail.fail_name);
			}

			memcpy(&vec_fal[net_fail.f],data_read,len);
		}

		if(			net_fail.f ==-1				)	// index -1 for menu data
		if(intrange(net_fail.g1,0,group_dim-1)	)
		if(			net_fail.g2==-1				)
			C_eq_C(group1_name[net_fail.g1],net_fail.fail_name,name_dim80);

		if(			net_fail.f ==-1				)	// index -1 for menu data
		if(intrange(net_fail.g1,0,group_dim-1)	)
		if(intrange(net_fail.g2,0,group_dim-1)	)
			C_eq_C(group2_name[net_fail.g1][net_fail.g2],net_fail.fail_name,name_dim80);
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// AVAILABLE AIRCRAFT ON DISK TO CHOOSE FROM
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(strncmp(label_read,"xACF",4)==0)
	{
		if(acf_paths.size()==0)shar.shar_per_init("acf_paths");

		string					input;
		for(xint c=0;c<len;c++)	input.push_back((xchr)data_read[c]);

		xint found_it=0;
		for(xint a=0;a<	acf_paths.size();a++)
		if(				acf_paths[a]==input){
			found_it=1;
			break;}

		if(!found_it)
		{
								acf_paths.push_back(input);
			for(xint a1=0;a1<=	acf_paths.size()  ;a1++)	// sort by alphabet
			for(xint a2=0;a2<	acf_paths.size()-1;a2++)
			if(					acf_paths[a2]> acf_paths[a2+1])
				str_switch(	   &acf_paths[a2],&acf_paths[a2+1]);
		}
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// AVAILABLE WEAPONS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(strncmp(label_read,"xWPN",4)==0)
	{
		string					input;
		for(xint c=0;c<len;c++)	input.push_back((xchr)data_read[c]);

		xint found_it=0;
		for(xint w=0;w<	wpn_files.size();w++)
		if(				wpn_files[w]==input){
			found_it=1;
			break;}

		if(!found_it)
		{
								wpn_files.push_back(input);
			for(xint w1=0;w1<=	wpn_files.size()  ;w1++)	// sort by alphabet
			for(xint w2=0;w2<	wpn_files.size()-1;w2++)
			if(					wpn_files[w2]> wpn_files[w2+1])
				str_switch(	   &wpn_files[w2],&wpn_files[w2+1]);
		}
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// NEXRAD:
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(acft_p_size()>0)					// only take this from p0 in those lab setups (the ONLY use in the prop setups)
	if(acft_p(0)->acft_addy == from_ip)	// only take this from p0 in those lab setups (the ONLY use in the prop setups)
	if(strncmp(label_read,"yRAD",4)==0)
	{
		buff_class		buff;
						buff.load_buff((xchr*)data_read,len);
		const xint i =	buff.ext_xint();
						buff.ext_xdim((xchr*)&storm_08[i],radar_region_jdim);
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ACF DATA SENT FOR <<<ALL>> PLANES ON THIS MACHINE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(strncmp(label_read,"acfX",4)==0)
	{
		acft_PX_struct acft_p_scan;

		if(len == acft_p_scan.len_to_net())							// right len for the job
		for(xint b=0;b<	becn.other_becns.size();b++)				// go thru all our beacons
		if(				becn.other_becns[b].beacon_used)			// if this is a beacon we are deciding to control
		if(				becn.other_becns[b].other_addy == from_ip)	// and this plane comes from that beacon ip
		{															// then we want to control this plane, yes
			memcpy(	&acft_p_scan,data_read,len);
					 acft_p_scan.acft_name			= becn.other_becns[b].other_name;
					 acft_p_scan.acft_addy			= becn.other_becns[b].other_addy;
					 acft_p_scan.acft_time_net_recv	= xtim.real_time				;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PRO-USE: LISTENING TO ONLY 1 BEACON, TAKE ALL THE PLANES ON THAT MACHINE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
			if(is_pro_use())			// pro-use: we control ALL the planes on ONE computer
			{
				if(	acft_p_size()			<acft_p_scan.p_index_sim+1)
					acft_p_var.resize		(acft_p_scan.p_index_sim+1);						// add as needed to fit
				if(	xtim.real_time > acft_p	(acft_p_scan.p_index_sim)->acft_time_map_move+1.0)	// and not dragging the plane on the map
					acft_p_var				[acft_p_scan.p_index_sim] = acft_p_scan;			// then grab it
			}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// LAB-USE: LISTENING TO MANY BEACONS, JUST TAKE P0 ON EACH MACHINE!
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
			if(is_lab_use())				// lab-use: we control P0 on each of MANY computers!
			if(acft_p_scan.p_index_sim==0)	// only take p=0 on each machine in that case!
			{
				xint found_it = 0;
				for(xint p=0;p<acft_p_size();p++)
				if(acft_p(p)->acft_addy == from_ip)
				{
					found_it = 1;
					if(xtim.real_time > acft_p(p)->acft_time_map_move+1.0)
					{
						path_class acft_path=	acft_p_var[p].acft_path;	// FLIGHT-path for this plane, be sure to not step on it on the net rcv
												acft_p_var[p]=acft_p_scan;
												acft_p_var[p].acft_path=acft_path;
					}
				}

				if(!found_it)
				{
					acft_p_var.push_back(acft_p_scan);
				}
			}
		}
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ACF DATA SENT FOR <<<p0 ONLY>> ON THIS MACHINE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(acft_p_size()>0)										// only take this from p0 in those lab setups (the ONLY use in the pro setups)
	if(acft_p(0)->acft_addy == from_ip)						// only take this from p0 in those lab setups (the ONLY use in the pro setups)
	if(xtim.real_time>real_time_mod_wgt+1.0)
	if(strncmp(label_read,"acf0",4)==0)
	if(len==sizeof(sim_con_struct))
	{
		memcpy(&con,data_read,len);

		if(	is_pro_use())									// pro-use only: just keep our acf in sync with the pro-use machine
		if(	acft_p_var.size		()>	con.num_acf_this_pc)	// we want to keep our vector in sync with what the user has
			acft_p_var.resize	(	con.num_acf_this_pc);	// for lab-use, it is based on the number of machines we are talking to!
	}														// don't ADD planes yet... just take extra ones out... the acfX message will add them

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// WEAPONS WE HAVE SENT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(strncmp(label_read,"wFLT",4)==0)
	{
		wpn_struct wpn;

		buff_class			buff;
							buff.load_buff((xchr*)data_read,len);
		wpn.lat			=	buff.ext_xdob();
		wpn.lon			=	buff.ext_xdob();
		wpn.time_left	=	1.0;	// send at puff-1, so keep in ram for 1.5 so we don't have to try to keep track of WHAT weapon it is

		if(buff.perfect_scan())
			vec_wpn.push_back(wpn);
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// 1208 SUPPORT: THE P0 MESSAGE HAS LAT NAD LON AND SPECS FOR P0, AND THE P1->PX ARE DIFF MESSAGES
// THIS IS DIFF THAN THE CURRENT STRUCTURE, WHERE EVERYPONE USES THE SAME STRUCT, BUT P0 ALSO SENDS IN SOME ADDITIONAL STUFF.
// SO THE 1208 SUPPORT IS DIFFERENT IN THAT IT IS A DIFFERENT STRUCT FOR P0 AND THAN PX
// SO WE DITCH THEN WHEN WE ARE PAST 1208
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	#if SUPPORT_1208
		struct xloc_struct
		{
			xchr tailnum[8]									={0};
			xflt lon_deg,lat_deg,ele_mtr,ele_agl_mtr		=0.0;	// location
			xflt phi_deg,the_deg,psi_deg_true,psi_deg_mag	=0.0;	// AHRS
			xflt slip_deg,AV_KEAS,vvi_fpm					=0.0;	// PFD

			xflt flap_req		=0.0;
			xflt sbrk_req		=0.0;
			xint gear_req		=0.0;

			xint nav_frq_int	=0.0;
			xint nav_ntf_int	=0.0;
			xint nav_gls_int	=0.0;

			xflt nav_OBS_deg	=0.0;
			xflt nav_CDI_lat	=0.0;
			xflt nav_CDI_vrt	=0.0;

			xflt wxr_lon1		=0.0;
			xflt wxr_lat1		=0.0;
			xflt wxr_lon2		=0.0;
			xflt wxr_lat2		=0.0;
		};
		xloc_struct xloc;

		struct ploc_struct
		{
			ploc_struct(){memset(this,0,sizeof(*this));}
		   ~ploc_struct(){}

			xint p;
			xchr tailnum[8];
			xdob drag_time,recv_time;
			xflt lat_deg,lon_deg,ele_mtr,psi_deg_true;
		};
		ploc_struct ploc;

		struct con_struct
		{
			xflt Vso_KIAS		;
			xflt  Vs_KIAS		;
			xflt Vfe_KIAS_full	;
			xflt Vno_KIAS		;
			xflt Vne_KIAS		;

			xflt min_wgt_lb_book;
			xflt max_wgt_lb_book;
			xflt for_cg_in_book	;
			xflt aft_cg_in_book	;
			xflt for_cg_in_enter;
			xflt aft_cg_in_enter;
		
			xflt max_fuel_lb	[fueltankDIM]	;
			xflt max_payl_lb	[fixedDIM]		;
			xchr chr_payl_name	[fixedDIM][64]	;
		
			xflt crt_fuel_lb	[fueltankDIM]	;
			xflt crt_payl_lb	[fixedDIM]		;
			xflt crt_cg_in						;
		};
		con_struct xcon;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// READ 1208 PLANES, AND 1208 HAS US IN THE PRO-USE CASE ONLY
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		if(strncmp(label_read,"xCON",4)==0)
		if(len==sizeof(con_struct))
		for(xint b=0;b<	becn.other_becns.size();b++)				// go thru all our beacons
		if(				becn.other_becns[b].beacon_used)			// if this is a beacon we are deciding to control
		if(				becn.other_becns[b].other_addy == from_ip)	// and this plane comes from that beacon ip
		{
			RUNNING_1208 = 1;
			memcpy(&xcon,data_read,len);

			if(	acft_p_var.size		()<	1)
				acft_p_var.resize	(	1);

			if(xtim.real_time > acft_p(ploc.p)->acft_time_map_move+1.0)		// and not dragging the plane on the map
			{
				con.Vso_KIAS		= xcon.Vso_KIAS		;
				con. Vs_KIAS		= xcon. Vs_KIAS		;
				con.Vfe_KIAS_full	= xcon.Vfe_KIAS_full;
				con.Vno_KIAS		= xcon.Vno_KIAS		;
				con.Vne_KIAS		= xcon.Vne_KIAS		;

				acft_p_var[0].min_wgt_lb_book	= xcon.min_wgt_lb_book	;
				acft_p_var[0].max_wgt_lb_book	= xcon.max_wgt_lb_book	;
				acft_p_var[0].for_cg_in_book	= xcon.for_cg_in_book	;
				acft_p_var[0].aft_cg_in_book	= xcon.aft_cg_in_book	;
				acft_p_var[0].for_cg_in_enter	= xcon.for_cg_in_enter	;
				acft_p_var[0].aft_cg_in_enter	= xcon.aft_cg_in_enter	;
		
				for(xint i=0;i<fueltankDIM	;i++)		acft_p_var[0].max_fuel_lb	[i]	= xcon.max_fuel_lb	[i]		;
				for(xint i=0;i<fixedDIM		;i++)		acft_p_var[0].max_payl_lb	[i]	= xcon.max_payl_lb	[i]		;
				for(xint i=0;i<fixedDIM		;i++)C_eq_C(acft_p_var[0].chr_payl_name	[i]	, xcon.chr_payl_name[i],64)	;
				for(xint i=0;i<fueltankDIM	;i++)		acft_p_var[0].crt_fuel_lb	[i]	= xcon.crt_fuel_lb	[i]		;
				for(xint i=0;i<fixedDIM		;i++)		acft_p_var[0].crt_payl_lb	[i]	= xcon.crt_payl_lb	[i]		;
														acft_p_var[0].crt_cg_in			= xcon.crt_cg_in			;
			}
		}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// READ 1208 PLANES, AND 1208 HAS US IN THE PRO-USE CASE ONLY
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		if(strncmp(label_read,"xLOC",4)==0)
		if(len==sizeof(xloc_struct))
		for(xint b=0;b<	becn.other_becns.size();b++)				// go thru all our beacons
		if(				becn.other_becns[b].beacon_used)			// if this is a beacon we are deciding to control
		if(				becn.other_becns[b].other_addy == from_ip)	// and this plane comes from that beacon ip
		{
			RUNNING_1208 = 1;
			memcpy(&xloc,data_read,len);

			if(	acft_p_var.size		()<	1)
				acft_p_var.resize	(	1);

			if(xtim.real_time > acft_p(ploc.p)->acft_time_map_move+1.0)	// and not dragging the plane on the map
			{
						acft_p_var[0].acft_name			= becn.other_becns[b].other_name;
						acft_p_var[0].acft_addy			= becn.other_becns[b].other_addy;
						acft_p_var[0].acft_time_net_recv= xtim.real_time				;

				C_eq_C(	acft_p_var[0].tailnum			, xloc.tailnum , 8)	;

						acft_p_var[0].lon_deg			= xloc.lon_deg		;
						acft_p_var[0].lat_deg			= xloc.lat_deg		;
						acft_p_var[0].ele_mtr			= xloc.ele_mtr		;
						acft_p_var[0].ele_agl_mtr		= xloc.ele_agl_mtr	;

						acft_p_var[0].phi_deg			= xloc.phi_deg		;
						acft_p_var[0].the_deg			= xloc.the_deg		;
						acft_p_var[0].psi_deg_true		= xloc.psi_deg_true	;
						acft_p_var[0].psi_deg_mag		= xloc.psi_deg_mag	;

						acft_p_var[0].slip_deg			= xloc.slip_deg		;
						acft_p_var[0].AV_KEAS			= xloc.AV_KEAS		;
						acft_p_var[0].vvi_fpm			= xloc.vvi_fpm		;

						acft_p_var[0].flap_req			= xloc.flap_req		;
						acft_p_var[0].sbrk_req			= xloc.sbrk_req		;
						acft_p_var[0].gear_req			= xloc.gear_req		;

						con.nav_frq_int					= xloc.nav_frq_int	;
						con.nav_ntf_int					= xloc.nav_ntf_int	;
						con.nav_gls_int					= xloc.nav_gls_int	;

						con.nav_OBS_deg					= xloc.nav_OBS_deg	;
						con.nav_CDI_lat					= xloc.nav_CDI_lat	;
						con.nav_CDI_vrt					= xloc.nav_CDI_vrt	;

						con.wxr_lon1					= xloc.wxr_lon1		;
						con.wxr_lat1					= xloc.wxr_lat1		;
						con.wxr_lon2					= xloc.wxr_lon2		;
						con.wxr_lat2					= xloc.wxr_lat2		;
			}
		}

		if(strncmp(label_read,"pLOC",4)==0)
		if(len==sizeof(ploc_struct))
		for(xint b=0;b<	becn.other_becns.size();b++)				// go thru all our beacons
		if(				becn.other_becns[b].beacon_used)			// if this is a beacon we are deciding to control
		if(				becn.other_becns[b].other_addy == from_ip)	// and this plane comes from that beacon ip
		{
			RUNNING_1208 = 1;
			memcpy(&ploc,data_read,len);

			xint p_this = 0;
			if(is_pro_use())
				p_this = ploc.p;

			if(is_lab_use())
			for(xint p=0;p<acft_p_size();p++)
			if(acft_p(p)->acft_addy == from_ip)
				p_this = p;

			if(	acft_p_var.size		()<	p_this+1)
				acft_p_var.resize	(	p_this+1);

			if(xtim.real_time > acft_p(p_this)->acft_time_map_move+1.0)	// and not dragging the plane on the map
			{
						acft_p_var[p_this].acft_name			= becn.other_becns[b].other_name;
						acft_p_var[p_this].acft_addy			= becn.other_becns[b].other_addy;
						acft_p_var[p_this].acft_time_net_recv	= xtim.real_time				;

				C_eq_C(	acft_p_var[p_this].tailnum				, ploc.tailnum , 8)				;
						acft_p_var[p_this].lat_deg				= ploc.lat_deg					;
						acft_p_var[p_this].lon_deg				= ploc.lon_deg					;
						acft_p_var[p_this].ele_mtr				= ploc.ele_mtr					;
						acft_p_var[p_this].psi_deg_true			= ploc.psi_deg_true				;
			}
		}
	#endif
}
