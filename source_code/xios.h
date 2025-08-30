//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// CONS AND ENUMS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
const xint fueltankDIM		=   9;	// it's what x-plane has, so we have to have it here to really control everything
const xint fixedDIM			=   9;
const xint group_dim		=  10;	// X-Plane uses 10

const xint cld_dim			= 3;	// number of cloud layers
const xint wnd_dim			= 3;	// number of wind  layers

const xint wpn_station_dim	=12;	// 11 in the F-15!

const xint loc_specify_lle	= 6;	// used by maps and the like
const xint loc_ram			=10;	// ramp
const xint loc_tak			=11;	// Takeoff on runway
const xint loc_vfr			=12;	// VFR app to runway
const xint loc_ifr			=13;	// IFR app to runway

const xint loc_entry		=14;	// this app only: pattern entry
const xint loc_down			=15;	// this app only: downwind
const xint loc_base			=16;	// this app only: base
const xint loc_45final		=17;	// this app only: 45 to final

enum
{
	team_none	=0,
	team_red	=1,
	team_blue	=2,
	team_green	=3,
	team_gold	=4
};

enum cld_typ_enum
{
	cld_cirrus	=0,	// in reality between 16,000 and 48,000 ft, always formed by ice crystals, so should only form above the freezing level, and can never give any precip!
	cld_stratus	=1,	// I DRIVE A DODGE STRATUS! also this is vague and overcast and covers large areas with breaks, and can dump precip.
	cld_cumulus	=2,	// the simpsons
	cld_cnimbus	=3	// thunderstorms
};

enum cld_cov_enum
{
	cld_clr=0,
	cld_few=1,
	cld_sct=2,
	cld_bkn=3,
	cld_ovc=4,
};

enum
{
	ter_eff_dry			,
	ter_eff_wet1		,
	ter_eff_wet2		,
	ter_eff_wet3		,
	ter_eff_puddly1		,
	ter_eff_puddly2		,
	ter_eff_puddly3		,
	ter_eff_snowy1		,
	ter_eff_snowy2		,
	ter_eff_snowy3		,
	ter_eff_icy1		,
	ter_eff_icy2		,
	ter_eff_icy3		,
	ter_eff_snowy_icy1	,
	ter_eff_snowy_icy2	,
	ter_eff_snowy_icy3	,
	ter_eff_dim
};

enum
{
	regn_rapidly_improving			=0,
	regn_improving					=1,
	regn_gradually_improving		=2,
	regn_static_wxr					=3,	// <-this is the default case!
	regn_gradually_deteriorating	=4,
	regn_deteriorating				=5,
	regn_rapidly_deteriorating		=6,
	regn_down_real_weather			=7,	// <-could add a SCAN real_weather after this for scanning presets
	regn_not_set_yet				=8
};

enum
{
	skill_very_easy	=0	,
	skill_easy			,
	skill_medium		,
	skill_hard			,
	skill_very_hard
};

enum
{
	map_PFD_MAP=0	,	// we show PFD and MAP 50/50
	map_MAP			,	// we show map plan only
	map_MAP_PROF	,	// we show MAP and glideslope 50/50
	map_WXR				// we show weather 100
};

enum
{
	cmnd_default=0	,
	cmnd_custom		,
	cmnd_failures	,
	cmnd_dim
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// FIX
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
struct fix_struct
{
	xflt fix_lon,fix_lat	= 0 ;
	xchr fix_name[id_dim8]	={0};	// do NOT use STL string! it is 16 bytes! the fix names are 6!
};
extern vector <fix_struct> vec_fix;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// NAV
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
struct nav_struct
{
	xchr nav_id[id_dim8]			={0};
	xint nav_type,nav_freq			= 0 ;
	xflt nav_lon,nav_lat,nav_ele_mtr= 0 ;
	xflt psi_or_gls_deg_true		= 0 ;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// DATA NOT IN THE X-PLANE STRUCT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	xint nav_is_failed	= 0;	// these vars are internal-use for Xavion only
};
extern vector <nav_struct> vec_nav;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// AIRWAY
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
struct awy_struct
{
	xflt lat1,lon1=0;
	xflt lat2,lon2=0;
	xchr awy_name[net_strDIM]={0};
};
extern vector <awy_struct> vec_awy;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// HOLDING PATTERN
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
struct hol_struct
{
	xdob lat		=0.0;
	xdob lon		=0.0;
	xflt inbnd_psi	=0.0;	// psi
	xchr turn_dir	=0.0;	// turn dir
	xflt time_min	=0.0;	// size: time in minutes
	xflt len_nm		=0.0;	// size: length in nm
};
extern vector <hol_struct> vec_hol;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// APT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
struct rwy_struct
{
	xchr rwy_num1[id_dim8]	={0};	// go to WALQ... yep, the helipad runway numbers are longer than 3 chars!
	xchr rwy_num2[id_dim8]	={0};	// go to WALQ... yep, the helipad runway numbers are longer than 3 chars!
	xflt rwy_lon1			= 0 ;
	xflt rwy_lon2			= 0 ;
	xflt rwy_lat1			= 0 ;
	xflt rwy_lat2			= 0 ;
	xflt rwy_wid_m			= 0 ;

