#ifndef ozui_h
#define ozui_h

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// DIMS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline xint font_dy(enum_tex t){return tman.tex_dy_vis[t]*0.25;}

const xint border_pix= 5;				// we work in int not flt to avoid blurry half-pixels
const xint ui_sep_pix= 5;				// we work in int not flt to avoid blurry half-pixels
const xint butn_siz_y=32;				// and menu separation based on that
const xint butn_sep_y=32+ui_sep_pix;	// and menu separation based on that
const xint scol_sep_x=32;				// scroll x width

inline xint bx1(){return									+border_pix;}
inline xint bx2(){return mnw.XRES_now()						-border_pix;}
inline xint by1(){return									+border_pix;}
inline xint by2(){return mnw.YRES_now()-font_dy(t_font_1)	-border_pix;}

inline xflt y_help_1	(){return by1		()+butn_sep_y									;}
inline xflt y_help_2	(){return y_help_1	()+font_dy(t_font_1)*4.0						;}
inline xflt y_add_del	(){return y_help_2	()+ui_sep_pix+butn_siz_y*0.5					;}
inline xflt y_list_1	(){return y_add_del	()+ui_sep_pix+butn_siz_y*0.5+font_dy(t_font_1)	;}
inline xflt y_list_2	(){return by2		()												;}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// CLASSES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class item_class
{
public:
   ~item_class(){}
 	item_class(){}

	string item_name;
};

class menu_class
{
public:
   ~menu_class(){}
 	menu_class(){menu_active=0;}

	xint menu_active;
	string menu_name;
	vector <item_class> items;
};

class ozui_class
{
public:
   ~ozui_class(){}
 	ozui_class();

	xint a_menu_active;
	vector <menu_class> menus;

	void ozui_exit();
	void ozui_menus_hi();
	void ozui_handle_menu_sel(xint en);
	string ozui_getfile(const string start_path,const xchr ext[4],const xflt x1,const xflt y1,const xflt x2,const xflt y2);

};
extern ozui_class ozui;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// UTILS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline xint char_to_ind(enum_tex tex,xchr index)
{
	switch(index)
	{
		case 'A': return  0; break;
		case 'B': return  1; break;
		case 'C': return  2; break;
		case 'D': return  3; break;
		case 'E': return  4; break;
		case 'F': return  5; break;
		case 'G': return  6; break;
		case 'H': return  7; break;
		case 'I': return  8; break;
		case 'J': return  9; break;
		case 'K': return 10; break;
		case 'L': return 11; break;
		case 'M': return 12; break;
		case 'N': return 13; break;
		case 'O': return 14; break;
		case 'P': return 15; break;
		case 'Q': return 16; break;
		case 'R': return 17; break;
		case 'S': return 18; break;
		case 'T': return 19; break;
		case 'U': return 20; break;
		case 'V': return 21; break;
		case 'W': return 22; break;
		case 'X': return 23; break;
		case 'Y': return 24; break;
		case 'Z': return 25; break;

		case 'a': return 26; break;
		case 'b': return 27; break;
		case 'c': return 28; break;
		case 'd': return 29; break;
		case 'e': return 30; break;
		case 'f': return 31; break;
		case 'g': return 32; break;
		case 'h': return 33; break;
		case 'i': return 34; break;
		case 'j': return 35; break;
		case 'k': return 36; break;
		case 'l': return 37; break;
		case 'm': return 38; break;
		case 'n': return 39; break;
		case 'o': return 40; break;
		case 'p': return 41; break;
		case 'q': return 42; break;
		case 'r': return 43; break;
		case 's': return 44; break;
		case 't': return 45; break;
		case 'u': return 46; break;
		case 'v': return 47; break;
		case 'w': return 48; break;
		case 'x': return 49; break;
		case 'y': return 50; break;
		case 'z': return 51; break;

		case '0': return 52; break;
		case '1': return 53; break;
		case '2': return 54; break;
		case '3': return 55; break;
		case '4': return 56; break;
		case '5': return 57; break;
		case '6': return 58; break;
		case '7': return 59; break;
		case '8': return 60; break;
		case '9': return 61; break;

		case '!': return 62; break;
		case '@': return 63; break;
		case '#': return 64; break;
		case '$': return 65; break;
		case '%': return 66; break;
		case '^': return 67; break;
		case '&': return 68; break;
		case '*': return 69; break;
		case '(': return 70; break;
		case ')': return 71; break;
		case '+': return 72; break;
		case '-': return 73; break;
		case '_': return 74; break;
		case '=': return 75; break;
		case '{': return 76; break;
		case '}': return 77; break;
		case '[': return 78; break;
		case ']': return 79; break;
		case '\\':return 80; break;
		case ':': return 81; break;
		case ';': return 82; break;
		case '\"':return 83; break;
		case '\'':return 84; break;
		case '<': return 85; break;
		case '>': return 86; break;
		case ',': return 87; break;
		case '.': return 88; break;
		case '/': return 89; break;
		case '?': return 90; break;
		case '|': return 91; break;
	}
	return 0;
}

