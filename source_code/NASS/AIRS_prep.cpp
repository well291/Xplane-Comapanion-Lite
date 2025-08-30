#include "hl_types.h"

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// FIND THE PLACE TO PUT THE LABELS FOR EACH PART
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// this is label per part (sits in the part, altitude for class B C D, and names for restricted warning)
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void part_class::part_prep()
{
	xflt sum_geo_lat=0.0;
	xflt sum_geo_lon=0.0;		xint num_geo=0;

	xflt ctr_lat	=0.0;
	xflt ctr_lon	=0.0;
	xflt crc_rad	=0.0;

	vector <xflt> arc_dir ;
	vector <xflt> arc_lat1;
	vector <xflt> arc_lon1;
	vector <xflt> arc_lat2;
	vector <xflt> arc_lon2;

	for(xint g=0;g<geoms.size();g++)
	{
		if(geoms[g].geom_typ==geom_point_lat	){sum_geo_lat+=geoms[g].geom_val;				}
		if(geoms[g].geom_typ==geom_point_lon	){sum_geo_lon+=geoms[g].geom_val;	num_geo+=1;	}

		if(geoms[g].geom_typ==geom_center_lat	)ctr_lat =geoms[g].geom_val;
		if(geoms[g].geom_typ==geom_center_lon	)ctr_lon =geoms[g].geom_val;

		if(geoms[g].geom_typ==geom_arc_dir		)arc_dir .push_back(geoms[g].geom_val);
		if(geoms[g].geom_typ==geom_arc_lat1		)arc_lat1.push_back(geoms[g].geom_val);
		if(geoms[g].geom_typ==geom_arc_lon1		)arc_lon1.push_back(geoms[g].geom_val);
		if(geoms[g].geom_typ==geom_arc_lat2		)arc_lat2.push_back(geoms[g].geom_val);
		if(geoms[g].geom_typ==geom_arc_lon2		)arc_lon2.push_back(geoms[g].geom_val);

		if(geoms[g].geom_typ==geom_circle_rad	)crc_rad=geoms[g].geom_val;
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// JUST A CIRCLE, INCLUDING NESTED CIRCLES LIKE FOR CLASS-C
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	if(crc_rad>0.0)
	{
		part_label_lat		=ctr_lat-(crc_rad/60.0);
		part_label_lon		=ctr_lon;
		part_label_psi_rad	=0.0;	// bottom of circle, moved up by text width
		part_label_dis_rat	=1.0;	// nominal scoot-up to clear text
	}
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// IF ANY ARCS ARE AVAIL, TAKE THE ONE THAT IS FARTHEST OUT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	else if(arc_dir.size()>0)		// 1 arc for the control zones!
	{
		xflt largest_arc_rad=0.0;

		for(xint a=0;a<arc_dir.size();a++)
		if(a<arc_lat1.size())
		if(a<arc_lon1.size())
		if(a<arc_lat2.size())
		if(a<arc_lon2.size())
		{
			xflt ang1_deg=crs_deg_with_dll(arc_lon1[a],ctr_lon , arc_lat1[a],ctr_lat);
			xflt ang2_deg=crs_deg_with_dll(arc_lon2[a],ctr_lon , arc_lat2[a],ctr_lat);
			xflt rad_mtr =dis_mtr_with_dll(arc_lon1[a],ctr_lon , arc_lat1[a],ctr_lat);

			if(rad_mtr>largest_arc_rad)
			{
				largest_arc_rad=rad_mtr;

				xflt rad_lat=rad_mtr*mtrstonm/(60.0							);
				xflt rad_lon=rad_mtr*mtrstonm/(60.0*cos(ctr_lat*degtorad)	);

				xflt				del_ang=0;
				if(arc_dir[a]>0.0)	del_ang=fltwrap(ang2_deg-ang1_deg,   0.0,360.0);
				if(arc_dir[a]<0.0)	del_ang=fltwrap(ang2_deg-ang1_deg,-360.0,  0.0);

				xflt									 ang_deg=ang1_deg+del_ang*0.5	;
				part_label_lat		=ctr_lat+rad_lat*cos(ang_deg		*degtorad)		;
				part_label_lon		=ctr_lon+rad_lon*sin(ang_deg		*degtorad)		;
				part_label_psi_rad	=					(ang_deg+180)	*degtorad		;
				part_label_dis_rat	=1.0;
			}
		}
	}
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// OTHERWISE JUST GO FOR THE CENTROID OF ALL POINTS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	else if(num_geo>0)
	{
		part_label_lat		=sum_geo_lat/(xflt)num_geo;
		part_label_lon		=sum_geo_lon/(xflt)num_geo;
		part_label_psi_rad	=0.0;	// we just plot right in the middle in this case
		part_label_dis_rat	=0.0;	// we just plot right in the middle in this case
	}
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// FIND THE PLACE TO PUT THE LABELS FOR EACH AIRSPACE (AND AIRSPACE DIMS FOR CULLING)
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// this is label per airspace (sits below airspace, like for class B C D)
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void airs_class::airs_prep()
{
	airs_lat_lo=180.0;	airs_lat_hi=-180.0;
	airs_lon_lo=180.0;	airs_lon_hi=-180.0;

	for(xint p=0;p<parts.size();p++)
	{
		xflt latc=0;
		xflt lonc=0;
//		xflt dir =0;
		xflt lat1=0;
		xflt lon1=0;
		xflt lat2=0;
		xflt lon2=0;

		for(xint g=0;g<parts[p].geoms.size();g++)
		{
			if(parts[p].geoms[g].geom_typ==geom_point_lat	){airs_lat_lo=fltmin2(airs_lat_lo,parts[p].geoms[g].geom_val);	airs_lat_hi=fltmax2(airs_lat_hi,parts[p].geoms[g].geom_val);}
			if(parts[p].geoms[g].geom_typ==geom_point_lon	){airs_lon_lo=fltmin2(airs_lon_lo,parts[p].geoms[g].geom_val);	airs_lon_hi=fltmax2(airs_lon_hi,parts[p].geoms[g].geom_val);}

			if(parts[p].geoms[g].geom_typ==geom_center_lat	)latc=parts[p].geoms[g].geom_val;
			if(parts[p].geoms[g].geom_typ==geom_center_lon	)lonc=parts[p].geoms[g].geom_val;
//			if(parts[p].geoms[g].geom_typ==geom_arc_dir		)dir =parts[p].geoms[g].geom_val;
			if(parts[p].geoms[g].geom_typ==geom_arc_lat1	)lat1=parts[p].geoms[g].geom_val;
			if(parts[p].geoms[g].geom_typ==geom_arc_lon1	)lon1=parts[p].geoms[g].geom_val;
			if(parts[p].geoms[g].geom_typ==geom_arc_lat2	)lat2=parts[p].geoms[g].geom_val;
			if(parts[p].geoms[g].geom_typ==geom_arc_lon2	)lon2=parts[p].geoms[g].geom_val;

			if(parts[p].geoms[g].geom_typ==geom_arc_lon2	)
			{
				const xflt rad1_mtr	=dis_mtr_with_dll(lon1,lonc , lat1,latc);
				const xflt rad2_mtr	=dis_mtr_with_dll(lon2,lonc , lat2,latc);
				const xflt rad_lat	=fltmax2(rad1_mtr,rad2_mtr)*mtrstonm/(60.0						);
				const xflt rad_lon	=fltmax2(rad1_mtr,rad2_mtr)*mtrstonm/(60.0*cos(latc*degtorad)	);

				airs_lat_lo=fltmin2(airs_lat_lo,latc-rad_lat);	// use center and radius for the bounds here, not the actual arc endpoints,
				airs_lat_hi=fltmax2(airs_lat_hi,latc+rad_lat);	// because the arc endpoints might be close together with the arc going the long way around in a circle!
				airs_lon_lo=fltmin2(airs_lon_lo,lonc-rad_lon);
				airs_lon_hi=fltmax2(airs_lon_hi,lonc+rad_lon);
			}

			if(parts[p].geoms[g].geom_typ==geom_circle_rad	)
			{
				const xflt rad_lat=parts[p].geoms[g].geom_val/(60.0						);
				const xflt rad_lon=parts[p].geoms[g].geom_val/(60.0*cos(latc*degtorad)	);

				airs_lat_lo=fltmin2(airs_lat_lo,latc-rad_lat);
				airs_lat_hi=fltmax2(airs_lat_hi,latc+rad_lat);
				airs_lon_lo=fltmin2(airs_lon_lo,lonc-rad_lon);
				airs_lon_hi=fltmax2(airs_lon_hi,lonc+rad_lon);
			}
		}
	}

	airs_label_lon=(airs_lon_lo+airs_lon_hi)*0.5;
	airs_label_lat= airs_lat_lo					;
}