	inline xflt rwy_len_m()
	{
		return pythag((rwy_lon2-rwy_lon1)
				 *cos((rwy_lat2+rwy_lat1)*0.5*degtorad),
					  (rwy_lat2-rwy_lat1))*60.0*nmtomtrs;
	}

	inline xflt rwy_psi_deg()
	{
		return atan2f((rwy_lon2-rwy_lon1)
				 *cos((rwy_lat2+rwy_lat1)*0.5*degtorad),
					  (rwy_lat2-rwy_lat1))*radtodeg;
	}

	inline xflt rwy_lon_ctr(){return (rwy_lon1+rwy_lon2)*0.5;}
	inline xflt rwy_lat_ctr(){return (rwy_lat1+rwy_lat2)*0.5;}
};

struct ram_struct
{
	xchr ram_name[name_dim32]	={0};
	xflt ram_lat				= 0 ;
	xflt ram_lon				= 0 ;
	xflt ram_psi				= 0 ;
};

struct nam_struct
{
	xchr apt_id  [  id_dim8 ]	={0};
	xchr apt_name[name_dim64]	={0};
	xflt apt_ele_mtr			= 0 ;
};

struct apt_struct
{
	nam_struct			    nam;
	vector <rwy_struct>	vec_rwy;
	vector <ram_struct>	vec_ram;

	inline xflt longest_runway_m()
	{
		xflt								ret=0;
		for(xint r=0;r<vec_rwy.size();r++)	ret=fltmax2(ret,vec_rwy[r].rwy_len_m());
		return								ret;
	}
};
extern vector <apt_struct> vec_apt;

struct wpn_struct
{
	xdob lat		=0.0;
	xdob lon		=0.0;
	xflt time_left	=0.0;
};
extern vector <wpn_struct> vec_wpn;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// APT/NAV MAPS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
extern map <string,xint> apt_icao_to_index;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// FAIL
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
struct fail_struct						// this is within xios_class so zeroed in the xios_class constructor
{
	xint f,g1,g2				= 0 ;	// failure index for net transmission, group and sub-group, and UI spacing
	xint fail_ui,fail_rand		= 0 ;	// start_new_group, start_new_column, and can fail randomly from MTBF
	xint gone_present_failed	= 0 ;	// -1 means not present on the plane, 0 means present not broken, 1 means present and broken
	xchr fail_name[name_dim80]	={0};	// long name
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// RADAR
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
const xint radar_region_idim = 4*60;
const xint radar_region_jdim = 3*60;

extern xbyt storm_08[radar_region_idim][radar_region_jdim];

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// STRUCTS WE SEND TO THE SIM THAT WE ALSO USE TO STORE OUR OWN DATA!
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
struct net_tim_struct
{
public:
   ~net_tim_struct(){}
	net_tim_struct(){}

	xflt date_0_364	=180.0;
	xflt time_24hr	= 12.0;
};

struct net_wxr_struct
{
public:
   ~net_wxr_struct(){}
	net_wxr_struct(){set_CAVOK();}

	xflt regn_variation_pct	=0;
	xint regn_change_en		=0;	// regn_rapidly_improving->regn_down_real_weather

	xflt apt_ele_m			=0;

	xflt vis_sm				=0;
	xflt ISA_offset_C		=0;
	xflt SL_press_baro		=0;
	xflt precip_rat			=0;

