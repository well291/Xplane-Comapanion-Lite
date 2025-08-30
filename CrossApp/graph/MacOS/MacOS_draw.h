#ifndef OGL_draw_h
#define OGL_draw_h

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// OGL UTILS, INT NOT FLOT TO AVOID BLURRY EDGES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline void OGL_scissors_end(								){glDisable(GL_SCISSOR_TEST);}
inline void OGL_scissors	(xint x,xint y,xint dx,xint dy	)
{
	xint x1=(x   );
	xint y1=(y   );
	xint x2=(x+dx);
	xint y2=(y+dy);

	dx=fltmax2(x2-x1,0);	// we get an ogl alert if we let this get less than 0
	dy=fltmax2(y2-y1,0);	// this can happen with the viggen veleocity-vector-tracking HUD

	glEnable(GL_SCISSOR_TEST);
	glScissor((GLint)x1,(GLint)y1,(GLsizei)dx,(GLsizei)dy);
}

inline void OGL_viewport(xint dims,	xflt rat_x1=0,xflt pix_x1=0,
									xflt rat_y1=0,xflt pix_y1=0,
									xflt rat_x2=0,xflt pix_x2=0,xflt near_clip=0,
									xflt rat_y2=0,xflt pix_y2=0,xflt  far_clip=0,xflt zoom=1.0)
{
	if(dims==2)
	{
		xflt x1_pix=0				;
		xflt y1_pix=0				;
		xflt x2_pix=mnw.XRES_now()	;
		xflt y2_pix=mnw.YRES_now()	;

		rat_x1=x1_pix;					// these are the coords that we actually USE WHEN WE MAKE THE CALLS.
		rat_x2=x2_pix;
		rat_y1=y1_pix;
		rat_y2=y2_pix;

		pix_x1=0.0					;	// these are the ACTUAL PIXELS ON THE MONITOR THAT GET LIT UP.
		pix_x2=mnw.XRES_now()		;
		pix_y1=0.0					;
		pix_y2=mnw.YRES_now()		;

		near_clip	=-1.0;
		far_clip	= 1.0;
	}

	if(dims==3)
	{
		rat_x1*=near_clip;		// this is needed to factor the near clipping range out of the FOV for OpenGL!
		rat_y1*=near_clip;		// NOT doing this causes vastly different "zoom" or "fisheye" depending on the near clip!
		rat_x2*=near_clip;		// I do not want that in my code!
		rat_y2*=near_clip;
	}

	xflt pix_dx=pix_x2-pix_x1;
	xflt pix_dy=pix_y2-pix_y1;

	if(dims==3)
	{							// AUTO-SCALE LETS ME SEND IN FOV'S THAT ARE THE SAME FOR X AND Y...
		rat_y1*=pix_dy/pix_dx;	// AND IT SCALES FOR THE ASPECT RATIO OF THE IMAGE AUTOMATICALLY
		rat_y2*=pix_dy/pix_dx;
	}

	glMatrixMode	(GL_PROJECTION	);
	glLoadIdentity	(				);

	if(dims==2)glOrtho(	rat_x1/zoom,
						rat_x2/zoom,
						rat_y1/zoom,
						rat_y2/zoom,near_clip,far_clip);

	if(dims==3)glFrustum(	rat_x1/zoom,
							rat_x2/zoom,
							rat_y1/zoom,
							rat_y2/zoom,near_clip,far_clip);

	glViewport((GLint)pix_x1,(GLint)pix_y1,(GLsizei)pix_dx,(GLsizei)pix_dy);

	glMatrixMode	(GL_MODELVIEW	);
	glLoadIdentity	(				);
}

