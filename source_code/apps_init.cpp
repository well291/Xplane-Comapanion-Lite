//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// VARIABLES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
vector <fix_struct> vec_fix;
vector <nav_struct> vec_nav;
vector <awy_struct> vec_awy;
vector <hol_struct> vec_hol;
vector <apt_struct> vec_apt;
vector <wpn_struct> vec_wpn;

xbyt storm_08[radar_region_idim][radar_region_jdim] = {0};

map <string,xint> apt_icao_to_index;

xios_class xios;
apps_class apps;

string file_path_prfs;
string file_path_sits;
string alert_message1;
string alert_message2;
string alert_message3;
string alert_message4;

vector	<string> acf_paths	;	// i use vector not set to keep it in alphabetical order
vector	<string> wpn_files	;	// i use vector not set to keep it in alphabetical order
set		<string> set_cmd	;
set		<string> set_cus	;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
/*
Now with USA airspace on the map.
All triggered failures shown in fail-screen to see what's failed at-a-glance.




on releases:
GET LATEST APT NAV FILES IN THERE WITH HOLDING PATTERNS..
WE ALWAYS SHIP WITH THE DEFAULT DATA IN CASE TCP IP BREAKS

*/

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void apps_class::apps_save()
{
	pref.prefs_txt(IO_write);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void apps_class::apps_init(void* ref,xint var)
{
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// INIT VARS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	cout.precision(4);	// allow this many numbers until we go into sci note, in theory
	xmap_class::set_granularity((xint)sysconf(_SC_PAGESIZE));

	gAppDelegateRef=ref;
	dev_alert_OGL("apps_init 1");	// be sure we inited OpenGl OK

	for(xint c=0;c<randim2;c++)
		rand_rat[c]=fltrand(0.0f,0.9999f);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// TEXTURES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	tman.load_pngX(t_font_1				,"Xavion-font-1.png"		);
	tman.load_pngX(t_font_2				,"Xavion-font-2.png"		);
	tman.load_pngX(t_font_3				,"Xavion-font-3.png"		);
	tman.load_pngX(t_font_4				,"Xavion-font-4.png"		);
	tman.load_pngX(t_font_1_shad		,"Xavion-font-1-shad.png"	);
	tman.load_pngX(t_font_3_shad		,"Xavion-font-3-shad.png"	);

	tman.load_pngX(t_white				,"white.png"				);

	tman.load_pngX(t_map_nexrad			,"map-nexrad.png"			);
	tman.load_pngX(t_map_icons			,"map-icons.png"			);

	tman.load_pngX(t_sky				,"PFD-sky.png"				);
	tman.load_pngX(t_no_shadow			,"PFD-no_shadow.png"		);
	tman.load_pngX(t_pfd				,"PFD-PFD.png"				);
	tman.load_pngX(t_rose_pfd			,"PFD-rose-PFD.png"			);
	tman.load_pngX(t_layers_short_wide	,"PFD-short_wide.png"		);
	tman.load_pngX(t_layers_tall_medium	,"PFD-tall_medium.png"		);
	tman.load_pngX(t_layers_tall_narrow	,"PFD-tall_narrow.png"		);
	tman.load_pngX(t_layers_tall_wide	,"PFD-tall_wide.png"		);
	tman.load_pngX(t_tickmarks			,"PFD-tickmarks.png"		);

	tman.load_pngX(t_earth_full_4k1		,"earth_full_4k1.png"		);	// two loads on these tex: once as PNG to load up the syn-vis colors for the hi-res terrain-plotter
	tman.load_pngX(t_earth_full_4k2		,"earth_full_4k2.png"		);	// once as PVR to save RAM! (and we save a LOT!)
	tman.load_pvrX(t_earth_full_4k1		,"earth_full_4k1.pvr"		);	// as need the hi res data with the rgb colors on it to look.... WAY BETTER onm the 2d map and 3d syn-vis
	tman.load_pvrX(t_earth_full_4k2		,"earth_full_4k2.pvr"		);	// it lets us shade for normal vectors at the DEM scale not global PVR scale, which is WAY better-looking

	tman.load_pngX(t_synvis_terrain		,"synvis-terrain.png"		);
	tman.load_pngX(t_synvis_runway		,"synvis-runway.png"		);

	tman.load_pngX(t_button_main		,"button-main.png"			);
	tman.load_pngX(t_button_slide		,"button-slide.png"			);
	tman.load_pngX(t_button_cir			,"button-cir.png"			);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// WAVS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	soun.fill_sound(soun_click			,"soun-click.wav"			);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// DATA
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	AIRS.AIRS_init();

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// NO-PREFS DEFAULTS, THEN PREFS, THEN CLOUD OVER-RIDE OF PREFS SINCE CLOUD ONLY UPDATES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	pref.prefs_txt(IO_read);
	pref.prefs_bin(IO_read);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// EVENT LOOPS ARE NOW READY TO GO!
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	alert_message1.clear();	// clear out any alerts from over-scan attempts in pref-read
	alert_message2.clear();	// clear out any alerts from over-scan attempts in pref-read
	alert_message3.clear();	// clear out any alerts from over-scan attempts in pref-read
	alert_message4.clear();	// clear out any alerts from over-scan attempts in pref-read

	init_is_done=1;
}