	xint cld_typ_enum	[3]	={0};	// this is what comes from con pad
	xint cld_cov_enum	[3]	={0};
	xflt base_msl_ft	[3]	={0};
	xflt tops_msl_ft	[3]	={0};

	xflt wind_alt_msl_ft[3]	={0};	// this is what comes from con pad
	xflt wind_spd_kt	[3]	={0};
	xflt wind_dir_true	[3]	={0};
	xflt wind_inc_kt	[3]	={0};
	xflt wind_shr_deg	[3]	={0};
	xflt CAT_rat		[3]	={0};

	xflt therm_vvi_fpm		=0;	// climb in m/s
	xflt wave_hgt_ft		=0;	// waves
	xflt wave_dir_deg		=0;	// waves
	xint ter_eff_net		=0;	// starting cons

	inline void set_CAVOK()
	{
		memset(this,0,sizeof(*this));

		regn_variation_pct	= 0.0				;
		regn_change_en		= regn_static_wxr	;
		vis_sm				= 20				;
		SL_press_baro		= 29.92				;

		base_msl_ft[0]		= 5000;
		tops_msl_ft[0]		= 1000;
		base_msl_ft[1]		=15000;
		tops_msl_ft[1]		=20000;
		base_msl_ft[2]		=25000;
		tops_msl_ft[2]		=30000;

		wind_alt_msl_ft[0]	= 4000;
		wind_alt_msl_ft[1]	= 8000;
		wind_alt_msl_ft[2]	=16000;
	}
};

struct PREL_struct	// LOAD UP HERE AND FLY, used between copies of X-Plane
{
public:
	PREL_struct(){memset(this,0,sizeof(*this));}
   ~PREL_struct(){}

	xint type_start	;
	xint p_idx		;
	xchr apt_id[8]	;
	xint apt_rwy_idx;
	xint apt_rwy_dir;
	xdob dob_lat_deg;
	xdob dob_lon_deg;
	xdob dob_ele_mtr;
	xdob dob_psi_tru;
	xdob dob_spd_msc;
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// CRAFT AND PATHS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
const xint path_dim = 12*60*60;	// i'll set it for a 12-hour test

class path_point_class
{
public:
   ~path_point_class(){}
	path_point_class(){}

	xflt path_lat		=0.0;
	xflt path_lon		=0.0;
	xflt path_ele		=0.0;
	xflt path_flap_req	=0.0;
	xflt path_sbrk_req	=0.0;
	xint path_gear_req	=0.0;
};

class path_class
{
public:
   ~path_class(){}
 	path_class(){}

	vector <path_point_class> path_points;

	inline void path_per_reset()
	{
		path_points.clear();
	}

	inline void path_per_sec(	const xflt in_lat,
								const xflt in_lon,
								const xflt in_ele,
								const xflt in_flap_req,
								const xflt in_sbrk_req,
								const xflt in_gear_req)
	{
		if(						path_points.size()>0)
		if(fltabs(path_points[	path_points.size()-1].path_lat - in_lat)<0.0025)
		if(fltabs(path_points[	path_points.size()-1].path_lon - in_lon)<0.0025)
			return;

		path_point_class	path_point;
							path_point.path_lat		= in_lat		;
							path_point.path_lon		= in_lon		;
							path_point.path_ele		= in_ele		;
							path_point.path_flap_req= in_flap_req	;
							path_point.path_sbrk_req= in_sbrk_req	;
							path_point.path_gear_req= in_gear_req	;

			path_points.push_back	(path_point);
		if(	path_points.size 		() >= path_dim)
			path_points.erase		(
			path_points.begin		(),
			path_points.begin		()+60);
	}
};

struct acft_PX_struct	// stuff kept per-airplane, whether the pro-use all the planes on one 1 PC, or lab-use just P0 per-machine
{
   ~acft_PX_struct(){}
	acft_PX_struct(){
		memset(this,0,len_to_clear());
		acft_time_map_move = -999.9;
		acft_time_net_recv = -999.9;}

