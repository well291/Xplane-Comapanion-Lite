#include "hl_types.h"



//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// THIS SHOWS WHETHER SOMETHING IS IN VISUAL RANGE... FADE OUT WITH DISTANCE, AND CUT OUT WITH FIELD OF VIEW.
// THUS, USED FOR THE 3-D DRAWING, NOT 2-D MAPS, BECAUSE 2-D MAPS ARE 180 DEG FOV TO A DIFFERENT DISTANCE, CURRENTLY, AND OBSTACLES ON THE MAP DO NOT FADE OUT.
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xflt ret_alpha_lon_lat_fov(	const xflt lon			,
							const xflt lat			,
							const xflt min_dis_mtr	,
							const xflt max_dis_mtr	,
							const xflt fov_use		,const acft_PX_struct acft_p0_detailed)
{
	const xflt   dist_m		=lle_xyz.dis_mtr_with_dll(lon,acft_p0_detailed.lon_deg,lat,acft_p0_detailed.lat_deg);	if(dist_m>max_dis_mtr									)return 0.0;
	const xflt   brg_deg	=lle_xyz.crs_deg_with_dll(lon,acft_p0_detailed.lon_deg,lat,acft_p0_detailed.lat_deg);
	const xflt d_brg_deg	=fltwrap(brg_deg-acft_p0_detailed.psi_deg_true,-180.0,180.0);							if(dist_m>min_dis_mtr && fltabs(d_brg_deg)>fov_use*0.5	)return 0.0;

	return sqrt(interp(	min_dis_mtr,1.0,
						max_dis_mtr,0.0,dist_m));
}