inline void OGL_sbuff()
{
	glColor4f(1.0,1.0,1.0,1.0);
	WIN_ogl_window_swap(cons.window);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// MY GEO CODE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// if we can find a glVertex2f in the app then we are not using this!
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline void ozGEO(	enum_tex		in_tex			,
					enum_col		in_color		,
					xflt			in_alpha		,
					xflt*			in_geo			,
					xint			in_num_verts	,
					GLenum			in_gl_type		)
{
	const xint textured=(in_tex!=t_tex_none);

				OGL_setstate	(fog_fals,textured,lit_fals,al_blend,deR_fals,deW_fals);
	if(textured)tman.bind_ref	(in_tex);

	glColor4f(	colr.private_col_rgb[in_color][0],
				colr.private_col_rgb[in_color][1],
				colr.private_col_rgb[in_color][2],in_alpha);

	if( textured)	glEnableClientState	(GL_TEXTURE_COORD_ARRAY);
	if(!textured)	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
					glDisableClientState(GL_COLOR_ARRAY			);
/*
if i ever want color vertices, just look at dems_plot_terrain
it's easy, just send in color that way and enable the GL_COLOR_ARRAY
*/

 	xint		stride			=0;	// 0 means tightly-packed
	if(textured)stride			=4;

	xint 		vertex_offset	=0;
	if(textured)vertex_offset	=2;

	if(textured)glTexCoordPointer	(2,GL_FLOAT,stride*sizeof(GL_FLOAT),&in_geo[0				]);	// coords per vertex, number type, offset in between consecutive vertices bytes, array pointer
				glVertexPointer		(2,GL_FLOAT,stride*sizeof(GL_FLOAT),&in_geo[vertex_offset	]);	// 2 or 3 dimensions, number type, offset in between consecutive vertices bytes, array pointer
				glDrawArrays		(in_gl_type,0,in_num_verts);									// poly type, offset in bytes, number of vertices
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// SHAPES, INT NOT FLOAT TO AVOID BLURRY EDGES
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
inline void OGL_line(enum_col color,xflt alpha,xflt x1,xflt y1,xflt x2,xflt y2)
{
	GLfloat	geo[2*2]={x1,y1,x2,y2};
	ozGEO(t_tex_none,color,alpha,geo,2,GL_LINES);
}

inline void OGL_framerect(enum_col color,xflt alpha,xflt x,xflt y,xflt dx,xflt dy)
{
	GLfloat	geo[4*2]={
		x	,y	 ,
		x	,y+dy,
		x+dx,y+dy,
		x+dx,y	 };	// note vertex order is for TRI STRIP - we don't have quads on iphone!

	ozGEO(t_tex_none,color,alpha,geo,4,GL_LINE_LOOP);
}

inline void OGL_framecircle(enum_col color,xflt alpha,xflt x,xflt y,xflt r)
{
	GLfloat	geo[36*2];

	for(xint a=0;a<36;a++)
	{
		geo[a*2  ]=x+r*sin(a*10.0*degtorad);
		geo[a*2+1]=y+r*cos(a*10.0*degtorad);
	}

	ozGEO(t_tex_none,color,alpha,geo,36,GL_LINE_LOOP);
}

inline void OGL_fillcircle(enum_col color,xflt alpha,xflt x,xflt y,xflt r)
{
	GLfloat	geo[36*2];

	for(xint a=0;a<36;a++)
	{
		geo[a*2  ]=x+r*sin(a*10.0*degtorad);
		geo[a*2+1]=y+r*cos(a*10.0*degtorad);
	}

	ozGEO(t_tex_none,color,alpha,geo,36,GL_TRIANGLE_FAN);
}

inline void OGL_tri(enum_col color,xflt alpha,
					xflt x1,xflt y1,
					xflt x2,xflt y2,
					xflt x3,xflt y3)
{
	GLfloat	geo[3*2]={
		x1,y1,
		x2,y2,
		x3,y3};

	ozGEO(t_tex_none,color,alpha,geo,3,GL_TRIANGLES);
}

inline void OGL_fillrect(enum_col color,xflt alpha,xflt x,xflt y,xflt dx,xflt dy)
{
	GLfloat	geo[4*2]={
		x	,y	 ,
		x	,y+dy,
		x+dx,y+dy,
		x+dx,y	 };	// note vertex order is for TRI STRIP - we don't have quads on iphone!

	ozGEO(t_tex_none,color,alpha,geo,4,GL_QUADS);
}

inline void OGL_shadow(xflt x,xflt y,xflt dx,xflt dy)
{
	for(xint i=0;i<5;i++)
		OGL_fillrect(col_black,0.1 , x+i,y-i,dx,dy);
}

inline void OGL_quad(	enum_tex in_tex,enum_col in_color,xflt in_alpha,
						xflt x1,xflt y1 , xflt s1,xflt t1,
						xflt x2,xflt y2 , xflt s2,xflt t2,
						xflt x3,xflt y3 , xflt s3,xflt t3,
						xflt x4,xflt y4 , xflt s4,xflt t4)
{
	GLfloat	geo[4*4]={
		s1,t1 , x1,y1,
		s2,t2 , x2,y2,
		s4,t4 , x4,y4,
		s3,t3 , x3,y3};	// note vertex order is for TRI STRIP - we don't have quads on iphone!

	ozGEO(in_tex,in_color,in_alpha,geo,4,GL_TRIANGLE_STRIP);
}

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// MY BATCHING CODE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class batch_class
{
public:
   ~batch_class(){}
	batch_class(){memset(this,0,sizeof(*this));}

	#define		vert_v_max 32000
	GLfloat vrt[vert_v_max]		;
	xint		vert_v			;

	enum_col	running_col		=col_dim;
	xflt		running_alpha	=0.0	;
	xint		running_type	=0		;

	inline void plot_batch(enum_tex tex)
	{
		if(vert_v>0)
		{
			ozGEO(tex,running_col,running_alpha,vrt,vert_v/4,running_type);
			vert_v=0;
		}
	}

	inline void add_line_2d_vertex(xflt x1,xflt y1,enum_col col,xflt alpha)
	{
		running_type=GL_LINE_STRIP;
		vrt[vert_v++]=0.5;	vrt[vert_v++]=0.5;	vrt[vert_v++]=x1;	vrt[vert_v++]=y1;	//vrt[vert_v++]=col_FLT1;

		if(col!=running_col || alpha!=running_alpha || vert_v>=vert_v_max-5)	// plot any previous icons in the running color if we changed color
		{																		// happens for twrd vs nontwrd apts
			plot_batch(t_white);

			running_col		=col	;
			running_alpha	=alpha	;
		}
	}

	inline void add_line_2d_strip(	const xflt s1,const xflt t1,const xflt x1,const xflt y1,
									const xflt s2,const xflt t2,const xflt x2,const xflt y2,const enum_col col,const xflt alpha)
	{
		running_col		=col				;
		running_alpha	=alpha				;
		running_type	=GL_TRIANGLE_STRIP	;

		vrt[vert_v++]=s1;	vrt[vert_v++]=t1;		vrt[vert_v++]=x1;	vrt[vert_v++]=y1;
		vrt[vert_v++]=s2;	vrt[vert_v++]=t2;		vrt[vert_v++]=x2;	vrt[vert_v++]=y2;

		if(vert_v>=vert_v_max)
			APP_alert("OOB vertex dim","","","");
	}

	inline void add_tris(	const xflt s1,const xflt t1,const xflt x1,const xflt y1,	// path moving right to left, station 1 left
							const xflt s2,const xflt t2,const xflt x2,const xflt y2,	// path moving right to left, station 1 right
							const xflt s3,const xflt t3,const xflt x3,const xflt y3,	// path moving right to left, station 2 left
							const xflt s4,const xflt t4,const xflt x4,const xflt y4,	// path moving right to left, station 2 right
							const enum_col col,const xflt alpha)
	{
		running_col		=col			;
		running_alpha	=alpha			;
		running_type	=GL_TRIANGLES	;

		vrt[vert_v++]=s1;	vrt[vert_v++]=t1;		vrt[vert_v++]=x1;	vrt[vert_v++]=y1;
		vrt[vert_v++]=s2;	vrt[vert_v++]=t2;		vrt[vert_v++]=x2;	vrt[vert_v++]=y2;
		vrt[vert_v++]=s3;	vrt[vert_v++]=t3;		vrt[vert_v++]=x3;	vrt[vert_v++]=y3;

		vrt[vert_v++]=s3;	vrt[vert_v++]=t3;		vrt[vert_v++]=x3;	vrt[vert_v++]=y3;
		vrt[vert_v++]=s2;	vrt[vert_v++]=t2;		vrt[vert_v++]=x2;	vrt[vert_v++]=y2;
		vrt[vert_v++]=s4;	vrt[vert_v++]=t4;		vrt[vert_v++]=x4;	vrt[vert_v++]=y4;

		if(vert_v>=vert_v_max)
			APP_alert("OOB vertex dim","","","");
	}

	inline void add_icon(enum_tex tex,enum_col col,xflt alpha , xflt x,xflt y , xflt val,xflt num_vals , xflt psi_deg,xflt scale)
	{
		running_type=GL_QUADS;

		if(col!=running_col || alpha!=running_alpha || vert_v>=vert_v_max-16)	// plot any previous icons in the running color if we changed color
		{																		// happens for twrd vs nontwrd apts
			plot_batch(tex);

			running_col		=col	;
			running_alpha	=alpha	;
		}

		const xflt ds=tman.tex_dx_vis[tex]/tman.tex_dx_tot[tex]	/num_vals	;
		const xflt dt=tman.tex_dy_vis[tex]/tman.tex_dy_tot[tex]				;
		const xint dx=tman.tex_dx_vis[tex]						 /num_vals	;	xint sdx=dx*0.5*scale;
		const xint dy=tman.tex_dy_vis[tex]									;	xint sdy=dy*0.5*scale;
		const xflt s1= val	 	*ds;
		const xflt s2=(val+1.0)	*ds;

		if(psi_deg==0.0)
		{
			vrt[vert_v++]=s1;	vrt[vert_v++]=dt;		vrt[vert_v++]=x-sdx;							vrt[vert_v++]=y+sdy;
			vrt[vert_v++]=s1;	vrt[vert_v++]= 0;		vrt[vert_v++]=x-sdx;							vrt[vert_v++]=y-sdy;
			vrt[vert_v++]=s2;	vrt[vert_v++]= 0;		vrt[vert_v++]=x+sdx;							vrt[vert_v++]=y-sdy;
			vrt[vert_v++]=s2;	vrt[vert_v++]=dt;		vrt[vert_v++]=x+sdx;							vrt[vert_v++]=y+sdy;
		}
		else
		{
			const xflt sin_psi=sin(psi_deg*degtorad);
			const xflt cos_psi=cos(psi_deg*degtorad);

			vrt[vert_v++]=s1;	vrt[vert_v++]=dt;		vrt[vert_v++]=x-sdx*cos_psi + sdy*sin_psi;		vrt[vert_v++]=y+sdy*cos_psi + sdx*sin_psi;
			vrt[vert_v++]=s1;	vrt[vert_v++]= 0;		vrt[vert_v++]=x-sdx*cos_psi - sdy*sin_psi;		vrt[vert_v++]=y-sdy*cos_psi + sdx*sin_psi;
			vrt[vert_v++]=s2;	vrt[vert_v++]= 0;		vrt[vert_v++]=x+sdx*cos_psi - sdy*sin_psi;		vrt[vert_v++]=y-sdy*cos_psi - sdx*sin_psi;
			vrt[vert_v++]=s2;	vrt[vert_v++]=dt;		vrt[vert_v++]=x+sdx*cos_psi + sdy*sin_psi;		vrt[vert_v++]=y+sdy*cos_psi - sdx*sin_psi;
		}
	}
};
extern batch_class batch;

//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
// MY REALLY PERFECT PATH-DRAWING CODE
//••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••••
class path_class
{
public:
   ~path_class(){}
	path_class(){memset(this,0,sizeof(*this));}

	xflt		use_s1,use_t1;
	xflt		use_s2,use_t2;

	xflt		run_x,run_y,run_w,run_a;
	xflt		run_sin_psi,run_cos_psi;
	enum_col	run_c;

	#define RENDER_AS_TRIS		0	// slower, more geo, not as good so turned off... we an attempt to avoid overlaps in tite turns but does not do that
	#define RENDER_AS_TRISTRIP	1	// faster, less geo, so using this

	#if RENDER_AS_TRIS
		inline void add_path(const xflt now_w,enum_col now_c,const xflt now_a,const xflt now_x,const xflt now_y,const xflt psi_deg,const xint is_egg_bounds=0)
		{
			const xflt now_sin_psi=sin(psi_deg*degtorad);
			const xflt now_cos_psi=cos(psi_deg*degtorad);

			use_s1=394.0/tman.tex_dx_tot[t_map_icons];
			use_s2=400.0/tman.tex_dx_tot[t_map_icons];
			use_t1=  0.4;
			use_t2=  0.6;

			if(is_egg_bounds)
			{
				use_s1=640.0/tman.tex_dx_tot[t_map_icons];
				use_s2=608.0/tman.tex_dx_tot[t_map_icons];
				use_t1=  0.0;
				use_t2=  1.0;
			}

			if(batch.vert_v==0)		// start endcap
			{
				batch.add_tris
				(
					use_s1,use_t1 , now_x-now_w*now_sin_psi	+now_w*now_cos_psi,now_y-now_w*now_cos_psi	-now_w*now_sin_psi,
					use_s1,use_t2 , now_x-now_w*now_sin_psi	-now_w*now_cos_psi,now_y-now_w*now_cos_psi	+now_w*now_sin_psi,
					use_s2,use_t1 , now_x					+now_w*now_cos_psi,now_y					-now_w*now_sin_psi,
					use_s2,use_t2 , now_x					-now_w*now_cos_psi,now_y					+now_w*now_sin_psi,now_c,now_a
				);
			}
			else
			{
				batch.add_tris		// path
				(
					use_s2,use_t1 , run_x					+run_w*run_cos_psi,run_y					-run_w*run_sin_psi,
					use_s2,use_t2 , run_x					-run_w*run_cos_psi,run_y					+run_w*run_sin_psi,
					use_s2,use_t1 , now_x					+now_w*now_cos_psi,now_y					-now_w*now_sin_psi,
					use_s2,use_t2 , now_x					-now_w*now_cos_psi,now_y					+now_w*now_sin_psi,now_c,now_a
				);
			}

			run_x		=now_x		;
			run_y		=now_y		;
			run_w		=now_w		;
			run_a		=now_a		;
			run_sin_psi	=now_sin_psi;
			run_cos_psi	=now_cos_psi;
			run_c		=now_c		;
		}

		inline void fin_path()
		{
			if(batch.vert_v>0)		// end endcap
			batch.add_tris
			(
				use_s2,use_t1 , run_x					+run_w*run_cos_psi,run_y					-run_w*run_sin_psi,
				use_s2,use_t2 , run_x					-run_w*run_cos_psi,run_y					+run_w*run_sin_psi,
				use_s1,use_t1 , run_x+run_w*run_sin_psi	+run_w*run_cos_psi,run_y+run_w*run_cos_psi	-run_w*run_sin_psi,
				use_s1,use_t2 , run_x+run_w*run_sin_psi	-run_w*run_cos_psi,run_y+run_w*run_cos_psi	+run_w*run_sin_psi,run_c,run_a
			);

			glEnable (GL_CULL_FACE);	// do this for path to avoid the egg shell tunnel doubling back on itself in tite turns
			batch.plot_batch(t_map_icons);
			glDisable(GL_CULL_FACE);	// back to disable so all icons draw regardless of winding order
		}
	#endif
	#if RENDER_AS_TRISTRIP
		inline void add_path(const xflt semi_width_pix,enum_col col,const xflt alpha,const xflt cx_pix,const xflt cy_pix,const xflt psi_deg,const xint is_egg_bounds=0)
		{
			use_s1=394.0/tman.tex_dx_tot[t_map_icons];
			use_s2=400.0/tman.tex_dx_tot[t_map_icons];
			use_t1=  0.4;
			use_t2=  0.6;

			if(is_egg_bounds)
			{
				use_s1=640.0/tman.tex_dx_tot[t_map_icons];
				use_s2=608.0/tman.tex_dx_tot[t_map_icons];
				use_t1=  0.0;
				use_t2=  1.0;
			}

			run_sin_psi	=sin(psi_deg*degtorad);
			run_cos_psi	=cos(psi_deg*degtorad);
			run_x		=cx_pix;
			run_y		=cy_pix;
			run_w		=semi_width_pix;
			run_c		=col;
			run_a		=alpha;

			if(batch.vert_v==0)		// start endcap
			batch.add_line_2d_strip
			(
				use_s1,use_t1 , run_x-run_w*run_sin_psi+run_w*run_cos_psi,run_y-run_w*run_cos_psi-run_w*run_sin_psi ,
				use_s1,use_t2 , run_x-run_w*run_sin_psi-run_w*run_cos_psi,run_y-run_w*run_cos_psi+run_w*run_sin_psi , run_c,run_a
			);

			batch.add_line_2d_strip	// path
			(
				use_s2,use_t1 , run_x+run_w*run_cos_psi,run_y-run_w*run_sin_psi ,
				use_s2,use_t2 , run_x-run_w*run_cos_psi,run_y+run_w*run_sin_psi , run_c,run_a
			);
		}

		inline void fin_path()
		{
			if(batch.vert_v>0)		// end endcap
			batch.add_line_2d_strip
			(
				use_s1,use_t1 , run_x+run_w*run_sin_psi+run_w*run_cos_psi,run_y+run_w*run_cos_psi-run_w*run_sin_psi ,
				use_s1,use_t2 , run_x+run_w*run_sin_psi-run_w*run_cos_psi,run_y+run_w*run_cos_psi+run_w*run_sin_psi , run_c,run_a
			);

			glEnable (GL_CULL_FACE);	// do this for path to avoid the egg shell tunnel doubling back on itself in tite turns
			batch.plot_batch(t_map_icons);
			glDisable(GL_CULL_FACE);	// back to disable so all icons draw regardless of winding order
		}
	#endif
};
extern path_class path;

#endif
