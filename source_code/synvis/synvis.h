#ifndef syn_vis_h
#define syn_vis_h

enum
{
	plane_code_grass		=0,
	plane_code_water		=1,
	plane_code_runway		=2,
	plane_code_out_of_region=3,	// not baked into the file... means we are out of the DEM region that we have data for
	plane_code_not_found	=4	// not baked into the file... means we have not found a value for the TAWS collision
};

const xflt fov_lat=60.0;
const xflt vis_deg= 0.5;

struct vec_rwy_geo_struct{vector <xflt> vrt;};
struct vec_apt_geo_struct{vector <vec_rwy_geo_struct> vec_rwy_geo;	xflt apt_lon,apt_lat;};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// LLE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class lle_xyz_class
{
public:
   ~lle_xyz_class(){}
	lle_xyz_class(){memset(this,0,sizeof(*this));
		mtrs_per_deg_lon=degtomtr;	// use a decent default
		mtrs_per_deg_lat=degtomtr;	// for init
		degs_lon_per_mtr=1.0/mtrs_per_deg_lon;
		degs_lat_per_mtr=1.0/mtrs_per_deg_lat;
		cos_lat			=1.0;}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
private:
xdob ref_lon_deg;
xdob ref_lat_deg;

public:
xdob mtrs_per_deg_lon	;
xdob mtrs_per_deg_lat	;
xdob degs_lon_per_mtr	;
xdob degs_lat_per_mtr	;
xint lon_wes,lat_sou	;
xint lon_eas,lat_nor	;
xdob cos_lat			;

inline xdob lat_with_j(const xint j  ){return			interp(0,lat_sou , arr_j_DIM,lat_nor , j  )					;}
inline xdob lon_with_i(const xint i  ){return			interp(0,lon_wes , arr_i_DIM,lon_eas , i  )					;}
inline xint j_with_lat(const xdob lat){return intmin2(	interp(lat_sou,0 , lat_nor,arr_j_DIM , lat),arr_j_DIM-1)	;}
inline xint i_with_lon(const xdob lon){return intmin2(	interp(lon_wes,0 , lon_eas,arr_i_DIM , lon),arr_i_DIM-1)	;}

inline xdob lon_with_x(const xdob x  ){return	  ref_lon_deg+x		*degs_lon_per_mtr;}	// opengl needs this
inline xdob lat_with_z(const xdob z  ){return	  ref_lat_deg-z		*degs_lat_per_mtr;}	// opengl needs this
inline xdob x_with_lon(const xdob lon){return  (lon-ref_lon_deg)	*mtrs_per_deg_lon;}	// opengl needs this
inline xdob z_with_lat(const xdob lat){return -(lat-ref_lat_deg)	*mtrs_per_deg_lat;}	// opengl needs this

inline void set_ref_lon_lat(const xdob lon_ctr,const xdob lat_ctr){
	cos_lat				= cos(lat_ctr*degtorad)					;
	ref_lon_deg			= lon_ctr								;
	ref_lat_deg			= lat_ctr								;
	lon_wes				= intround(lon_ctr-(xdob)lon_secs*0.5)	;
	lat_sou				= intround(lat_ctr-(xdob)lat_secs*0.5)	;
	lon_eas				= lon_wes+lon_secs						;
	lat_nor				= lat_sou+lat_secs						;
	mtrs_per_deg_lon	= degtomtr*cos_lat						;
	mtrs_per_deg_lat	= degtomtr								;
	degs_lon_per_mtr	= 1.0/mtrs_per_deg_lon					;
	degs_lat_per_mtr	= 1.0/mtrs_per_deg_lat					;}

inline xint in_reg(xdob lon,xdob lat){
	if(!fltrange_180(lon,lon_wes,lon_eas))	return xfals;
	if(!fltrange	(lat,lat_sou,lat_nor))	return xfals;
											return xtrue;}

inline xdob dis_mtr_with_dll(const xdob lon2,const xdob lon1,const xdob lat2,const xdob lat1){
	#if DEV
		if(!fltrange(lat1,-90,90))APP_alert("oob lat!","","","");
		if(!fltrange(lat2,-90,90))APP_alert("oob lat!","","","");
	#endif
	return pythag(	dobwrap	(lon2-lon1,-180.0,180.0	)*degtomtr*cos((lat1+lat2)*0.5*degtorad),
							(lat2-lat1				)*degtomtr);}

inline xdob crs_deg_with_dll(const xdob lon2,const xdob lon1,const xdob lat2,const xdob lat1){
	#if DEV
		if(!fltrange(lat1,-90,90))APP_alert("oob lat!","","","");
		if(!fltrange(lat2,-90,90))APP_alert("oob lat!","","","");
	#endif
	return atan2f(	dobwrap	(lon2-lon1,-180.0,180.0	)*cos((lat1+lat2)*0.5*degtorad	),
							(lat2-lat1				)								)*radtodeg;}
};
extern lle_xyz_class lle_xyz;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class synv_class
{
public:
   ~synv_class(){}
	synv_class(){}

xssh plane_ele		[arr_i_DIM][arr_j_DIM]		;	// the LOCATIONS of these verts are as-defined in the lle_xyz_class
xbyt plane_col_3byt	[arr_i_DIM][arr_j_DIM][3]	;	// THIS IS WHERE ALL THE RAM IN THE ENTIRE APP IS USED, ALMOST!
xbyt plane_code		[arr_i_DIM][arr_j_DIM]		;	// SO BE CAREFUL HERE... USE THE SMALLEST VALUES I CAN.

vector <vec_apt_geo_struct> vec_apt_geo;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline xflt terr_ret_ele(xflt lon,xflt lat,xbyt* code=NULL,	xbyt assign_code=plane_code_not_found	,
															xssh assign_ele	=0.0					,
															xflt assign_rat =0.0					)
{
	xflt fi=interp(lle_xyz.lon_wes,0 , lle_xyz.lon_eas,arr_i_DIM , lon);	// do not use i_with_lon here... it returns an int!
	xflt fj=interp(lle_xyz.lat_sou,0 , lle_xyz.lat_nor,arr_j_DIM , lat);	// do not use j_with_lat here... it returns an int!
	xint i1=intmin2(fi,arr_i_DIM-2);
	xint j1=intmin2(fj,arr_j_DIM-2);
	xint i2=i1+1;
	xint j2=j1+1;
	xflt frac_i=fi-(xflt)i1;
	xflt frac_j=fj-(xflt)j1;

	if(assign_code!=plane_code_not_found)	// only assign if we have a code we are assigning to
	for(xint i=i1;i<=i2;i++)
	for(xint j=j1;j<=j2;j++)
	if(	plane_code[i][j]!=assign_code){		// only assign one time so the assign-ratio is really followed
		plane_code[i][j] =assign_code;
		plane_ele [i][j] =
		plane_ele [i][j] *(1.0-assign_rat)+assign_ele*assign_rat;}

	if(code)
	  *code=plane_code[i1][j1];				// the code we store for the poly is in the lower-left corner... the i-j of the poly

	if(inside_cw_reg_lon_lat(			lon				,		 lat			,
								lle_xyz.lon_with_i(i1)	,lle_xyz.lat_with_j(j2)	,
								lle_xyz.lon_with_i(i2)	,lle_xyz.lat_with_j(j1)	))	// lower-left tri
	{
		return					 plane_ele[i1][j1]
			 +(plane_ele[i2][j1]-plane_ele[i1][j1])*(frac_i)
			 +(plane_ele[i1][j2]-plane_ele[i1][j1])*(frac_j);
	}
	else																			// upper-right tri
	{
		return					 plane_ele[i2][j2]
			 +(plane_ele[i1][j2]-plane_ele[i2][j2])*(1.0-frac_i)
			 +(plane_ele[i2][j1]-plane_ele[i2][j2])*(1.0-frac_j);
	}
}

void plot_airports_3d	(const acft_PX_struct acft_p0_detailed);
void plot_terrain_3d	(const acft_PX_struct acft_p0_detailed);
void plot_terrain_2d	();
void synv_load_region	(const xflt lat,const xflt lon);
void synv_per_frame		();
};
extern class synv_class synv;
#endif
