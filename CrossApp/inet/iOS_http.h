#ifndef down_h
#define down_h

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//	mike evans
//	mike@downstairsgeek.com
//	301-830-8390
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
struct DSGDownloadItemImpl;			// the implementation must be declared in this class, but be defined in the mm since it is obejective c

class down_class
{
    DSGDownloadItemImpl* impl;		// the implementation must be declared in this class, but be defined in the mm since it is obejective c

public:
    down_class();
   ~down_class();

	xint	downloader_type		;	// map or plate
	xint	downloader_index	;	// which file index
	xint	needs_to_scan		;	// running now

	string	downloader_disk		;	// disk destination
	string	downloader_password	;	// password
	string	downloader_descrip	;	// output description

	xint	download_tries	= 0	;
	xint	download_wills	= 0	;

	inline string message_down()
	{
		if(downloading())
		{
			return downloader_descrip+" "+int_to_str(percent_done())+"%";
		}
		else
		{
			if(was_successful			())	return "";												// oh good.
			if(!downloader_descrip.size	())	return "";												// nothing asked for. ok fine.
											return downloader_descrip+" download not successful!";	// oops.
		}

		return "";
	}

			void start_download	(std::string URL,std::string fileName);
			xint downloading	();

    inline	xint awaiting_scan	(){return needs_to_scan		;}
	inline	void mark_as_scanned(){		  needs_to_scan = 0	;}

			xflt percent_done	();
			xint was_successful	();		// if we are trying to get a file we do not have permission to get, then this will still return true! so paste the link into safari to see what it says if we do not get the file!
};
#endif
