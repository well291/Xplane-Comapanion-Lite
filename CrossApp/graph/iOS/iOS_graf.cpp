#include "hl_types.h"

class graf_class graf;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
batch_class	batch;

xflt vrt_geo[vrt_dim]={0};
xint		 vrt_ind = 0 ;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void OGL_line(	const enum_col col,const xflt alpha,
				const xflt x1,const xflt y1,
				const xflt x2,const xflt y2)
{
	GLfloat vrt[]={	x1,y1 , s_white_tex,t_white_tex,
					x2,y2 , s_white_tex,t_white_tex};

	xint a=8;
	plot_geo(t_white,col,alpha,vrt,2,&a,GL_LINES , col_vert_fals,dep_read_fals,dep_writ_fals);
}

void OGL_arc(const enum_col col,const xflt alpha,const xflt x,const xflt y,const xflt r,const xflt a1,const xflt da)
{
	const xint dim = fltabs(da/5.0);

	vector <GLfloat>	vrt;
						vrt.resize(dim*4);

	for(xint d=0;d<dim;d++)
	{
		const xflt a = a1 + interp(0.0,0.0 , dim-1,da , d);

		vrt[4*d+0]= x + r * sin(a*degtorad);
		vrt[4*d+1]= y + r * cos(a*degtorad);
		vrt[4*d+2]= s_white_tex;
		vrt[4*d+3]= t_white_tex;
	}

	xint a = 4*dim;
	plot_geo(t_white,col,alpha,&vrt[0],2,&a,GL_LINE_STRIP , col_vert_fals,dep_read_fals,dep_writ_fals);
}

void OGL_framerect(	const enum_col col,const xflt alpha,
					const xflt  x,const xflt  y,
					const xflt dx,const xflt dy)
{
	GLfloat vrt[]={
		x	+0.5f,y   +0.5f,s_white_tex,t_white_tex,	// we really need to add 0.5 or the corners get clipped!
		x+dx+0.5f,y   +0.5f,s_white_tex,t_white_tex,
		x+dx+0.5f,y+dy+0.5f,s_white_tex,t_white_tex,
		x	+0.5f,y+dy+0.5f,s_white_tex,t_white_tex};

	xint a=16;
	plot_geo(t_white,col,alpha,vrt,2,&a,GL_LINE_LOOP , col_vert_fals,dep_read_fals,dep_writ_fals);
}

void OGL_fillrect(	const enum_col col,const xflt alpha,
					const xflt  x,const xflt  y,
					const xflt dx,const xflt dy)
{
	GLfloat	vrt[]={
		x+dx,y	 ,s_white_tex,t_white_tex,	// do not add 0.5 here!
		x	,y	 ,s_white_tex,t_white_tex,	// it does not clear the bottom row of pix!
		x+dx,y+dy,s_white_tex,t_white_tex,	// really!
		x	,y+dy,s_white_tex,t_white_tex};

	xint a=16;
	plot_geo(t_white,col,alpha,vrt,2,&a,GL_TRIANGLE_STRIP , col_vert_fals,dep_read_fals,dep_writ_fals);
}

void OGL_filltri(const enum_col col,const xflt alpha,const xflt x1,const xflt y1,const xflt x2,const xflt y2,const xflt x3,const xflt y3)
{
	GLfloat	vrt[]={
		x1,y1 , s_white_tex,t_white_tex,
		x2,y2 , s_white_tex,t_white_tex,
		x3,y3 , s_white_tex,t_white_tex};

	xint a=12;
	plot_geo(t_white,col,alpha,vrt,2,&a,GL_TRIANGLES , col_vert_fals,dep_read_fals,dep_writ_fals);
}

void OGL_quad(	const enum_tex tex,const enum_col col,const xflt alpha,		// this is CW order
				const xflt x1,const xflt y1 , const xflt s1,const xflt t1,
				const xflt x2,const xflt y2 , const xflt s2,const xflt t2,
				const xflt x3,const xflt y3 , const xflt s3,const xflt t3,
				const xflt x4,const xflt y4 , const xflt s4,const xflt t4)
{
	GLfloat	vrt[]={
		x1,y1 , s1,t1,
		x2,y2 , s2,t2,
		x4,y4 , s4,t4,
		x3,y3 , s3,t3};

	xint a=16;
	plot_geo(tex,col,alpha,vrt,2,&a,GL_TRIANGLE_STRIP , col_vert_fals,dep_read_fals,dep_writ_fals);
}

