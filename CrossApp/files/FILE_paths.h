#ifndef FILE_paths_h
#define FILE_paths_h

enum app_path_type
{
	app_path_binary		=0,
	app_path_package	=1,
	app_path_parent_dir	=2
};

void set_path_to_right_dirchars(string& path);
bool is_path_full(const string& path);
void set_path_to_full(string& path);
void APP_set_app_path(string* path, app_path_type what_do_i_want_exactly_now);

#endif