void poly_offset(const xint amount)
{
	static xint last_amount =0		;
	if(			last_amount!=amount	){
				last_amount =amount	;
		if(amount==0)glDisable(GL_POLYGON_OFFSET_FILL);
		else		{glEnable (GL_POLYGON_OFFSET_FILL);	// BEN SAYS: I tried using a larger constant offset and less slope offset - the results were TERRIBLE.  Basically everything z-thrashed and it sucked.  So - we cannot get away from slope-dependent poly_os.
			glPolygonOffset(-10.0*amount,				// this is the offset based on the SLOPE
							-1.0		);}}			// this is the offset in static untis... -1.0 is the smallest amount to get one unit CLOSER to us
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void synv_class::plot_airports_3d(const acft_PX_struct acft_p0_detailed)
{
	poly_offset(10);

	for(xint a=0;a<vec_apt_geo.size();a++)
	{
		const xflt alpha=ret_alpha_lon_lat_fov(	vec_apt_geo[a].apt_lon,
												vec_apt_geo[a].apt_lat,5000.0,20000.0,fov_lat*1.41,acft_p0_detailed);
		if(alpha>0.5)
		for(xint r=0;r<vec_apt_geo[a].vec_rwy_geo.size();r++)
		{
			const xint dim=(xint)vec_apt_geo[a].vec_rwy_geo[r].vrt.size();

			plot_geo(	(enum_tex	)t_synvis_runway,
						(enum_col 	)col_white,
						(xflt		)alpha,
						(xflt* 		)&vec_apt_geo[a].vec_rwy_geo[r].vrt[0],
						(xint 		)3,
						(xint* 		)&dim,
						(GLenum 	)GL_TRIANGLES,
						(xint		)col_vert_fals,
						(xint		)dep_read_true,
						(xint		)dep_writ_fals);	// NO DEPTH-WRITE HERE! THE RUNWAYS Z-THRASH WITH EACH OTHER AND DEPTH-WRITE OVER THE HOOPS THANKS TO THEIR POLY-OS!
		}
	}

	poly_offset(0);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// 3-D TERRAIN
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void synv_class::plot_terrain_3d(const acft_PX_struct acft_p0_detailed)
{
	shar.shar_per_check("PFD_start");

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SIZE AND RES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// IF WE RUN THE ij_step UP HI ON THE GROUND THEN WE WIND UP BEING UNDER THE VISIBLE TERRAIN!
// WE SEE SKY UNDERGROUND IN THAT CASE! SO, WE MUST ALWAYS USE STEP==1 DOWN LOW AND ONLY INCREASE IT AS WE CLIMB AWAY FROM THE GROUND!
// THIS APPLIES TO THE PHONE AS WELL
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	const xflt alt_lo=  500.0*fttomtrs;	// very close to the ground, we must always have a terrain step of 1 to prevent seeing thru the terrain!
	const xflt alt_hi=50000.0*fttomtrs;	// at higher altitude, we can go to sparser spacing, but higher vis

	const xflt dj_lo=1.0;				// MUST ALWAYS BE 1 TO AVOID SEEING UNDER THE GROUND WHEN TAXIING!
	const xflt dj_hi=4.0;
	const xflt di_lo=1.0/cos(fltlim(acft_p0_detailed.lat_deg,-60.0,60.0)*degtorad);
	const xflt di_hi=4.0/cos(fltlim(acft_p0_detailed.lat_deg,-60.0,60.0)*degtorad);

	const xflt vis_use_deg=interp(	alt_lo,vis_deg*0.5,
									alt_hi,vis_deg*1.0,acft_p0_detailed.ele_agl_mtr);

	const xint i_step=intround(interp(	alt_lo,di_lo,
										alt_hi,di_hi,acft_p0_detailed.ele_agl_mtr));

	const xint j_step=intround(interp(	alt_lo,dj_lo,
										alt_hi,dj_hi,acft_p0_detailed.ele_agl_mtr));

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// WITH SIZE AND RES SET, DO THE SETUP VARS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	const xflt vis_lon			=	 vis_use_deg/cos(acft_p0_detailed.lat_deg*degtorad)	;
	const xflt vis_lat			=	 vis_use_deg										;
	const xflt vis_lat_sqr		=sqr(vis_use_deg	)									;
	const xflt vis_lat_nofog_sqr=sqr(vis_use_deg*0.5)									;

	const xflt lon_acf_lft=acft_p0_detailed.lon_deg+vis_lon*sin((acft_p0_detailed.psi_deg_true-fov_lat*0.5)*degtorad);
	const xflt lat_acf_lft=acft_p0_detailed.lat_deg+vis_lat*cos((acft_p0_detailed.psi_deg_true-fov_lat*0.5)*degtorad);
	const xflt lon_acf_rgt=acft_p0_detailed.lon_deg+vis_lon*sin((acft_p0_detailed.psi_deg_true+fov_lat*0.5)*degtorad);
	const xflt lat_acf_rgt=acft_p0_detailed.lat_deg+vis_lat*cos((acft_p0_detailed.psi_deg_true+fov_lat*0.5)*degtorad);

	const xflt lat_buff=fltmax2(0.02,acft_p0_detailed.ele_mtr*lle_xyz.degs_lat_per_mtr);
	const xflt lon_buff=fltmax2(0.02,acft_p0_detailed.ele_mtr*lle_xyz.degs_lon_per_mtr);

	const xflt lon1=fltmin3(acft_p0_detailed.lon_deg,lon_acf_lft,lon_acf_rgt)-lon_buff;
	const xflt lat1=fltmin3(acft_p0_detailed.lat_deg,lat_acf_lft,lat_acf_rgt)-lat_buff;
	const xflt lon2=fltmax3(acft_p0_detailed.lon_deg,lon_acf_lft,lon_acf_rgt)+lon_buff;
	const xflt lat2=fltmax3(acft_p0_detailed.lat_deg,lat_acf_lft,lat_acf_rgt)+lat_buff;

	xint i2_plot=intnear(lle_xyz.i_with_lon(lon2),i_step);
	xint j2_plot=intnear(lle_xyz.j_with_lat(lat2),j_step);
	xint i1_plot=intnear(lle_xyz.i_with_lon(lon1),i_step);
	xint j1_plot=intnear(lle_xyz.j_with_lat(lat1),j_step);

	while(i1_plot<				i_step)i1_plot+=i_step;	// remember that at least the j-loop goes to one past the counter,
	while(i1_plot>=arr_i_DIM-	i_step)i1_plot-=i_step;	// so keep us in limits here to go one past.
	while(i2_plot<				i_step)i2_plot+=i_step;	// also make sure we move in increments of step for the loop below to work
	while(i2_plot>=arr_i_DIM-	i_step)i2_plot-=i_step;	// also limit both hi and lo values since shifting to new coords can give us CRAZY i-j plot values!
	while(j1_plot<				j_step)j1_plot+=j_step;
	while(j1_plot>=arr_j_DIM-	j_step)j1_plot-=j_step;
	while(j2_plot<				j_step)j2_plot+=j_step;
	while(j2_plot>=arr_j_DIM-	j_step)j2_plot-=j_step;

	const xint ic_tex=intnear((i1_plot+i2_plot)/2,10);
	const xint jc_tex=intnear((j1_plot+j2_plot)/2,10);

	const xint idir=(fltrange(acft_p0_detailed.psi_deg_true,180,360))?-1:1;
	const xint jdir=(fltrange(acft_p0_detailed.psi_deg_true, 90,270))?-1:1;

	if(idir==-1)int_switch(&i1_plot,&i2_plot);
	if(jdir==-1)int_switch(&j1_plot,&j2_plot);

	if(i2_plot > i1_plot && idir!= 1)APP_alert("i dir wrong!","","","");
	if(i2_plot < i1_plot && idir!=-1)APP_alert("i dir wrong!","","","");
	if(j2_plot > j1_plot && jdir!= 1)APP_alert("j dir wrong!","","","");
	if(j2_plot < j1_plot && jdir!=-1)APP_alert("j dir wrong!","","","");

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// WE REALLY CANNOT PRE-COMPUTE RGB CAUSE:
//	THE GEO CHANGES EVERY FRAME BASED ON OUR HEADING... WE RUN STRIPS IN AN ORDER TO MINIMIZE Z-BUFF OVER-WRITING... REMEMBER WE DRAW TERRAIN WITH ALPHA IN THE DISTANCE
//	THE RGB CHANGES EVERY FRAME BASED ON TERRAIN-WARNING AND BRIGHTNESS AND THE DRAW-LOOP DIRECTION
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	#if TEST_OZ_SHARK
		string sd1="i1="+int_to_str(i1_plot)+", i2="+int_to_str(i2_plot)+", step="+int_to_str(i_step)+", idir="+int_to_str(idir);
		string sd2="j1="+int_to_str(j1_plot)+", j2="+int_to_str(j2_plot)+", step="+int_to_str(j_step)+", jdir="+int_to_str(jdir);
		shar.shar_per_check("PFD_prep",sd1+" "+sd2);
	#endif

	const xflt k=0.1;

	for(xint j=j1_plot;j!=j2_plot+j_step*jdir;j+=j_step*jdir)	// this is for the winding order needed to get our tris to be lower-left / upper-right, as neded for our y-tester to keep the physics in sync with the graphics.
	{
		xint j0=j				;
		xint j1=j+j_step*jdir	;
		if(sign(idir)!=sign(jdir))								// this keeps the winding order always in terms of lower-left and upper-right triangles, as used by the map and the depth-tester
			int_switch(&j0,&j1);								// we use a lower-left / upper-right triangle convention... anything off of that is wrong and should be corrected!

		const xflt lat0			=lle_xyz.lat_with_j(j0);
		const xflt lat1			=lle_xyz.lat_with_j(j1);
		const xflt dlat0_60nm	=lat0-acft_p0_detailed.lat_deg;
		const xflt dlat1_60nm	=lat1-acft_p0_detailed.lat_deg;
		const xflt z0			=lle_xyz.z_with_lat(lat0);
		const xflt z1			=lle_xyz.z_with_lat(lat1);

		xint vrt_ind=0;

		for(xint i=i1_plot;i!=i2_plot;i+=i_step*idir)			// WE DO NOT USE THE MAP TECHNIQUE OF ONLY PLOTTING A CIRCLE NOT SQUARE HERE CAUSE THESE BOUNDS ARE NOT A SQUARE AROUND THE PLANE!
		{
			#if DEV
				if(i <		   0)APP_alert("pfd-i  OOB!","","","");
				if(i >=arr_i_DIM)APP_alert("pfd-i  OOB!","","","");
				if(j0<		   0)APP_alert("pfd-j0 OOB!","","","");
				if(j0>=arr_j_DIM)APP_alert("pfd-j0 OOB!","","","");
				if(j1<		   0)APP_alert("pfd-j1 OOB!","","","");
				if(j1>=arr_j_DIM)APP_alert("pfd-j1 OOB!","","","");
			#endif

			const xflt lon		=lle_xyz.lon_with_i(i);
			const xflt dlon_60nm=fltwrap(lon-acft_p0_detailed.lon_deg,-180.0,180.0)*lle_xyz.cos_lat;
			const xflt dis0_sqr	=pythag_sqr(dlon_60nm,dlat0_60nm);
			const xflt dis1_sqr	=pythag_sqr(dlon_60nm,dlat1_60nm);

			if(dis0_sqr<vis_lat_sqr || dis1_sqr<vis_lat_sqr)
			{
				xflt x=lle_xyz.x_with_lon(lon);

				vrt_geo[vrt_ind++]=x					;
				vrt_geo[vrt_ind++]=plane_ele[i][j0]		;
				vrt_geo[vrt_ind++]=z0					;
				vrt_geo[vrt_ind++]=colr.rgba_byt_to_flt(plane_col_3byt[i][j0][0],
														plane_col_3byt[i][j0][1],
														plane_col_3byt[i][j0][2],interp(vis_lat_nofog_sqr,255.0 , vis_lat_sqr,0.0 ,dis0_sqr));
				vrt_geo[vrt_ind++]=(xflt)(i -ic_tex)*k	;	// gotta set this every frame for the checks to be the right size for the LOD!
				vrt_geo[vrt_ind++]=(xflt)(j0-jc_tex)*k	;	// gotta set this every frame for the checks to be the right size for the LOD!

				vrt_geo[vrt_ind++]=x					;
				vrt_geo[vrt_ind++]=plane_ele[i][j1]		;
				vrt_geo[vrt_ind++]=z1					;
				vrt_geo[vrt_ind++]=colr.rgba_byt_to_flt(plane_col_3byt[i][j1][0],
												 		plane_col_3byt[i][j1][1],
														plane_col_3byt[i][j1][2],interp(vis_lat_nofog_sqr,255.0 , vis_lat_sqr,0.0 ,dis1_sqr));
				vrt_geo[vrt_ind++]=(xflt)(i -ic_tex)*k	;	// gotta set this every frame for the checks to be the right size for the LOD!
				vrt_geo[vrt_ind++]=(xflt)(j1-jc_tex)*k	;	// gotta set this every frame for the checks to be the right size for the LOD!
			}
		}

		if(vrt_ind>=12)
			plot_geo(t_synvis_terrain,col_white,1.0,vrt_geo,3,&vrt_ind,GL_TRIANGLE_STRIP , col_vert_true,dep_read_true,dep_writ_true);	// NO BATCH HERE: I LIKE TO SEE EACH MEMORY SET ABOVE!
	}

	#if TEST_OZ_SHARK
		shar.shar_per_check("PFD_plot_terrain_stop",
							"alt="		+int_to_str(xios.acft_p(0)->ele_agl_mtr*mtrstoft)+
							" ft, step="+int_to_str(j_step));
	#endif

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// RUNWAYS AFTER GROUND
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	synv.plot_airports_3d(acft_p0_detailed);
	shar.shar_per_check("PFD_syn_vis_plot_airports_3d");
}
