#ifndef x_vis_butns_h
#define x_vis_butns_h

enum enum_just
{
	 just_lft=0	,
	 just_ctr	,
	 just_rgt
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// OVERLAYS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline void plot_ins_overlay(xint x,xint y,const xflt ang,const xflt val,const xflt num_vals,const enum_tex tex,const enum_col color=col_white,const xflt alpha=1.0)
{
	const xflt ds	=tman.tex_dx_vis[tex]/tman.tex_dx_tot[tex]/num_vals	;
	const xflt dt	=tman.tex_dy_vis[tex]/tman.tex_dy_tot[tex]			;
	const xint dx	=tman.tex_dx_vis[tex]					 /num_vals	;	const xint sdx=dx*0.5;
	const xint dy	=tman.tex_dy_vis[tex]								;	const xint sdy=dy*0.5;
	const xflt s1	= val	 *ds;
	const xflt s2	=(val+1.0)*ds;

	if(ang==0.0)
	{
		OGL_quad(tex,color,alpha,
			x+sdx,y-sdy,s2, 0,
			x-sdx,y-sdy,s1, 0,
			x-sdx,y+sdy,s1,dt,
			x+sdx,y+sdy,s2,dt);	// note vertex order is for TRI STRIP - we don't have quads on iphone!
	}
	else
	{
		const xflt sin_ang=sin(ang*degtorad);
		const xflt cos_ang=cos(ang*degtorad);

		OGL_quad(tex,color,alpha,
			x+sdx*cos_ang-sdy*sin_ang , y-sdy*cos_ang-sdx*sin_ang , s2, 0,
			x-sdx*cos_ang-sdy*sin_ang , y-sdy*cos_ang+sdx*sin_ang , s1, 0,
			x-sdx*cos_ang+sdy*sin_ang , y+sdy*cos_ang+sdx*sin_ang , s1,dt,
			x+sdx*cos_ang+sdy*sin_ang , y+sdy*cos_ang-sdx*sin_ang , s2,dt);
	}
}

inline void plot_text_box(xflt y1,xflt y2,xint is_alert)
{
	OGL_fillrect(col_text_reg,0.5,	0.0					,y1		,
									graf.dev_dx_use()	,y2-y1	);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// PLOT NUMBER AND STRING
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline xint char_to_ind(enum_tex tex,xchr index)
{
	if(tex==t_font_4)
	switch(index)
	{
		case '0': return  0; break;
		case '1': return  1; break;
		case '2': return  2; break;
		case '3': return  3; break;
		case '4': return  4; break;
		case '5': return  5; break;
		case '6': return  6; break;
		case '7': return  7; break;
		case '8': return  8; break;
		case '9': return  9; break;
		case '-': return 10; break;
		case '.': return 11; break;
		case ',': return 12; break;
	}

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

inline xflt str_dx_pix(enum_tex tex,string input)
{
	xflt dx=0;

	for(xuin c=0;c<input.size();c++){
		xint				i=char_to_ind(tex,input[c]	);
		if(input[c]==' ')	i=char_to_ind(tex,'0'		);	// we use this for spacing on the ' ' char in the string-plotter, so use the same here to stay in sync

		xflt s1=tman.font_s1[tex][i];
		xflt s2=tman.font_s2[tex][i];
		dx+=(s2-s1)*tman.tex_dx_tot[tex];}

	return dx;
}

enum enum_draw_type
{
	draw_normal=0	,	// draw the string now, like most procs
	draw_buildup	,	// build up a batch, but don't draw yet
	draw_finish			// draw the built-up batch all at once for speed!
};

inline xflt plot_string(enum_tex tex,xflt x,xflt y,string input,enum_col col=col_white,xflt alpha=1.0,enum_just just=just_lft,enum_draw_type draw_type=draw_normal)
{
	xflt dy=(tex==t_font_4)	?intround(tman.tex_dy_vis[tex]		)	// we need to intround this or we get blurry fonts
							:intround(tman.tex_dy_vis[tex]*0.25	);	// we need to intround this or we get blurry fonts

	if(just==just_ctr)	x-=str_dx_pix(tex,input)*0.5;
	if(just==just_rgt)	x-=str_dx_pix(tex,input)	;
						y-=dy					*0.5;	// the y-value of a string is the CENTER of the char vertically. this lines up buttons, tick-marks... everything!

	x=intround(x);		// we work in floats since opengl wants it, and we may partial-pixal anti-alias some day
	y=intround(y);		// but right now, our fonts and the like look better if they are locked to an int pixel to avoid blurriness

	static vector <xflt> geo_str	;					// the geo_str we will use for the strings
	static xint			 geo_ind=0	;

	if(draw_type!=draw_finish)							// if we are drawing or building, then add some geometry!
	{
		xflt fcol=colr.colr_enum_to_flt(col,alpha);

		xint ns=geo_ind + (xint)input.size()*30;
		if(	geo_str.size()<ns)
			geo_str.resize(ns);							// now tack on the size for the new string!

		for(xuin c=0;c<input.size();c++)
		{
			if(input[c]!=' ' && input[c]>0)
			{
				xint i =char_to_ind(tex,input[c]);
				xflt s1=tman.font_s1[tex][i];
				xflt t1=tman.font_t1[tex][i];
				xflt s2=tman.font_s2[tex][i];
				xflt t2=tman.font_t2[tex][i];
				xflt dx=intround((s2-s1)*tman.tex_dx_tot[tex]);	// we need to int this or we get blurry fonts from the float mults above

				if(input[c]=='|')
				if(input!="||")															// do not flash the pause icon
					fcol=colr.colr_enum_to_flt(col,0.5+0.5*sin(xtim.real_time*10.0));	// i use '|' for flashing cursor

				geo_str[geo_ind++]=x	;	geo_str[geo_ind++]=y	;	geo_str[geo_ind++]=fcol;	geo_str[geo_ind++]=s1;	geo_str[geo_ind++]=t1;
				geo_str[geo_ind++]=x	;	geo_str[geo_ind++]=y+dy	;	geo_str[geo_ind++]=fcol;	geo_str[geo_ind++]=s1;	geo_str[geo_ind++]=t2;
				geo_str[geo_ind++]=x+dx	;	geo_str[geo_ind++]=y	;	geo_str[geo_ind++]=fcol;	geo_str[geo_ind++]=s2;	geo_str[geo_ind++]=t1;

				geo_str[geo_ind++]=x+dx	;	geo_str[geo_ind++]=y	;	geo_str[geo_ind++]=fcol;	geo_str[geo_ind++]=s2;	geo_str[geo_ind++]=t1;
				geo_str[geo_ind++]=x	;	geo_str[geo_ind++]=y+dy	;	geo_str[geo_ind++]=fcol;	geo_str[geo_ind++]=s1;	geo_str[geo_ind++]=t2;
				geo_str[geo_ind++]=x+dx	;	geo_str[geo_ind++]=y+dy	;	geo_str[geo_ind++]=fcol;	geo_str[geo_ind++]=s2;	geo_str[geo_ind++]=t2;

				if(input[c]=='|')fcol=colr.colr_enum_to_flt(col,alpha);	// i use '|' for flashing cursor

				x+=dx;
			}
			else if (input[c] > 0)
			{
				xint i=char_to_ind(tex,'0');			// use this much for space until i hear otherwise.

				x+=(tman.font_s2[tex][i]-
					tman.font_s1[tex][i])*tman.tex_dx_tot[tex];
			}
		}
	}

	if(draw_type!=draw_buildup || geo_ind>vrt_dim-str_dim*30)
		plot_geo(tex,col_white,1.0,&geo_str[0],2,&geo_ind,GL_TRIANGLES,col_vert_true,dep_read_fals,dep_writ_fals);	// this is because we often batch strings and labels at the same time, building up in parallel!

	return x;
}

inline void plot_number(enum_tex tex,xflt x,xflt y,xflt input,xint dig_and_dec,const xint dec,
						const enum_col col=col_white,
						const xflt alpha=1.0,
						const enum_str_format str_format=str_lead_with_spaces,enum_just just=just_lft,xint dark_digits=0)
{
	string str_lite;
	string str_dark;

	if(dark_digits==0)
	{
		str_lite=flt_to_str(input,dig_and_dec,dec,str_format);
		plot_string(tex,x,y,str_lite,col,alpha,just);
	}
	else
	{
		xint i_lite=input		/pow10(dark_digits);	str_lite=flt_to_str(	   i_lite ,dig_and_dec-dark_digits,dec,str_format			);
		xint i_dark=input-i_lite*pow10(dark_digits);	str_dark=flt_to_str(fltabs(i_dark),			   dark_digits,dec,str_lead_with_zeros	);

		if(i_dark <0)	// get neg sign in front of the big digits
		if(i_lite==0)	// used for vvi, and maybe alt!
			str_lite="-"+str_lite;

		x-=dark_digits*tman.tex_dx_vis[t_font_2]/(xflt)TEXT_DIM;

		plot_string(t_font_3,x-4,y,str_lite,col,alpha		,just_rgt);	// this only works for rgt-just strings of full-size, for now
		plot_string(t_font_2,x+4,y,str_dark,col,alpha*0.75	,just_lft);
	}
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
enum
{
	menu_lft=0	,	// left menu
	menu_ctr	,	// center menu
	menu_rgt	,	// right menu
	main_btn	,	// button set to on or off
	slide_over
};

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// BUTTON
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline xint butn_main(xint en,const xint lit,const xint x1,const xint y,const xint dx,const string label1="",const string label2="")
{
	enum_tex			t=t_button_main	;
	if(en==slide_over)	t=t_button_slide;

	const xflt tdx=tman.tex_dx_tot[t];
	const xflt tdy=tman.tex_dy_tot[t];
	const xflt pdy=tman.tex_dy_vis[t];

	const xflt butn_reg_x1=x1			;
	const xflt butn_reg_x2=x1+ dx		;
	const xflt butn_reg_y1=y -pdy*0.5	;
	const xflt butn_reg_y2=y +pdy*0.5	;

	const xint pressing=apps.mouse_box_drag(butn_reg_x1,butn_reg_y1,
											butn_reg_x2,butn_reg_y2);

	xflt s1=42.0/tdx;	/* start with a hard left edge here. go to rounded left edge if a left-side case */	if(en==menu_lft || en==main_btn || en==slide_over)s1= 0.0			;
	xflt s2=62.0/tdx;	/* start with a hard left edge here. go to rounded left edge if a left-side case */	if(en==menu_lft || en==main_btn || en==slide_over)s2=20.0/tdx		;

	xflt s3=62.0/tdx;	// center section spred out as long as needed to fill the center
	xflt s4=66.0/tdx;	// center section spred out as long as needed to fill the center

	xflt s5=66.0/tdx;	/* start with a hard rigt edge here. go to rounded rigt edge if a rigt-side case */	if(en==menu_rgt || en==main_btn || en==slide_over)s5=1.0-20.0/tdx	;
	xflt s6=86.0/tdx;	/* start with a hard rigt edge here. go to rounded rigt edge if a rigt-side case */	if(en==menu_rgt || en==main_btn || en==slide_over)s6=1.0			;

	enum_col col=(lit)?col_menu_brt:col_menu_drk;	// menu colors

	if(en==main_btn)
	{
					col=(lit		)?col_butn_brt:col_butn_drk;	// non-menu colors
		if(lit==-1)	col=(pressing	)?col_butn_brt:col_butn_med;	// non-menu colors
	}

	if(en==slide_over)
	{
		col	=col_white	;	// slider edges to stay white around the corners
		en	=main_btn	;
	}

	OGL_quad(t,col,1.0,
		x1		,y-pdy*0.5 , s1,0.0		,
		x1		,y+pdy*0.5 , s1,pdy/tdy	,
		x1+   20,y+pdy*0.5 , s2,pdy/tdy	,
		x1+   20,y-pdy*0.5 , s2,0.0		);

	OGL_quad(t,col,1.0,
		x1+   20,y-pdy*0.5 , s3,0.0		,
		x1+   20,y+pdy*0.5 , s3,pdy/tdy	,
		x1+dx-20,y+pdy*0.5 , s4,pdy/tdy	,
		x1+dx-20,y-pdy*0.5 , s4,0.0		);

	OGL_quad(t,col,1.0,
		x1+dx-20,y-pdy*0.5 , s5,0.0		,
		x1+dx-20,y+pdy*0.5 , s5,pdy/tdy	,
		x1+dx	,y+pdy*0.5 , s6,pdy/tdy	,
		x1+dx	,y-pdy*0.5 , s6,0.0		);

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// TEXT
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
	const xint dual_line	=(label2.size()>0);
	const xflt y_text_os1	=(dual_line)? 12:0;
	const xflt y_text_os2	=(dual_line)?-12:0;

	enum_tex		font=t_font_2;
	if(dual_line)	font=t_font_1;
					plot_string(font,x1+dx*0.5,y+y_text_os1,label1,col_text_butt,1.0,just_ctr);
	if(dual_line)	plot_string(font,x1+dx*0.5,y+y_text_os2,label2,col_text_butt,1.0,just_ctr);

	return apps.mouse_box_release(	butn_reg_x1,butn_reg_y1,
									butn_reg_x2,butn_reg_y2);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// BUTTONS
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline xint butn_on_off(const xint is_on,	const xflt x,const xflt y,const xflt dx,const string label1="",const string label2=""){return butn_main(main_btn,is_on	,x,y,dx,label1,label2);}
inline xint butn_action(					const xflt x,const xflt y,const xflt dx,const string label1="",const string label2=""){return butn_main(main_btn,-1		,x,y,dx,label1,label2);}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline void butn_2d(const xflt tx1,const xflt ty1,const xflt tx2,const xflt ty2,const xflt alpha)
{
	const xflt tdx=tman.tex_dx_tot[t_button_main];
	const xflt tdy=tman.tex_dy_tot[t_button_main];
	const xflt vdx=tman.tex_dx_vis[t_button_main];
	const xflt vdy=tman.tex_dy_vis[t_button_main];

	for(xint i=0;i<3;i++)
	for(xint j=0;j<3;j++)
	{
		xflt s1=0.5*vdx/tdx;	// center
		xflt t1=0.5*vdy/tdy;	// center
		xflt s2=0.5*vdx/tdx;	// center
		xflt t2=0.5*vdy/tdy;	// center

		xflt x1=tx1+20;			// center
		xflt y1=ty1+20;			// center
		xflt x2=tx2-20;			// center
		xflt y2=ty2-20;			// center

		if(i==0){x1=tx1		;	x2=tx1+20.0	;		s1=0.0		;	s2=s1+20.0/tdx;}
		if(i==2){x1=tx2-20.0;	x2=tx2		;		s2=vdx/tdx	;	s1=s2-20.0/tdx;}

		if(j==0){y1=ty1		;	y2=ty1+20.0	;		t1=0.0		;	t2=t1+20.0/tdy;}
		if(j==2){y1=ty2-20.0;	y2=ty2		;		t2=vdy/tdy	;	t1=t2-20.0/tdy;}

		OGL_quad(t_button_main,col_butn_med,alpha,
			x1 , y1 , s1 , t1,
			x1 , y2 , s1 , t2,
			x2 , y2 , s2 , t2,
			x2 , y1 , s2 , t1);
	}
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline xint butn_slide(	const xflt x,const xflt y,const xflt dx,
						string label1,xflt* var,string label2,
						const xflt del,const xflt lo,const xflt hi,
						const xflt unit_conv=1.0)
{
	const xint ret = apps.mouse_btn_slide(x,y-35,x+dx,y+35,var);
	const xflt rat = interp(x,0.0,x+dx,1.0,apps.mou_drag_x[0]);

	butn_on_off(0,x,y,dx);	// base dark button, after the btn_slide proc grabs the mouse-down

	if(ret)
	{
		*var=interp(0.0,lo,1.0,hi,rat);
		*var=fltnear(*var,del);
	}

	const xflt dxr=dx*interp(lo,0.0,hi,1.0,*var);

		  xflt x1=x			;
		  xflt x2=x		+20	;
	const xflt x3=x+dx	-20	;

	const xflt dx1=fltlim(x+dxr-x1,0,		20);
	const xflt dx2=fltlim(x+dxr-x2,0,dx-	40);
	const xflt dx3=fltlim(x+dxr-x3,0,		20);

	const xflt tdx=tman.tex_dx_tot[t_button_slide];
	const xflt tdy=tman.tex_dy_tot[t_button_slide];
	const xflt pdy=tman.tex_dy_vis[t_button_slide];

	const xflt s1=0.0					;
	const xflt s2=			 dx1	/tdx;
	const xflt s3=		62.0		/tdx;
	const xflt s4=		66.0		/tdx;
	const xflt s5=1.0-(	20.0	)	/tdx;
	const xflt s6=1.0-(	20.0-dx3)	/tdx;

	if(dx1>0.0)
	OGL_quad(t_button_slide,col_butn_brt,1.0,
		x1		,y-pdy*0.5 , s1,0.0		,
		x1		,y+pdy*0.5 , s1,pdy/tdy	,
		x1+dx1	,y+pdy*0.5 , s2,pdy/tdy	,
		x1+dx1	,y-pdy*0.5 , s2,0.0		);

	if(dx2>0.0)
	OGL_quad(t_button_slide,col_butn_brt,1.0,
		x2		,y-pdy*0.5 , s3,0.0		,
		x2		,y+pdy*0.5 , s3,pdy/tdy	,
		x2+dx2	,y+pdy*0.5 , s4,pdy/tdy	,
		x2+dx2	,y-pdy*0.5 , s4,0.0		);

	if(dx3>0.0)
	OGL_quad(t_button_slide,col_butn_brt,1.0,
		x3		,y-pdy*0.5 , s5,0.0		,
		x3		,y+pdy*0.5 , s5,pdy/tdy	,
		x3+dx3	,y+pdy*0.5 , s6,pdy/tdy	,
		x3+dx3	,y-pdy*0.5 , s6,0.0		);

	xint		dec=0;
	if(del<0.5 )dec=1;
	if(del<0.05)dec=2;

	x1=x								+20;
	x2=x+dx-str_dx_pix(t_font_2,label2)	-20;

	string l1	=label1	;
	xflt num	=*var	;
	string l2	=label2	;

	if(lo==  0.0)
	if(hi==364.0)
	{
		const xint days_in_month[12]={31,28,31,30,31,30,31,31,30,31,30,31};

		xint	i_month    =0;
		xint	days_so_far=0;
		while(	days_so_far+ days_in_month[	i_month]<=num){
				days_so_far+=days_in_month[	i_month];
											i_month+=1;}

		const xint		i_day	= num - days_so_far + 1;
		num=			i_day	;
		l2 =month_name(	i_month);
	}

	plot_string(t_font_2,x1		,y,l1					,col_text_butt,1.0						,just_lft);
	plot_number(t_font_3,x2-5	,y,num*unit_conv,5,dec	,col_text_butt,1.0,str_lead_with_spaces	,just_rgt);
	plot_string(t_font_2,x2		,y,l2					,col_text_butt,1.0						,just_lft);

	return ret;
}

inline xint butn_dir(xflt x,xflt y,string s_label,xflt* var,xint is_wind)
{
	xint ret=apps.mouse_btn_slide(x-50,y-50,x+50,y+50,var);
	if(ret)*var=fltwrap(atan2(apps.mou_drag_x[0]-x,apps.mou_drag_y[0]-y)*radtodeg+((is_wind)?180.0:0.0),0.0,360.0);

	plot_ins_overlay(x,y,(is_wind)?(*var):(*var+180.0)	,1,2,t_button_cir,col_butn_drk);
	plot_ins_overlay(x,y,0.0							,0,2,t_button_cir,col_butn_med);

	plot_string(t_font_3,x,y			,int_to_str(*var)	,col_text_butt,1.0,just_ctr);
	plot_string(t_font_2,x,y-label_off	,s_label			,col_text_back,1.0,just_ctr);

	return ret;
}

inline xflt butn_scroll_reg(xflt max_pix,const xflt y1,const xflt y2)
{
	static xflt y_off=0;

	if(max_pix<=y2-y1)
	{
		y_off=0;
	}
	else
	{
		max_pix-=(y2-y1);	// max_pix comes in as the total pixel height of the data. we really scroll that much minus one screen height.

		if(apps.mouse_box_drag(bx1(),y1,bx2(),y2))	y_off+=apps.mou_drag_dy[0];
													y_off =fltlim(y_off,0,max_pix);
	}

	return y_off;
}

inline xint butn_menus(xint* sel,const xint num,xchr names[20][name_dim80],const xflt level)
{
	xint ret=0;

	for(xint n=0;n<num;n++)
	{
		xint		 en=menu_ctr;
		if(n==0		)en=menu_lft;
		if(n==num-1	)en=menu_rgt;

		const xint x1 = interp(0,ui_sep_pix , num,graf.dev_dx_use()-ui_sep_pix , n  );	// menus actually run tighter than the other stuff
		const xint x2 = interp(0,ui_sep_pix , num,graf.dev_dx_use()-ui_sep_pix , n+1);	// otherwise they get crowded and have borders we just don't need

		string				 s1=names[n];
		string				 s2="";
		if(level>0			)s2=" ";					// go to small fonts for the sub-level menus
		if(level>0 && num>=4)half_name(names[n],s1,s2);	// landing gear in the failures menu, but not ai-instructor in the sits menu

		if(butn_main(en,(*sel==n),x1,butn_dy*(level+0.5),x2-x1,s1,s2)){
			*sel=n;
			ret=1;}
	}

	return ret;
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline xint plot_key(string s,xflt x,xflt y,xflt dx){return butn_action(x,y,dx-ui_sep_pix,s);}

inline xchr plot_keyboard(xflt y,xint max_dim)
{
	xflt dx=(bx2()-bx1()+ui_sep_pix)/10.0;	// add ui_sep_pix since that is taken off the right end of every key incl the last one of course
	xint ret=0;

	xflt x=ui_sep_pix-dx;
		 y-=butn_dy*0.5;	// go from y top of keyboard to y center of button as we use in butn_action

	if(plot_key("1",x+=dx,y,dx))ret='1';
	if(plot_key("2",x+=dx,y,dx))ret='2';
	if(plot_key("3",x+=dx,y,dx))ret='3';
	if(plot_key("4",x+=dx,y,dx))ret='4';
	if(plot_key("5",x+=dx,y,dx))ret='5';
	if(plot_key("6",x+=dx,y,dx))ret='6';
	if(plot_key("7",x+=dx,y,dx))ret='7';
	if(plot_key("8",x+=dx,y,dx))ret='8';
	if(plot_key("9",x+=dx,y,dx))ret='9';
	if(plot_key("0",x+=dx,y,dx))ret='0';

	x =ui_sep_pix-dx;
	y-=butn_dy;
	if(plot_key("Q",x+=dx,y,dx))ret='Q';
	if(plot_key("W",x+=dx,y,dx))ret='W';
	if(plot_key("E",x+=dx,y,dx))ret='E';
	if(plot_key("R",x+=dx,y,dx))ret='R';
	if(plot_key("T",x+=dx,y,dx))ret='T';
	if(plot_key("Y",x+=dx,y,dx))ret='Y';
	if(plot_key("U",x+=dx,y,dx))ret='U';
	if(plot_key("I",x+=dx,y,dx))ret='I';
	if(plot_key("O",x+=dx,y,dx))ret='O';
	if(plot_key("P",x+=dx,y,dx))ret='P';

	x =ui_sep_pix-dx*0.5;
	y-=butn_dy;
	if(plot_key("A",x+=dx,y,dx))ret='A';
	if(plot_key("S",x+=dx,y,dx))ret='S';
	if(plot_key("D",x+=dx,y,dx))ret='D';
	if(plot_key("F",x+=dx,y,dx))ret='F';
	if(plot_key("G",x+=dx,y,dx))ret='G';
	if(plot_key("H",x+=dx,y,dx))ret='H';
	if(plot_key("J",x+=dx,y,dx))ret='J';
	if(plot_key("K",x+=dx,y,dx))ret='K';
	if(plot_key("L",x+=dx,y,dx))ret='L';

	x =ui_sep_pix-dx;
	y-=butn_dy;
	if(plot_key("SPA"	,x+=dx		,y,dx*1.5	))ret=' ';
	if(plot_key("Z"		,x+=dx*1.5	,y,dx		))ret='Z';
	if(plot_key("X"		,x+=dx		,y,dx		))ret='X';
	if(plot_key("C"		,x+=dx		,y,dx		))ret='C';
	if(plot_key("V"		,x+=dx		,y,dx		))ret='V';
	if(plot_key("B"		,x+=dx		,y,dx		))ret='B';
	if(plot_key("N"		,x+=dx		,y,dx		))ret='N';
	if(plot_key("M"		,x+=dx		,y,dx		))ret='M';
	if(plot_key("DEL"	,x+=dx		,y,dx*1.5	))ret='~';

	return ret;
}

inline xint get_keyboard(xflt y,xchr* name,xint max_dim)
{
	xchr plot_key=plot_keyboard(y,max_dim)	;
	xint edit_dig=max_dim					;

	for(xint c=max_dim-1;c>=0;c--)
	if(name[c]==0)
		edit_dig=c;

			if(plot_key=='~'){edit_dig=intmax2(edit_dig-1,0			);	name[edit_dig]=0		;}	// backspace
	else	if(plot_key!= 0 ){edit_dig=intlim (edit_dig,0,max_dim-2	);	name[edit_dig]=plot_key	;}	// plot_key
	return	  (plot_key!= 0 );																		// get that update from the fms entry so we know to update the destination
}
#endif