	static inline xint len_to_net	(){return offsetof(acft_PX_struct,acft_addy);}	// THIS KEEPS ALL THE LONG-V-INT, SIGNED-V-UNSIGNED ETC ALL CLEAR AND CORRECT. DONE BY COMPILER.
	static inline xint len_to_clear	(){return offsetof(acft_PX_struct,acft_addy);}	// THIS KEEPS ALL THE LONG-V-INT, SIGNED-V-UNSIGNED ETC ALL CLEAR AND CORRECT. DONE BY COMPILER.
	static inline xint len_to_disk	(){return offsetof(acft_PX_struct,acft_addy);}	// THIS KEEPS ALL THE LONG-V-INT, SIGNED-V-UNSIGNED ETC ALL CLEAR AND CORRECT. DONE BY COMPILER.

	xint p_index_sim	;
	xint livery_dim		;
	xint livery_now		;
	xint ai_flies_now	;
	xint team_now		;
	xint skill_now		;

	xchr acfpath[160]	;
	xchr tailnum[  8]	;
	
	xflt lon_deg,lat_deg,ele_mtr,ele_agl_mtr;
	xflt phi_deg,the_deg,psi_deg_true,psi_deg_mag;
	xflt slip_deg,AV_KEAS,vvi_fpm;
	xflt flap_req,sbrk_req,gear_req;

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

	xint wpn_num_stations;
	xchr wpn_chr_now[wpn_station_dim][name_dim64];

	IPAddress	acft_addy			;	// track what machine this is coming from
	string		acft_name			;	// this is the BEACON name we are hearing from!
	xdob		acft_time_map_move	;	// last time moved on our map to reject net for a second
	xdob		acft_time_net_recv	;	// last time heard from in x-plane, so dis-continued planes drop out

	path_class	acft_path			;	// FLIGHT-path for this plane, be sure to not step on it on the net rcv

	inline	xflt min_wgt_lb_enter(){return min_wgt_lb_book*0.9;}
	inline	xflt max_wgt_lb_enter(){return max_wgt_lb_book*1.2;}
	inline	xflt ter_ele_ft(){return (ele_mtr-ele_agl_mtr)*mtrstoft;}
			void plot_asi_alt_tape(const xflt x_tic,const xflt y_cen,const xflt y_siz,const xflt val,const xint is_asi);
			void plot_PFD(const xflt x1,const xflt y1,const xflt x2,const xflt y2);
};

struct sim_con_struct	// stuff kept only for p0, no matter the type of layout. this is for p0 only, even in a lab setup.
{						// this is within xios_class so zeroed in the xios_class constructor
   ~sim_con_struct(){}
	sim_con_struct(){memset(this,0,sizeof(*this));}

	xint num_acf_this_pc;

	xflt Vso_KIAS		;	// p0 only on theses map displays
	xflt  Vs_KIAS		;	// p0 only on theses map displays
	xflt Vfe_KIAS_full	;	// p0 only on theses map displays
	xflt Vno_KIAS		;	// p0 only on theses map displays
	xflt Vne_KIAS		;	// p0 only on theses map displays

	xint nav_frq_int	;	// p0 only on theses map displays
	xint nav_ntf_int	;	// p0 only on theses map displays
	xint nav_gls_int	;	// p0 only on theses map displays

	xflt nav_OBS_deg	;	// p0 only on theses map displays
	xflt nav_CDI_lat	;	// p0 only on theses map displays
	xflt nav_CDI_vrt	;	// p0 only on theses map displays

