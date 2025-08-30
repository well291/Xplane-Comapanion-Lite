#include "hl_types.h"

long granularity=0;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// What the hell is this?!?  Well, some virtual memory systems (Win, Lin) have the weird property that if you want to map
// a SUBSECTION of a file, the START of the file offset must align to a page boundary.  This is what we call the "granularity"
// of mapping.  (The OSes don't care about the length being a multiple of the granularity...they just map more than we want and
// don't tell us.)  Anyway, this func is called by the init code to tell us what our particular OS wants for alignment.
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void xmap_class::set_granularity(long g)
{
	granularity=g;
}

xint xmap_class::check_file(const xint req_version)
{
	if(mBegin==NULL)return xfals;	// no file? get out.
	if(mEnd==mBegin)return xfals;	// no size? get out.

	if(size() > 3 && mBegin[0] == 0xEF && mBegin[1] == 0xBB && mBegin[2] == 0xBF)mCrt += 3;

	xchr scan_frst_chr=BIN_chr_scan();	TXT_str_scan_eoln(NULL);	if(scan_frst_chr!='A' && scan_frst_chr!='I')return xfals;
	xint scan_vers_int=TXT_int_scan();	TXT_str_scan_eoln(NULL);

	return (scan_vers_int==req_version);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
#include <CoreFoundation/CoreFoundation.h>

typedef	unsigned char UTF8;	// Definitions for chars in each of the 3 encoding formats.

void CFStringToSTL_xmap(CFStringRef cfstr,string& stl_str,CFStringEncoding encoding)
{
	CFIndex blen,blen2,clen,clen2,slen=CFStringGetLength(cfstr);
	clen=CFStringGetBytes(cfstr,CFRangeMake(0,slen),encoding,0,xfals,NULL,0,&blen);

	vector<UTF8> buf(blen);
	clen2=CFStringGetBytes(cfstr,CFRangeMake(0,slen),encoding,0,xfals,&*buf.begin(),blen,&blen2);

	stl_str=string(buf.begin(),buf.end());
}

string resource_name_to_pad_path(const string res_name)
{
	string ret;

	CFStringRef	 res_str = NULL;
	CFStringRef	path_str = NULL;
	CFURLRef	 res_url = NULL;

	if((res_str =CFStringCreateWithCString(kCFAllocatorDefault,res_name.c_str(),kCFStringEncodingUTF8))	!= NULL)
	if((res_url =CFBundleCopyResourceURL(CFBundleGetMainBundle(),res_str,NULL,NULL))					!= NULL)
	if((path_str=CFURLCopyFileSystemPath(res_url,kCFURLPOSIXPathStyle))									!= NULL)CFStringToSTL_xmap(path_str,ret,kCFStringEncodingUTF8);

	if (path_str)CFRelease(path_str	);
	if (res_url )CFRelease(res_url	);
	if (res_str )CFRelease(res_str	);

	return ret;
}

void udpate_file_path(string& res_path)
{
	const string res_marker	= "RESOURCE/";
	const string init_path	= res_path;

	if(			res_path.size () >=	res_marker.size ())
	if(strncmp(	res_path.c_str() ,	res_marker.c_str() , res_marker.size()) == 0)
	{			res_path.erase(0 ,						 res_marker.size());	// pull the RESOURCE/ out

			res_path = resource_name_to_pad_path(res_path);		// get the right path in
		if(	res_path.size() == 0)
		{
			return;	// this can happen for DEM files in the middle of the ocean that we don't have... or want!
		}			// so we better not alert here or it looks broken
	}
}

#include <errno.h>
#include <fcntl.h>

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// LOAD THE FILE INTO A BUFFER REALLY FAST
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xmap_class::~xmap_class()
{
	if(mBegin)munmap(mBegin,mEnd-mBegin	);
	if(mFile)close((int)mFile			);
}

xbyt* xmap_class::retry(string path,long offset,long len)
{
	crt_path	=path	;
	mBegin=mEnd	=NULL	;						// null for reset and to indicate that the file is not there
	mFile		=0		;						// null for reset and to indicate that the file is not there

	udpate_file_path(path);						// for hard-wired files

	if(path.empty()		 )return 0;				// rapid-exit case if name not there
	if(!file_exists(path))return 0;				// rapid-exit case if file not there

	if(path.size()>0)							// just an empty directoty
	if(is_a_dir_char(path[path.size()-1]))		// just an empty directoty
		return 0;								// rapid-exit case if file not there

		mFile=open(path.c_str(),O_RDONLY,0);	// open the file
	if(	mFile==-1)return 0;						// rapid-exit case if not happy
	if(	mFile== 0)return 0;						// rapid-exit case if not happy

	long	extra=(granularity)?(offset%granularity):0;
	offset-=extra;

	if(len==0)
	{
		len=lseek(mFile,0,SEEK_END)-offset	;
			lseek(mFile,0,SEEK_SET)			;
	}
	else len+=extra;

	if(len==0)return 0;								// disk_mods_full_name_len might be 0-size, saved on the first run of the app, even though no plates have been downloaded yet!
													// so just treat perfectly legal 0-size file as no file and return now, or the mBegin below will throw a confusing error!
	mBegin=(xbyt*)mmap(0,len,PROT_READ,MAP_FILE|MAP_PRIVATE,mFile,offset);

		 if(mBegin==NULL						){mEnd=mBegin=NULL;	APP_alert("mmap() returned null",get_file_with_path(path),"","");}
	else if(mBegin==(xbyt*)-1 && errno==ENOMEM	){mEnd=mBegin=NULL;	APP_alert("mmap() out of memory",get_file_with_path(path),"","");}
	else if(mBegin==(xbyt*)-1 && errno==EPERM	){mEnd=mBegin=NULL;	APP_alert("mmap() invalid arg"	,get_file_with_path(path),"","");}
	else if(mBegin==(xbyt*)-1					)
	{
		APP_alert("mmap() error "+int_to_str(errno),get_file_with_path(path),"","");
		mEnd=mBegin=NULL;
	}
	else										 {mEnd=mBegin+len;}

	mCrt=	mBegin+extra;
	return	mBegin		;
}