void OGL_quad_lines(const enum_tex tex,const enum_col col,const xflt alpha,		// this is CW order
					const xflt x1,const xflt y1 , const xflt s1,const xflt t1,
					const xflt x2,const xflt y2 , const xflt s2,const xflt t2,
					const xflt x3,const xflt y3 , const xflt s3,const xflt t3,
					const xflt x4,const xflt y4 , const xflt s4,const xflt t4)
{
	GLfloat	vrt[]={
		x1,y1 , s1,t1,
		x2,y2 , s2,t2,
		x3,y3 , s3,t3,
		x4,y4 , s4,t4,
		x1,y1 , s1,t1};

	xint a=20;
	plot_geo(tex,col,alpha,vrt,2,&a,GL_LINE_STRIP , col_vert_fals,dep_read_fals,dep_writ_fals);
}

void OGL_quad_alpha(const enum_tex tex,const enum_col col,		// this is CW order
					const xflt x1,const xflt y1 , const xflt s1,const xflt t1,const xflt a1,
					const xflt x2,const xflt y2 , const xflt s2,const xflt t2,const xflt a2,
					const xflt x3,const xflt y3 , const xflt s3,const xflt t3,const xflt a3,
					const xflt x4,const xflt y4 , const xflt s4,const xflt t4,const xflt a4)
{
	GLfloat	vrt[]=
	{
		x1,y1 , colr.colr_enum_to_flt(col,a1) , s1,t1,
		x2,y2 , colr.colr_enum_to_flt(col,a2) , s2,t2,
		x4,y4 , colr.colr_enum_to_flt(col,a4) , s4,t4,
		x3,y3 , colr.colr_enum_to_flt(col,a3) , s3,t3
	};

	xint a=20;
	plot_geo(tex,col_white,1.0,vrt,2,&a,GL_TRIANGLE_STRIP , col_vert_true,dep_read_fals,dep_writ_fals);
}

void OGL_tri_alpha(const enum_tex tex,const enum_col col,		// this is CW order
					const xflt x1,const xflt y1,const xflt z1 , const xflt s1,const xflt t1,const xflt a1,
					const xflt x2,const xflt y2,const xflt z2 , const xflt s2,const xflt t2,const xflt a2,
					const xflt x3,const xflt y3,const xflt z3 , const xflt s3,const xflt t3,const xflt a3)
{
	GLfloat	vrt[]=
	{
		x1,y1,z1 , colr.colr_enum_to_flt(col,a1) , s1,t1,
		x2,y2,z2 , colr.colr_enum_to_flt(col,a2) , s2,t2,
		x3,y3,z3 , colr.colr_enum_to_flt(col,a3) , s3,t3
	};

	xint a=20;
	plot_geo(tex,col_white,1.0,vrt,3,&a,GL_TRIANGLES , col_vert_true,dep_read_true,dep_writ_true);
}

void OGL_line_fade(	const enum_col col,
					const xflt a1,
					const xflt a2,
					const xflt x1,const xflt y1,
					const xflt x2,const xflt y2)
{
	GLfloat vrt[]=
	{
		x1,y1 , colr.colr_enum_to_flt(col,a1) , s_white_tex,t_white_tex,
		x2,y2 , colr.colr_enum_to_flt(col,a2) , s_white_tex,t_white_tex
	};

	xint a=10;
	plot_geo(t_white,col_white,1.0,vrt,2,&a,GL_LINES , col_vert_true,dep_read_fals,dep_writ_fals);
}

void OGL_rect_fade(	xflt x1,xflt y1 , xflt col_FLT1,
					xflt x2,xflt y2 , xflt col_FLT2,
					xflt x3,xflt y3 , xflt col_FLT3,
					xflt x4,xflt y4 , xflt col_FLT4)
{
	GLfloat vrt[]=
	{
		x1,y1,col_FLT1,s_white_tex,t_white_tex,
		x2,y2,col_FLT2,s_white_tex,t_white_tex,
		x3,y3,col_FLT3,s_white_tex,t_white_tex,
		x3,y3,col_FLT3,s_white_tex,t_white_tex,
		x4,y4,col_FLT4,s_white_tex,t_white_tex,
		x1,y1,col_FLT1,s_white_tex,t_white_tex
	};

	xint a=30;
	plot_geo(t_white,col_white,1.0,vrt,2,&a,GL_TRIANGLES , col_vert_true,dep_read_fals,dep_writ_fals);
}

