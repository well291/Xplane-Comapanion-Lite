#include "hl_types.h"

ozui_class ozui;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// OZUI ALERT AND EXIT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void ozui_class::ozui_exit()
{
	cons.cons_nuke();
	xer.close();
	APP_quit();
	exit(0);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// INIT AND HANDLE MENU SELECTION
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
enum
{
	mi_menu_SkyOS	=0,
	mi_item_quit	=1
};

ozui_class::ozui_class()
{
	menu_class menu;
	item_class item;

	menu.menu_name="SkyOS"		;	menus.push_back(menu);
	item.item_name="Quit SkyOS"	;	menus[mi_menu_SkyOS].items.push_back(item);
}

void ozui_class::ozui_handle_menu_sel(xint en)
{
	if(en==mi_item_quit)SkyOS.SkyOS_exit();
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// MENU GRAPHICS AND HANDLING
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void ozui_class::ozui_menus_hi()
{
	xint menu_bar_x1=0;
	xint menu_bar_x2=mnw.XRES_now();
	xint menu_bar_y1=mnw.YRES_now()-font_dy(t_font_1);
	xint menu_bar_y2=mnw.YRES_now();

	ozui_rect(menu_bar_x1,menu_bar_y1,menu_bar_x2-menu_bar_x1,menu_bar_y2-menu_bar_y1);	// menu bar

	xint runx			=0;
	xint good_click		=0;
		 a_menu_active	=0;
	for(xint m=0;m<menus.size();m++)
	if(menus[m].menu_active)
		a_menu_active=1;

	for(xint m=0;m<menus.size();m++)
	{
		xint title_x1=runx;
		xint title_x2=runx+ui_sep_pix+str_dx_pix(t_font_1,menus[m].menu_name)+ui_sep_pix;
		xint title_y2=mnw.YRES_now();
		xint title_y1=mnw.YRES_now()-font_dy(t_font_1);

		if(a_menu_active && mnw.mouse_over(title_x1,title_y1,title_x2,title_y2))
		{
			for(xint sm=0;sm<menus.size();sm++)
				menus[sm].menu_active=(sm==m);
		}

		if(mnw.mouse_down(title_x1,title_y1,title_x2,title_y2))
		{
			good_click=1;
			menus[m].menu_active=!menus[m].menu_active;

			for(xint sm=0;sm<menus.size();sm++)
			if(sm!=m)
				menus[sm].menu_active=0;
		}

		if(menus[m].menu_active)
			OGL_fillrect(col_butn_brt,1.0 , title_x1,title_y1,title_x2-title_x1,title_y2-title_y1);							// menu title selected

		ozui_string(t_font_1,title_x1+ui_sep_pix,(title_y1+title_y2)*0.5,menus[m].menu_name,col_text_reg,1.0,just_lft);		// menu title

		if(menus[m].menu_active)
		{
			xint menu_x1=title_x1;
			xint menu_x2=title_x2;
			xint menu_y2=title_y1;
			xint menu_y1=title_y1;

			for(xint i=0;i<menus[m].items.size();i++)
			{
				menu_y1-=font_dy(t_font_1);
				menu_x2 =fltmax2(menu_x2,title_x1+ui_sep_pix+str_dx_pix(t_font_1,menus[m].items[i].item_name)+ui_sep_pix);
			}

			ozui_rect(menu_x1,menu_y1,menu_x2-menu_x1,menu_y2-menu_y1);	// menu

			for(xint i=0;i<menus[m].items.size();i++)
			{
				xint item_x1=menu_x1;
				xint item_x2=menu_x2;
				xint item_y2=menu_y2-font_dy(t_font_1)*i;
				xint item_y1=item_y2-font_dy(t_font_1);

				if(mnw.mouse_over(item_x1,item_y1,item_x2,item_y2))
					OGL_fillrect(col_butn_brt,1.0 , item_x1,item_y1,item_x2-item_x1,item_y2-item_y1);	// item selected

				ozui_string(t_font_1,item_x1+ui_sep_pix,(item_y1+item_y2)*0.5,menus[m].items[i].item_name,col_text_reg,1.0,just_lft);

				if(mnw.mouse_down(item_x1,item_y1,item_x2,item_y2))
					ozui_handle_menu_sel(m*100+i+1);
			}
		}

		runx=title_x2;
	}

	if(mnw.mouse_clicked && !good_click)
	for(xint m=0;m<menus.size();m++)
		menus[m].menu_active=0;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// FILE-CHOOSER THAST CAN NEST INTO FOLDERS AS DESIRED
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
string ozui_class::ozui_getfile(const string start_path,const xchr ext[4],const xflt x1,const xflt y1,const xflt x2,const xflt y2)
{
	if(				(xint)start_path.size()		==0			)return "";	// at least a dirchar for outer level!
	if(start_path[	(xint)start_path.size()-1]	!=dirchar	)return "";	// end in dirchar!

	static xflt 	work_scroll=0	;	// scroll as needed to see everything
	static string	work_path		;	// this is reset to start_path for each new start_path call (which means a different opener)
	static vector <xint> scrollbars	;	// offset in items

	static string	start_path_old;
	if(				start_path_old!=start_path){
					start_path_old =start_path;		work_path=start_path;		work_scroll=0;		scrollbars.clear();}

	xint level=0;
	xint x1_this_level=x1+work_scroll;

	for(;;)
	{
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// FIND PATH FOR THIS LEVEL
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		string path_this_level=work_path;

		xint dirchar_to_nuke=-1;
		xint dirchars_found	= 0;

		for(xint c=(xint)start_path.size();c<(xint)path_this_level.size()+1;c++)	// +1 since we have to go 1 PAST the last / to see that last /!
		if(path_this_level[c-1]==dirchar)											// if we just passed into a new directory in the path...
		{
			if(dirchars_found++<=level)	// delete farther back into the directory structure as needed to get progressively-longer paths for each level
			{
				dirchar_to_nuke=c;
			}
			else
			{
				break;					// do not delete any more dirchars for this level.. our path is fine for this level
			}
		}

		if(intrange(dirchar_to_nuke,0,(xint)path_this_level.size()-1))	// ok get the part of the path that goes to this level
		{
			path_this_level.erase(path_this_level.begin()+dirchar_to_nuke,path_this_level.end());
		}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// FIND FILES FOR THIS PATH
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		vector <string>		files	;
		vector <file_type>	ftypes	;
		xint num_f=opensave_getitems(path_this_level,files,ftypes,ext);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// DRAW FILES AT THIS PATH
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
		xint dx=0;
		for(xint f=0;f<num_f;f++)
			dx=intmax2(dx,str_dx_pix(t_font_1,files[f]));

		xint x_icon			=x1_this_level	+intround(tman.tex_dx_vis[t_button_file_folder]/2.0);
		xint x_text			=x_icon			+intround(tman.tex_dx_vis[t_button_file_folder]/2.0);
		xint x_scroll		=x_text			+ui_sep_pix+dx+ui_sep_pix;
		xint x2_this_level	=x_scroll		+ui_sep_pix+scol_sep_x;

		if(	scrollbars.size()<=level)
			scrollbars.push_back(0);

		xflt dy_per_item=font_dy(t_font_1);
		ozui_scroll_bar(x_scroll,y1,y2 , &scrollbars[level],num_f,dy_per_item);

		xint y=y2-dy_per_item*0.5+scrollbars[level]*dy_per_item;

		for(xint f=0;f<num_f;f++)
		{
			if(intrange(y,y1,y2))
			{
				string s1=path_this_level+files[f]+dirchar;
				string s2=work_path;

				if(strncmp(s1.c_str(),s2.c_str(),s1.size())==0)	// show the selected file path so far
					OGL_fillrect(col_lgray,1.0 , x1_this_level,y-font_dy(t_font_1)*0.5,x_scroll-x1_this_level-ui_sep_pix,font_dy(t_font_1));

				ozui_string(t_font_1,x_text,y,files[f],col_text_reg,1.0,just_lft);

				if(ftypes[f]==t_folder)
				{
					ozui_overlay(x_icon,y,0,0,2,t_button_file_folder,col_butn_med,1.0);
					if(mnw.mouse_down(x1_this_level,y-font_dy(t_font_1)*0.5,x_scroll,y+font_dy(t_font_1)*0.5))
					{
						work_path=path_this_level+files[f]+dirchar;
						scrollbars.resize(level);
						return "";
					}
				}

				if(ftypes[f]==t_file)
				{
					ozui_overlay(x_icon,y,0,1,2,t_button_file_folder,col_butn_med,1.0);
					if(mnw.mouse_down(x1_this_level,y-font_dy(t_font_1)*0.5,x_scroll,y+font_dy(t_font_1)*0.5))
					{
						return work_path+files[f];
					}
				}
			}
			y-=dy_per_item;
		}

		x1_this_level=x2_this_level;				// scoot right for the next one!==

		if(path_this_level==work_path)				// all done... we are now at the full path
		{											// time to scroll and return
			work_scroll+=(bx2()-x2_this_level)*10.0*xtim.prd_act;
			work_scroll =fltmin2(work_scroll,0);	// only scroll left not right
			return "";
		}

		level++;
	}
	return "";
}
