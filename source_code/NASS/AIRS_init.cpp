#include "hl_types.h"

AIRS_class AIRS;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SCAN NAME-UPDATER
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
string label_to_airport(string s)
{
	if(s.size()>3)					// nuke the ' ##' at the end of the comments to get us down to the airport name at the end
	{								// we use this to see if we re really changing airports below...
		xint len=(xint)s.size();
		if(	s[len-3]==' ')
		if(	s[len-2]=='#')
		if(	s[len-1]=='#')
			s.erase(
			s.end()-3,
			s.end());
	}

	if(s.size()>7)					// if we have CHARLOTTE CLASS B AREA A, CHARLOTTE CLASS B AREA B, etc,
	{								// I only want one label and to consider it to be ONE airspace for labelling clarity and memory.
		xint len=(xint)s.size();	// they report a class-B like Charlotts as a buch of airpsaces ith different names
		if(	s[len-7]==' ')			// this clutters the labels big-time: i only want one label at the bottom like all the others!
		if(	s[len-6]=='A')			// so we go to 1 airpace here for a class-b: perfect for labelling and de-cluttering and ram
		if(	s[len-5]=='R')
		if(	s[len-4]=='E')
		if(	s[len-3]=='A')
		if(	s[len-2]==' ')
			s.erase(
			s.end()-7,
			s.end());
	}
	return s;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// GRAB AIRSPACE FROM INTERNAL FILE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// The file format is called "Open AIR" and downloads for many countries are available.
// It is free but not official or guaranteed to be current:
// 	http://soaringweb.org/Airspace/
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void AIRS_class::AIRS_init()
{
	if(airss.size())
		APP_alert("Multiple AIRS_init!","","","");

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	airs_class airs;
	part_class part;
	xmap_class m("RESOURCE/usa.txt");

	xflt lat ,lon =0;			// these are outside the loop to preserve their value from one scan to the next to set the min and max bounds based on the radius and rthe previous-scan lat and lon
	xflt lat1,lon1=0;
	xflt lat2,lon2=0;

	string	old_comment;
	string	new_comment;
	xint	this_is_new_airspace=0;
	xint	this_is_a_drop_zone	=0;

	while(m.can_scan(2))
	{
		xchr c_label[4]={0};	// there are 2-char and 3-char labels
			 c_label[0]=*m.crt();	if(m.TXT_eol()){m.TXT_run_to_EOL();	continue;}		m.inc_crt(1);
			 c_label[1]=*m.crt();	if(m.TXT_eol()){m.TXT_run_to_EOL();	continue;}		m.inc_crt(1);
			 c_label[2]=*m.crt();	if(m.TXT_eol()){m.TXT_run_to_EOL();	continue;}		m.inc_crt(1);	if(c_label[2]==' ')c_label[2]=0;	// go from 3-char to 2-char labels if it is a 2-char label
			 c_label[3]=0;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SEE IF WE HAVE A NEW AIRSPACE BY THE COMMENT AND LOCATION CHANGING.
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		if(strcmp(c_label,"*##")==0)
		{
			m.TXT_str_scan_eoln		(			 &new_comment);
												  new_comment=label_to_airport(
												  new_comment);
			this_is_new_airspace=	(old_comment!=new_comment);			// load all parts of the same name into the same airspace to save RAM and make each airspace a thing with multiple parts
									 old_comment =new_comment ;

			this_is_a_drop_zone=find_string(new_comment,"DROP ZONE");	// these use type Q, danger, for both MOAs and drop zones. we follow sectional chart convention and show the MOAs

			if(!this_is_new_airspace)
			{
				xflt this_apt_lat=lat	;	// airports are listed in alphabetical order, and there are many AURORA, LANCASTER, WILMINGTON, etc airports
				xflt this_apt_lon=lon	;	// so, when we get an airspace with an identical name in the comment, we have to scan ahead to see if it will be a in a different location!
				xflt next_apt_lat=0		;	// if it is then it is a new airspace... even though the name is the same!
				xflt next_apt_lon=0		;

				xbyt* m_start=m.crt();
				while(m.inc_crt(1))
				if(*(m.crt()-3)=='V')
				if(*(m.crt()-2)==' ')
				if(*(m.crt()-1)=='X')
				if(*(m.crt()  )=='='){
					m.TXT_lat_lon_deg_min_sec(&next_apt_lat,&next_apt_lon);
					break;}

				if(dis_mtr_with_dll(this_apt_lon,next_apt_lon,
									this_apt_lat,next_apt_lat)*mtrstonm>100.0)this_is_new_airspace=1;

				m.set_crt(m_start);
			}
		}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// TYPE AND NAME
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		else if(strcmp(c_label,"AC")==0)	// type
		{
				if(	part.geoms.size			()>0){
					part.part_prep			();	airs.parts.push_back(part);}	// if we have been working then push back the previous airspace
					part.part_reset			();									// then reset the new one for building!

			if(this_is_new_airspace)
			{
				if(	airs.show_this_airspace	() && !this_is_a_drop_zone){		// if we have been working on a valid airspace type to show
					airs.airs_prep			();	airss.push_back(airs);}			// then push back the previous airspace
					airs.airs_reset			();									// then reset the new one for building!

				xchr				c_airspace[10]={0}	;
				m.TXT_c_scan_eoln(	c_airspace,10)		;
				m.TXT_run_to_EOL();
													 airs.airs_type=UNKNOWN_TYPE		;
				if(strcmp(c_airspace,"R"		)==0)airs.airs_type=RESTRICTED			;
				if(strcmp(c_airspace,"Q"		)==0)airs.airs_type=DANGER				;
				if(strcmp(c_airspace,"P"		)==0)airs.airs_type=PROHIBITED			;
				if(strcmp(c_airspace,"A"		)==0)airs.airs_type=ALPHA				;
				if(strcmp(c_airspace,"B"		)==0)airs.airs_type=BRAVO				;
				if(strcmp(c_airspace,"C"		)==0)airs.airs_type=CHARLIE				;
				if(strcmp(c_airspace,"D"		)==0)airs.airs_type=DELTA				;
				if(strcmp(c_airspace,"GP"		)==0)airs.airs_type=GLIDER_PROHIBITED	;
				if(strcmp(c_airspace,"CTA"		)==0)airs.airs_type=CTA_CTR				;
				if(strcmp(c_airspace,"CTR"		)==0)airs.airs_type=CTA_CTR				;
				if(strcmp(c_airspace,"CTA/CTR"	)==0)airs.airs_type=CTA_CTR				;
				if(strcmp(c_airspace,"RMZ"		)==0)airs.airs_type=CTA_CTR				;
				if(strcmp(c_airspace,"TMZ"		)==0)airs.airs_type=XPDR_MANDATORY		;
				if(strcmp(c_airspace,"W"		)==0)airs.airs_type=WAVE_WINDOW			;
			}
		}
		else if(strcmp(c_label,"AN")==0)		// title
		{
			string				 name	;
			m.TXT_str_scan_eoln(&name)	;
								 name=label_to_airport(
								 name)	;

			string airs_name1=name;				// default if no spaces found
			string airs_name2="";				// default if no spaces found

				xint cmax=(xint)name.size();
			for(xint del= 0;del<cmax/2;del++ )	// break the line of text in 2 closest to the middle of the string
			for(xint dir=-1;dir<=    2;dir+=2)	// break the line of text in 2 closest to the middle of the string
			{
				xint c=intlim(cmax/2+del*dir,0,cmax-1);
				if(name[c]==' ')
				{
					airs_name1=name.substr(0  ,     c  );
					airs_name2=name.substr(c+1,cmax-c-1);
					del=999;					// break out of BOTH loops
					dir=999;					// break out of BOTH loops
					break;						// break out of BOTH loops
				}
			}

			C_eq_C(airs.airs_name1,airs_name1.c_str(),name_dim64);	// default if no spaces found
			C_eq_C(airs.airs_name2,airs_name2.c_str(),name_dim64);	// default if no spaces found
		}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ALT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		else if(strcmp(c_label,"AL")==0)		// low alt
		{
			 		  part.part_alt_lo_ft=m.TXT_alt_scan(
					 &part.part_alt_lo_is_MSL	);
			dev_alert_nan(part.part_alt_lo_ft		);
			dev_alert_nan(part.part_alt_lo_is_MSL	);
		}
		else if(strcmp(c_label,"AH")==0)		// hi alt
		{
					  part.part_alt_hi_ft=m.TXT_alt_scan(
					 &part.part_alt_hi_is_MSL	);
			dev_alert_nan(part.part_alt_hi_ft		);
			dev_alert_nan(part.part_alt_hi_is_MSL	);
		}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// GEO: POLYGON POINTS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// DP is a polygon point. Some airspace definitions close the polygon, some don't.
// So if the coordinates don't match, you need to add an edge from coordinate n back to coordinate 1.
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		else if(strcmp(c_label,"DP")==0)			// POLYGON POINT
		{
			m.TXT_lat_lon_deg_min_sec(&lat,&lon);
			m.TXT_run_to_EOL();
			part.push_lat(geom_point_lat,lat);
			part.push_lon(geom_point_lon,lon);
		}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// GEO CENTER AND RADIUS OF A CIRCLE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		else if(strcmp(c_label,"V X")==0)			// CENTER OF CIRCLE OR ARC SEGMENT
		{
			m.TXT_lat_lon_deg_min_sec(&lat,&lon);
			m.TXT_run_to_EOL();
			part.push_lat(geom_center_lat,lat);
			part.push_lon(geom_center_lon,lon);
		}
		else if(strcmp(c_label,"DC")==0)			// RADIUS OF CIRCLE
		{
			part.push_rad(geom_circle_rad,	m.TXT_flt_scan	());
											m.TXT_run_to_EOL() ;
		}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ARC SEGMENT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// V X=center
// V D=+ or - direction
// DB coordinates1, coordinates2
// The center point X and the direction D, where + means a right turn and - means left turn.
// Then, DB means an arc around center point X from coordinate 1 to coordinate 2, wound in the direction D.
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		else if(strcmp(c_label,"V D")==0)			// ARC SEGMENT DIRECTION (FROM THE CIRCLE CENTER LISTED ABOVE)
		{
			xchr				dir_type[3]={0}	;
			m.TXT_c_scan_eoln(	dir_type,3)		;
			if(strcmp(			dir_type,"=+")==0)part.push_dir(geom_arc_dir, 1);	// right_turn == 1
			if(strcmp(			dir_type,"=-")==0)part.push_dir(geom_arc_dir,-1);	// right_turn == 0
		}
		else if(strcmp(c_label,"DB")==0)			// ARC SEGMENT ENPOINT LOCATIONS (FROM THE CIRCLE CENTER LISTED ABOVE)
		{
			m.TXT_lat_lon_deg_min_sec(&lat1,&lon1);
			m.TXT_lat_lon_deg_min_sec(&lat2,&lon2);
			m.TXT_run_to_EOL();

			part.push_lat(geom_arc_lat1,lat1);
			part.push_lon(geom_arc_lon1,lon1);
			part.push_lat(geom_arc_lat2,lat2);
			part.push_lon(geom_arc_lon2,lon2);
		}
		else m.TXT_run_to_EOL();
	}
}