inline xint str_dx_pix(enum_tex tex,string input)
{
	xint dx=0;

	for(xuin c=0;c<input.size();c++)
	{
		xint				i=char_to_ind(tex,input[c]	);
		if(input[c]==' ')	i=char_to_ind(tex,'0'		);	// we use this for spacing on the ' ' char in the string-plotter, so use the same here to stay in sync

		xflt s1=tman.font_s1[i];
		xflt s2=tman.font_s2[i];
		dx+=(s2-s1)*tman.tex_dx_tot[tex];
	}

	return dx;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PLOT SRING AND NUMBER
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
enum enum_draw_type
{
	draw_normal=0	,	// draw the string now, like most procs
	draw_buildup	,	// build up a batch, but don't draw yet
	draw_finish			// draw the built-up batch all at once for speed!
};

enum enum_just
{
	just_lft=0	,
	just_ctr	,
	just_rgt
};

inline xint ozui_string(enum_tex tex,xflt x,xflt y,string input,enum_col col=col_text_reg,xflt alpha=1.0,enum_just just=just_lft,enum_draw_type draw_type=draw_normal)
{
	xflt dy=intround(font_dy(tex));						// we need to intround this or we get blurry fonts

	if(just==just_ctr)	x-=str_dx_pix(tex,input)*0.5;
	if(just==just_rgt)	x-=str_dx_pix(tex,input)	;
						y-=dy					*0.5;	// the y-value of a string is the CENTER of the char vertically. this lines up buttons, tick-marks... everything!

	x=intround(x);	// we work in floats since opengl wants it, and we may partial-pixel anti-alias some day
	y=intround(y);	// but right now, our fonts and the like look better if they are locked to an int pixel to avoid blurriness

	static vector <GLfloat> geo			;				// the geo we will use for the strings
	static xint				geo_ind=0	;

	if(draw_type!=draw_finish)							// if we are drawing or building, then add some geometry!
	{
		xint ns=geo_ind + (xint)input.size()*30;
		if(	geo.size()<ns)
			geo.resize(ns);								// now tack on the size for the new string!

		for(xuin c=0;c<input.size();c++)
		{
			if(input[c]!=' ' && input[c]>0)
			{
				xint i =char_to_ind(tex,input[c]);
				xflt s1=tman.font_s1[i];
				xflt t1=tman.font_t1[i];
				xflt s2=tman.font_s2[i];
				xflt t2=tman.font_t2[i];
				xflt dx=intround((s2-s1)*tman.tex_dx_tot[tex]);	// we need to int this or we get blurry fonts from the float mults above
					  x=intround(x);							// we work in floats since opengl wants it, and we may partial-pixel anti-alias some day
					  y=intround(y);							// but right now, our fonts and the like look better if they are locked to an int pixel to avoid blurriness

				geo[geo_ind++]=s1;	geo[geo_ind++]=t1;		geo[geo_ind++]=x	;	geo[geo_ind++]=y	;
				geo[geo_ind++]=s1;	geo[geo_ind++]=t2;		geo[geo_ind++]=x	;	geo[geo_ind++]=y+dy	;
				geo[geo_ind++]=s2;	geo[geo_ind++]=t1;		geo[geo_ind++]=x+dx	;	geo[geo_ind++]=y	;
				geo[geo_ind++]=s2;	geo[geo_ind++]=t1;		geo[geo_ind++]=x+dx	;	geo[geo_ind++]=y	;
				geo[geo_ind++]=s1;	geo[geo_ind++]=t2;		geo[geo_ind++]=x	;	geo[geo_ind++]=y+dy	;
				geo[geo_ind++]=s2;	geo[geo_ind++]=t2;		geo[geo_ind++]=x+dx	;	geo[geo_ind++]=y+dy	;

				x+=dx;
			}
			else if(input[c]>0)
			{
				const xint i=char_to_ind(tex,'0');			// use this much for space until i hear otherwise.

				x+=(tman.font_s2[i]-
					tman.font_s1[i])*tman.tex_dx_tot[tex];
			}
		}
	}

	if(draw_type!=draw_buildup)
	{
		ozGEO(tex,col,alpha,&geo[0],geo_ind/4,GL_TRIANGLES);
		geo_ind=0;
	}

	return x;
}

inline void ozui_number(enum_tex tex,xint x,xint y,xflt input,xint dig_and_dec,const xint dec,
						const enum_col col=col_text_reg,
						const xflt alpha=1.0,
						enum_just just=just_lft)
{
	ozui_string(tex,x,y,flt_to_str(input,dig_and_dec,dec),col,alpha,just);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// BUTTONS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
enum
{
	menu_lft=0	,	// left menu
	menu_ctr	,	// center menu
	menu_rgt	,	// right menu
	main_btn		// button set to on or off
};

inline void ozui_overlay(xflt x,xflt y,const xflt ang,const xflt val,const xint num_vals,const enum_tex tex,const enum_col color=col_white,const xflt alpha=1.0)
{
	xflt ds=tman.tex_dx_vis[tex]/tman.tex_dx_tot[tex]/num_vals	;
	xflt dt=tman.tex_dy_vis[tex]/tman.tex_dy_tot[tex]			;
	xflt dx=tman.tex_dx_vis[tex]					 /num_vals	;	xint sdx=dx*0.5;
	xflt dy=tman.tex_dy_vis[tex]								;	xint sdy=dy*0.5;
	xflt s1= val	 *ds;
	xflt s2=(val+1.0)*ds;

	if(ang==0.0)
	{
		GLfloat	geo[4*4]={
			s2, 0 , x+sdx,y-sdy,
			s1, 0 , x-sdx,y-sdy,
			s2,dt , x+sdx,y+sdy,
			s1,dt , x-sdx,y+sdy};	// note vertex order is for TRI STRIP - we don't have quads on iphone!

		ozGEO(tex,color,alpha,geo,4,GL_TRIANGLE_STRIP);
	}
	else
	{
		xflt sin_ang=sin(ang*degtorad);
		xflt cos_ang=cos(ang*degtorad);
		GLfloat	geo[4*4]={
			s2, 0 , x+sdx*cos_ang-sdy*sin_ang , y-sdy*cos_ang-sdx*sin_ang,
			s1, 0 , x-sdx*cos_ang-sdy*sin_ang , y-sdy*cos_ang+sdx*sin_ang,
			s2,dt , x+sdx*cos_ang+sdy*sin_ang , y+sdy*cos_ang-sdx*sin_ang,
			s1,dt , x-sdx*cos_ang+sdy*sin_ang , y+sdy*cos_ang+sdx*sin_ang};

		ozGEO(tex,color,alpha,geo,4,GL_TRIANGLE_STRIP);
	}
}

inline xint ozui_main(xint en,xint lit,xint x1,xint y,xint dx,string label1="",string label2="",enum_col lit_col=col_dim)
{
			xint ret=0;
	const	xint tdx=tman.tex_dx_tot[t_button_main];
	const	xint tdy=tman.tex_dy_tot[t_button_main];
	const	xint pdy=tman.tex_dy_vis[t_button_main];

	if(mnw.mouse_grabbed(	x1		,y-pdy*0.5,
							x1+dx	,y+pdy*0.5))lit=1;	// lit as long as held in the button
	if(mnw.mouse_down(		x1		,y-pdy*0.5,
							x1+dx	,y+pdy*0.5))ret=1;

	xflt s1=42.0/tdx;	if(en==menu_lft || en==main_btn)s1= 0.0		;	// start with a hard left edge here. go to rounded left edge if a left-side case
	xflt s2=62.0/tdx;	if(en==menu_lft || en==main_btn)s2=20.0/tdx	;	// start with a hard left edge here. go to rounded left edge if a left-side case

	xflt s3=62.0/tdx;	// center section spred out as long as needed to fill the center
	xflt s4=66.0/tdx;	// center section spred out as long as needed to fill the center

	xflt s5=66.0/tdx;if(en==menu_rgt || en==main_btn)s5=1.0-20.0/tdx;	// start with a hard rigt edge here. go to rounded rigt edge if a rigt-side case
	xflt s6=86.0/tdx;if(en==menu_rgt || en==main_btn)s6=1.0			;	// start with a hard rigt edge here. go to rounded rigt edge if a rigt-side case

	enum_col col=(lit)?col_menu_brt:col_menu_drk;						// menu colors

	if(en==main_btn)
	{
								 col=(lit)?col_butn_brt	:col_butn_drk;	// ui colors
		if(lit_col	!=col_dim	)col=(lit)?lit_col		:col_butn_drk;
		if(lit		==-1		)col=	   				 col_butn_med;
	}

	OGL_quad(t_button_main,col,1.0,
		x1		,y-pdy*0.5 , s1,0.0		,
		x1		,y+pdy*0.5 , s1,pdy/tdy	,
		x1+   20,y+pdy*0.5 , s2,pdy/tdy	,
		x1+   20,y-pdy*0.5 , s2,0.0		);

	OGL_quad(t_button_main,col,1.0,
		x1+   20,y-pdy*0.5 , s3,0.0		,
		x1+   20,y+pdy*0.5 , s3,pdy/tdy	,
		x1+dx-20,y+pdy*0.5 , s4,pdy/tdy	,
		x1+dx-20,y-pdy*0.5 , s4,0.0		);

	OGL_quad(t_button_main,col,1.0,
		x1+dx-20,y-pdy*0.5 , s5,0.0		,
		x1+dx-20,y+pdy*0.5 , s5,pdy/tdy	,
		x1+dx	,y+pdy*0.5 , s6,pdy/tdy	,
		x1+dx	,y-pdy*0.5 , s6,0.0		);

	xint dual_line	=(label2.size()>0);
	xint y_text_os1	=(dual_line)?((en==main_btn)? 8: 12):0;
	xint y_text_os2	=(dual_line)?((en==main_btn)?-8:-12):0;

	enum_tex		font=t_font_1;
					ozui_string(font,x1+dx*0.5,y+y_text_os1,label1,col_text_reg,1.0,just_ctr);
	if(dual_line)	ozui_string(font,x1+dx*0.5,y+y_text_os2,label2,col_text_reg,1.0,just_ctr);

	return ret;
}

inline xint ozui_on_off(xint is_on,	xint x,xint y,xint dx,string label1="",string label2="",enum_col lit_col=col_dim){return ozui_main(main_btn,is_on,x,y,dx,label1,label2,lit_col	);}
inline xint ozui_action(			xint x,xint y,xint dx,string label1="",string label2=""							){return ozui_main(main_btn,   -1,x,y,dx,label1,label2			);}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// MENU OR WINDOW RECT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline void ozui_rect(xint x,xint y,xint dx,xint dy)
{
	OGL_shadow	 (					 x,y,dx,dy);
	OGL_fillrect (col_butn_med,1.0 , x,y,dx,dy);
	OGL_framerect(col_butn_drk,1.0 , x,y,dx,dy);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SCROLL BAR
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline void ozui_scroll_bar(xint x1,xint y1,xint y2,xint* ind,xint num_ind,xint dy_per_item)
{
	if(num_ind<2)return;
	if((y2-y1)%2)y1-=1;	// scroll bar must be even number of pix tall to perfectly fit the slider since that works in semi-widths, so will always be an even number of pixels

	const	xint scroll_dx	=tman.tex_dx_vis[t_button_scroll]	;
	const	xint sdy_tex	=0.5*scroll_dx						;
			xint x2			= x1+scroll_dx						;

	xflt dy_scroll_vis_region	=y2-y1;
	xflt dy_total_list			=num_ind*dy_per_item;
	xflt slider_size_fraction	=dy_scroll_vis_region/fltmax2(dy_total_list,1);
		 slider_size_fraction	=fltlim(slider_size_fraction,0.0,1.0);
	xint sdy_slider				=dy_scroll_vis_region*slider_size_fraction*0.5;
		 sdy_slider				=intmax2(sdy_slider,sdy_tex);

		 if(dy_total_list<=dy_scroll_vis_region	)*ind=0;															// no interpolation to do since there is no movement of the scroll bar! so keep scroll at 0
	else if(mnw.mouse_grabbed(x1,y1,x2,y2)		)*ind=interp(y1+sdy_slider,num_ind-1 , y2-sdy_slider,0 , mnw.my);	// drag the scroll bar based on an interpolation, which is only valid if there is some motion to be had and thus interp range to use
												 *ind=intlim(*ind,0,num_ind-1);										// in case the input changed somehow

	xflt s1=(								  0			)/tman.tex_dx_tot[t_button_scroll];
	xflt s2=(tman.tex_dx_vis[t_button_scroll]-0			)/tman.tex_dx_tot[t_button_scroll];
	xflt t1=(    							  0			)/tman.tex_dy_tot[t_button_scroll];
	xflt t2=(    							  sdy_tex	)/tman.tex_dy_tot[t_button_scroll];
	xflt t3=(tman.tex_dy_vis[t_button_scroll]-sdy_tex	)/tman.tex_dy_tot[t_button_scroll];
	xflt t4=(tman.tex_dy_vis[t_button_scroll]-0			)/tman.tex_dy_tot[t_button_scroll];

	for(xint i=0;i<2;i++)											// 0 is scroll bar base, 1 is the actual slider part
	{
		xint y_slider_pix=interp(	0.0			,y2-sdy_slider,		// slider location
									num_ind-1	,y1+sdy_slider,*ind);

		enum_col	col=(i==0)?col_butn_drk	:col_butn_med;
		xint		py1=(i==0)?y1			:y_slider_pix-sdy_slider;
		xint		py2=(i==0)?y2			:y_slider_pix+sdy_slider;

		OGL_quad(t_button_scroll,col,1.0,
			x1,py1			, s1,t1,
			x1,py1+sdy_tex	, s1,t2,
			x2,py1+sdy_tex	, s2,t2,
			x2,py1			, s2,t1);

		OGL_quad(t_button_scroll,col,1.0,
			x1,py1+sdy_tex	, s1,t2,
			x1,py2-sdy_tex	, s1,t3,
			x2,py2-sdy_tex	, s2,t3,
			x2,py1+sdy_tex	, s2,t2);

		OGL_quad(t_button_scroll,col,1.0,
			x1,py2-sdy_tex	, s1,t3,
			x1,py2			, s1,t4,
			x2,py2			, s2,t4,
			x2,py2-sdy_tex	, s2,t3);
	}
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// MENUS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline xint ozui_menus_lo(xint* sel,xint num,xchr names[10][name_dim64],xint level)
{
	xint ret=0;

	for(xint n=0;n<num;n++)
	{
		xint		 en=menu_ctr;
		if(n==0		)en=menu_lft;
		if(n==num-1	)en=menu_rgt;

		xint x1=interp(0,bx1() , num,bx2() , n  );	// menus actually run tighter than the other stuff
		xint x2=interp(0,bx1() , num,bx2() , n+1);	// otherwise they get crowded and have borders we just don't need

		if(ozui_main(en,(*sel==n),x1,by1()+butn_siz_y*(level+0.5),x2-x1,names[n],""))
		{
			*sel=n;
			ret=1;
			mnw.key_target_code=0;
		}
	}

	return ret;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// MUMBER-ENTRY BUTTONS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline xint ozui_edit_string(xint x,xint y,string* var,xint sdx,xint can_edit)
{
	xint cx1=x		;
	xint cx2=x+sdx	;
	xint cy1=y-font_dy(t_font_1)*0.5;
	xint cy2=y+font_dy(t_font_1)*0.5;

	xint 									 just_sel_this_click=0;
	xint sel=mnw.get_target(cx1,cy1,cx2,cy2,&just_sel_this_click);	if(!can_edit)sel=0;
	if(sel		)OGL_fillrect (col_pulse_orange,0.25,cx1,cy1,cx2-cx1,cy2-cy1);
	if(can_edit	)OGL_framerect(col_text_reg ,0.25,cx1,cy1,cx2-cx1,cy2-cy1);

	if(sel)
	{
		mnw.edit_target(var);
		OGL_line(col_text_reg,1.0,	x+ui_sep_pix+str_dx_pix(t_font_1,*var),y-font_dy(t_font_1)*0.5,
									x+ui_sep_pix+str_dx_pix(t_font_1,*var),y+font_dy(t_font_1)*0.5);
	}

	ozui_string(t_font_1,x+ui_sep_pix,y,*var,col_text_reg,1.0);
	return sel;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// MUMBER-ENTRY BUTTONS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline xint ozui_edit_xflt(xint x,xint y,xflt* var,xint dig_and_dec,xint dec,string label,xint sdx,xint can_edit)
{
	xint cx1=x-sdx	;
	xint cx2=x		;
	xint cy1=y-font_dy(t_font_1)*0.5;
	xint cy2=y+font_dy(t_font_1)*0.5;

	xint 									 just_sel_this_click=0;
	xint sel=mnw.get_target(cx1,cy1,cx2,cy2,&just_sel_this_click);			if(!can_edit)sel=0;
	if(sel		)OGL_fillrect (col_pulse_orange,0.25,cx1,cy1,cx2-cx1,cy2-cy1);
	if(can_edit	)OGL_framerect(col_text_reg ,0.25,cx1,cy1,cx2-cx1,cy2-cy1);

	static string edit_str;

	if(just_sel_this_click)
	{
		edit_str=flt_to_str(*var,dig_and_dec,dec);
	}
	else if(sel)
	{
		mnw.edit_target(&edit_str);
		*var=str_to_flt( edit_str);

		ozui_string(t_font_1,		x-ui_sep_pix,y,edit_str	,col_text_reg,1.0,just_rgt);
		ozui_string(t_font_1,		x+ui_sep_pix,y,label	,col_text_reg,0.6,just_lft);
		OGL_line(col_text_reg,1.0,	x-ui_sep_pix,y-font_dy(t_font_1)*0.5,
									x-ui_sep_pix,y+font_dy(t_font_1)*0.5);
	}
	else
	{
		ozui_string(t_font_1,x-ui_sep_pix,y,flt_to_str(*var,dig_and_dec,dec),col_text_reg,1.0,just_rgt);
		ozui_string(t_font_1,x+ui_sep_pix,y,label							,col_text_reg,0.6,just_lft);
	}
	return sel;
}

inline xint ozui_edit_xint(xint x,xint y,xint* var,xint dig,string label,xint sdx,xint can_edit)
{
	xflt fvar=*var;		xint sel=ozui_edit_xflt(x,y,&fvar,dig,0,label,sdx,can_edit);
		 *var=fvar;
	return sel;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// MUMBER-ENTRY BUTTONS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline xint ozui_edit_xtim(xint x,xint y,time_t* var,xint sdx,xint can_edit)
{
	xint cx1=x		;
	xint cx2=x+sdx	;
	xint cy1=y-font_dy(t_font_1)*0.5;
	xint cy2=y+font_dy(t_font_1)*0.5;

	xint 									 just_sel_this_click=0;
	xint sel=mnw.get_target(cx1,cy1,cx2,cy2,&just_sel_this_click);	if(!can_edit)sel=0;
	if(sel		)OGL_fillrect (col_pulse_orange	,0.25,cx1,cy1,cx2-cx1,cy2-cy1);
	if(can_edit	)OGL_framerect(col_text_reg		,0.25,cx1,cy1,cx2-cx1,cy2-cy1);

	static string edit_str;

	if(just_sel_this_click)
	{
		edit_str=timet_to_number_date_str(*var);
	}
	else if(sel)
	{
		mnw.edit_target(&edit_str);
		*var=date_time_str_to_timet(edit_str);

		ozui_string(t_font_1,		x+ui_sep_pix,y,edit_str,col_text_reg,1.0,just_lft);
		OGL_line(col_text_reg,1.0,	x+ui_sep_pix+str_dx_pix(t_font_1,edit_str),y-font_dy(t_font_1)*0.5,
									x+ui_sep_pix+str_dx_pix(t_font_1,edit_str),y+font_dy(t_font_1)*0.5);
	}
	else
	{
		ozui_string(t_font_1,x		+ui_sep_pix,y,timet_to_number_date_str	(*var),col_text_reg,1.0,just_lft);
		ozui_string(t_font_1,x+sdx	+ui_sep_pix,y,timet_to_day_date_str		(*var),col_text_reg,0.6,just_lft);
	}

	return sel;
}
#endif
