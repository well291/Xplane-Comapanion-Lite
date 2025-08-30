#ifndef tman_h
#define tman_h

enum enum_tex
{
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// UI
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	t_font_1=0			,	// we have s/t plotting params for font texes only, indexed as the tex number, so start fonts at 0
	t_font_2			,	// we have s/t plotting params for font texes only, indexed as the tex number, so start fonts at 0
	t_font_3			,	// we have s/t plotting params for font texes only, indexed as the tex number, so start fonts at 0
	t_font_4			,	// we have s/t plotting params for font texes only, indexed as the tex number, so start fonts at 0
	t_font_1_shad		,
	t_font_3_shad		,
	t_font_x			,

	t_white				,

	t_map_nexrad		,
	t_map_icons			,

	t_sky				,
	t_no_shadow			,
	t_pfd				,
	t_rose_pfd			,
	t_layers_short_wide	,
	t_layers_tall_medium,
	t_layers_tall_narrow,
	t_layers_tall_wide	,
	t_tickmarks			,

	t_earth_full_4k1	,	// 4k is max size for pad pro in mid-2020
	t_earth_full_4k2	,	// 4k is max size for pad pro in mid-2020
	t_synvis_terrain	,
	t_synvis_runway		,

	t_button_main		,
	t_button_slide		,
	t_button_cir		,

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// INTERNAL
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	t_shadow	,
	t_fog		,
	tex_NULL	,
	tex_dim
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class tman_class
{
public:
   ~tman_class(){}
	tman_class(){memset(this,0,sizeof(*this));}

xflt font_s1[t_font_x][TEXT_DIM];
xflt font_t1[t_font_x][TEXT_DIM];
xflt font_s2[t_font_x][TEXT_DIM];
xflt font_t2[t_font_x][TEXT_DIM];

xflt tex_dx_vis[tex_dim];	// FLOAT FOR DIVISION TO GET S/T RATIOS
xflt tex_dy_vis[tex_dim];	// FLOAT FOR DIVISION TO GET S/T RATIOS
xflt tex_dx_tot[tex_dim];	// FLOAT FOR DIVISION TO GET S/T RATIOS
xflt tex_dy_tot[tex_dim];	// FLOAT FOR DIVISION TO GET S/T RATIOS

xflt map_xlo,map_xhi;		// this is in tman since it is loaded with the track map texture file
xflt map_zlo,map_zhi;		// simply keeps the track map texture file drawn in the right place in-cockpit, without getting involved with the track variables! perfect!

xint tex_loaded		(const enum_tex t);
void tex_unload		(const enum_tex t);
void load_scratch	(const enum_tex t,const string path,const GLenum filter_type,const xint dx_vis,const xint dy_vis,xint dx_tot,xint dy_tot,GLubyte* rgb_data,const xint is_pvr);
void load_pngX		(const enum_tex t,const string path);
void load_pvrX		(const enum_tex t,const string path);
};
extern class tman_class tman;

const xint planet_di=8192;
const xint planet_dj=4096;
extern GLubyte planet_array[planet_di][planet_dj][3];

#endif
