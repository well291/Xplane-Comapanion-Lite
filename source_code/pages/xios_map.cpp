//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// FROM X-PLANE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
enum
{
	nt_ndb		= 2,
	nt_vor		= 3,
	nR_loc_ils	= 4,
	nR_loc_loc	= 5,
	nR_gls_ils	= 6,
	nt_omk		= 7,
	nt_mmk		= 8,
	nt_imk		= 9,
	nt_dme_nofrq=12,
	nt_dme_wifrq=13
};

enum storm_add
{
	storm_none			,

	Small_Cell			,
	Small_Line_NS		,
	Small_Line_EW		,
	Medium_Cell			,
	Medium_Line_NS		,
	Medium_Line_EW		,
	Large_Cell			,
	Large_Line_NS		,
	Large_Line_EW		,
	Very_Large_Cell		,
	Very_Large_Cell_NS	,
	Very_Large_Cell_EW	,

	front_draw			,
	front_erase			,

	microburst1			,
	microburst2			,
	microburst3			,
	
	storm_clear
};

static storm_add storm_sel = storm_none;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xflt	app_lon_ref		= 0.0;
xflt	app_lat_ref		= 0.0;
xflt	app_ele_ref		= 0.0;
xflt	app_psi_deg_true= 0.0;
xflt	app_gls_deg		= 0.0;
string	app_str			= "Init Condition";

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xflt s1_path = 0.0;
xflt s2_path = 0.0;
xflt t1_path = 0.0;
xflt t2_path = 0.0;

xflt app_x1	= 0.0;
xflt app_x2	= 0.0;
xflt app_y1	= 0.0;
xflt app_y2	= 0.0;

xflt lon1	= 0.0;
xflt lon2	= 0.0;
xflt lat1	= 0.0;
xflt lat2	= 0.0;

inline xflt deg_per_pix_lon(){return (lon2-lon1) / (app_x2-app_x1);}
inline xflt deg_per_pix_lat(){return (lat2-lat1) / (app_y2-app_y1);}

inline xflt lon_to_x(const xflt lon){return extrap(lon1,app_x1 , lon2,app_x2 , lon);}
inline xflt lat_to_y(const xflt lat){return extrap(lat1,app_y1 , lat2,app_y2 , lat);}
inline xflt x_to_lon(const xflt x  ){return extrap(app_x1,lon1 , app_x2,lon2 , x  );}
inline xflt y_to_lat(const xflt y  ){return extrap(app_y1,lat1 , app_y2,lat2 , y  );}

void lonlat_to_xy(const xflt lon,const xflt lat,xflt*x,xflt*y)
{
	*x = lon_to_x(lon);
	*y = lat_to_y(lat);
}