void OGL_quad3d(const enum_tex tex,const enum_col col,const xflt alpha,
				const xflt x1,const xflt y1,const xflt z1 , const xflt s1,const xflt t1,
				const xflt x2,const xflt y2,const xflt z2 , const xflt s2,const xflt t2,
				const xflt x3,const xflt y3,const xflt z3 , const xflt s3,const xflt t3,
				const xflt x4,const xflt y4,const xflt z4 , const xflt s4,const xflt t4)
{
	GLfloat vrt[]={	x1,y1,z1 , s1,t1,
					x2,y2,z2 , s2,t2,
					x3,y3,z3 , s3,t3,
					x4,y4,z4 , s4,t4};

	xint a=20;
	plot_geo(tex,col,alpha,vrt,3,&a,GL_TRIANGLE_STRIP , col_vert_fals,dep_read_true,dep_writ_true);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline xflt image_rotation_deg()
{
	if(graf.dev_orient_now()==orient_invert		)return 180;
	if(graf.dev_orient_now()==orient_land_lft	)return -90;
	if(graf.dev_orient_now()==orient_land_rgt	)return  90;
												 return   0;
}

void app_rect_to_device_rect(	const xflt x1_app_space,xflt* x1_ipad,
								const xflt y1_app_space,xflt* y1_ipad,
								const xflt x2_app_space,xflt* x2_ipad,
								const xflt y2_app_space,xflt* y2_ipad)
{
	*x1_ipad = x1_app_space;	// GO FROM THE APP-SPACE PIXELS TO THE PIXELS ON THE IPAD ITSELF, 0-0 BEING LOWER-LEFT CORNER OF THE IPAD ITSELF.
	*y1_ipad = y1_app_space;	// FOR BOTH MATRIX AND ORTHO, WE SIMPLY WANT TO DESCRIBE THE COORDS OF THE BOX THAT WE ARE FILLING ON THE IPAD IN IPAD-SPACE... SIMPLE!!!
	*x2_ipad = x2_app_space;	// VERY VERY EASY WHEN WE UNDERSTAND WE ARE SIMPLY GOING FROM MY APP-SPACE PIXELS TO THE DEVICE PIXELS... THAT'S ALL THERE IS TO IT!
	*y2_ipad = y2_app_space;

	if(graf.dev_orient_now()==orient_invert		){	*x1_ipad=graf.dev_dx_raw-	x2_app_space;		*y1_ipad=graf.dev_dy_raw-	y2_app_space;
													*x2_ipad=graf.dev_dx_raw-	x1_app_space;		*y2_ipad=graf.dev_dy_raw-	y1_app_space;}

	if(graf.dev_orient_now()==orient_land_lft	){	*x1_ipad=					y1_app_space;		*y1_ipad=graf.dev_dy_raw-	x2_app_space;
													*x2_ipad=					y2_app_space;		*y2_ipad=graf.dev_dy_raw-	x1_app_space;}

	if(graf.dev_orient_now()==orient_land_rgt	){	*x1_ipad=graf.dev_dx_raw-	y2_app_space;		*y1_ipad=					x1_app_space;
													*x2_ipad=graf.dev_dx_raw-	y1_app_space;		*y2_ipad=					x2_app_space;}
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void OGL_poly_offset(const xint amount)
{
	static xint last_amount =0		;
	if(			last_amount!=amount	){
				last_amount =amount	;
		if(amount==0)	glDisable		(GL_POLYGON_OFFSET_FILL);
		else		{	glEnable		(GL_POLYGON_OFFSET_FILL);
						glPolygonOffset	(-10.0*amount,	// this is the offset based on the SLOPE
										  -1.0);}}		// this is the offset in static untis... -1.0 is the smallest amount to get one unit CLOSER to us
}

void sci_stop	(																								){glDisable(GL_SCISSOR_TEST);	graf.sci_on=0;}	// plot batch when done with the scissors region we were in when the batch was built!
void sci_start	(const xflt x1_app_space,const xflt y1_app_space,const xflt dx_app_space,const xflt dy_app_space)
{
//	OGL_fillrect (col_red,0.05,x1_app_space,y1_app_space,dx_app_space,dy_app_space);
//	OGL_framerect(col_red,1.00,x1_app_space,y1_app_space,dx_app_space,dy_app_space);

	if(dx_app_space<=0.0)APP_alert("sci_start with dx<0!","","","");
	if(dy_app_space<=0.0)APP_alert("sci_start with dy<0!","","","");

	dev_alert_OGL("sci_1");
	glEnable(GL_SCISSOR_TEST);	graf.sci_on=1;

	graf.sci_x1 = x1_app_space				;	xflt x1_ipad;
	graf.sci_y1 = y1_app_space				;	xflt y1_ipad;
	graf.sci_x2 = x1_app_space+dx_app_space	;	xflt x2_ipad;
	graf.sci_y2 = y1_app_space+dy_app_space	;	xflt y2_ipad;

	app_rect_to_device_rect(graf.sci_x1,&x1_ipad,
							graf.sci_y1,&y1_ipad,
							graf.sci_x2,&x2_ipad,
							graf.sci_y2,&y2_ipad);

	glScissor(x1_ipad,y1_ipad,x2_ipad-x1_ipad,y2_ipad-y1_ipad);

	dev_alert_OGL("sci_2");
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
/*
glViewPort is in screen pixel units: that's it, it has nothing to do with the 3D world "inside" your graphics card.
It just tells which part of the window will be used for rendering (or just will be visible).

glOrtho instead changes the "inner" world and is OpenGL units: More OpenGL units will fit into the visible part of the screen,
so "bigger" objects will fit easily into viewable area if you increase the ortho size.
*/
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void OGL_set_viewport_2d(	const xflt x1_app_space,	// we use full-screen args here, not always a window arg,
							const xflt y1_app_space,	// because this sets us up for the entire device,
							const xflt x2_app_space,	// not just a single editing window
							const xflt y2_app_space)
{
	xflt x1_ipad;
	xflt y1_ipad;
	xflt x2_ipad;
	xflt y2_ipad;
	app_rect_to_device_rect(x1_app_space,&x1_ipad,
							y1_app_space,&y1_ipad,
							x2_app_space,&x2_ipad,
							y2_app_space,&y2_ipad);

	glMatrixMode	(GL_PROJECTION	);
	glLoadIdentity	(				);

	glViewport		(x1_ipad , y1_ipad , x2_ipad-x1_ipad , y2_ipad-y1_ipad);

	if(graf.dev_orient_now()==orient_portrait	)glOrtho(x1_app_space,x2_app_space , y1_app_space,y2_app_space , -1,1);
	if(graf.dev_orient_now()==orient_invert		)glOrtho(x1_app_space,x2_app_space , y1_app_space,y2_app_space , -1,1);
	if(graf.dev_orient_now()==orient_land_lft	)glOrtho(y1_app_space,y2_app_space , x1_app_space,x2_app_space , -1,1);
	if(graf.dev_orient_now()==orient_land_rgt	)glOrtho(y1_app_space,y2_app_space , x1_app_space,x2_app_space , -1,1);

	glMatrixMode	(GL_MODELVIEW	);
	glLoadIdentity	(				);

	if(graf.dev_orient_now()==orient_invert		)glTranslatef	(						graf.dev_dx_raw	,graf.dev_dy_raw,0);
	if(graf.dev_orient_now()==orient_land_lft	)glTranslatef	(						0				,graf.dev_dy_raw,0);
	if(graf.dev_orient_now()==orient_land_rgt	)glTranslatef	(						graf.dev_dx_raw	,0				,0);
												 glRotatef		(image_rotation_deg(),	0				,0				,1);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
void OGL_set_viewport_3d(	xflt fov_l,
							xflt fov_r,
							xflt fov_d,
							xflt fov_u,
							xflt vis_m)
{
	const xflt dome_rad	=pythag(vis_m,vis_m,vis_m);		// GOTTA PYTHAG TO GET THE LEFT/RIGHT/TOP/BOTTOM EDGES OF THE EARTH IN SPACE, WHICH CAN BE AS FAR OUT TO SIDE/ABOVE/BELOW AS WE ARE FAR FROM THEM!
	const xflt near_clip=dome_rad*0.00001;
	const xflt  far_clip=dome_rad*1.00001;

	if(	graf.dev_orient_now()==orient_land_lft ||
		graf.dev_orient_now()==orient_land_rgt )
	{
		flt_switch(&fov_l,&fov_d);
		flt_switch(&fov_r,&fov_u);
	}

	if(	graf.dev_orient_now()==orient_invert)
	{
		flt_switch(&fov_d,&fov_u);
		fov_d*=-1.0;
		fov_u*=-1.0;
	}

	glMatrixMode	(GL_PROJECTION	);
	glLoadIdentity	(				);

	glFrustum		(near_clip*tan(fov_l*degtorad),	// The distances left, right, bottom and top, are the distances from the center of the view to the side faces of the frustum, on the near plane: https://stackoverflow.com/questions/52462082/glfrustum-isnt-creating-perspective
					 near_clip*tan(fov_r*degtorad),	// The distances left, right, bottom and top, are the distances from the center of the view to the side faces of the frustum, on the near plane: https://stackoverflow.com/questions/52462082/glfrustum-isnt-creating-perspective
					 near_clip*tan(fov_d*degtorad),	// The distances left, right, bottom and top, are the distances from the center of the view to the side faces of the frustum, on the near plane: https://stackoverflow.com/questions/52462082/glfrustum-isnt-creating-perspective
					 near_clip*tan(fov_u*degtorad),	// The distances left, right, bottom and top, are the distances from the center of the view to the side faces of the frustum, on the near plane: https://stackoverflow.com/questions/52462082/glfrustum-isnt-creating-perspective
					 near_clip,far_clip);

	glMatrixMode	(GL_MODELVIEW	);	// RESET the modelview matrix! Whatever the hell was there before, DO NOT TRUST IT!
	glLoadIdentity	(				);	// Let's start with a CLEAN slate for 3-d.

	glRotatef		(image_rotation_deg(),0,0,1);
}
