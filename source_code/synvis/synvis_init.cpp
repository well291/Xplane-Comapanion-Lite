#include "hl_types.h"

lle_xyz_class lle_xyz;

synv_class synv;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
enum enum_pave
{
	t_conc_01=0	,	// these are the enums in the runway texture atlas... IN THE RIGHT ORDER FOR THE S AND T TO WORK!
	t_conc_02	,
	t_conc_03	,
	t_conc_04	,
	t_conc_05	,
	t_conc_06	,
	t_conc_07	,
	t_conc_08	,
	t_conc_09	,
	t_conc_10	,
	t_conc_11	,
	t_conc_12	,
	t_conc_13	,
	t_conc_14	,
	t_conc_15	,
	t_conc_16	,
	t_conc_17	,
	t_conc_18	,
	t_conc_19	,
	t_conc_20	,
	t_conc_21	,
	t_conc_22	,
	t_conc_23	,
	t_conc_24	,
	t_conc_25	,
	t_conc_26	,
	t_conc_27	,
	t_conc_28	,
	t_conc_29	,
	t_conc_30	,
	t_conc_31	,
	t_conc_32	,
	t_conc_33	,
	t_conc_34	,
	t_conc_35	,
	t_conc_36	,

	t_conc_blas	,

	t_conc_dis1	,
	t_conc_dis2	,
	t_conc_dis3	,

	t_conc_fdmk	,
	t_conc_line	,

	t_conc_C	,
	t_conc_L	,
	t_conc_R	,

	t_conc_thre,

	t_conc_thre_HUD,
	t_conc_line_HUD
};