inline xint on_screen(const xflt lon,const xflt lat)
{
	xflt k=0.2;								// 12-mile buffer will capture any wayward runways and ILS's and holding patterns and the like
	return (fltrange(lon,lon1-k,lon2+k)&&	// add a buffer for runways and ILS's and the like
			fltrange(lat,lat1-k,lat2+k));	// that can be off screen a bit and still enter the screen area
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
const xflt gs_disp_dis_mtr	=  10.0*nmtomtrs;
const xflt gs_disp_hgt_ft	=3000.0			;

xflt path_vrt	[path_dim*8	] = {0};	// rendering buffer
xflt path_len1	[path_dim	] = {0};
xflt path_len2	[path_dim	] = {0};

inline xflt radar_region_lon(const xint i){return interp(0,xios.con.wxr_lon1 , radar_region_idim-1,xios.con.wxr_lon2 , i);}
inline xflt radar_region_lat(const xint j){return interp(0,xios.con.wxr_lat1 , radar_region_jdim-1,xios.con.wxr_lat2 , j);}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
vector <xflt> cell_lons_lats;

void make_a_storm(const storm_add t)
{
	xflt													deg_NS=0.1;		xflt	deg_EW=0.1;
	if(intrange(t,Small_Cell		,Small_Line_EW		)){	deg_NS=0.2;				deg_EW=0.2/cos(app_lat_ref*degtorad);}
	if(intrange(t,Medium_Cell		,Medium_Line_EW		)){	deg_NS=0.4;				deg_EW=0.4/cos(app_lat_ref*degtorad);}
	if(intrange(t,Large_Cell		,Large_Line_EW		)){	deg_NS=0.8;				deg_EW=0.8/cos(app_lat_ref*degtorad);}
	if(intrange(t,Very_Large_Cell	,Very_Large_Cell_EW	)){	deg_NS=1.6;				deg_EW=1.6/cos(app_lat_ref*degtorad);}

	if(t==Small_Line_NS || t==Medium_Line_NS || t==Large_Line_NS || t==Very_Large_Cell_NS)deg_EW *= 0.5;
	if(t==Small_Line_EW || t==Medium_Line_EW || t==Large_Line_EW || t==Very_Large_Cell_EW)deg_NS *= 0.5;

	const xflt ctr_lon = x_to_lon(apps.mou_drag_x[0]);
	const xflt ctr_lat = y_to_lat(apps.mou_drag_y[0]);

	const xflt perlin_wavelen = sqrt(pythag(deg_NS,deg_EW));

	cell_lons_lats.clear();

	for(xflt lon=ctr_lon-deg_EW ; lon<=ctr_lon+deg_EW ; lon+=0.06)
	for(xflt lat=ctr_lat-deg_NS ; lat<=ctr_lat+deg_NS ; lat+=0.06)
	{
		const xflt rat	= interp_noise_smooth_2d(lon/perlin_wavelen , lat/perlin_wavelen , 0)
						* interp(ctr_lon - deg_EW,0.0 , ctr_lon,1.0 , lon)
						* interp(ctr_lon + deg_EW,0.0 , ctr_lon,1.0 , lon)
						* interp(ctr_lat - deg_NS,0.0 , ctr_lat,1.0 , lat)
						* interp(ctr_lat + deg_NS,0.0 , ctr_lat,1.0 , lat);

		if(rat > fltrand(0.0,1.0))
		{
			cell_lons_lats.push_back(lon);
			cell_lons_lats.push_back(lat);
		}
	}
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline xflt alt_to_y(const xflt alt_ft)
{
	const xflt rat=extrap(	app_ele_ref*mtrstoft				,0.0,
							app_ele_ref*mtrstoft+gs_disp_hgt_ft	,1.0,alt_ft);

	return extrap(	0.0,app_y1,
					1.0,app_y2,xpow(rat,0.75));
}

inline xflt deg_to_x(const xflt lon,const xflt lat)
{
	const xflt dlon		=  lon-app_lon_ref;
	const xflt dlat		=  lat-app_lat_ref;
	const xflt mtr_x	= dlon*60.0*nmtomtrs*cos(app_lat_ref*degtorad);
	const xflt mtr_z	=-dlat*60.0*nmtomtrs;
	const xflt mtr_out	=-mtr_x*sin(app_psi_deg_true*degtorad)
						 +mtr_z*cos(app_psi_deg_true*degtorad);

	const xflt rat=extrap(	0.0				,0.0,
							gs_disp_dis_mtr	,1.0,mtr_out);

	return extrap(	0.0,app_x1,
					1.0,app_x2,xpow(rat,0.75));
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
static xint p_draggging	=  -1	;
static xflt ref_hdg_mag	=   0.0	;

void plot_acf(	xdob* tim,
				xflt* lon,
				xflt* lat,
				const xflt ele,
				const xflt psi,
				const xchr tailnum[8],
				const enum_col col,
				const string name,
				class path_class* path,
				const xint px,
				const xint map_en)
{
	const xflt x=lon_to_x(*lon);
	const xflt y=lat_to_y(*lat);

	if(map_en != map_WXR)
	{
		if(p_draggging==px)
		{
			path->path_per_reset();

			*tim=			xtim.real_time		;
			*lon=x_to_lon(	apps.mou_drag_x[0])	;
			*lat=y_to_lat(	apps.mou_drag_y[0])	;

			if(!apps.mou_dragging(0) && !apps.mou_dragging(1))
			{
				PREL_struct PREL;
							PREL.p_idx		=(is_pro_use()) ? px : 0						;	// for lab use, it is always 0 on that machine!
							PREL.type_start	=loc_specify_lle								;
							PREL.dob_lon_deg=					*lon						;
							PREL.dob_lat_deg=					*lat						;
							PREL.dob_ele_mtr=					xios.map_set_alt*fttomtrs	;
							PREL.dob_psi_tru=atmo.get_psi_true(	ref_hdg_mag					,
																*lon						,
																*lat						);
							PREL.dob_spd_msc=					xios.map_set_KIAS*ktstomsc/sqrt(atmo.get_ISA_sigma(ele));

				xios.xios_send_PREL(px,PREL);

				p_draggging=-1;
			}
		}
		else if(p_draggging==-1)
		{
			if(apps.mou_clicked(0) && !apps.mou_dragging(1))	// DOUBLE drags the map
			if(apps.mouse_box_click(x-100,y-100,				// SINGLE drags the airplane or places weather
									x+100,y+100))
			{
				p_draggging=px;
			}
		}
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	batch.init_batch(t_map_icons,col_vert_true,dep_read_fals,dep_writ_fals);
	batch.add_icon(x,y,0,10,colr.colr_enum_to_flt(col,1.0),psi*degtorad);
	batch.plot_batch();

	if(name.size())
	{
		const xflt yt=y-75;
		const xflt dx=intmax2(str_dx_pix(t_font_2,name)+10,100);	// room for alt string

		OGL_fillrect(col_black,0.75	,x-dx*0.5	,yt-35,dx,70);
		plot_string(t_font_2		,x			,yt+24	,tailnum																,col,1.0,just_ctr);
		plot_string(t_font_2		,x			,yt		,int_to_str(intnear(ele*mtrstoft,100.0),1,str_show_with_commas)+"ft"	,col,1.0,just_ctr);
		plot_string(t_font_2		,x			,yt-24	,name																	,col,1.0,just_ctr);
	}
	else
	{
		const xflt yt=y-50;

		OGL_fillrect(col_black,0.75	,x-50		,yt-25	,100,50);
		plot_string(t_font_2		,x			,yt+12	,tailnum																,col,1.0,just_ctr);
		plot_string(t_font_2		,x			,yt-12	,int_to_str(intnear(ele*mtrstoft,100.0),1,str_show_with_commas)+"ft"	,col,1.0,just_ctr);
	}
}

void plot_config(const xint x,xint y,const string s)
{
	y-=50;
	OGL_line		(col_black	,0.67 , x,y+50 , x,y);
	OGL_fillrect	(col_black	,0.67 , x-50,y-10,100,20);
	plot_string(t_font_1,x,y,s,col_white,1.0,just_ctr);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// 2-D TERRAIN
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void synv_class::plot_terrain_2d()
{
	const xflt range_deg = pythag(lon2-lon1,lat2-lat1);
	const xint dij		 = intmax2(range_deg*0.5,1);

	xint i1_plot = intnear(lle_xyz.i_with_lon(lon1)-dij,dij);	while(i1_plot<		   0)i1_plot+=dij;	// MAKE SURE WE SAY %STEP AS WE LIMIT OUR RANGE!
	xint i2_plot = intnear(lle_xyz.i_with_lon(lon2)+dij,dij);	while(i2_plot>=arr_i_DIM)i2_plot-=dij;	// WE MUST HAVE ALL RANGE VALUES %STEP OR WE WILL NEVER PLOT AT THE END OF A LOOP THAT GOES TO THE EDGE!!!
	xint j1_plot = intnear(lle_xyz.j_with_lat(lat1)-dij,dij);	while(j1_plot<		   0)j1_plot+=dij;
	xint j2_plot = intnear(lle_xyz.j_with_lat(lat2)+dij,dij);	while(j2_plot>=arr_j_DIM)j2_plot-=dij;

	const xflt k=0.1;

	for(xint j=j1_plot;j<=j2_plot-dij;j+=dij)		// winding order needed to get our tris to be lower-left / upper-right, as neded for our y-tester to keep the physics in sync with the graphics.
	{
		const xint j1 = j		;	const xflt lat1 = lle_xyz.lat_with_j(j1);
		const xint j2 = j+dij	;	const xflt lat2 = lle_xyz.lat_with_j(j2);

		#if DEV
			if(vrt_ind!=0)
				APP_alert("Terrain starting with non-zero vertex input!","","","");
		#endif

		for(xint i=i1_plot;i<=i2_plot;i+=dij)	// nested tri strip, currently fastest
		{
			const xflt lon	= lle_xyz.lon_with_i(i);
			const xflt alpha= interp(lle_xyz.lon_wes,0.0 , lle_xyz.lon_wes+1.0,1.0 , lon )
							* interp(lle_xyz.lon_eas,0.0 , lle_xyz.lon_eas-1.0,1.0 , lon )
							* interp(lle_xyz.lat_sou,0.0 , lle_xyz.lat_sou+1.0,1.0 , lat1)
							* interp(lle_xyz.lat_nor,0.0 , lle_xyz.lat_nor-1.0,1.0 , lat1) * 255.0;

			vrt_geo[vrt_ind++] = lon_to_x(lon );
			vrt_geo[vrt_ind++] = lat_to_y(lat1);
			vrt_geo[vrt_ind++] = colr.rgba_byt_to_flt(	plane_col_3byt[i][j1][0],
														plane_col_3byt[i][j1][1],
														plane_col_3byt[i][j1][2],alpha);
			vrt_geo[vrt_ind++] = (xflt)(i )*k;
			vrt_geo[vrt_ind++] = (xflt)(j1)*k;

			vrt_geo[vrt_ind++] = lon_to_x(lon );
			vrt_geo[vrt_ind++] = lat_to_y(lat2);
			vrt_geo[vrt_ind++] = colr.rgba_byt_to_flt(	plane_col_3byt[i][j2][0],
														plane_col_3byt[i][j2][1],
														plane_col_3byt[i][j2][2],alpha);
			vrt_geo[vrt_ind++] = (xflt)(i )*k;
			vrt_geo[vrt_ind++] = (xflt)(j2)*k;
		}

		plot_geo(t_synvis_terrain,col_white,1.0,vrt_geo,2,&vrt_ind,GL_TRIANGLE_STRIP , col_vert_true,dep_read_fals,dep_writ_fals);	// NO BATCH HERE: I LIKE TO SEE EACH MEMORY SET ABOVE!
		vrt_ind=0;																													// RESET THE COUNTER FOR THE NEXT STRIP!
	}
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void xios_class::xios_map_PFD()
{
	acft_p(0)->plot_PFD(app_x1 , app_y1,
						app_x2 , app_y2);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void xios_class::xios_map_plan(const xint map_en)
{
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// MAP PAN AND ZOOM
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	enum_col c_icon = col_butn_brt;

	static xflt zoom_clon = 0.0;
	static xflt zoom_clat = 0.0;
	static xflt zoom_dlat = 0.2;

	static xdob prev_real_time = -999.0;

	if(xtim.real_time > prev_real_time + 1.0)	// reset if we went away and came back
	{
		zoom_dlat = 1.0;
		zoom_clon = 0.0;
		zoom_clat = 0.0;
	}

	prev_real_time = xtim.real_time;

	if(apps.mou_dragging(0) && apps.mou_dragging(1))	// DOUBLE drags the map
	if(apps.mouse_box_drag(	app_x1,app_y1,				// SINGLE drags the airplane or places weather
							app_x2,app_y2))				// check for dragging the map "button"... if we are doing that, then do not activate any other buttons!
	{
		zoom_clon -= (apps.mou_drag_dx[0]+apps.mou_drag_dx[1])*0.5*deg_per_pix_lon();
		zoom_clat -= (apps.mou_drag_dy[0]+apps.mou_drag_dy[1])*0.5*deg_per_pix_lat();
		zoom_dlat /=  apps.mou_dis_rat;
		zoom_dlat  = fltlim(zoom_dlat,0.002,100.0);
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// LOCAL MAP
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	shar.shar_per_check("xios_map ini");

	sci_start(	app_x1			,
				app_y1			,
				app_x2-app_x1	,
				app_y2-app_y1	);

	const xflt pixels		= pythag(app_x2-app_x1,app_y2-app_y1);
	const xflt degs			= zoom_dlat;
	const xflt deg_per_kpix	= degs/(pixels*0.001);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// TERRAIN
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	colr.init_col(col_variable,0.85,0.85,0.85);				// we just darken up a bit to match the zoomed-out terrain TEXTURE is all

	for(xint t=t_earth_full_4k1;t<=t_earth_full_4k2;t++)	// remember in track-up view this texture could be anywhere
	{
		const xflt lon_os=180.0*(t-t_earth_full_4k1);

		OGL_quad((enum_tex)t,col_variable,1.0,
			lon_to_x(-180.0+lon_os),lat_to_y(-90.0) , 0.0,0.0,
			lon_to_x(-180.0+lon_os),lat_to_y( 90.0) , 0.0,1.0,
			lon_to_x(   0.0+lon_os),lat_to_y( 90.0) , 1.0,1.0,
			lon_to_x(   0.0+lon_os),lat_to_y(-90.0) , 1.0,0.0);
	}

	shar.shar_per_check("xios_map terrain lo");

	synv.plot_terrain_2d();

	shar.shar_per_check("xios_map terrain hi");

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// WXR
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	vector <xflt> ligt_x;
	vector <xflt> ligt_y;

	for(xint j=0;j<radar_region_jdim-1;j++)
	{
		xflt vrt[radar_region_idim][8] = {0};

		for(xint i=0;i<radar_region_idim;i++)
		{
			vrt[i][0] = lon_to_x(radar_region_lon(i  ));						// x
			vrt[i][1] = lat_to_y(radar_region_lat(j  ));						// y
			vrt[i][2] = interp(0.0,2.0 , 9.0,38.0 ,storm_08[i][j  ]%10)/64.0;	// s texture, %10 to get the litening out
			vrt[i][3] = 0.5;													// t texture

			vrt[i][4] = lon_to_x(radar_region_lon(i  ));						// x
			vrt[i][5] = lat_to_y(radar_region_lat(j+1));						// y
			vrt[i][6] = interp(0.0,2.0 , 9.0,38.0 , storm_08[i][j+1]%10)/64.0;	// s texture, %10 to get the litening out
			vrt[i][7] = 0.5;													// t texture

			if(storm_08[i][j]>=10)
			{
				ligt_x.push_back(vrt[i][0]);
				ligt_y.push_back(vrt[i][1]);
			}
		}

		xint dim = radar_region_idim*8;
		plot_geo(t_map_nexrad,col_white,0.5,&vrt[0][0],2,&dim,GL_TRIANGLE_STRIP , col_vert_fals,dep_read_fals,dep_writ_fals);
	}

	shar.shar_per_check("xios_map wxr");

	batch.init_batch(t_map_icons,col_vert_true,dep_read_fals,dep_writ_fals);

	for(xint i=0;i<ligt_x.size();i++)
		batch.add_icon(ligt_x[i],ligt_y[i],7,10,colr.colr_enum_to_flt(col_white,1.0));

	batch.plot_batch();

	shar.shar_per_check("xios_map litening");

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// APTS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	const xflt lat_deg_to_mtrs = degtomtr								;
	const xflt lon_deg_to_mtrs = degtomtr*cos((lat1+lat2)*0.5*degtorad)	;

	batch.init_batch(t_map_icons,col_vert_true,dep_read_fals,dep_writ_fals);

	for(xint a=0;a<	vec_apt.size();a++)
	if(on_screen(	vec_apt[a].vec_rwy[0].rwy_lon_ctr(),
					vec_apt[a].vec_rwy[0].rwy_lat_ctr()))
	{
		const xflt apt_alpha = interp(4.0,1.0 , 8.0,0.0 , zoom_dlat*4000.0/vec_apt[a].longest_runway_m());

		if(apt_alpha > 0.01)
		{
			const xflt x=lon_to_x(vec_apt[a].vec_rwy[0].rwy_lon_ctr());
			const xflt y=lat_to_y(vec_apt[a].vec_rwy[0].rwy_lat_ctr());

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// APT ICON
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
			batch.add_icon(x,y,1,10,colr.colr_enum_to_flt(c_icon,apt_alpha));

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// RUNWAYS ON TOP OF ICON
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
			for(xint r=0;r<vec_apt[a].vec_rwy.size();r++)
			{
				const xflt width_mult = 2.0;	// just make the runways more visible
				const xflt psi_deg = vec_apt[a].vec_rwy[r].rwy_psi_deg();
				const xflt sin_psi = sin(psi_deg*degtorad);
				const xflt cos_psi = cos(psi_deg*degtorad);
				const xflt lat_wid = vec_apt[a].vec_rwy[r].rwy_wid_m*0.5*sin_psi/lat_deg_to_mtrs*width_mult;
				const xflt lon_wid = vec_apt[a].vec_rwy[r].rwy_wid_m*0.5*cos_psi/lon_deg_to_mtrs*width_mult;

				OGL_quad
				(
					t_white,col_black,apt_alpha,
					lon_to_x(vec_apt[a].vec_rwy[r].rwy_lon1+lon_wid) , lat_to_y(vec_apt[a].vec_rwy[r].rwy_lat1-lat_wid) , s_white_tex,t_white_tex,
					lon_to_x(vec_apt[a].vec_rwy[r].rwy_lon1-lon_wid) , lat_to_y(vec_apt[a].vec_rwy[r].rwy_lat1+lat_wid) , s_white_tex,t_white_tex,
					lon_to_x(vec_apt[a].vec_rwy[r].rwy_lon2-lon_wid) , lat_to_y(vec_apt[a].vec_rwy[r].rwy_lat2+lat_wid) , s_white_tex,t_white_tex,
					lon_to_x(vec_apt[a].vec_rwy[r].rwy_lon2+lon_wid) , lat_to_y(vec_apt[a].vec_rwy[r].rwy_lat2-lat_wid) , s_white_tex,t_white_tex
				);
			}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// APT NAME ON TOP OF RUNWAY
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
			OGL_fillrect(col_butn_brt,apt_alpha	,x-30	,y-34-10,60,20);
			plot_string	(t_font_1				,x		,y-34	,vec_apt[a].nam.apt_id,col_black,apt_alpha*0.75,just_ctr,draw_buildup);
		}

		batch.plot_batch();													// plot the apt icons and strings for the runways to plot on top of the icons
		plot_string(t_font_1,0,0,"",col_black,1.0,just_ctr,draw_finish);	// plot the apt icons and strings for the runways to plot on top of the icons
		shar.shar_per_check("xios_map apt");
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// FIXES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	const xflt fix_alpha = interp(1.0,1.0 , 2.0,0.0 , zoom_dlat);

	batch.init_batch(t_map_icons,col_vert_true,dep_read_fals,dep_writ_fals);

	if(fix_alpha > 0.01)
	for(xint f=0;f<	vec_fix.size();f++)
	if(on_screen(	vec_fix[f].fix_lon,
					vec_fix[f].fix_lat))
	{
		const xflt x = lon_to_x(vec_fix[f].fix_lon);
		const xflt y = lat_to_y(vec_fix[f].fix_lat);

		batch.add_icon	(			x,y		,5,10,colr.colr_enum_to_flt(c_icon,fix_alpha*0.75));
		plot_string		(t_font_1,	x,y-20	,vec_fix[f].fix_name,col_white,fix_alpha,just_ctr,draw_buildup);
	}

	batch.plot_batch();
	plot_string(t_font_1,0,0,"",col_white,1.0,just_ctr,draw_finish);
	shar.shar_per_check("xios_map fix");

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// NAVAIDS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	const xflt nav_alpha = interp(2.0,1.0 , 4.0,0.0 , zoom_dlat);

	batch.init_batch(t_map_icons,col_vert_true,dep_read_fals,dep_writ_fals);

	if(nav_alpha > 0.01)
	for(xint n=0;n<	vec_nav.size();n++)
	if(on_screen(	vec_nav[n].nav_lon,
					vec_nav[n].nav_lat))
	{
		const xflt		x		 = lon_to_x(vec_nav[n].nav_lon);
		const xflt		y		 = lat_to_y(vec_nav[n].nav_lat);
		const enum_col	col_icon = (vec_nav[n].nav_is_failed)?col_red:c_icon;
		const enum_col	col_text = (vec_nav[n].nav_is_failed)?col_red:col_white;

		if(vec_nav[n].nav_type==nt_ndb)batch.add_icon(x,y,3,10,colr.colr_enum_to_flt(col_icon,nav_alpha*0.75));
		if(vec_nav[n].nav_type==nt_vor)batch.add_icon(x,y,2,10,colr.colr_enum_to_flt(col_icon,nav_alpha*0.75));
		if(vec_nav[n].nav_type==nt_omk)batch.add_icon(x,y,4,10,colr.colr_enum_to_flt(col_icon,nav_alpha*0.75));
		if(vec_nav[n].nav_type==nt_mmk)batch.add_icon(x,y,4,10,colr.colr_enum_to_flt(col_icon,nav_alpha*0.75));
		if(vec_nav[n].nav_type==nt_imk)batch.add_icon(x,y,4,10,colr.colr_enum_to_flt(col_icon,nav_alpha*0.75));
		if(vec_nav[n].nav_type!=nt_mmk)
		if(vec_nav[n].nav_type!=nt_imk)
			plot_string(t_font_1,x,y-22,vec_nav[n].nav_id,col_text,nav_alpha*0.75,just_ctr,draw_buildup);

		if(	vec_nav[n].nav_type==nR_loc_ils ||
			vec_nav[n].nav_type==nR_loc_loc )
		{
			const xflt lenlr			= 7.5*nmtomtrs;
			const xflt lenc				= 6.5*nmtomtrs;
			const xflt swidth			= 6.0;
			const xflt mtr_to_deg_lat	= 1.0/(60.0*nmtomtrs									);
			const xflt mtr_to_deg_lon	= 1.0/(60.0*nmtomtrs*cos(vec_nav[n].nav_lat*degtorad)	);

			const xflt lon_l = vec_nav[n].nav_lon - lenlr*sin((vec_nav[n].psi_or_gls_deg_true+swidth)*degtorad)*mtr_to_deg_lon;
			const xflt lat_l = vec_nav[n].nav_lat - lenlr*cos((vec_nav[n].psi_or_gls_deg_true+swidth)*degtorad)*mtr_to_deg_lat;
			const xflt lon_c = vec_nav[n].nav_lon - lenc *sin((vec_nav[n].psi_or_gls_deg_true		)*degtorad)*mtr_to_deg_lon;
			const xflt lat_c = vec_nav[n].nav_lat - lenc *cos((vec_nav[n].psi_or_gls_deg_true		)*degtorad)*mtr_to_deg_lat;
			const xflt lon_r = vec_nav[n].nav_lon - lenlr*sin((vec_nav[n].psi_or_gls_deg_true-swidth)*degtorad)*mtr_to_deg_lon;
			const xflt lat_r = vec_nav[n].nav_lat - lenlr*cos((vec_nav[n].psi_or_gls_deg_true-swidth)*degtorad)*mtr_to_deg_lat;

			const xflt xl = lon_to_x(lon_l);
			const xflt yl = lat_to_y(lat_l);
			const xflt xc = lon_to_x(lon_c);
			const xflt yc = lat_to_y(lat_c);
			const xflt xr = lon_to_x(lon_r);
			const xflt yr = lat_to_y(lat_r);

			OGL_filltri	(col_icon,nav_alpha*0.1 , x,y,xc,yc,xl,yl);
			OGL_filltri	(col_icon,nav_alpha*0.1 , x,y,xr,yr,xc,yc);

			OGL_line	(col_icon,nav_alpha*0.5 , x ,y ,xl,yl);
			OGL_line	(col_icon,nav_alpha*0.5 , x ,y ,xc,yc);
			OGL_line	(col_icon,nav_alpha*0.5 , x ,y ,xr,yr);
			OGL_line	(col_icon,nav_alpha*0.5 , xc,yc,xl,yl);
			OGL_line	(col_icon,nav_alpha*0.5 , xc,yc,xr,yr);
		}

		if(nav_menu_up==n)
		{
			OGL_fillrect(col_text	,0.5,x-40,y-120,  80, 160);
			OGL_line	(col_black	,1.0,x-40,y- 40,x+40,y-40);
	
			if(!vec_nav[n].nav_is_failed)plot_string(t_font_1,x,y-80,"OK"	,col_black,1.0,just_ctr,draw_buildup);
			if( vec_nav[n].nav_is_failed)plot_string(t_font_1,x,y-80,"INOP"	,col_black,1.0,just_ctr,draw_buildup);

			if(zoom_dlat<0.4)
			{
				if(apps.mouse_box_release(x-40,y -40,x+40,y+40))nav_menu_up					=-1;
				if(apps.mouse_box_release(x-40,y-120,x+40,y-40))vec_nav[n].nav_is_failed	=!vec_nav[n].nav_is_failed;
			}

			if(xtim.puff10)
			{
				if( vec_nav[n].nav_is_failed)inet.inet_send("NFAL",vec_nav[n].nav_id,(xint)strlen(vec_nav[n].nav_id));
				if(!vec_nav[n].nav_is_failed)inet.inet_send("NREC",vec_nav[n].nav_id,(xint)strlen(vec_nav[n].nav_id));
			}
		}
		else if(zoom_dlat<0.4 && apps.mouse_box_release(x-40,y-40,x+40,y+40))
		{
			nav_menu_up = n;
		}
	}

	batch.plot_batch();
	plot_string(t_font_1,0,0,"",col_white,1.0,just_ctr,draw_finish);
	shar.shar_per_check("xios_map nav");

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// HOLDING PATTERNS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	static xflt	KEAS = 90.0;
				KEAS = closer_assymp(
				KEAS , fltmax2(acft_p(0)->AV_KEAS , 90.0),xtim.prd_act*0.1);

	const xflt pattern_alpha = interp(4.0,1.0 , 8.0,0.0 , zoom_dlat);

	if(pattern_alpha > 0.01)
	for(xint h=0;h<	vec_hol.size();h++)
	if(on_screen(	vec_hol[h].lon,
					vec_hol[h].lat))
	{
		const xflt sin_psi	= sin( vec_hol[h].inbnd_psi	* degtorad );
		const xflt cos_psi	= cos( vec_hol[h].inbnd_psi	* degtorad );
		const xflt cos_lat	= cos( vec_hol[h].lat		* degtorad );

		const xflt turn_sign= (vec_hol[h].turn_dir=='R')?1.0:-1.0;
		const xflt circ_deg	=  KEAS * 2.0 / 3600.0;												// 2-minute turn: we describe this circumfrence in 2 minutes, minute is 60th of an hour, nm is 60th of a degree
		const xflt diam_deg	= (circ_deg / con_pi) * turn_sign;									// c = 2 pi r
		const xflt len_deg	= (vec_hol[h].len_nm>0.01)	?(vec_hol[h].len_nm			/  60.0)	//  nm is 60th of a degree
														:(vec_hol[h].time_min*KEAS	/3600.0);	// minute is 60th of an hour, nm is 60th of a degree

		const xdob hlon4 = vec_hol[h].lon						;	const xflt x4 = lon_to_x(hlon4);
		const xdob hlat4 = vec_hol[h].lat						;	const xflt y4 = lat_to_y(hlat4);
		const xdob hlon3 = hlon4 -  len_deg * sin_psi/cos_lat	;	const xflt x3 = lon_to_x(hlon3);
		const xdob hlat3 = hlat4 -  len_deg * cos_psi			;	const xflt y3 = lat_to_y(hlat3);
		const xdob hlon2 = hlon3 + diam_deg * cos_psi/cos_lat	;	const xflt x2 = lon_to_x(hlon2);
		const xdob hlat2 = hlat3 - diam_deg * sin_psi			;	const xflt y2 = lat_to_y(hlat2);
		const xdob hlon1 = hlon4 + diam_deg * cos_psi/cos_lat	;	const xflt x1 = lon_to_x(hlon1);
		const xdob hlat1 = hlat4 - diam_deg * sin_psi			;	const xflt y1 = lat_to_y(hlat1);

		glLineWidth(4);
		OGL_line(col_white,pattern_alpha*0.5 , x1,y1,x2,y2);
		OGL_line(col_white,pattern_alpha*0.5 , x3,y3,x4,y4);
		OGL_arc	(col_white,pattern_alpha*0.5 , (x1+x4)*0.5 , (y1+y4)*0.5 , pythag((x1-x4),(y1-y4))*0.5 , vec_hol[h].inbnd_psi-90.0*turn_sign , 180.0*turn_sign);
		OGL_arc	(col_white,pattern_alpha*0.5 , (x2+x3)*0.5 , (y2+y3)*0.5 , pythag((x2-x3),(y2-y3))*0.5 , vec_hol[h].inbnd_psi+90.0*turn_sign , 180.0*turn_sign);
		glLineWidth(1);
	}

	shar.shar_per_check("xios_map hol");

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// AIRSPACE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	const xflt NAS_alpha_space = interp(16.0,1.0 ,32.0,0.0 , zoom_dlat);
	const xflt NAS_alpha_label = interp( 1.0,1.0 , 2.0,0.0 , zoom_dlat);

	if(	(NAS_alpha_space > 0.01) ||
		(NAS_alpha_label > 0.01) )
		AIRS.AIRS_plot_2d(app_x1,app_y1,app_x2,app_y2,NAS_alpha_space,NAS_alpha_label);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// AIRWAYS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	const xflt AIR_alpha_line = interp(2.0,1.0 , 4.0,0.0 , zoom_dlat);
	const xflt AIR_alpha_label= interp(1.0,1.0 , 2.0,0.0 , zoom_dlat);

	if(	(AIR_alpha_line  > 0.01) ||
		(AIR_alpha_label > 0.01) )
	for(xint a=0;a<vec_awy.size();a++)
	{
		const xflt min_lon = fltmin2(vec_awy[a].lon1,vec_awy[a].lon2);
		const xflt max_lon = fltmax2(vec_awy[a].lon1,vec_awy[a].lon2);
		const xflt min_lat = fltmin2(vec_awy[a].lat1,vec_awy[a].lat2);
		const xflt max_lat = fltmax2(vec_awy[a].lat1,vec_awy[a].lat2);

		if(min_lon<lon2 && max_lon>lon1)
		if(min_lat<lat2 && max_lat>lat1)
		if(pythag(max_lon-min_lon,max_lat-min_lat)<3.0)	// we seem to get really long segments othewise that sort of criss-cross the map...
		{
			const xflt x1 = lon_to_x(vec_awy[a].lon1);
			const xflt y1 = lat_to_y(vec_awy[a].lat1);
			const xflt x2 = lon_to_x(vec_awy[a].lon2);
			const xflt y2 = lat_to_y(vec_awy[a].lat2);

			glLineWidth(3);
			OGL_line(col_white,AIR_alpha_line*0.5 , x1,y1,x2,y2);
			glLineWidth(1);

			xflt psi_deg=atan2(y1-y2 , x2-x1)*radtodeg;
			if(fltabs(psi_deg)>90.0)psi_deg-=180.0;

			const xflt cx = (x1+x2)*0.5;
			const xflt cy = (y1+y2)*0.5;

			glPushMatrix();
			glTranslatef(cx,cy,0.0);
			glRotatef	(-psi_deg,0.0,0.0,1.0);

			const xflt sdx = str_dx_pix(t_font_1,vec_awy[a].awy_name)+6;

			OGL_fillrect(col_white,AIR_alpha_label , -sdx*0.5,-10,sdx,20);
			plot_string(t_font_1,0,0,vec_awy[a].awy_name,col_black,AIR_alpha_label*0.75,just_ctr);

			glPopMatrix();
		}

		shar.shar_per_check("xios_map awy");
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// HORIZ PATH
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	for(xint p=0;p<acft_p_size();p++)
	{
		memset(path_len1,0,sizeof(path_len1));
		memset(path_len2,0,sizeof(path_len2));

		for(xint v=													1;v<(xint)acft_p(p)->acft_path.path_points.size()	;v++)path_len1[v] = path_len1[v-1] + pythag(lon_to_x(acft_p(p)->acft_path.path_points[v].path_lon)-lon_to_x(acft_p(p)->acft_path.path_points[v-1].path_lon) , lat_to_y(acft_p(p)->acft_path.path_points[v].path_lat)-lat_to_y(acft_p(p)->acft_path.path_points[v-1].path_lat) );
		for(xint v=(xint)acft_p(p)->acft_path.path_points.size()-	2;v>=0												;v--)path_len2[v] = path_len2[v+1] + pythag(lon_to_x(acft_p(p)->acft_path.path_points[v].path_lon)-lon_to_x(acft_p(p)->acft_path.path_points[v+1].path_lon) , lat_to_y(acft_p(p)->acft_path.path_points[v].path_lat)-lat_to_y(acft_p(p)->acft_path.path_points[v+1].path_lat) );

		xint i=0;

		if((xint)acft_p(p)->acft_path.path_points.size()>1)	// room for heading
		for(xint v=0;v<(xint)acft_p(p)->acft_path.path_points.size();v++)
		{
			const xint v_lo		= intmax2(v-1,0													);
			const xint v_hi		= intmin2(v+1,(xint)acft_p(p)->acft_path.path_points.size()-1	);

			const xflt cx_lo	= lon_to_x(acft_p(p)->acft_path.path_points[v_lo].path_lon);
			const xflt cy_lo	= lat_to_y(acft_p(p)->acft_path.path_points[v_lo].path_lat);
			const xflt cx		= lon_to_x(acft_p(p)->acft_path.path_points[v	].path_lon);
			const xflt cy		= lat_to_y(acft_p(p)->acft_path.path_points[v	].path_lat);
			const xflt cx_hi	= lon_to_x(acft_p(p)->acft_path.path_points[v_hi].path_lon);
			const xflt cy_hi	= lat_to_y(acft_p(p)->acft_path.path_points[v_hi].path_lat);

			const xflt psi_rad	= atan2	(cx_hi-cx_lo , cy_hi-cy_lo);
			const xflt sin_psi	= sin	(psi_rad);
			const xflt cos_psi	= cos	(psi_rad);

			const xflt len_to_an_end= fltmin2(path_len1[v],path_len2[v]);
			const xflt sx_path		= interp(0.0,s1_path , 50.0,s2_path , len_to_an_end);

			path_vrt[i++] = cx+6.0*cos_psi;
			path_vrt[i++] = cy-6.0*sin_psi;
			path_vrt[i++] = sx_path;
			path_vrt[i++] = t1_path;

			path_vrt[i++] = cx-6.0*cos_psi;
			path_vrt[i++] = cy+6.0*sin_psi;
			path_vrt[i++] = sx_path;
			path_vrt[i++] = t2_path;

			if(i >= vrt_dim-8)	// if the path length exceeds the buffer size, then plot in steps if needed
			{
				plot_geo(t_map_icons,col_magenta,1.0,&path_vrt[0],2,&i,GL_TRIANGLE_STRIP , col_vert_fals,dep_read_fals,dep_writ_fals);
				v -= 1;			// go back a step to start the next tri strip right at the end of this one! cool!
			}
		}

		plot_geo(t_map_icons,col_magenta,1.0,&path_vrt[0],2,&i,GL_TRIANGLE_STRIP , col_vert_fals,dep_read_fals,dep_writ_fals);
		shar.shar_per_check("xios_map path");
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PLANES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	for(xint p=0;p<acft_p_size();p++)
	{
		enum_col col=col_magenta;

		if(acft_p(p)->team_now == team_red	)col=col_red	;
		if(acft_p(p)->team_now == team_blue	)col=col_blue	;
		if(acft_p(p)->team_now == team_green)col=col_green	;
		if(acft_p(p)->team_now == team_gold	)col=col_yellow	;

		plot_acf(	&acft_p(p)->acft_time_map_move,
					&acft_p(p)->lon_deg,
					&acft_p(p)->lat_deg,
					 acft_p(p)->ele_mtr,
					 acft_p(p)->psi_deg_true,
					 acft_p(p)->tailnum,col,
					 acft_p(p)->acft_name,
					&acft_p(p)->acft_path,p,map_en);
	}

	for(xint w=0;w<vec_wpn.size();w++)
	{
		const xflt x = lon_to_x(vec_wpn[w].lon);
		const xflt y = lat_to_y(vec_wpn[w].lat);
		plot_string(t_font_2,x,y,"X",col_magenta,1.0,just_ctr);
	}

	if(p_draggging==-1)	// don't re-scale map when dragging the plane!
	{
		lon1 = acft_p(0)->lon_deg + zoom_clon - (zoom_dlat/cos(acft_p(0)->lat_deg*degtorad)) * ((app_x2-app_x1)/(app_y2-app_y1));
		lon2 = acft_p(0)->lon_deg + zoom_clon + (zoom_dlat/cos(acft_p(0)->lat_deg*degtorad)) * ((app_x2-app_x1)/(app_y2-app_y1));
		lat1 = acft_p(0)->lat_deg + zoom_clat -  zoom_dlat;
		lat2 = acft_p(0)->lat_deg + zoom_clat +  zoom_dlat;
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// STORM / FRONT LINES / RECTS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(map_en == map_WXR)
	{
		if(storm_sel == front_draw || storm_sel == front_erase)
		for(xint c=0;c<=(xint)cell_lons_lats.size()-4;c+=2)	// lines
			OGL_line(col_white,1.0,	lon_to_x(cell_lons_lats[c+0]),
									lat_to_y(cell_lons_lats[c+1]),
									lon_to_x(cell_lons_lats[c+2]),
									lat_to_y(cell_lons_lats[c+3]));

		for(xint c=0;c<=(xint)cell_lons_lats.size()-2;c+=2)	// rects
			OGL_fillrect(	col_white,1.0,
							lon_to_x(cell_lons_lats[c+0])-4,
							lat_to_y(cell_lons_lats[c+1])-4,8,8);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// TIC-TAC-TOE BOARD SHOWING MIDDLE CLEAR WHERE WE CAN DRAG WEATHER
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		const xflt px_lo = lon_to_x(con.wxr_lon1);		// ben runs wxr thru 512 km so I want to show that. we wrap outside of that range.
		const xflt py_lo = lat_to_y(con.wxr_lat1);		// ben runs wxr thru 512 km so I want to show that. we wrap outside of that range.
		const xflt px_hi = lon_to_x(con.wxr_lon2);		// ben runs wxr thru 512 km so I want to show that. we wrap outside of that range.
		const xflt py_hi = lat_to_y(con.wxr_lat2);		// ben runs wxr thru 512 km so I want to show that. we wrap outside of that range.

		const xflt px[4] = {app_x1,px_lo,px_hi,app_x2};	// map edges and weather data edges
		const xflt py[4] = {app_y1,py_lo,py_hi,app_y2};	// map edges and weather data edges

		for(xint i=0;i<3;i++)
		for(xint j=0;j<3;j++)
		if(i==1 && j==1)
		{
			// center area that we show wxr in!
		}
		else
		{
			OGL_fillrect(col_black,0.5,	px[i],
										py[j],
										px[i+1]-px[i],
										py[j+1]-py[j]);
		}
	}
	sci_stop();

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// STORM OPTIONS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(map_en == map_WXR)
	{
		const xflt butn_dx2		= 200								;
		const xflt butn_dx1		= 100 - ui_sep_pix/2				;
		const xflt butn_x1		= ui_sep_pix*2						;
		const xflt butn_x2		= butn_x1 + butn_dx1 + ui_sep_pix	;

		const xflt butn_y2_pix	= graf.dev_dy_use() + butn_dy*0.5 - ui_sep_pix;

		storm_add t = storm_none;

		xflt															y =butn_y2_pix;
		t=Small_Cell		;	if(butn_on_off(storm_sel==t,butn_x1,y-=butn_dy	,butn_dx2,"Small Cell"				))storm_sel	= (storm_sel==t) ? storm_none : t;
		t=Small_Line_NS		;	if(butn_on_off(storm_sel==t,butn_x1,y-=butn_dy	,butn_dx1,"N/S"						))storm_sel	= (storm_sel==t) ? storm_none : t;
		t=Small_Line_EW		;	if(butn_on_off(storm_sel==t,butn_x2,y			,butn_dx1,"E/W"						))storm_sel	= (storm_sel==t) ? storm_none : t;
																	y-=butn_dy*0.5;
		t=Medium_Cell		;	if(butn_on_off(storm_sel==t,butn_x1,y-=butn_dy	,butn_dx2,"Medium Cell"				))storm_sel	= (storm_sel==t) ? storm_none : t;
		t=Medium_Line_NS	;	if(butn_on_off(storm_sel==t,butn_x1,y-=butn_dy	,butn_dx1,"N/S"						))storm_sel	= (storm_sel==t) ? storm_none : t;
		t=Medium_Line_EW	;	if(butn_on_off(storm_sel==t,butn_x2,y			,butn_dx1,"E/W"						))storm_sel	= (storm_sel==t) ? storm_none : t;
																	y-=butn_dy*0.5;
		t=Large_Cell		;	if(butn_on_off(storm_sel==t,butn_x1,y-=butn_dy	,butn_dx2,"Large Cell"				))storm_sel	= (storm_sel==t) ? storm_none : t;
		t=Large_Line_NS		;	if(butn_on_off(storm_sel==t,butn_x1,y-=butn_dy	,butn_dx1,"N/S"						))storm_sel	= (storm_sel==t) ? storm_none : t;
		t=Large_Line_EW		;	if(butn_on_off(storm_sel==t,butn_x2,y			,butn_dx1,"E/W"						))storm_sel	= (storm_sel==t) ? storm_none : t;
																	y-=butn_dy*0.5;
		t=Very_Large_Cell	;	if(butn_on_off(storm_sel==t,butn_x1,y-=butn_dy	,butn_dx2,"System"					))storm_sel	= (storm_sel==t) ? storm_none : t;
		t=Very_Large_Cell_NS;	if(butn_on_off(storm_sel==t,butn_x1,y-=butn_dy	,butn_dx1,"N/S"						))storm_sel	= (storm_sel==t) ? storm_none : t;
		t=Very_Large_Cell_EW;	if(butn_on_off(storm_sel==t,butn_x2,y			,butn_dx1,"E/W"						))storm_sel	= (storm_sel==t) ? storm_none : t;
																	y-=butn_dy*0.5;
		t=front_draw		;	if(butn_on_off(storm_sel==t,butn_x1,y-=butn_dy	,butn_dx2,"Draw Fronts"	,"(Drag!)"	))storm_sel	= (storm_sel==t) ? storm_none : t;
		t=front_erase		;	if(butn_on_off(storm_sel==t,butn_x1,y-=butn_dy	,butn_dx2,"Erase Fronts","(Drag!)"	))storm_sel	= (storm_sel==t) ? storm_none : t;
																	y-=butn_dy*0.5;
		t=microburst1		;	if(butn_on_off(storm_sel==t,butn_x1,y-=butn_dy	,butn_dx2,"Microburst","30 kt"		))storm_sel	= (storm_sel==t) ? storm_none : t;
		t=microburst2		;	if(butn_on_off(storm_sel==t,butn_x1,y-=butn_dy	,butn_dx2,"Microburst","45 kt"		))storm_sel	= (storm_sel==t) ? storm_none : t;
		t=microburst3		;	if(butn_on_off(storm_sel==t,butn_x1,y-=butn_dy	,butn_dx2,"Microburst","60 kt"		))storm_sel	= (storm_sel==t) ? storm_none : t;
																	y-=butn_dy*0.5;
		t=storm_clear		;	if(butn_on_off(storm_sel==t,butn_x1,y-=butn_dy	,butn_dx2,"CLEAR"					)){storm_sel= t;	inet.inet_send("CEL0","",0);}

		if(intrange(storm_sel , Small_Cell , Very_Large_Cell_EW))
		if(apps.mou_clicked(0))
		{
			make_a_storm(storm_sel);
			storm_sel = storm_none;
		}

		if(	storm_sel == front_draw  ||
			storm_sel == front_erase )
		{
			if(apps.mou_dragging(0) && !apps.mou_dragging(1))		// DOUBLE drags the map
			{														// SINGLE drags the airplane or places weather
				const xflt new_lon = x_to_lon(apps.mou_drag_x[0]);
				const xflt new_lat = y_to_lat(apps.mou_drag_y[0]);

				if(cell_lons_lats.size()==0)
				{
					cell_lons_lats.push_back(new_lon);	// drop first point
					cell_lons_lats.push_back(new_lat);	// drop first point
				}
				else
				{
					const xflt old_lon = cell_lons_lats[cell_lons_lats.size()-2];
					const xflt old_lat = cell_lons_lats[cell_lons_lats.size()-1];
					const xflt dis_deg = pythag(new_lon-old_lon,
												new_lat-old_lat);

					const xint steps = dis_deg/0.2;	// the user (incl me) drags the line super fast, skipping spacing, so auto-grid into the rite step-size for the line
													// now, no matter the map frame-rate and how fast the user drags, we get consistent results
					for(xint s=1;s<=steps;s++)
					{
						cell_lons_lats.push_back(interp(0.0,old_lon , steps,new_lon , s));
						cell_lons_lats.push_back(interp(0.0,old_lat , steps,new_lat , s));
					}
				}
			}
		}


		if(!apps.mou_dragging(0) && !apps.mou_dragging(1))	// this applies to fronts and also storm-placement!
		if(cell_lons_lats.size())
		{
			if(storm_sel == front_erase)inet.inet_send("CLER",(xchr*)&cell_lons_lats[0],(xint)cell_lons_lats.size()*sizeof(xflt));
			else						inet.inet_send("CELL",(xchr*)&cell_lons_lats[0],(xint)cell_lons_lats.size()*sizeof(xflt));

			storm_sel = storm_none;

			cell_lons_lats.clear();
		}

		if(apps.mou_clicked(0))
		{
			xflt						kts=-1.0;
			if(storm_sel == microburst1)kts=30.0;
			if(storm_sel == microburst2)kts=45.0;
			if(storm_sel == microburst3)kts=60.0;

			if(kts>0.0)
			{
				buff_class	buff;
							buff.com_xflt(y_to_lat(apps.mou_drag_y[0]));
							buff.com_xflt(x_to_lon(apps.mou_drag_x[0]));
							buff.com_xflt(acft_p(0)->ele_mtr - acft_p(0)->ele_agl_mtr);
							buff.com_xflt(kts);

				inet.inet_send("MBUR",(xchr*)&buff.m_data[0],buff.m_index);

				storm_sel = storm_none;
			}
		}
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// REPLAY
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(map_en != map_WXR)
	{
		enum
		{
			rep_off=0,
			rep_rev_2,
			rep_rev_1,
			rep_pause,
			rep_fwd_1,
			rep_fwd_2
		};
	
		const xflt cx = (app_x1+app_x2)*0.5;
		const xflt dx = 100;
		const xflt y1 = app_y2-ui_sep_pix-butn_dy*1.5;
	
		static xint replay_mode = rep_fwd_1;	// replay mode
	
		string cmnd;
		if(butn_on_off((replay_mode==rep_rev_2) , cx - dx*0.5 - (dx+ui_sep_pix)*2.0	, y1+butn_dy , dx, "<<"	)){replay_mode=(replay_mode==rep_rev_2)?rep_off:rep_rev_2;	if(replay_mode==rep_off){cmnd="sim/replay/replay_off";} else {cmnd="sim/replay/rep_play_fr"	;}}
		if(butn_on_off((replay_mode==rep_rev_1) , cx - dx*0.5 - (dx+ui_sep_pix)*1.0	, y1+butn_dy , dx, "<"	)){replay_mode=(replay_mode==rep_rev_1)?rep_off:rep_rev_1;	if(replay_mode==rep_off){cmnd="sim/replay/replay_off";} else {cmnd="sim/replay/rep_play_rr"	;}}
		if(butn_on_off((replay_mode==rep_pause) , cx - dx*0.5						, y1+butn_dy , dx, "||"	)){replay_mode=(replay_mode==rep_pause)?rep_off:rep_pause;	if(replay_mode==rep_off){cmnd="sim/replay/replay_off";} else {cmnd="sim/replay/rep_pause"	;}}
		if(butn_on_off((replay_mode==rep_fwd_1) , cx - dx*0.5 + (dx+ui_sep_pix)*1.0	, y1+butn_dy , dx, ">"	)){replay_mode=(replay_mode==rep_fwd_1)?rep_off:rep_fwd_1;	if(replay_mode==rep_off){cmnd="sim/replay/replay_off";} else {cmnd="sim/replay/rep_play_rf"	;}}
		if(butn_on_off((replay_mode==rep_fwd_2) , cx - dx*0.5 + (dx+ui_sep_pix)*2.0	, y1+butn_dy , dx, ">>"	)){replay_mode=(replay_mode==rep_fwd_2)?rep_off:rep_fwd_2;	if(replay_mode==rep_off){cmnd="sim/replay/replay_off";} else {cmnd="sim/replay/rep_play_ff"	;}}
		if(cmnd.size()>1)
			inet.inet_send("CMND",(xchr*)cmnd.c_str(),(xint)cmnd.size());
	
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ACFT DRAG
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		const xflt x2	= app_x1+125;
		const xflt x3	= app_x1+250;
		const xflt x4	= app_x2;
		const xflt y2	= app_y1;//+butn_dy*1.5+ui_sep_pix;
	
		const xflt v_lo	=(map_set_alt<200.0)? 0.0:con.Vso_KIAS;
		const xflt v_hi	=(map_set_alt<200.0)?20.0:con.Vne_KIAS;
	
		butn_dir	(x2,y2+butn_dy*1.0			,"hdng"							,&ref_hdg_mag	,xfals				);
		butn_slide	(x3,y2+butn_dy*1.5	,x4-x3	,"KIAS when we move aircraft"	,&map_set_KIAS	,"kt",1,v_lo,v_hi	);
		butn_slide	(x3,y2+butn_dy*0.5	,x4-x3	,"alt MSL when we move aircraft",&map_set_alt	,"ft",1000,0,40000	);
	}
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void xios_class::xios_map_profile()
{
	sci_start(		app_x1			,
					app_y1			,
					app_x2-app_x1	,
					app_y2-app_y1	);

	OGL_fillrect(	col_butn_drk,1.0,
					app_x1			,
					app_y1			,
					app_x2-app_x1	,
					app_y2-app_y1	);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// FIND RWAY
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	app_lon_ref		=0.0;
	app_lat_ref		=0.0;
	app_ele_ref		=0.0;
	app_psi_deg_true=0.0;
	app_gls_deg		=0.0;
	app_str			="Cannot find a destination runway!";

	xflt cdi_max_mtr=999999.9;

	for(xint a=0;a<vec_apt				.size();a++)	// have to use global not local to get the rite one even if we are zoomed in enough to not see it on the main map!
	for(xint r=0;r<vec_apt[a].vec_rwy	.size();r++)
	{
		xflt rwy_lon1			=vec_apt[a].vec_rwy[r].rwy_lon1		;
		xflt rwy_lat1			=vec_apt[a].vec_rwy[r].rwy_lat1		;
		xflt rwy_lon2			=vec_apt[a].vec_rwy[r].rwy_lon2		;
		xflt rwy_lat2			=vec_apt[a].vec_rwy[r].rwy_lat2		;
		xflt rwy_psi_deg_true	=vec_apt[a].vec_rwy[r].rwy_psi_deg();

		for(xint d=0;d<2;d++)
		{
			if(d==1)	// both directions on each runway
			{
				flt_switch(&rwy_lon1,&rwy_lon2);
				flt_switch(&rwy_lat1,&rwy_lat2);

				rwy_psi_deg_true = fltwrap(
				rwy_psi_deg_true + 180.0 , 0.0,360.0);
			}

			if(fltabs(fltwrap(acft_p(0)->psi_deg_true-rwy_psi_deg_true,-180.0,180.0))<90.0)	// don't look at rev-dir runways
			{
				const xflt crs_to_tdz_deg=lle_xyz.crs_deg_with_dll(rwy_lon2,acft_p(0)->lon_deg , rwy_lat2,acft_p(0)->lat_deg);
				const xflt dis_to_tdz_mtr=lle_xyz.dis_mtr_with_dll(rwy_lon2,acft_p(0)->lon_deg , rwy_lat2,acft_p(0)->lat_deg);
				const xflt ele_to_tdz_mtr=									acft_p(0)->ele_mtr-vec_apt[a].nam.apt_ele_mtr;

				if(dis_to_tdz_mtr<gs_disp_dis_mtr)
				{
					const xflt cdi_loc_deg=fltabs(fltwrap(crs_to_tdz_deg-rwy_psi_deg_true,-180.0,180.0));
					const xflt cdi_gls_deg=atan2(ele_to_tdz_mtr,dis_to_tdz_mtr)*radtodeg-3.0			;

					const xflt cdi_loc_mtr=cdi_loc_deg*degtorad*dis_to_tdz_mtr;
					const xflt cdi_gls_mtr=cdi_gls_deg*degtorad*dis_to_tdz_mtr;

					const xflt cdi_tot_mtr=pythag(cdi_gls_mtr,cdi_loc_mtr);

					if(	cdi_tot_mtr<cdi_max_mtr)
					{	cdi_max_mtr=cdi_tot_mtr;

						app_lon_ref		=interp(0.0,rwy_lon1 , 1.0,rwy_lon2 ,0.1);
						app_lat_ref		=interp(0.0,rwy_lat1 , 1.0,rwy_lat2 ,0.1);
						app_ele_ref		=vec_apt[a].nam.apt_ele_mtr;
						app_psi_deg_true=rwy_psi_deg_true;
						app_gls_deg		=3.0;
						app_str			=(string)vec_apt[a].nam.apt_name+" "+(string)((d==0)?vec_apt[a].vec_rwy[r].rwy_num1:vec_apt[a].vec_rwy[r].rwy_num2);
					}
				}
			}
		}
	}

	xflt lon_ref_loc=0.0;
	xflt lat_ref_loc=0.0;
	xflt lon_ref_gls=0.0;
	xflt lat_ref_gls=0.0;

	for(xint n=0;n<vec_nav.size();n++)	// have to use global not local to get the rite one even if we are zoomed in enough to not see it on the main map!
	if(vec_nav[n].nav_freq==con.nav_frq_int)
	{
		const xflt dis_to_tdz_mtr=lle_xyz.dis_mtr_with_dll(	vec_nav[n].nav_lon,acft_p(0)->lon_deg,
															vec_nav[n].nav_lat,acft_p(0)->lat_deg);

		if(dis_to_tdz_mtr<gs_disp_dis_mtr)
		{
			if(vec_nav[n].nav_type==nR_loc_ils && fltabs(fltwrap(vec_nav[n].psi_or_gls_deg_true-app_psi_deg_true,-180.0,180.0))<90.0)	// don't grab reverse-direction runways
			{
				lon_ref_loc		=vec_nav[n].nav_lon				;	// if we plot dist along the runway heading, then we can use the gls as the ref point
				lat_ref_loc		=vec_nav[n].nav_lat				;	// even though it is off to the side, since we only measure distance along the ILS path!
				app_psi_deg_true=vec_nav[n].psi_or_gls_deg_true	;
			}

			if(vec_nav[n].nav_type==nR_gls_ils)
			{
				lon_ref_gls		=vec_nav[n].nav_lon				;	// if we plot dist along the runway heading, then we can use the gls as the ref point
				lat_ref_gls		=vec_nav[n].nav_lat				;	// even though it is off to the side, since we only measure distance along the ILS path!
				app_ele_ref		=vec_nav[n].nav_ele_mtr			;
				app_gls_deg		=vec_nav[n].psi_or_gls_deg_true	;
			}
		}
	}

	if(lon_ref_loc!=0.0)	// if we found a loc and gls that we re turned into, then set the touchdown point to intersection of those two points
	if(lat_ref_loc!=0.0)	// if we found a loc and gls that we re turned into, then set the touchdown point to intersection of those two points
	if(lon_ref_gls!=0.0)	// if we found a loc and gls that we re turned into, then set the touchdown point to intersection of those two points
	if(lat_ref_gls!=0.0)	// if we found a loc and gls that we re turned into, then set the touchdown point to intersection of those two points
	{
		const xflt crs_loc_to_gls_deg	=lle_xyz.crs_deg_with_dll(lon_ref_loc,lon_ref_gls , lat_ref_loc,lat_ref_gls);
		const xflt dis_loc_to_gls_mtr	=lle_xyz.dis_mtr_with_dll(lon_ref_loc,lon_ref_gls , lat_ref_loc,lat_ref_gls);
		const xflt sep_loc_to_gls_deg	=fltwrap(crs_loc_to_gls_deg-app_psi_deg_true,-180.0,180.0);
		const xflt dis_loc_downrange	=dis_loc_to_gls_mtr*cos(sep_loc_to_gls_deg*degtorad)	;
		const xflt lat_deg_to_mtrs		=degtomtr												;
		const xflt lon_deg_to_mtrs		=degtomtr*cos((lat_ref_gls+lat_ref_loc)*0.5*degtorad)	;

		app_lon_ref = lon_ref_loc - dis_loc_downrange*sin(app_psi_deg_true*degtorad)/lon_deg_to_mtrs;
		app_lat_ref = lat_ref_loc - dis_loc_downrange*cos(app_psi_deg_true*degtorad)/lat_deg_to_mtrs;
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PLOT ALTITUDE TICKMARKS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(app_lon_ref!=0.0)
	if(app_lat_ref!=0.0)
	if(app_gls_deg!=0.0)
	{
		xflt step_ft=font_1_dy*2.0*gs_disp_hgt_ft/(app_y2-app_y1);	// this will cause 1 line every 2 lines of text in pixel height

			 if(step_ft< 150.0)	step_ft= 100.0;
		else if(step_ft< 350.0)	step_ft= 200.0;
		else if(step_ft< 750.0)	step_ft= 500.0;
		else if(step_ft<1500.0)	step_ft=1000.0;
		else if(step_ft<3000.0)	step_ft=2000.0;
		else					step_ft=5000.0;

		for(xflt	alt_ft =fltnear(app_ele_ref*mtrstoft				-step_ft*0.5,	step_ft);
					alt_ft< fltnear(app_ele_ref*mtrstoft+gs_disp_hgt_ft	+step_ft*0.5,	step_ft)+1.0;
					alt_ft+=															step_ft)
		{
			OGL_line	(col_text_back,0.5,	app_x1   ,alt_to_y(alt_ft),
											app_x2-55,alt_to_y(alt_ft));
			plot_string	(t_font_1,			app_x2- 5,alt_to_y(alt_ft),int_to_str(alt_ft),col_text_back,1.0,just_rgt);
		}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PLOT GLIDESLOPE LINE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		const xflt gs_lon_1=app_lon_ref;
		const xflt gs_lat_1=app_lat_ref;
		const xflt gs_lon_2=app_lon_ref-gs_disp_dis_mtr*sin(app_psi_deg_true*degtorad)*mtrtodeg/cos(app_lat_ref*degtorad);
		const xflt gs_lat_2=app_lat_ref-gs_disp_dis_mtr*cos(app_psi_deg_true*degtorad)*mtrtodeg;

		const xflt x0=deg_to_x(gs_lon_1,gs_lat_1);
		const xflt xt=deg_to_x(gs_lon_2,gs_lat_2);
		const xflt xc=deg_to_x(gs_lon_2,gs_lat_2);
		const xflt xb=deg_to_x(gs_lon_2,gs_lat_2);

		const xflt y0=alt_to_y(app_ele_ref*mtrstoft																	);
		const xflt yt=alt_to_y(app_ele_ref*mtrstoft+gs_disp_dis_mtr*sin((app_gls_deg-0.7	)*degtorad)*mtrstoft	);
		const xflt yc=alt_to_y(app_ele_ref*mtrstoft+gs_disp_dis_mtr*sin((app_gls_deg	 	)*degtorad)*mtrstoft	);
		const xflt yb=alt_to_y(app_ele_ref*mtrstoft+gs_disp_dis_mtr*sin((app_gls_deg+0.7	)*degtorad)*mtrstoft	);

		OGL_filltri (col_black,0.2,x0,y0,xt,yt,xb,yb);
		OGL_line	(col_black,1.0,x0,y0,xc,yc);
		OGL_line	(col_black,1.0,x0,y0,xb,yb);
		OGL_line	(col_black,1.0,x0,y0,xt,yt);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PLOT MARKER BEACONS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		for(xint n=0;n<vec_nav.size();n++)	// have to use global not local to get the rite one even if we are zoomed in enough to not see it on the main map!
		if(intrange(vec_nav[n].nav_type, nt_omk,nt_imk))
		if(			vec_nav[n].nav_type==nt_omk ||
					vec_nav[n].nav_type==nt_mmk ||
					vec_nav[n].nav_type==nt_imk )
		{
			const xflt crs_to_rwy_deg	=lle_xyz.crs_deg_with_dll(app_lon_ref,vec_nav[n].nav_lon , app_lat_ref,vec_nav[n].nav_lat);
			const xflt dis_to_rwy_mtr	=lle_xyz.dis_mtr_with_dll(app_lon_ref,vec_nav[n].nav_lon , app_lat_ref,vec_nav[n].nav_lat);
			const xflt cdi_loc_deg		=fltabs(fltwrap(crs_to_rwy_deg-app_psi_deg_true,-180.0,180.0));

			if(fltabs(cdi_loc_deg)	<1.0			)	// these are the tolerances we seem to need to plot all the navaids
			if(dis_to_rwy_mtr		<gs_disp_dis_mtr)	// these are the tolerances we seem to need to plot all the navaids
			{
				const xflt px =deg_to_x(vec_nav[n].nav_lon,vec_nav[n].nav_lat	);
				const xflt py1=alt_to_y(vec_nav[n].nav_ele_mtr*mtrstoft			);
				const xflt py2=alt_to_y(vec_nav[n].nav_ele_mtr*mtrstoft+2500.0	);

				enum_col						col=col_white	;
				if(vec_nav[n].nav_type==nt_omk)	col=col_blue	;
				if(vec_nav[n].nav_type==nt_mmk)	col=col_yellow	;
				if(vec_nav[n].nav_type==nt_imk)	col=col_lgray	;

				OGL_filltri	(col,0.5 , px,py1 , px-20,py2 , px+20,py2);
				OGL_line	(col,1.0 , px,py1 , px-20,py2);
				OGL_line	(col,1.0 , px,py1 , px+20,py2);
			}
		}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// VERTICAL PATH
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		for(xint p=0;p<acft_p_size();p++)
		{
			memset(path_len1,0,sizeof(path_len1));
			memset(path_len2,0,sizeof(path_len2));

			for(xint v=													1;v<(xint)acft_p(p)->acft_path.path_points.size()	;v++)path_len1[v] = path_len1[v-1] + pythag(deg_to_x(acft_p(p)->acft_path.path_points[v].path_lon,acft_p(p)->acft_path.path_points[v].path_lat)-deg_to_x(acft_p(p)->acft_path.path_points[v-1].path_lon,acft_p(p)->acft_path.path_points[v-1].path_lat) , alt_to_y(acft_p(p)->acft_path.path_points[v].path_lat)-alt_to_y(acft_p(p)->acft_path.path_points[v-1].path_lat));
			for(xint v=(xint)acft_p(p)->acft_path.path_points.size()-	2;v>=0												;v--)path_len2[v] = path_len2[v+1] + pythag(deg_to_x(acft_p(p)->acft_path.path_points[v].path_lon,acft_p(p)->acft_path.path_points[v].path_lat)-deg_to_x(acft_p(p)->acft_path.path_points[v+1].path_lon,acft_p(p)->acft_path.path_points[v+1].path_lat) , alt_to_y(acft_p(p)->acft_path.path_points[v].path_lat)-alt_to_y(acft_p(p)->acft_path.path_points[v+1].path_lat));

			xint i=0;

			xflt flap_req_old=acft_p(p)->flap_req;
			xflt sbrk_req_old=acft_p(p)->sbrk_req;
			xint gear_req_old=acft_p(p)->gear_req;

			if((xint)acft_p(p)->acft_path.path_points.size()>1)	// room for heading
			for(xint v=0;v<(xint)acft_p(p)->acft_path.path_points.size();v++)
			{
				const xint v_lo		=intmax2(v-1,0													);
				const xint v_hi		=intmin2(v+1,(xint)acft_p(p)->acft_path.path_points.size()-1	);

				const xflt cx_hi	=deg_to_x(acft_p(p)->acft_path.path_points[v_hi].path_lon,acft_p(p)->acft_path.path_points[v_hi].path_lat	);
				const xflt cy_hi	=alt_to_y(acft_p(p)->acft_path.path_points[v_hi].path_ele*mtrstoft											);

				const xflt cx		=deg_to_x(acft_p(p)->acft_path.path_points[v   ].path_lon,acft_p(p)->acft_path.path_points[v   ].path_lat	);
				const xflt cy		=alt_to_y(acft_p(p)->acft_path.path_points[v   ].path_ele*mtrstoft											);

				const xflt cx_lo	=deg_to_x(acft_p(p)->acft_path.path_points[v_lo].path_lon,acft_p(p)->acft_path.path_points[v_lo].path_lat	);
				const xflt cy_lo	=alt_to_y(acft_p(p)->acft_path.path_points[v_lo].path_ele*mtrstoft											);

				const xflt psi_rad	=atan2(cx_hi-cx_lo,cy_hi-cy_lo);
				const xflt cos_psi	=cos(psi_rad);
				const xflt sin_psi	=sin(psi_rad);

				const xflt len_to_an_end=fltmin2(path_len1[v],path_len2[v]);
				const xflt sx_path		=interp(0.0,s1_path,50.0,s2_path,len_to_an_end);

				path_vrt[i++]=cx+6.0*cos_psi;
				path_vrt[i++]=cy-6.0*sin_psi;
				path_vrt[i++]=sx_path;
				path_vrt[i++]=t1_path;

				path_vrt[i++]=cx-6.0*cos_psi;
				path_vrt[i++]=cy+6.0*sin_psi;
				path_vrt[i++]=sx_path;
				path_vrt[i++]=t2_path;

				if(i>=vrt_dim-8)	// the path length can exceed the buffer size, so plot in steps if needed
				{
					plot_geo(t_map_icons,col_magenta,1.0,&path_vrt[0],2,&i,GL_TRIANGLE_STRIP , col_vert_fals,dep_read_fals,dep_writ_fals);
					v-=1;	// go back a step to start the next tri strip right at the end of this one! cool!
				}

				if(	flap_req_old!=									acft_p(p)->acft_path.path_points[v].path_flap_req){
					flap_req_old =									acft_p(p)->acft_path.path_points[v].path_flap_req;
					plot_config(cx,cy,(string)"FLAP "+int_to_str(	acft_p(p)->acft_path.path_points[v].path_flap_req*100)+"%");}

				if(	sbrk_req_old!=									acft_p(p)->acft_path.path_points[v].path_sbrk_req){
					sbrk_req_old =									acft_p(p)->acft_path.path_points[v].path_sbrk_req;
					plot_config(cx,cy,(string)"SBRK "+int_to_str(	acft_p(p)->acft_path.path_points[v].path_sbrk_req*100)+"%");}

				if(	gear_req_old!=									acft_p(p)->acft_path.path_points[v].path_gear_req){
					gear_req_old =									acft_p(p)->acft_path.path_points[v].path_gear_req;
					plot_config(cx,cy,(string)"GEAR "+(string)((	acft_p(p)->acft_path.path_points[v].path_gear_req)?"DN":"UP"));}
			}

			plot_geo(t_map_icons,col_magenta,1.0,&path_vrt[0],2,&i,GL_TRIANGLE_STRIP , col_vert_fals,dep_read_fals,dep_writ_fals);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// MINI-ME AND APPROACH WE THINK WE ARE FLYING
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
			plot_string(t_font_1,(app_x1+app_x2)*0.5,app_y1+font_1_dy*0.5,app_str,col_text_back,1.0,just_ctr);

			batch.init_batch(t_map_icons,col_vert_true,dep_read_fals,dep_writ_fals);

			batch.add_icon(	deg_to_x(	acft_p(p)->lon_deg,
										acft_p(p)->lat_deg),
							alt_to_y(	acft_p(p)->ele_mtr*mtrstoft),0,10,colr.colr_enum_to_flt(col_magenta,1.0));

			batch.plot_batch();
		}
	}
	sci_stop();
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void xios_class::xios_map_start(const xint is_wxr)
{
	s1_path = 6.0/10.0 * tman.tex_dx_vis[t_map_icons]/tman.tex_dx_tot[t_map_icons];	// init this now that icons loaded!
	s2_path = 6.5/10.0 * tman.tex_dx_vis[t_map_icons]/tman.tex_dx_tot[t_map_icons];	// init this now that icons loaded!
	t1_path = 0.4;																	// init this now that icons loaded!
	t2_path = 0.6;																	// init this now that icons loaded!

	const xflt pad_x1 = bx1()							;
	const xflt pad_x2 = bx2()							;
	const xflt pad_y1 = butn_dy*2			+ ui_sep_pix;	// menus
	const xflt pad_y2 = graf.dev_dy_use()	- ui_sep_pix;	// screen top

	app_x1 = pad_x1;
	app_x2 = pad_x2;
	app_y1 = pad_y1;
	app_y2 = pad_y2;

	if(is_wxr)
	{
		xios_map_plan(map_WXR);
	}
	else
	{
		static xint map_en = map_MAP;

		xchr names[map_MAP_PROF+1][name_dim80] = {"PFD / MAP","MAP","MAP / PROFILE"};
		butn_menus(&map_en,map_MAP_PROF+1,names,1);

		if(	map_en == map_PFD_MAP ||
			map_en == map_MAP_PROF )
		{
			if(graf.in_landscape())
			{
				app_x1 = pad_x1;
				app_x2 = pad_x2*0.5-ui_sep_pix;
				app_y1 = pad_y1;
				app_y2 = pad_y2;
			}
			else
			{
				app_x1 = pad_x1;
				app_x2 = pad_x2;
				app_y1 = pad_y2*0.5+ui_sep_pix;
				app_y2 = pad_y2;
			}

			if(map_en == map_PFD_MAP	)xios_map_PFD();		// left half
			if(map_en == map_MAP_PROF	)xios_map_plan(map_en);	// left half
	
			if(graf.in_landscape())
			{
				app_x1 = pad_x2*0.5+ui_sep_pix;
				app_x2 = pad_x2;
				app_y1 = pad_y1;
				app_y2 = pad_y2;
			}
			else
			{
				app_x1 = pad_x1;
				app_x2 = pad_x2;
				app_y1 = pad_y1;
				app_y2 = pad_y2*0.5-ui_sep_pix;
			}
	
			if(map_en == map_PFD_MAP	)xios_map_plan(map_en);	// right half
			if(map_en == map_MAP_PROF	)xios_map_profile();	// right half
		}
		else
		{
			xios_map_plan(map_en);
		}
	}
}
