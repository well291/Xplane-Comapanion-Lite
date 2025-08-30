#ifndef xmap_h
#define xmap_h

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// QUICK UTILS IN THE CROSS-APP FOLDER SO EVERYONE CAN USE THEM
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
#include <errno.h>
#include <fcntl.h>

void udpate_file_path(string& res_path);

inline xint file_exists(string path)		// this is the fastest file-checker that ben can come up with
{
	udpate_file_path(path);					// the dems-loded needs this since it nmeeds this to work for RESOURCE/ files as well

	struct stat				  ss		;	// faster than x-map since it does not try to map anything
	return(stat(path.c_str(),&ss)>=0)	;	// it checks to see if the file is there
}

#include <sys/stat.h>
#include <sys/types.h>

inline off_t file_size(const char *filename)
{
    struct stat st;

    if(stat(filename,&st)==0)
        return st.st_size;

    return -1;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// ENUMS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
enum IO_type
{
	IO_read =0,
	IO_write=1
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// CLASS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class xmap_class	// memory-mapped file... it's fast!
{					// calling this with the name opens AND READS AND THEN CLOSES THE ENTIRE FILE!
public:
	xbyt* retry(string path			,long offset=0,long len=0);							// leaving the default zero opens the beginning of the file and allocates ram to go to the end of the file... a nice way to crash an ipad with a 1.5 gig dem file
	xmap_class (string path=string(),long offset=0,long len=0){retry(path,offset,len);}	// we use one giant file for dems. the first arg is how far into the file to open, the second arg is how much space to be ready to read.
   ~xmap_class ();

	string crt_path;

	static void set_granularity(long bytes);				// See CPP for this evil!

	inline xint	  exists	(		){return mBegin!=NULL		;}
	inline long	remaining	(		){return mEnd-mCrt			;}
	inline long		size	(		){return mEnd-mBegin		;}
	inline xbyt*   begin	(		){return mBegin				;}
	inline xbyt*	 crt	(		){return mCrt				;}
	inline xbyt*	 end	(		){return mEnd				;}

	inline xint in_file		(		){return mCrt  < mEnd		;}
	inline xint can_scan	(long c	){return mCrt+c<=mEnd		;}	// we can scan this much
	inline void set_bytes_in(long c	){		 mCrt   =mBegin+c	;}
	inline long ret_bytes_in(		){return mCrt-  mBegin		;}

	inline xbyt* get_loc	(xint c )	// get a byte any place in the file.
	{
		if(c< 0		)APP_alert("Trying to get a location before the start of a file!"	,"","","");
		if(c>=size())APP_alert("Trying to get a location past the end of a file!"		,"","","");
		return mBegin+c;
	}

	inline void set_crt(xbyt* c){mCrt=c;}	// int does go to 2 gig! LONG is different per compiler
	inline xint inc_crt(long  c)
	{
		if(	mCrt+ c>mEnd)	return xfals;
			mCrt+=c;		return xtrue;
	}

			xint check_file			(const xint req_version);
	inline	xint check_file_version	(const xint req_version)
	{
		if(mBegin==NULL)return xfals;	// no file? get out.
		if(mEnd==mBegin)return xfals;	// no size? get out.

		if(size()>3 && mBegin[0]==0xEF && mBegin[1]==0xBB && mBegin[2]==0xBF)mCrt+=3;

		const xchr scan_frst_chr=BIN_chr_scan();	TXT_run_to_EOL();	if(scan_frst_chr!='A' && scan_frst_chr!='I')return xfals;
		const xint scan_vers_int=TXT_int_scan();	TXT_run_to_EOL();

		return (scan_vers_int==req_version);
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// THERE MUST BE 50 WAYS TO SCAN YOUR BUFFERS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	inline xu64 BIN_u64_scan(						){if(mCrt+sizeof(xu64)	>mEnd){APP_alert("u64_scan past file end.",crt_path,"","");	return 0;}		xu64 retval=*((xu64*)mCrt);					mCrt+=sizeof(xu64);		return retval;}
	inline xdob BIN_dob_scan(						){if(mCrt+sizeof(xdob)	>mEnd){APP_alert("dob_scan past file end.",crt_path,"","");	return 0;}		xdob retval=*((xdob*)mCrt);					mCrt+=sizeof(xdob);		return retval;}
	inline xflt BIN_flt_scan(						){if(mCrt+sizeof(xflt)	>mEnd){APP_alert("flt_scan past file end.",crt_path,"","");	return 0;}		xflt retval=*((xflt*)mCrt);					mCrt+=sizeof(xflt);		return retval;}


	inline xint BIN_lon_scan()
	{
		if(mCrt+sizeof(xint)>mEnd)
		{
			APP_alert("lon_scan past file end.",crt_path,"","");	return 0;
		}
		xint retval=*((xint*)mCrt);
		mCrt+=sizeof(xint);
		return retval;

	}

	inline xint BIN_int_scan()
	{
		if(mCrt+sizeof(xint)>mEnd)
		{
			APP_alert("int_scan past file end.",crt_path,"","");
			return 0;
		}
		xint retval=*((xint*)mCrt);
		mCrt+=sizeof(xint);
		return retval;
	}


	inline xssh BIN_ssh_scan(						){if(mCrt+sizeof(xssh)	>mEnd){APP_alert("ssh_scan past file end.",crt_path,"","");	return 0;}		xssh retval=*((xssh*)mCrt);					mCrt+=sizeof(xssh);		return retval;}
	inline xchr BIN_chr_scan(						){if(mCrt+sizeof(xchr)	>mEnd){APP_alert("chr_scan past file end.",crt_path,"","");	return 0;}		xchr retval=*((xchr*)mCrt);					mCrt+=sizeof(xchr);		return retval;}
	inline xbyt BIN_byt_scan(						){if(mCrt+sizeof(xbyt)	>mEnd){APP_alert("byt_scan past file end.",crt_path,"","");	return 0;}		xbyt retval=*((xbyt*)mCrt);					mCrt+=sizeof(xbyt);		return retval;}
	inline xint BIN_buf_scan(xchr* io,const xint dim){if(mCrt+dim			>mEnd){APP_alert("buf_scan past file end.",crt_path,"","");	return 0;}		for(xint c=0;c<dim;c++)io[c]=*(mCrt+c);		mCrt+=dim		  ;		return 1	 ;}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// TEXT AS WELL
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	inline xint TXT_eol		(){return(*mCrt== 13  || *mCrt== 30  || *mCrt=='\n' || *mCrt==0	);}	// chars that indicate a new line.. 30 is 'record seperator' used in wind weather files in tech
	inline xint TXT_spa		(){return(*mCrt=='\t' || *mCrt=='\r' || *mCrt==' '				);}	// chars that indicate whitespace
	inline xint TXT_tab		(){return(*mCrt=='\t' || *mCrt=='\r'							);}	// chars that indicate whitespace
	inline xint TXT_comma	(){return(*mCrt==','											);}	// chars that indicate comma, which we use in our prefs files

	inline void TXT_run_to_EOL()
	{
		while(mCrt<mEnd && !TXT_eol())mCrt++;
		while(mCrt<mEnd &&  TXT_eol())mCrt++;	// go to EOLN here since windows can have TWO eoln chars!
	}

	inline void TXT_scan_thru(const xchr* c)
	{
		const xint len=(xint)strlen(c);
		while(mCrt<mEnd-len && strncmp((xchr*)mCrt,c,len))mCrt++;
		mCrt+=len;
	}

	inline void TXT_scan_comma_str(string* input=NULL)	// scan until comma for the FDR files
	{
		while(mCrt<mEnd &&  TXT_spa  ()					)mCrt++;	xbyt* c1=mCrt;
		while(mCrt<mEnd && !TXT_comma() && !TXT_eol()	)mCrt++;	xbyt* c2=mCrt;					// circuit-breaker: don't go through end of line here: CSV files have no cmma after the last label in the line!
		if   (mCrt<mEnd &&  TXT_comma()					)mCrt++;	if(input)*input=string(c1,c2);	// scan through the comma itself
	}

	inline void TXT_c_scan_space(xchr* input,xint dim)
	{
		while(mCrt<mEnd &&  TXT_spa())mCrt++;	xbyt* c1=mCrt;
		while(mCrt<mEnd && !TXT_spa())mCrt++;	xbyt* c2=mCrt;

		xint i=0;
		for(xbyt* c=c1;c<c2;c++)
		if(i<dim-1)input[i++]=*c;
		if(i<dim  )input[i++]= 0;	// null-term the scanned string!
	}

	inline void TXT_c_scan_eoln(xchr* input,xint dim)
	{
		while(mCrt<mEnd &&  TXT_spa())mCrt++;	xbyt* c1=mCrt;
		while(mCrt<mEnd && !TXT_eol())mCrt++;	xbyt* c2=mCrt;
		while(mCrt<mEnd &&  TXT_eol())mCrt++;

		xint i=0;
		for(xbyt* c=c1;c<c2;c++)
		if(i<dim-1)input[i++]=*c;
		if(i<dim  )input[i++]= 0;	// null-term the scanned string!
	}

	inline void TXT_str_scan_space(string* input)								// scan until whitespace
	{
		while(mCrt<mEnd &&  TXT_spa()				)mCrt++;	xbyt* c1=mCrt;
		while(mCrt<mEnd && !TXT_spa() && !TXT_eol()	)mCrt++;	xbyt* c2=mCrt;			if(input)*input=string(c1,c2);
	}
																				// THIS IS THE REAL SCAN BETWEEN SINGLE TABS... ANY DATA BETWEEN TABS, EVEN IF THERE IS NO DATA BETWEEN SINGLE TABS. THIS IS REALLY THE RIGHT WAY TO DO IT.
	inline void TXT_str_scan_eoln(string* input)								// build a string from the first char to the end of the line (including trailing whitespace).
	{
		while(mCrt<mEnd &&  TXT_spa()				)mCrt++;	xbyt* c1=mCrt;
		while(mCrt<mEnd && !TXT_eol()				)mCrt++;	xbyt* c2=mCrt;			if(input)*input=string(c1,c2);
		while(mCrt<mEnd && (TXT_eol() || TXT_spa())	)mCrt++;					// go to EOLN here since windows can have TWO eoln chars!
	}

	inline void TXT_str_scan_tab(string* input)									// scan until tab, and be able to handle nothing at all in between the tabs for missing data, as in the seattle avionics ICAO airport ID field, for example
	{																			// this handles the tab-separated data, even if that data is missing, or has spaces in it!
																xbyt* c1=mCrt;
		while(mCrt<mEnd && !TXT_tab() && !TXT_eol()	)mCrt++;	xbyt* c2=mCrt;	mCrt++;	if(input)*input=string(c1,c2);
	}

	inline string TXT_str_scan_comma(xint* EOLN)
	{
																xbyt* c1=mCrt;
		while(mCrt<mEnd && (*mCrt)!=',' && !TXT_eol())mCrt++;	xbyt* c2=mCrt;	*EOLN = TXT_eol();	mCrt++;	return string(c1,c2);
	}

	inline time_t TXT_tim_scan()
	{
		while(mCrt<mEnd && TXT_spa())mCrt++;

		time_t retval=0;
		while(mCrt<mEnd && !TXT_spa() && !TXT_comma() && !TXT_eol()){
			retval=(10*retval)+(*mCrt)-'0';
			mCrt++;}

		if(TXT_comma())mCrt++;		// go thru the comma if we scanned until a comma, which we have in prefs files but not apt.dat files for example
		return retval;				// return val
	}

	inline xint TXT_int_scan()
	{
		while(mCrt<mEnd && !chr_is_int(*mCrt) && (*mCrt)!='-'  && (*mCrt)!='+')mCrt++;

		xint sign_mult=1;
		if(mCrt<mEnd && *mCrt=='-'){sign_mult=-1;	mCrt++;}
		if(mCrt<mEnd && *mCrt=='+'){sign_mult= 1;	mCrt++;}

		xint retval=0;
		while(mCrt<mEnd && !TXT_spa() && !TXT_eol() && (*mCrt)!=',' && (*mCrt)!='-' && chr_is_int(*mCrt)){	// stop at the - used in the day part of a date so we can read the month and day
			retval=(10*retval)+(*mCrt)-'0';
			mCrt++;}
		if (mCrt<mEnd && (*mCrt)==',') mCrt++;

		return sign_mult*retval;
	}

	inline xflt TXT_flt_scan()
	{
		while(mCrt<mEnd && (TXT_spa() || *mCrt==','))mCrt++;	// go thru , in CSV to get to number

		xflt sign_mult	=1;
		xflt ret_val	=0;
		xint decimals	=0;
		xint has_decimal=xfals;

		while(mCrt<mEnd && !TXT_spa() && !TXT_eol() && *mCrt!=',')	// dont go thru comma of a CSV
		{
				 if(*mCrt=='-')sign_mult	=-1.0;
			else if(*mCrt=='+')sign_mult	= 1.0;
			else if(*mCrt=='.')has_decimal	=xtrue;
			else
			{
				ret_val=(10.0*ret_val)+(*mCrt)-'0';
				if(has_decimal)decimals++;
			}

			mCrt++;
		}
		return ret_val/pow((xflt)10,(xflt)decimals)*sign_mult;
	}

	inline xdob TXT_dob_scan()
	{
		while(mCrt<mEnd && TXT_spa())mCrt++;

		xdob sign_mult	=1;
		xdob ret_val	=0;
		xint decimals	=0;
		xint has_decimal=xfals;

		while(mCrt<mEnd && !TXT_spa() && !TXT_comma() && !TXT_eol())
		{
				 if(*mCrt=='-')sign_mult	=-1.0;
			else if(*mCrt=='+')sign_mult	= 1.0;
			else if(*mCrt=='.')has_decimal	=xtrue;
			else
			{
				ret_val=(10*ret_val)+(*mCrt)-'0';
				if(has_decimal)decimals++;
			}

			mCrt++;
		}

		if(TXT_comma())mCrt++;									// go thru the comma if we scanned until a comma, which we have in prefs files but not apt.dat files for example
		return ret_val/pow((xdob)10,(xdob)decimals)*sign_mult;	// return val
	}

	inline string TXT_label_scan_CSV(xint* EOLN)
	{
		while(mCrt<mEnd &&  TXT_spa())mCrt++;
																xbyt* c1=mCrt;
		while(mCrt<mEnd && (*mCrt)!=',' && !TXT_eol())mCrt++;	xbyt* c2=mCrt;	*EOLN = TXT_eol();	mCrt++;
		return string(c1,c2);
	}

	inline xflt TXT_flt_scan_CSV()	// THIS HANDLES THINGS LIKE THIS: 456456,465465,4567,,,,,,5675,,,567757,,5675567,,,,,567,57,879,23442,,,, which my test stand puts out
	{
		while(mCrt<mEnd && TXT_spa())mCrt++;

		xflt sign_mult	=1;
		xflt ret_val	=0;
		xint decimals	=0;
		xint has_decimal=xfals;

		while(mCrt<mEnd && !TXT_spa() && !TXT_eol() && *mCrt!=',')	// dont go thru comma of a CSV
		{
				 if(*mCrt=='-')sign_mult	=-1.0;
			else if(*mCrt=='+')sign_mult	= 1.0;
			else if(*mCrt=='.')has_decimal	=xtrue;
			else
			{
				ret_val=(10.0*ret_val)+(*mCrt)-'0';
				if(has_decimal)decimals++;
			}

			mCrt++;
		}

		mCrt++;	// go through the comma

		return ret_val/pow((xflt)10,(xflt)decimals)*sign_mult;
	}

	inline xdob TXT_dob_scan_CSV()	// THIS HANDLES THINGS LIKE THIS: 456456,465465,4567,,,,,,5675,,,567757,,5675567,,,,,567,57,879,23442,,,, which my test stand puts out
	{
		while(mCrt<mEnd && TXT_spa())mCrt++;

		xdob sign_mult	=1;
		xdob ret_val	=0;
		xint decimals	=0;
		xint has_decimal=xfals;

		while(mCrt<mEnd && !TXT_spa() && !TXT_eol() && *mCrt!=',')	// dont go thru comma of a CSV
		{
				 if(*mCrt=='-')sign_mult	=-1.0;
			else if(*mCrt=='+')sign_mult	= 1.0;
			else if(*mCrt=='.')has_decimal	=xtrue;
			else
			{
				ret_val=(10.0*ret_val)+(*mCrt)-'0';
				if(has_decimal)decimals++;
			}

			mCrt++;
		}

		mCrt++;	// go through the comma

		return ret_val/pow((xdob)10,(xdob)decimals)*sign_mult;
	}

	inline void TXT_lat_lon_deg_min_sec(xflt* lat,xflt* lon)
	{
		xflt lat_deg	=		TXT_int_scan();
		xflt lat_min	=		TXT_int_scan();
		xflt lat_sec	=		TXT_int_scan();
		xchr lat_dir[2]	={0};	TXT_c_scan_space(lat_dir,2);	// 2 CHARS SINCE THIS DOES NULL-TERM

		xflt lon_deg	=		TXT_int_scan();
		xflt lon_min	=		TXT_int_scan();
		xflt lon_sec	=		TXT_int_scan();
		xchr lon_dir[2]	={0};	TXT_c_scan_space(lon_dir,2);	// 2 CHARS SINCE THIS DOES NULL-TERM

		*lat=0.0;
		*lon=0.0;

		if(lat_dir[0]=='N')*lat= (lat_deg + lat_min/60.0 + lat_sec/3600.0);
		if(lat_dir[0]=='S')*lat=-(lat_deg + lat_min/60.0 + lat_sec/3600.0);
		if(lon_dir[0]=='E')*lon= (lon_deg + lon_min/60.0 + lon_sec/3600.0);
		if(lon_dir[0]=='W')*lon=-(lon_deg + lon_min/60.0 + lon_sec/3600.0);
	}

	inline xflt TXT_alt_scan(xint* is_MSL)
	{
		xchr label1[str_dim]={0};
		xchr label2[str_dim]={0};

			*is_MSL	=0;	// default to 0 AGL, which is the surface, and indicated by label "SFC", which will not give a scan int value below
		xint int_alt=0;	// default to 0 AGL, which is the surface, and indicated by label "SFC", which will not give a scan int value below

		while(mCrt<mEnd && !chr_is_int(*mCrt) && !TXT_eol()){label1[strlen(label1)]=*mCrt;			mCrt++;}
		while(mCrt<mEnd &&  chr_is_int(*mCrt) && !TXT_eol()){int_alt=(10*int_alt)+(*mCrt)-'0';		mCrt++;}

		TXT_c_scan_eoln(label2,str_dim);
		if(strcmp(label1,"SFC")==0){int_alt =  0;	*is_MSL=0;}
		if(strcmp(label1,"FL ")==0){int_alt*=100;	*is_MSL=1;}	// listed as flight level
		if(strcmp(label2,"AGL")==0){				*is_MSL=0;}
		if(strcmp(label2,"MSL")==0){				*is_MSL=1;}

		return int_alt;
	}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PRIVATE VARS TO KEEP TRACK OF WHATS GOING ON IN THE FILE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
private:
	xbyt* mBegin;
	xbyt* mCrt	;
	xbyt* mEnd	;
	xint  mFile	;

	xmap_class(			  const xmap_class&);	// These are in the private section and have no implementation. That's because they are ILLEGAL!
	xmap_class& operator=(const xmap_class&);	// You cannot go around copying one xmap var with another!
};
#endif
