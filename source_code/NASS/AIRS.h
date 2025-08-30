#ifndef AIRS_h
#define AIRS_h

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ENUMS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
enum
{
	UNKNOWN_TYPE=0		,
	RESTRICTED			,
	DANGER				,	// thsy use thos for both drop zone and MOAs
	PROHIBITED			,
	ALPHA				,
	BRAVO				,
	CHARLIE				,
	DELTA				,
	GLIDER_PROHIBITED	,
	CTA_CTR				,
	XPDR_MANDATORY		,
	WAVE_WINDOW
};

enum
{
	geom_none=0			,
	geom_point_lat		,
	geom_point_lon		,
	geom_center_lat		,
	geom_center_lon		,
	geom_circle_rad		,
	geom_arc_dir		,
	geom_arc_lat1		,
	geom_arc_lon1		,
	geom_arc_lat2		,
	geom_arc_lon2
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// GEOMETRIC BITS OF THE AIRSPACE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class geom_class
{
	public:
	   ~geom_class(){}
		geom_class(xint typ,xflt val){
			geom_typ=typ;
			geom_val=val;}

	xint geom_typ;
	xflt geom_val;
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// AIRSPACE OF A GIVEN ALTITUDE RANGE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class part_class
{
	public:
	   ~part_class(){}
		part_class(){part_reset();}

	xint part_alt_lo_ft,part_alt_lo_is_MSL;
	xint part_alt_hi_ft,part_alt_hi_is_MSL;

	vector <geom_class> geoms;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ACCESSORS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	inline string hi_alt(){																return int_to_str(part_alt_hi_ft,1,str_show_with_commas)+(string)((part_alt_hi_is_MSL)?" MSL":" AGL");}
	inline string lo_alt(){if(part_alt_lo_ft==0 && !part_alt_lo_is_MSL)return "SFC";	return int_to_str(part_alt_lo_ft,1,str_show_with_commas)+(string)((part_alt_lo_is_MSL)?" MSL":" AGL");}

	inline void push_lat(xint typ,xflt lat){
		geoms.push_back(geom_class(typ,lat));}

	inline void push_lon(xint typ,xflt lon){
		geoms.push_back(geom_class(typ,lon));}

	inline void push_rad(xint typ,xflt rad){
		dev_alert_nan(rad);
		geoms.push_back(geom_class(typ,rad));}

	inline void push_dir(xint typ,xflt dir){
		dev_alert_nan(dir);
		geoms.push_back(geom_class(typ,dir));}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PREP AND RESET
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
			xflt part_label_lat		;
			xflt part_label_lon		;
			xflt part_label_psi_rad	;	// 0 means move north from this point to draw the label
			xflt part_label_dis_rat	;	// 0 do not move at all, 1 means move the normal amount for the text label

			void part_prep ();
	inline	void part_reset()
	{
		part_alt_lo_ft=part_alt_lo_is_MSL=0;
		part_alt_hi_ft=part_alt_hi_is_MSL=0;

		part_label_lat		=0;
		part_label_lon		=0;
		part_label_psi_rad	=0;
		part_label_dis_rat	=0;

		geoms.clear();
	}
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// AIRSPACE OF A GIVEN TYPE AND NAME
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class airs_class
{
	public:
	   ~airs_class(){}
		airs_class(){airs_reset();}

	xint airs_type;
	xchr airs_name1[name_dim64];		// must be chars not strings to have constant struct size to send across for database updates
	xchr airs_name2[name_dim64];		// must be chars not strings to have constant struct size to send across for database updates
	xflt airs_lat_lo,airs_lat_hi;
	xflt airs_lon_lo,airs_lon_hi;

	vector <part_class> parts;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ACCESSORS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	inline xint show_this_airspace()
	{
		return ((airs_type==RESTRICTED	) ||
				(airs_type==DANGER		) ||
				(airs_type==PROHIBITED	) ||
				(airs_type==ALPHA		) ||
				(airs_type==BRAVO		) ||
				(airs_type==CHARLIE		) ||
				(airs_type==DELTA		));
	}

	inline xint title_sits_below_airspace()		// title sits below each total airspace,
	{											// altitude is ALWAYS shown per-PART
		return ((airs_type==BRAVO		) ||
				(airs_type==CHARLIE		) ||
				(airs_type==DELTA		));
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PREP AND RESET
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
			xflt airs_label_lat;
			xflt airs_label_lon;
			void airs_prep ();
	inline	void airs_reset()
	{
		airs_type=0;
		memset(airs_name1,0,sizeof(airs_name1));
		memset(airs_name2,0,sizeof(airs_name2));
		airs_lat_lo=airs_lat_hi=airs_label_lat=0.0;
		airs_lon_lo=airs_lon_hi=airs_label_lon=0.0;
		parts.clear();
	}

	void airs_plot_3d(const xflt vis_mtr_use);
	void airs_plot_2d(const xflt x1_window,const xflt y1_window,const xflt x2_window,const xflt y2_window,const xflt NAS_alpha_space,const xflt NAS_alpha_label);
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// NATIONAL AIRSPACE SYSTEM CLASS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class AIRS_class
{
	public:
	   ~AIRS_class(){}
		AIRS_class(){plot_airspace_off_2d_3d=2;}

	xint plot_airspace_off_2d_3d;
	vector <airs_class> airss;

	void AIRS_plot_3d	();
	void AIRS_plot_2d	(const xflt x1_window,const xflt y1_window,const xflt x2_window,const xflt y2_window,const xflt NAS_alpha_space,const xflt NAS_alpha_label);
	void AIRS_init		();
};
extern AIRS_class AIRS;

#endif
