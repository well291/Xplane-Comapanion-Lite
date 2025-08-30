#ifndef FILE_ops_h
#define FILE_ops_h

enum file_type
{
	t_file	=0,
	t_folder=1
};

xint opensave_getitems(const string& wrk_path,vector<string>& out_files,vector<file_type>& out_types,const xchr ext[4]);
xint make_folder(const string& folder);

#endif