	xflt wxr_lon1		;	// p0 only on theses map displays
	xflt wxr_lat1		;	// p0 only on theses map displays
	xflt wxr_lon2		;	// p0 only on theses map displays
	xflt wxr_lat2		;	// p0 only on theses map displays
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// IOS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class xios_class
{
public:
   ~xios_class(){}
	xios_class()
	{
		intptr_t add1=(intptr_t)( this				);
		intptr_t add2=(intptr_t)(&end_data_to_zero	);
		memset(this,0,add2-add1);

		nav_menu_up			=  -1	;
		real_time_mod_wgt	=-999.9	;

		tim.date_0_364		= 180	;	// wxr must be set here not in its own constructor to avoid being over-written by the memset above
		tim.time_24hr		=  12	;	// wxr must be set here not in its own constructor to avoid being over-written by the memset above
		wxr.set_CAVOK()				;	// wxr must be set here not in its own constructor to avoid being over-written by the memset above

		map_set_KIAS		= 250.0	;
		map_set_alt			=5000.0	;
	};

	#if SUPPORT_1208
		xint RUNNING_1208		;		// RUNNING 1208... DELETE WHEN I WANT TO DROP 1208
	#endif

	xint acf_edit				;
	xint prefs_save_armed		;
	xint nav_menu_up			;

	xflt real_time_mod_wgt		;

	xint vis_meters				;
	xint pres_hectopas			;
	xint weight_kg				;
	xflt map_set_KIAS			;
	xflt map_set_alt			;

	xflt ai_rat_load_0_100		;
	xflt ai_rat_fail_0_100		;
	xflt ai_rat_wexr_0_100		;
	xflt ai_rat_rand_0_100		;

	xchr cmd_search[ str_dim	];
	xchr apt_search[name_dim64	];
	xchr apt_ref_id[  id_dim8	];

	xchr group2_name[group_dim][group_dim][name_dim80];
	xchr group1_name[group_dim]			  [name_dim80];

	net_tim_struct tim			;
	net_wxr_struct wxr			;
	sim_con_struct con			;

	xint end_data_to_zero;

	vector <acft_PX_struct>	acft_p_var;	// connect to any number of machines, this is p0 on each machine, and we do track flight path
	inline xint				acft_p_size(				){return (xint)acft_p_var.size();}
	inline acft_PX_struct*	acft_p		(const xint p	)
	{
		if(p>=acft_p_size())
		{
			APP_alert("Asking for aircraft that we don't have.","","","");
			return NULL;
		}

		return &acft_p_var[p];
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SYSTEM FAILURES, STORED FOR P0 ONLY
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// i only store one of these, because having a diff set of failures per-airplane seems over-kill
// so if we apply the failure it goes to every PC on the list
// that means it goes to p0 on the pro-use case, and all p0 in the lab-use case
// that is almost certainly what people want
//
// if I ever change my mind about that, then i need to:
//		TCP-IP BLAST all the failures on SIM start, since it is useless to try to save these in prefs because we would save by p-index, which will be different the next time we run the App!
//		UDP the CUSTOM failures ever frame if a new plane with custom failures is loaded
//		store the total failure struct per-plane and make sure it is scanned on the incoming to the correct index, whether by TCP-IP blast or per-frame UDP!
//
// so if i ever want diff failures PER-ACF, be sure we do all the stuff above properly
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	vector <fail_struct> vec_fal;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// FAIL NAMES WE SHOW IN THE UI, WHICH CAN BE SET IN PREFS TO REMOVE UN-DESIRED FAILURES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// this can NOT live in the fail struct, because that data comes from SIM!
// so it must NOT be part fo the fail struct or it will get over-written on read
// it can NOT be a boolean per-index, because it will go out of sync when we add new failures in the middle of the list!
// so the only way to track this is what what the user really wants:
// no matter what comes in from the sim, no matter what index the failure becomes... do we want to show this failure name?
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	set <string> fail_names_we_show;

	xint xios_sits_size		(const xint num_acf	);
	void xios_sits_save		(const string path	);
	xint xios_sits_load		(const string path,const xint just_check_file);
	void xios_sits			();

	void xios_command_list	(const xint command_type);
	void xios_commands		();

	void xios_map_PFD		();
	void xios_map_plan		(const xint map_en);
	void xios_map_profile	();
	void xios_map_start		(const xint is_wxr);

	void xios_weather		();

	void xios_failures_avail();
	void xios_failures_sel	();
	void xios_failures		();

	void xios_weight		();
	void xios_start			();
	void xios_aircraft		();
	void xios_settings		();
	void xios_main			();

	void xios_acf_menus		(const xint allow_mod);
	void xios_per_frame		();

	void xios_send_xWGT		(const xint p);
	void xios_send_LWPN		(const xint p,const xint s,const string name);
	void xios_send_COMB		(const xint p);
	void xios_send_PREL		(const xint p,const PREL_struct PREL);
	void xios_inet_rcv		(const IPAddress from_ip,xchr* label_read,xchr* data_read,const xint len);
};
extern class xios_class xios;

extern vector	<string> acf_paths	;	// i use vector not set to keep it in alphabetical order
extern vector	<string> wpn_files	;	// i use vector not set to keep it in alphabetical order
extern set		<string> set_cmd	;
extern set		<string> set_cus	;
