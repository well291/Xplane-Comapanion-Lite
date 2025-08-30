#include "hl_types.h"
#include "UTL_unicode.h"

void APP_set_app_path(string* path,app_path_type what_do_i_want_exactly_now)
{
	char raw_path[2048];

	CFURLRef	main_url = what_do_i_want_exactly_now == app_path_binary ? CFBundleCopyExecutableURL(CFBundleGetMainBundle()) : CFBundleCopyBundleURL(CFBundleGetMainBundle());
	CFStringRef	main_path = CFURLCopyFileSystemPath(main_url, kCFURLPOSIXPathStyle);
	CFStringGetCString(main_path,raw_path,sizeof(raw_path),kCFStringEncodingUTF8);
	CFRelease(main_url);				// Main bundle URL is the PACKAGE of the app, which is what we want
	CFRelease(main_path);				// e.g. /home/design/X-Plane.app

	*path=raw_path;
	set_path_to_right_dirchars(*path);

	if(what_do_i_want_exactly_now == app_path_parent_dir)
		*path=get_path_with_path(*path);
}

void CFStringToSTL(CFStringRef cfstr, string& stl_str, CFStringEncoding encoding)
{
	CFIndex blen, blen2, clen, clen2, slen = CFStringGetLength(cfstr);
	clen = CFStringGetBytes(cfstr,CFRangeMake(0, slen), encoding, 0, xfals, NULL, 0, &blen);	
	vector<UTF8>	buf(blen);
	clen2 = CFStringGetBytes(cfstr,CFRangeMake(0,slen), encoding, 0, xfals, &*buf.begin(), blen, &blen2);	
	dev_assert(clen==clen2);
	dev_assert(blen==blen2);
	stl_str = string(buf.begin(),buf.end());
}

void set_path_to_right_dirchars(string& path)
{
	if(!path.empty())
	if(!UTF8_IsValid(path))
	{
		CFStringRef	s = CFStringCreateWithCString(kCFAllocatorDefault, path.c_str(), kCFStringEncodingMacRoman);
		if(s)
		{
			CFStringToSTL(s,path,kCFStringEncodingUTF8);
			CFRelease(s);
		}
	}
			
	xint n1=0;
	for(xint n=n1;n<path.length();n++)
		if(is_a_bad_dir_char(path[n]))
			path[n]=dirchar;
}

bool is_path_full(const string& path)
{
	return !path.empty() && is_a_dir_char(path[0]);	
}

void set_path_to_full(string& path)
{
	if(!is_path_full(path))
		path = app_path+path;
	if(!app_path.empty())
		dev_assert(is_path_full(path));
}