string get_dem_path(xint lon,xint lat)
{
	string		lon_str="-180";
	if(lon>=-90)lon_str="-090";
	if(lon>=  0)lon_str="+000";
	if(lon>= 90)lon_str="+090";

	string		lat_str= "-90";
	if(lat>=  0)lat_str= "+00";	// this breakpoint for the files gives us evenly sized files, so no one file goes over our 2-gig file-size limit in linux

	static string	ret												;
					ret="RESOURCE/hires/dem"+lon_str+lat_str+".dat"	;
	return			ret												;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void set_up_runway_geo(xint apt_wrl,xint rwy_wrl,vec_rwy_geo_struct* rwy_geo)
{
	xflt width	=vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_wid_m	;
	xflt tot_len=vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_len_m()	;

	xint num_quads=intmax2(intround(tot_len/width),2);		// WE ADD THE BLASTPADS TO THE RUNWAY LEN HERE, BUT WE DO NOT ADD THE DISPLACED THRESHOLDS!

	xint thre=0;	xint threR=num_quads-0-1;
	xint lcrx=1;	xint lcrxR=num_quads-1-1;
	xint rnum=2;	xint rnumR=num_quads-2-1;
	xint tdz1=4;	xint tdz1R=num_quads-4-1;
	xint tdz2=6;	xint tdz2R=num_quads-6-1;
	xint tdz3=8;	xint tdz3R=num_quads-8-1;

	xint num_=intwrap(chr_to_int(vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_num1[0])*10
					 +chr_to_int(vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_num1[1]),1,36);	// fltwrap here to be on the safe side... bad numbers will overflow us!
	xint numR=intwrap(chr_to_int(vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_num2[0])*10
					 +chr_to_int(vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_num2[1]),1,36);	// fltwrap here to be on the safe side... bad numbers will overflow us!

	xflt end_x_near	[2];	// we do our interp from the endpoints on x, y, z not lat, lon, ele
	xflt end_z_near	[2];	// cause interp on floating-point lat-lon-ele does not have enough resolution!
	xflt end_x_far	[2];	// jaggy runways do result!
	xflt end_z_far	[2];	// so, this x-y-z interp is needed to give straight runways in regular float numbers

	for(xint c=0;c<2;c++)
	{
		xflt lon_wid=-vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_wid_m*0.5*cos(vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_psi_deg()*degtorad)*lle_xyz.degs_lon_per_mtr;
		xflt lat_wid= vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_wid_m*0.5*sin(vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_psi_deg()*degtorad)*lle_xyz.degs_lat_per_mtr;

		if(c==1){
			lon_wid*=-1.0;
			lat_wid*=-1.0;}

		end_x_near	[c]=lle_xyz.x_with_lon(extrap(0,vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_lon1+lon_wid , num_quads,vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_lon2+lon_wid , 0		 ));
		end_x_far	[c]=lle_xyz.x_with_lon(extrap(0,vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_lon1+lon_wid , num_quads,vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_lon2+lon_wid , num_quads));
		end_z_near	[c]=lle_xyz.z_with_lat(extrap(0,vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_lat1+lat_wid , num_quads,vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_lat2+lat_wid , 0		 ));
		end_z_far	[c]=lle_xyz.z_with_lat(extrap(0,vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_lat1+lat_wid , num_quads,vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_lat2+lat_wid , num_quads));
	}

	vector <enum_pave>	quad_tex;
						quad_tex.resize(num_quads);

	for(xint q=0;q<num_quads;q++)	// run the quad strip from begining to end
	{
			 if(q==thre||q==threR												)quad_tex[q]=t_conc_thre	;
		else if(q==lcrx	 && vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_num1[2]=='L'	)quad_tex[q]=t_conc_L		;
		else if(q==lcrx	 && vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_num1[2]=='C'	)quad_tex[q]=t_conc_C		;
		else if(q==lcrx	 && vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_num1[2]=='R'	)quad_tex[q]=t_conc_R		;
		else if(q==lcrxR && vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_num1[2]=='L'	)quad_tex[q]=t_conc_R		;
		else if(q==lcrxR && vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_num1[2]=='C'	)quad_tex[q]=t_conc_C		;
		else if(q==lcrxR && vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_num1[2]=='R'	)quad_tex[q]=t_conc_L		;
		else if(q==rnum															)quad_tex[q]=(enum_pave)((xint)t_conc_01-1+(xint)num_);
		else if(		   q==rnumR												)quad_tex[q]=(enum_pave)((xint)t_conc_01-1+(xint)numR);
		else if(q==tdz1 || q==tdz1R												)quad_tex[q]=t_conc_fdmk	;
		else if(q==tdz2 || q==tdz2R												)quad_tex[q]=t_conc_fdmk	;
		else if(q==tdz3 || q==tdz3R												)quad_tex[q]=t_conc_fdmk	;
		else																	 quad_tex[q]=t_conc_line	;
	}

	for(xint q=0;q<num_quads;q++)	// now turn the basic displaced threshold arrows into beginning and ending segments.
	if(quad_tex[q]==t_conc_dis2)	// if we are a basic displaced threshold, then consider going to beginning or ending threshold texture.
	{
		if(q==			0																					)quad_tex[q]=t_conc_dis1;	// starting displaced threshold at beginning of runway
		if(q==num_quads-1																					)quad_tex[q]=t_conc_dis1;
		if(q>			0 &&	q<num_quads/2 && quad_tex[q-1]==t_conc_blas									)quad_tex[q]=t_conc_dis1;	// starting displaced threshold after blastpad overun
		if(q< num_quads-1 &&	q>num_quads/2 && quad_tex[q+1]==t_conc_blas									)quad_tex[q]=t_conc_dis1;
		if(						q<num_quads/2 && quad_tex[q+1]!=t_conc_dis2 && quad_tex[q+1]!=t_conc_dis3	)quad_tex[q]=t_conc_dis3;	// ending displaced threshold
		if(						q>num_quads/2 && quad_tex[q-1]!=t_conc_dis2 && quad_tex[q-1]!=t_conc_dis3	)quad_tex[q]=t_conc_dis3;
	}

	rwy_geo->vrt.resize(num_quads*30);

	xint v=0;
	for(xint q=0;q<=num_quads;q++)
	{
		xflt x2_lft=extrap(0,end_x_near[0] , num_quads,end_x_far[0] , q);
		xflt z2_lft=extrap(0,end_z_near[0] , num_quads,end_z_far[0] , q);
		xflt x2_rgt=extrap(0,end_x_near[1] , num_quads,end_x_far[1] , q);
		xflt z2_rgt=extrap(0,end_z_near[1] , num_quads,end_z_far[1] , q);

		xflt ele_apt2=vec_apt[apt_wrl].nam.apt_ele_mtr;	// start with apt ele
		xflt ele_ratx=0.75;								// use mostly the airport ele, since that is the data we care about: not as much the hills or valleys nearby

		xflt lon2=extrap(0,vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_lon1 , num_quads,vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_lon2 , q);
		xflt lat2=extrap(0,vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_lat1 , num_quads,vec_apt[apt_wrl].vec_rwy[rwy_wrl].rwy_lat2 , q);
		xflt ele2=synv.terr_ret_ele(lon2,lat2,NULL,plane_code_runway,ele_apt2,ele_ratx);	// do NOT put the terrain BELOW the runway just for rendering... the HOOPS go to the TERRAIN! so the terrain must be placed accurately!

		static xflt x1_lft=0.0;
		static xflt z1_lft=0.0;
		static xflt x1_rgt=0.0;
		static xflt z1_rgt=0.0;
		static xflt ele1  =0.0;

		if(q>0)
		{
			xint i=quad_tex[q-1]%8;	xflt s_ref=		   (xflt)i/8.0;
			xint j=quad_tex[q-1]/8;	xflt t_ref=7.0/8.0-(xflt)j/8.0;

			xflt s_lo=s_ref			;
			xflt s_hi=s_ref+1.0/8.0	;
			xflt t_lo=t_ref			;
			xflt t_hi=t_ref+1.0/8.0	;

			if(q>num_quads/2){
				flt_switch(&s_lo,&s_hi);
				flt_switch(&t_lo,&t_hi);}

			rwy_geo->vrt[v++]=x1_lft;	rwy_geo->vrt[v++]=ele1;		rwy_geo->vrt[v++]=z1_lft;		rwy_geo->vrt[v++]=s_lo;	rwy_geo->vrt[v++]=t_lo;
			rwy_geo->vrt[v++]=x2_lft;	rwy_geo->vrt[v++]=ele2;		rwy_geo->vrt[v++]=z2_lft;		rwy_geo->vrt[v++]=s_lo;	rwy_geo->vrt[v++]=t_hi;
			rwy_geo->vrt[v++]=x1_rgt;	rwy_geo->vrt[v++]=ele1;		rwy_geo->vrt[v++]=z1_rgt;		rwy_geo->vrt[v++]=s_hi;	rwy_geo->vrt[v++]=t_lo;

			rwy_geo->vrt[v++]=x1_rgt;	rwy_geo->vrt[v++]=ele1;		rwy_geo->vrt[v++]=z1_rgt;		rwy_geo->vrt[v++]=s_hi;	rwy_geo->vrt[v++]=t_lo;
			rwy_geo->vrt[v++]=x2_lft;	rwy_geo->vrt[v++]=ele2;		rwy_geo->vrt[v++]=z2_lft;		rwy_geo->vrt[v++]=s_lo;	rwy_geo->vrt[v++]=t_hi;
			rwy_geo->vrt[v++]=x2_rgt;	rwy_geo->vrt[v++]=ele2;		rwy_geo->vrt[v++]=z2_rgt;		rwy_geo->vrt[v++]=s_hi;	rwy_geo->vrt[v++]=t_hi;
		}

		x1_lft	=x2_lft	;
		z1_lft	=z2_lft	;
		x1_rgt	=x2_rgt	;
		z1_rgt	=z2_rgt	;
		ele1	=ele2	;
	}

	if(v!=rwy_geo->vrt.size())
		APP_alert("Wrong number of runway polys!","","","");
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// READ GEOMETRY
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void synv_class::synv_load_region(const xflt lat,const xflt lon)
{
	lle_xyz.set_ref_lon_lat(lon,lat);

	static xint add_off[360][180]={0};	// offset into the one big dem file of each degree. using 1 big file rather than 1 per degree cuts my install/run time from 9+ minutes and an x-code crash to 4 minutes with no crash.

	static xint inited=0;
	if(!inited)
	{
		inited=1;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// WHAT DEGREES ARE LAND OR WATER, SO WE KNOW HOW TO COLOR DEGREES WE DO NOT HAVE!
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		static xmap_class m_map("RESOURCE/map.dat");

		if		(m_map.exists	(				))
		while	(m_map.can_scan	(3*sizeof(xint)	))
		{
			xint lon		=m_map.BIN_int_scan();	xint i=intwrap(lon+180,0,359);
			xint lat		=m_map.BIN_int_scan();	xint j=intlim (lat+ 90,0,179);
			add_off[i][j]	=m_map.BIN_int_scan();

			#if DEV
				if(!intrange(lon,-180,180))APP_alert("Out of bounds lon in map file!","","","");
				if(!intrange(lat, -90, 90))APP_alert("Out of bounds lat in map file!","","","");
			#endif
		}
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	static xint o_lon=-999;
	static xint o_lat=-999;
	const xint londir=-o_lon+lle_xyz.lon_wes;
	const xint latdir=-o_lat+lle_xyz.lat_sou;
					   o_lon=lle_xyz.lon_wes;
					   o_lat=lle_xyz.lat_sou;

	const xint grab_all=((	intabs(londir)+
							intabs(latdir)) !=1 );

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SHIFT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(!grab_all)
	{
		xint i1=0;	xint i2=arr_i_DIM-1;
		xint j1=0;	xint j2=arr_j_DIM-1;

		if(londir== 1){i2-=hires_dem_di;						}
		if(londir==-1){i1+=hires_dem_di;	int_switch(&i1,&i2);}
		if(latdir== 1){j2-=hires_dem_dj;						}
		if(latdir==-1){j1+=hires_dem_dj;	int_switch(&j1,&j2);}

		xint id=sign(i2-i1);	xint io=hires_dem_di*londir;
		xint jd=sign(j2-j1);	xint jo=hires_dem_dj*latdir;

		for(xint i=i1 ; (i-id)!=i2 ; i+=id)
		for(xint j=j1 ; (j-jd)!=j2 ; j+=jd)
		{
			xint is=i+io;
			xint js=j+jo;

			plane_ele		[i][j]		=plane_ele		[is][js]	;
			plane_col_3byt	[i][j][0]	=plane_col_3byt	[is][js][0]	;
			plane_col_3byt	[i][j][1]	=plane_col_3byt	[is][js][1]	;
			plane_col_3byt	[i][j][2]	=plane_col_3byt	[is][js][2]	;
			plane_code		[i][j]		=plane_code		[is][js]	;
		}
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// READ
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	for(xint lon_os=0;lon_os<lon_secs;lon_os++)
	for(xint lat_os=0;lat_os<lat_secs;lat_os++)

	if(((londir==-1 && lon_os==			0)||
		(londir== 1 && lon_os==lon_secs-1)||
		(latdir==-1 && lat_os==			0)||
		(latdir== 1 && lat_os==lat_secs-1)) || grab_all)
	{
		const xint lon_file=intwrap(lle_xyz.lon_wes+lon_os,-180,179);	// wrap-around for around-planet flights, intwrap needs 179 not 180 at one end to get right range, unlike fltwrap!
		const xint lat_file=intlim (lle_xyz.lat_sou+lat_os, -90, 90);	// wrap-around for around-planet flights (AFTER the box-check above)
		const xint i1=(lon_os  )*hires_dem_di;
		const xint i2=(lon_os+1)*hires_dem_di;
		const xint j1=(lat_os  )*hires_dem_dj;
		const xint j2=(lat_os+1)*hires_dem_dj;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// GOBAL DEM CHECKS ARE DONE ELSEWHERE. THIS IS TO SEE IF WE CAN FLY WITH OUR REGION ONLY!
// SO, WE DO NOT SET ANY GLOBAL CHECKS HERE... WE ONLY WANT TO SEE IF OUR REGION IS USEABLE!
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// THIS IS RLE COMPRESSION. WE OUTPUT A VALUE FOR THE ELEVATION
// THEN, IF WE GET THAT VALUE MORE TIMES THAN ONE, THEN WE SIMPLY OUTPUT THE NUMBER OF TIMES WE GOT IT INSTEAD OF OUTPUTTING IT OVER AND OVER AGAIN!		string path=get_dem_path(lon_file,lat_file);
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		const xssh dem_file_version=2;

		const xint address_offset=add_off	[intwrap(lon_file+180,0,359)]
											[intlim (lat_file+ 90,0,179)];

		if(address_offset>0)
		{
			const string path=get_dem_path(lon_file,lat_file);
			xmap_class m_dem(path,address_offset,(hires_dem_di*hires_dem_dj+100)*sizeof(xssh));	// open up the file at the proper offset into the file, and allow enough pointer space to read the whole file... as big as it could possibly be

			if(m_dem.exists			()					)
			if(m_dem.BIN_ssh_scan	()==dem_file_version)
			if(m_dem.BIN_ssh_scan	()==hires_dem_di	)
			if(m_dem.BIN_ssh_scan	()==hires_dem_dj	)
			for(xint j=j1;j<j2;j++)
			{
				xint i=i1;					// THE NUMBER OF TIMES WE GOT IT IS SHOWN AS A NEGATIVE NUMBER
				xssh val_src=0;				// SO, THIS ONLY WORKS AS LONG AS THE ELEVATIONS ARE ALWAYS POSITIVE, SO WE OFFSET BY 1000 METERS TO ASSURE THIS
											// ALSO, SINCE OUR SHORT CAN GO TO 32768, WE ARE LIMITED TO FILES OF SIZE 32768 x 32768... WHICH IS FINE.
				while(i<i2)					// REL COMPRESSION REDUCES FILE SIZE BY 25%, AND HAS NO MEASURABLE IMPACT ON LOAD-TIME!
				{							// IT MIGHT EVEN BE A BIT FASTER TO LOAD! LESS READING FROM THE DRIVE! ONLY A BIT OF FAST MATH BELOW! AWESOME!
					xssh scan=m_dem.BIN_ssh_scan();

					if(scan<0)				// if scan<0, then this is the (negative) number of elevations that have the same value as the last elevation we read in
					{
						for(xint si=0;si<-scan;si++){	plane_ele[i][j]=val_src		;	i++;}
					}
					else
					{
														plane_ele[i][j]=val_src=scan;	i++;
					}
				}
			}
		}

		for(xint i=i1;i<i2;i++)	// extract the plane-code type from crazy elevations
		for(xint j=j1;j<j2;j++)	// gotta do this before doing the normals!
		{
			if(address_offset>0)
			{
					plane_code	[i][j] =plane_code_grass;	// assume normal plane code
				if(	plane_ele	[i][j]>=10000){				// if the ele is above 10,000 meters (everest us 8,848 m)
					plane_ele	[i][j]-=10000;				// then that means
					plane_code	[i][j] =plane_code_water;}	// we have water (no trees, color blue)
					plane_ele	[i][j]-= 1000;				// sub 1000 from all ele cause the numbers are stored as positive (valley is -418 m)
			}												// WE MUST DO THIS AFTER THE SUBTRACTIONS ABOVE TO GET WATER OR AIRPORTS THAT ARE BELOW SEA LEVEL!
			else
			{
					plane_code	[i][j]=plane_code_water;
					plane_ele	[i][j]=0;
			}
		}

		const xflt mtrs_per_deg_lon=degtomtr*cos(((xflt)lat_file+0.5)*degtorad)	;
		const xflt mtrs_per_deg_lat=degtomtr									;

		const xflt d_x_m2=mtrs_per_deg_lon/(xflt)hires_dem_di;
		const xflt d_z_m2=mtrs_per_deg_lat/(xflt)hires_dem_dj;

		for(xint i=i1;i<i2;i++)	// figure the colors. doing this here rather than reading them makes us load maybe 8% slower, but takes 67% less file size!
		for(xint j=j1;j<j2;j++)	// max file size for the app store is 2 gig, which we are pushing right now
		{
			const xint im1=(i==i1  ) ? i1   : i-1;
			const xint jm1=(j==j1  ) ? j1   : j-1;
			const xint ip1=(i==i2-1) ? i2-1 : i+1;
			const xint jp1=(j==j2-1) ? j2-1 : j+1;
			const xflt lite	=extrap(0.0,1.0 , d_x_m2,0.5 , (plane_ele[ip1][j]-plane_ele[im1][j]))	// light comes from upper-right!
							*extrap(0.0,1.0 , d_z_m2,0.5 , (plane_ele[i][jp1]-plane_ele[i][jm1]));	// if it came from upper-left, i would have to change my triangle layout to look decent!

			const xflt lon=(xflt)lon_file+(xflt)(i-i1)/(xflt)hires_dem_di;
			const xflt lat=(xflt)lat_file+(xflt)(j-j1)/(xflt)hires_dem_dj;

			const xflt tif=interp(-180.0,0.0 , 180.0,planet_di , lon);
			const xflt tjf=interp( -90.0,0.0 ,  90.0,planet_dj , lat);

			const xint ti1=intlim(tif,0,planet_di-2);
			const xint tj1=intlim(tjf,0,planet_dj-2);

			const xint ti2=ti1+1;
			const xint tj2=tj1+1;

			const xflt fr=extrap2(	tif-(xflt)ti1,
									tjf-(xflt)tj1,
									planet_array[ti1][tj1][0],	// lower left
									planet_array[ti2][tj1][0],	// lower right
									planet_array[ti1][tj2][0],	// upper left
									planet_array[ti2][tj2][0]);	// upper right

			const xflt fg=extrap2(	tif-(xflt)ti1,
									tjf-(xflt)tj1,
									planet_array[ti1][tj1][1],	// lower left
									planet_array[ti2][tj1][1],	// lower right
									planet_array[ti1][tj2][1],	// upper left
									planet_array[ti2][tj2][1]);	// upper right

			const xflt fb=extrap2(	tif-(xflt)ti1,
									tjf-(xflt)tj1,
									planet_array[ti1][tj1][2],	// lower left
									planet_array[ti2][tj1][2],	// lower right
									planet_array[ti1][tj2][2],	// upper left
									planet_array[ti2][tj2][2]);	// upper right

			plane_col_3byt[i][j][0]=intlim(fr*lite , 0,255);
			plane_col_3byt[i][j][1]=intlim(fg*lite , 0,255);
			plane_col_3byt[i][j][2]=intlim(fb*lite , 0,255);
		}
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// BUILD AIRPORT GEO FOR REGION
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	vec_apt_geo.clear();

	for(xint apt_wrl=0;apt_wrl<vec_apt.size();apt_wrl++)
	if(lle_xyz.in_reg(vec_apt[apt_wrl].vec_rwy[0].rwy_lon_ctr(),
					  vec_apt[apt_wrl].vec_rwy[0].rwy_lat_ctr()))
	{
		if(	vec_apt[apt_wrl].nam.apt_ele_mtr==0.0)																										// sometimes government agencies do not report airport elevation.
			vec_apt[apt_wrl].nam.apt_ele_mtr=synv.terr_ret_ele(vec_apt[apt_wrl].vec_rwy[0].rwy_lon_ctr(),vec_apt[apt_wrl].vec_rwy[0].rwy_lat_ctr());	// in that case, we will have 0 here, and we do the best we can by using terrain elevation here as a backup.

		xint apt_geo=(xint)	vec_apt_geo.size();
							vec_apt_geo.push_back(vec_apt_geo_struct());	// this does NOT automatically zero data, but it won't give me filled-in vectors!

		vec_apt_geo[apt_geo].apt_lat=vec_apt[apt_wrl].vec_rwy[0].rwy_lat_ctr();
		vec_apt_geo[apt_geo].apt_lon=vec_apt[apt_wrl].vec_rwy[0].rwy_lon_ctr();

		for(xint rwy=0;rwy<vec_apt[apt_wrl].vec_rwy.size();rwy++)
		{
			xint rwy_geo=(xint)	vec_apt_geo[apt_geo].vec_rwy_geo.size();
								vec_apt_geo[apt_geo].vec_rwy_geo.push_back(vec_rwy_geo_struct());

			set_up_runway_geo(apt_wrl,rwy,&vec_apt_geo[apt_geo].vec_rwy_geo[rwy_geo]);
		}
	}
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void synv_class::synv_per_frame()
{
	for(xint p=0;p<xios.acft_p_size();p++)
	{
		const xint new_lat = intround(xios.acft_p(p)->lat_deg-(xflt)lat_secs*0.5);
		const xint new_lon = intround(xios.acft_p(p)->lon_deg-(xflt)lon_secs*0.5);

		if(	lle_xyz.lat_sou!=new_lat ||
			lle_xyz.lon_wes!=new_lon )
		{
			synv.synv_load_region(	xios.acft_p(p)->lat_deg,
									xios.acft_p(p)->lon_deg);
		}

		break;	// only do the first current plane we find
	}
}
