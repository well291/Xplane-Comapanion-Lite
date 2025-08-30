#include "hl_types.h"

#include <dirent.h>
#include <sys/param.h>
#include <sys/mount.h>

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// UTILS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
struct no_case_compare_char{
	bool operator()(const char lhs,const char rhs){
	return tolower(lhs)<tolower(rhs);}};

struct no_case_compare_string{
	bool operator()(const string& lhs,const string& rhs)const{
	return lexicographical_compare(lhs.begin(),lhs.end(),rhs.begin(),rhs.end(),no_case_compare_char());}};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// GET FILES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xint opensave_getitems(const string& wrk_path,vector<string>& out_files,vector<file_type>& out_file_types,const xchr ext[4])
{
	out_files		.clear();
	out_file_types	.clear();

	string wrk_path_abs(wrk_path);
	set_path_to_full(wrk_path_abs);

	if(                            wrk_path_abs.size()>1)
	if(!is_a_dir_char(wrk_path_abs[wrk_path_abs.size()-1]))APP_alert("Working path does not appear to be a directory!",
																		"This is not consistent with my labelling practices!",
																		"I need to end all paths with the directory indicator!",wrk_path);
	DIR* dir=opendir(wrk_path_abs.c_str());
	if(!dir)return 0;

	struct dirent* ent;
	while((ent=readdir(dir)))
	{
		struct stat ss;
		string path_rel=wrk_path_abs+ent->d_name;
		if(stat(path_rel.c_str(),&ss)<0)
			continue;

		file_type	this_type=(S_ISDIR(ss.st_mode))?t_folder:t_file;
		string 		this_name=ent->d_name;

		if(this_name.size()>0)
		if(this_name[0]!='.')
		{
			if(this_type==t_folder)
			{
				out_files		.push_back(this_name);
				out_file_types	.push_back(this_type);
			}

			if(this_type==t_file)
			{
				if(strlen(ext)==0)	// no extension-check, like the microstrain files which have no extension
				{
					out_files		.push_back(this_name);
					out_file_types	.push_back(this_type);
				}

				if(strlen(ext)==3)	// with extension-check, like the xavion data files
				if(					  this_name.size()>4)
				if(			this_name[this_name.size()-4] =='.'				)
				if(tolower(	this_name[this_name.size()-3])==tolower(ext[0])	)
				if(tolower(	this_name[this_name.size()-2])==tolower(ext[1])	)
				if(tolower(	this_name[this_name.size()-1])==tolower(ext[2])	)
				{
					out_files		.push_back(this_name);
					out_file_types	.push_back(this_type);
				}

				if(strlen(ext)==4)	// with extension-check, like the xavion data files
				if(					  this_name.size()>5)
				if(			this_name[this_name.size()-5] =='.'				)
				if(tolower(	this_name[this_name.size()-4])==tolower(ext[0])	)
				if(tolower(	this_name[this_name.size()-3])==tolower(ext[1])	)
				if(tolower(	this_name[this_name.size()-2])==tolower(ext[2])	)
				if(tolower(	this_name[this_name.size()-1])==tolower(ext[3])	)
				{
					out_files		.push_back(this_name);
					out_file_types	.push_back(this_type);
				}
			}
		}
	}

	closedir(dir);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//•••AUSTIN SAYS THIS BUBBLE-SORT IS BETTER THAN A BUBBLE-BATH WHEN YOU ARE TIRED AND DIRTY... IT IS THE SAME ON EVERY PLATFORM!••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	no_case_compare_string	comparator;

	for(xint c1=0;c1<out_files.size();c1++)
	for(xint c2=0;c2<c1     		 ;c2++)
	if(comparator(out_files[c1],out_files[c2]))
	{
		swap		(		 out_files		[c1],		 out_files		[c2]);
		int_switch	((xint*)&out_file_types	[c1],(xint*)&out_file_types	[c2]);
	}

	return (xint)out_files.size();
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
xint make_folder(const string& inFolder)
{
	int 	err = 0;
	string 	folder(inFolder);

	if (!folder.empty() && folder[folder.size()-1] == dirchar)
		folder.erase(folder.size()-1);

		set_path_to_full(folder);
		if (mkdir(folder.c_str(),0755) != 0)
		{
//			APP_alert("make_folder failed!!!",inFolder,"","");
		}
bail:
	return err;
}

