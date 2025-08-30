#include "hl_types.h"

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// EXTERN
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
extern void lonlat_to_xy(const xflt lon,const xflt lat,xflt*x,xflt*y);

extern xflt lon1;
extern xflt lon2;
extern xflt lat1;
extern xflt lat2;

const xflt border_pix = 2.0;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// LABELS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class label_class
{
	public:
	   ~label_class(){}
		label_class(const xflt x,const xflt y,const xflt dx,const xflt dy,const enum_col col,const xflt alpha,const string str1,const string str2,const string str3)
		{
			label_x		=x		;
			label_y		=y		;
			label_dx	=dx		;
			label_dy	=dy		;
			label_col	=col	;
			label_alpha	=alpha	;
			label_str1	=str1	;
			label_str2	=str2	;
			label_str3	=str3	;
		}

	xflt		label_x		;
	xflt		label_y		;
	xflt		label_dx	;
	xflt		label_dy	;
	enum_col	label_col	;
	xflt		label_alpha	;	// CLASS ABC AIRSPACE	CLASS ABC PART		RESTRICTED AND WARNING PART
	string		label_str1	;	// name 1				alt hi				name 1
	string		label_str2	;	// name 2				alt lo				name 2
	string		label_str3	;	// 											alt lo to alt hi
};

vector <label_class> labels;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PLOT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void airs_class::airs_plot_2d(const xflt x1_window,const xflt y1_window,const xflt x2_window,const xflt y2_window,const xflt NAS_alpha_space,const xflt NAS_alpha_label)
{
	enum_col				  geo_col=col_white			;	enum_col	txt_col=col_white		;
	if(airs_type==RESTRICTED){geo_col=col_NAS_blue		;				txt_col=col_NAS_blue	;}
	if(airs_type==DANGER	){geo_col=col_NAS_magenta	;				txt_col=col_NAS_magenta	;}	// the text has to be briter to look good
	if(airs_type==PROHIBITED){geo_col=col_NAS_blue		;				txt_col=col_NAS_blue	;}
	if(airs_type==ALPHA		){geo_col=col_NAS_blue		;				txt_col=col_NAS_blue	;}
	if(airs_type==BRAVO		){geo_col=col_NAS_blue		;				txt_col=col_NAS_blue	;}
	if(airs_type==CHARLIE	){geo_col=col_NAS_magenta	;				txt_col=col_NAS_magenta	;}	// the text has to be briter to look good
	if(airs_type==DELTA		){geo_col=col_NAS_magenta	;				txt_col=col_NAS_magenta	;}	// the text has to be briter to look good

	const xflt geo_fcol=colr.colr_enum_to_flt(geo_col,NAS_alpha_space);

	xint set_vrt_typ=geom_none;

	xflt set_vrt_lat =0.0;
	xflt set_vrt_lon =0.0;

	xflt set_ctr_lat =0.0;
	xflt set_ctr_lon =0.0;
	xflt set_rad_lat =0.0;	// radius in degrees latitude
	xflt set_rad_lon =0.0;	// radius in degrees longitude

	xflt set_arc_dir =0.0;
	xflt set_arc_lat1=0.0;
	xflt set_arc_lon1=0.0;
	xflt set_arc_lat2=0.0;
	xflt set_arc_lon2=0.0;

	const	xflt mag_ang_step_deg	= 3.0;
			xflt x_pix				= 0.0;
			xflt y_pix				= 0.0;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// AIRSPACE GEO
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	for(xint p=0;p<parts.size();p++)
	{
		batch.init_batch(t_white , col_vert_true,dep_read_fals,dep_writ_fals);

		xflt part_pix_x_lo=x2_window;	// start opp and work in
		xflt part_pix_y_lo=y2_window;	// start opp and work in
		xflt part_pix_x_hi=x1_window;	// start opp and work in
		xflt part_pix_y_hi=y1_window;	// start opp and work in

		for(xint g=0;g<parts[p].geoms.size();g++)
		{
			if(parts[p].geoms[g].geom_typ==geom_point_lat	){set_vrt_typ=geom_point_lat	;	set_vrt_lat =parts[p].geoms[g].geom_val;}
			if(parts[p].geoms[g].geom_typ==geom_point_lon	){set_vrt_typ=geom_point_lon	;	set_vrt_lon =parts[p].geoms[g].geom_val;}

			if(parts[p].geoms[g].geom_typ==geom_center_lat	){set_vrt_typ=geom_center_lat	;	set_ctr_lat =parts[p].geoms[g].geom_val;}
			if(parts[p].geoms[g].geom_typ==geom_center_lon	){set_vrt_typ=geom_center_lon	;	set_ctr_lon =parts[p].geoms[g].geom_val;}
			if(parts[p].geoms[g].geom_typ==geom_circle_rad	){set_vrt_typ=geom_circle_rad	;	set_rad_lat =parts[p].geoms[g].geom_val/(60.0							);	// rad in lat
																								set_rad_lon =parts[p].geoms[g].geom_val/(60.0*cos(set_ctr_lat*degtorad)	);}	// rad in lon

			if(parts[p].geoms[g].geom_typ==geom_arc_dir		){set_vrt_typ=geom_arc_dir		;	set_arc_dir =parts[p].geoms[g].geom_val;}									// step in deg
			if(parts[p].geoms[g].geom_typ==geom_arc_lat1	){set_vrt_typ=geom_arc_lat1		;	set_arc_lat1=parts[p].geoms[g].geom_val;}
			if(parts[p].geoms[g].geom_typ==geom_arc_lon1	){set_vrt_typ=geom_arc_lon1		;	set_arc_lon1=parts[p].geoms[g].geom_val;}
			if(parts[p].geoms[g].geom_typ==geom_arc_lat2	){set_vrt_typ=geom_arc_lat2		;	set_arc_lat2=parts[p].geoms[g].geom_val;}
			if(parts[p].geoms[g].geom_typ==geom_arc_lon2	){set_vrt_typ=geom_arc_lon2		;	set_arc_lon2=parts[p].geoms[g].geom_val;}

			if(set_vrt_typ==geom_point_lon)		// any vertex
			{
				lonlat_to_xy(set_vrt_lon,set_vrt_lat,&x_pix,&y_pix);
				batch.add_line_2d_vertex(x_pix,y_pix,geo_fcol);

				part_pix_x_lo=fltmin2(part_pix_x_lo,x_pix);
				part_pix_y_lo=fltmin2(part_pix_y_lo,y_pix);
				part_pix_x_hi=fltmax2(part_pix_x_hi,x_pix);
				part_pix_y_hi=fltmax2(part_pix_y_hi,y_pix);
			}

			if(set_vrt_typ==geom_arc_lon2)		// arc about the center from point 1 to point 2 direction as indicated by arc dir
			{
				const xflt ang1		=crs_deg_with_dll(set_arc_lon1,set_ctr_lon , set_arc_lat1,set_ctr_lat);
				const xflt ang2		=crs_deg_with_dll(set_arc_lon2,set_ctr_lon , set_arc_lat2,set_ctr_lat);
				const xflt rad_mtr	=dis_mtr_with_dll(set_arc_lon1,set_ctr_lon , set_arc_lat1,set_ctr_lat);
				const xflt rad_lat	=rad_mtr*mtrstonm/(60.0								);
				const xflt rad_lon	=rad_mtr*mtrstonm/(60.0*cos(set_ctr_lat*degtorad)	);

				xflt				del_ang=0;
				if(set_arc_dir>0.0)	del_ang=fltwrap(ang2-ang1,   0.0,360.0);
				if(set_arc_dir<0.0)	del_ang=fltwrap(ang2-ang1,-360.0,  0.0);

				xint num_steps=intmax2(fltabs(del_ang/mag_ang_step_deg),2);

				for(xint step=0;step<=num_steps;step++)
				{
					xflt rat=(xflt)step/(xflt)num_steps;
					xflt ang=ang1+del_ang*rat;
					xflt lat=set_ctr_lat+rad_lat*cos(ang*degtorad);
					xflt lon=set_ctr_lon+rad_lon*sin(ang*degtorad);

					if(step==0)					// lock the endpoint to perfect value to avoid rounding error in the trig above
					{
						lat=set_arc_lat1;
						lon=set_arc_lon1;
					}
					else if(step==num_steps)	// lock the endpoint to perfect value to avoid rounding error in the trig above
					{
						lat=set_arc_lat2;
						lon=set_arc_lon2;
					}

					lonlat_to_xy(lon,lat,&x_pix,&y_pix);
					batch.add_line_2d_vertex(x_pix,y_pix,geo_fcol);

					part_pix_x_lo=fltmin2(part_pix_x_lo,x_pix);
					part_pix_y_lo=fltmin2(part_pix_y_lo,y_pix);
					part_pix_x_hi=fltmax2(part_pix_x_hi,x_pix);
					part_pix_y_hi=fltmax2(part_pix_y_hi,y_pix);
				}
			}

			if(set_vrt_typ==geom_circle_rad)	// radius like control zone
			for(xint ang=0;ang<=360;ang+=mag_ang_step_deg)
			{
				const xflt lat=set_ctr_lat+set_rad_lat*cos(ang*degtorad);
				const xflt lon=set_ctr_lon+set_rad_lon*sin(ang*degtorad);

				lonlat_to_xy(lon,lat,&x_pix,&y_pix);
				batch.add_line_2d_vertex(x_pix,y_pix,geo_fcol);

				part_pix_x_lo=fltmin2(part_pix_x_lo,x_pix);
				part_pix_y_lo=fltmin2(part_pix_y_lo,y_pix);
				part_pix_x_hi=fltmax2(part_pix_x_hi,x_pix);
				part_pix_y_hi=fltmax2(part_pix_y_hi,y_pix);
			}
		}

		batch.plot_batch();

/*
		OGL_framerect(col_cyan,1.0,
			part_pix_x_lo,
			part_pix_y_lo,
			part_pix_x_hi-part_pix_x_lo,
			part_pix_y_hi-part_pix_y_lo);
*/

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ALT AND PERHAPS LABELS PER PART
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		if(NAS_alpha_label>0.01)
		{
			lonlat_to_xy(	parts[p].part_label_lon,	// start with label in pix being at the lat and lon of the airspace label location
							parts[p].part_label_lat,&x_pix,&y_pix);

			if(title_sits_below_airspace())
			{
				x_pix+=font_2_dy*3.0*parts[p].part_label_dis_rat*sin(parts[p].part_label_psi_rad);	// now offset by the psi_rad and dis_rat as neede to best fit the airspace
				y_pix+=font_2_dy*1.5*parts[p].part_label_dis_rat*cos(parts[p].part_label_psi_rad);

				const string s1=parts[p].hi_alt();
				const string s2=parts[p].lo_alt();

				const xflt str_dx=fltmax2(	str_dx_pix(t_font_2,s1),
											str_dx_pix(t_font_2,s2))+border_pix*2.0;

				labels.push_back(label_class(x_pix,y_pix,str_dx,font_2_dy*2.0,txt_col,NAS_alpha_label,s1,s2,""));
			}
			else
			{
				x_pix+=font_2_dy*4.0*parts[p].part_label_dis_rat*sin(parts[p].part_label_psi_rad);	// now offset by the psi_rad and dis_rat as neede to best fit the airspace
				y_pix+=font_2_dy*2.0*parts[p].part_label_dis_rat*cos(parts[p].part_label_psi_rad);

				const string s1=airs_name1									;
				const string s2=airs_name2									;
				const string s3=parts[p].lo_alt()+" to "+parts[p].hi_alt()	;

				const xflt str_dx=fltmax3(	str_dx_pix(t_font_2,s1),
											str_dx_pix(t_font_2,s2),
											str_dx_pix(t_font_2,s3))+border_pix*2.0;

				const xflt x1_pix=x_pix-str_dx		*0.5;	// labels are centered, look at half label width each direction
				const xflt x2_pix=x_pix+str_dx		*0.5;	// labels are centered, look at half label width each direction
				const xflt y1_pix=y_pix-font_2_dy	-1.5;	// labels are 3 units tall... leave space 1.5 either direction
				const xflt y2_pix=y_pix+font_2_dy	+1.5;	// labels are 3 units tall... leave space 1.5 either direction

				for(xint l=0;l<	labels.size();l++)			// now move other labels that might be in the way
				if(pythag_sqr(	labels[l].label_x-x_pix,	// vertically-stacked restricted areas and MOAs need to show labels for all alts
								labels[l].label_y-y_pix)<sqr(font_2_dy*4.0))
				{
					labels[l].label_y	+=font_2_dy*2.0;		// scoot previous label up to center the group of labels, which are the various altitudes in a restricted area for example
					y_pix				-=font_2_dy*2.0;		// scoot this label down to center the group of labels, which are the various altitudes in a restricted area for example

					if(y1_pix<part_pix_y_lo)				// delete any label pushed out of the part
						labels.erase(labels.begin()+l);
				}

				if(x1_pix>part_pix_x_lo)					// only plot label that fits in the part
				if(x2_pix<part_pix_x_hi)
				if(y1_pix>part_pix_y_lo)
				if(y2_pix<part_pix_y_hi)
					labels.push_back(label_class(x_pix,y_pix,str_dx,font_2_dy*3.0,txt_col,NAS_alpha_label,s1,s2,s3));
			}
		}
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// AIRSPACE LABELS PER AIRSPACE THAT SIT BELOW THE AIRSPACE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(NAS_alpha_label>0.01)
	if(title_sits_below_airspace())
	{
		lonlat_to_xy(	airs_label_lon,
						airs_label_lat,&x_pix,&y_pix);

		y_pix-=border_pix;

		plot_string(t_font_2,	x_pix				,
								y_pix-font_2_dy*0.5	,
								airs_name1			,
								txt_col				,NAS_alpha_label,just_ctr,draw_normal);

		plot_string(t_font_2,	x_pix				,
								y_pix-font_2_dy*1.5	,
								airs_name2			,
								txt_col				,NAS_alpha_label,just_ctr,draw_normal);
	}
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PLOT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void AIRS_class::AIRS_plot_2d(const xflt x1_window,const xflt y1_window,const xflt x2_window,const xflt y2_window,const xflt NAS_alpha_space,const xflt NAS_alpha_label)
{
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// AIRSPACE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	glLineWidth(3);
	for(xint a=0;a<airss.size();a++)
	if(airss[a].airs_lon_hi>lon1)
	if(airss[a].airs_lon_lo<lon2)
	if(airss[a].airs_lat_hi>lat1)
	if(airss[a].airs_lat_lo<lat2)
	{
		#define	SHOW_AIRS_CULL_DIMS 0
		#if		SHOW_AIRS_CULL_DIMS
			xflt x_pix1,y_pix1;
			xflt x_pix2,y_pix2;
			lonlat_to_xy(airss[a].airs_lon_lo,airss[a].airs_lat_lo,&x_pix1,&y_pix1);
			lonlat_to_xy(airss[a].airs_lon_hi,airss[a].airs_lat_hi,&x_pix2,&y_pix2);
			OGL_framerect(col_cyan,0.25,x_pix1,y_pix1,x_pix2-x_pix1,y_pix2-y_pix1);
		#endif

		airss[a].airs_plot_2d(x1_window,y1_window,x2_window,y2_window,NAS_alpha_space,NAS_alpha_label);
	}
	glLineWidth(1);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// LABELS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	glLineWidth(2);
	batch.init_batch(t_white , col_vert_true,dep_read_fals,dep_writ_fals);	// altitude seperator lines

	for(xint l=0;l<labels.size();l++)										// BATCHED STRINGS WHEN BATCHED AIRSPACE DONE
	{
		OGL_fillrect(col_black,gray_rect_alpha*labels[l].label_alpha,		// THIS HAS TO BE REALLY SOLID IN CASE THESE NUMEBRS ARE SITTING ON TOP OF NEXRAD OF THE SAME COLOR!
			labels[l].label_x-	labels[l].label_dx*0.5	-border_pix		,
			labels[l].label_y-	labels[l].label_dy*0.5	-border_pix		,
								labels[l].label_dx		+border_pix*2	,
								labels[l].label_dy		+border_pix*2	);

		OGL_framerect(labels[l].label_col,labels[l].label_alpha,
			labels[l].label_x-	labels[l].label_dx*0.5	-border_pix		,
			labels[l].label_y-	labels[l].label_dy*0.5	-border_pix		,
								labels[l].label_dx		+border_pix*2	,
								labels[l].label_dy		+border_pix*2	);

		if(labels[l].label_str3=="")	// altitude label
		{
			plot_string(t_font_2,	labels[l].label_x				,
									labels[l].label_y+font_2_dy*0.5	,
									labels[l].label_str1			,
									labels[l].label_col				,labels[l].label_alpha,just_ctr,draw_buildup);

			plot_string(t_font_2,	labels[l].label_x				,
									labels[l].label_y-font_2_dy*0.5	,
									labels[l].label_str2			,
									labels[l].label_col				,labels[l].label_alpha,just_ctr,draw_buildup);
		}
		else							// normal label
		{
			plot_string(t_font_2,	labels[l].label_x				,
									labels[l].label_y+font_2_dy		,
									labels[l].label_str1			,
									labels[l].label_col				,labels[l].label_alpha,just_ctr,draw_buildup);

			plot_string(t_font_2,	labels[l].label_x				,
									labels[l].label_y				,
									labels[l].label_str2			,
									labels[l].label_col				,labels[l].label_alpha,just_ctr,draw_buildup);

			plot_string(t_font_2,	labels[l].label_x				,
									labels[l].label_y-font_2_dy		,
									labels[l].label_str3			,
									labels[l].label_col				,labels[l].label_alpha,just_ctr,draw_buildup);
		}
	}

	plot_string(t_font_2,0,0,"",col_white,1.0,just_ctr,draw_finish);	// all labels
	labels.clear();														// clear labels for RAM
	glLineWidth(1);
}
